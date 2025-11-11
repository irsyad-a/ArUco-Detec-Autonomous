#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/aruco.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <ctime>
#include <cmath>
#include <algorithm>
#include "serial_transport.hpp"
#include "robot_bridge.hpp"
#include "command_protocol.hpp"

static bool detectYellowBoundaryAhead(const cv::Mat &frameBgr, double &coverageRatio)
{
    // Analyze bottom band for yellow pixels. If over threshold, consider boundary ahead.
    int h = frameBgr.rows;
    int bandTop = int(h * 0.80); // bottom 20%
    cv::Mat roi = frameBgr.rowRange(bandTop, h);
    cv::Mat hsv, mask;
    cv::cvtColor(roi, hsv, cv::COLOR_BGR2HSV);
    // Yellow HSV range (tuned)
    cv::Scalar lower_yellow(18, 110, 120);
    cv::Scalar upper_yellow(40, 255, 255);
    cv::inRange(hsv, lower_yellow, upper_yellow, mask);
    int yellowPixels = cv::countNonZero(mask);
    int total = mask.rows * mask.cols;
    coverageRatio = total > 0 ? (double)yellowPixels / (double)total : 0.0;
    return coverageRatio > 0.02; // 2% of bottom band indicates boundary
}

// ========== Simple config loader (key: value, dotted keys supported) ==========
static std::map<std::string, std::string> loadConfig(const std::vector<std::string> &candidatePaths)
{
    auto trim = [](const std::string &s) {
        size_t b = s.find_first_not_of(" \t\r\n");
        if (b == std::string::npos) return std::string();
        size_t e = s.find_last_not_of(" \t\r\n");
        return s.substr(b, e - b + 1);
    };
    std::map<std::string, std::string> cfg;
    for (const auto &path : candidatePaths)
    {
        std::ifstream in(path);
        if (!in.is_open()) continue;
        std::string line;
        while (std::getline(in, line))
        {
            if (line.empty()) continue;
            if (line[0] == '#') continue;
            auto pos = line.find(':');
            if (pos == std::string::npos) continue;
            std::string key = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 1));
            // remove surrounding quotes if any
            if (!value.empty() && (value.front() == '"' || value.front() == '\'')) value.erase(0, 1);
            if (!value.empty() && (value.back() == '"' || value.back() == '\'')) value.pop_back();
            if (!key.empty()) cfg[key] = value;
        }
        break; // stop at the first file found
    }
    return cfg;
}

static std::string cfgStr(const std::map<std::string, std::string> &cfg, const std::string &key, const std::string &defVal)
{
    auto it = cfg.find(key);
    return it == cfg.end() ? defVal : it->second;
}

static int cfgInt(const std::map<std::string, std::string> &cfg, const std::string &key, int defVal)
{
    auto it = cfg.find(key);
    if (it == cfg.end()) return defVal;
    try { return std::stoi(it->second); } catch (...) { return defVal; }
}

// ==================== Visited markers persistence (very small JSON) ====================
static std::set<int> loadVisited(const std::string &jsonPath)
{
    std::set<int> visited;
    std::ifstream in(jsonPath);
    if (!in.is_open()) return visited;
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    // very small parser: expect [1,2,3]
    int num = 0;
    bool inNum = false, neg = false;
    for (char c : content)
    {
        if (c == '-') { neg = true; continue; }
        if (c >= '0' && c <= '9')
        {
            inNum = true;
            num = num * 10 + (c - '0');
        }
        else
        {
            if (inNum)
            {
                visited.insert(neg ? -num : num);
                num = 0; inNum = false; neg = false;
            }
        }
    }
    if (inNum) visited.insert(neg ? -num : num);
    return visited;
}

static void saveVisited(const std::string &jsonPath, const std::set<int> &visited)
{
    std::ofstream out(jsonPath, std::ios::trunc);
    if (!out.is_open()) return;
    out << "[";
    bool first = true;
    for (int id : visited)
    {
        if (!first) out << ",";
        out << id;
        first = false;
    }
    out << "]";
}

// ==================== Autonomous Navigator (trimmed from existing) ====================
enum RobotState { SCANNING, TRACKING, LOCKED, APPROACHING, SCAN_COMPLETE };

struct MarkerInfo
{
    int id;
    cv::Point2f center;
    float size;
    float distance;
    float angle;
    time_t lastSeen;
    bool isScanned;
};

class AutonomousNavigator
{
private:
    RobotState currentState;
    std::map<int, MarkerInfo> detectedMarkers;
    std::map<int, bool> scannedMarkers;
    int targetMarkerId;

    int totalMarkersFound;
    int totalMarkersScanned;
    time_t sessionStartTime;
    std::deque<std::string> navigationHistory;
    int maxHistorySize;

    float centerThreshold;
    float approachDistance;
    float minMarkerSize;
    float farMarkerSize;
    int frameWidth;
    int frameHeight;
    cv::Point2f frameCenter;

public:
    AutonomousNavigator()
        : currentState(SCANNING),
          targetMarkerId(-1),
          totalMarkersFound(0),
          totalMarkersScanned(0),
          sessionStartTime(time(0)),
          maxHistorySize(10),
          centerThreshold(120.0f),
          approachDistance(180.0f),
          minMarkerSize(150.0f),
          farMarkerSize(50.0f),
          frameWidth(640),
          frameHeight(480)
    {
        frameCenter = cv::Point2f(frameWidth / 2.0f, frameHeight / 2.0f);
    }

    void setFrameSize(int width, int height)
    {
        frameWidth = width;
        frameHeight = height;
        frameCenter = cv::Point2f(frameWidth / 2.0f, frameHeight / 2.0f);
    }

    void preloadVisited(const std::set<int> &visited)
    {
        for (int id : visited) scannedMarkers[id] = true;
    }

    void updateMarkers(const std::vector<int> &markerIds,
                       const std::vector<std::vector<cv::Point2f>> &markerCorners)
    {
        time_t currentTime = time(0);
        std::set<int> currentFrameMarkers;

        for (size_t i = 0; i < markerIds.size(); i++)
        {
            int id = markerIds[i];
            currentFrameMarkers.insert(id);

            cv::Point2f center(0, 0);
            for (int j = 0; j < 4; j++)
            {
                center.x += markerCorners[i][j].x;
                center.y += markerCorners[i][j].y;
            }
            center.x /= 4.0f;
            center.y /= 4.0f;

            float dx = markerCorners[i][0].x - markerCorners[i][2].x;
            float dy = markerCorners[i][0].y - markerCorners[i][2].y;
            float size = std::sqrt(dx * dx + dy * dy);

            float angle = std::atan2(center.y - frameCenter.y, center.x - frameCenter.x) * 180.0f / M_PI;
            float distance = 1000.0f / (size + 1.0f);

            if (detectedMarkers.find(id) == detectedMarkers.end())
            {
                totalMarkersFound++;
            }

            MarkerInfo info;
            info.id = id;
            info.center = center;
            info.size = size;
            info.distance = distance;
            info.angle = angle;
            info.lastSeen = currentTime;
            info.isScanned = scannedMarkers[id];

            detectedMarkers[id] = info;
        }
    }

    int selectTargetMarker()
    {
        if (detectedMarkers.empty()) return -1;
        int bestTarget = -1;
        float bestScore = FLT_MAX;
        for (const auto &pair : detectedMarkers)
        {
            const MarkerInfo &info = pair.second;
            if (info.isScanned) continue;
            float dx = info.center.x - frameCenter.x;
            float dy = info.center.y - frameCenter.y;
            float distFromCenter = std::sqrt(dx * dx + dy * dy);
            float score = distFromCenter - info.size * 0.5f;
            if (score < bestScore)
            {
                bestScore = score;
                bestTarget = info.id;
            }
        }
        return bestTarget;
    }

    std::string getNavigationInstruction()
    {
        if (detectedMarkers.empty())
        {
            currentState = SCANNING;
            return "PUTAR KIRI - Scan lingkungan";
        }

        if (targetMarkerId == -1 ||
            detectedMarkers.find(targetMarkerId) == detectedMarkers.end() ||
            detectedMarkers[targetMarkerId].isScanned)
        {
            targetMarkerId = selectTargetMarker();
            if (targetMarkerId == -1)
            {
                currentState = SCANNING;
                return "SCAN COMPLETE - Semua marker ditemukan!";
            }
        }

        const MarkerInfo &target = detectedMarkers[targetMarkerId];
        float offsetX = target.center.x - frameCenter.x;
        float offsetY = target.center.y - frameCenter.y;
        float distFromCenter = std::sqrt(offsetX * offsetX + offsetY * offsetY);

        std::string instruction;
        bool isCentered = distFromCenter < centerThreshold;
        bool isCloseEnough = target.size > minMarkerSize;
        bool isFar = target.size < farMarkerSize;
        bool isModeratelyCentered = distFromCenter < (centerThreshold * 1.5f);

        if (isCentered && isCloseEnough)
        {
            currentState = LOCKED;
            scannedMarkers[targetMarkerId] = true;
            totalMarkersScanned++;
            return "LOCKED! - Scan otomatis";
        }
        else if (isFar)
        {
            currentState = APPROACHING;
            std::string majuInstr = "MAJU 100 cm";
            if (!isModeratelyCentered)
            {
                if (offsetX > centerThreshold * 0.5f) majuInstr += " + Sedikit KANAN";
                else if (offsetX < -centerThreshold * 0.5f) majuInstr += " + Sedikit KIRI";
            }
            return majuInstr;
        }
        else if (target.size < minMarkerSize)
        {
            currentState = APPROACHING;
            if (isModeratelyCentered)
            {
                float distance = (minMarkerSize - target.size) * 0.8f;
                return "MAJU " + std::to_string((int)distance) + " cm";
            }
            else
            {
                std::string direction = (offsetX > 0) ? "KANAN" : "KIRI";
                float distance = (minMarkerSize - target.size) * 0.5f;
                return "MAJU " + std::to_string((int)distance) + " cm + PUTAR " + direction;
            }
        }
        else
        {
            currentState = TRACKING;
            if (std::abs(offsetX) > std::abs(offsetY))
            {
                if (offsetX > centerThreshold * 0.3f)
                {
                    float angle = std::abs(offsetX) / 8.0f;
                    instruction = "PUTAR KANAN " + std::to_string((int)angle) + "°";
                }
                else if (offsetX < -centerThreshold * 0.3f)
                {
                    float angle = std::abs(offsetX) / 8.0f;
                    instruction = "PUTAR KIRI " + std::to_string((int)angle) + "°";
                }
                else
                {
                    instruction = "MAJU 20 cm";
                }
            }
            else
            {
                instruction = "MAJU 15 cm";
            }
        }

        navigationHistory.push_back(instruction);
        if (navigationHistory.size() > maxHistorySize) navigationHistory.pop_front();
        return instruction;
    }

    RobotState getState() const { return currentState; }
    int getTargetId() const { return targetMarkerId; }
    int getTotalFound() const { return totalMarkersFound; }
    int getTotalScanned() const { return totalMarkersScanned; }
    const std::map<int, MarkerInfo> &getMarkers() const { return detectedMarkers; }
};

static std::string nowString()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[64];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf);
}

int main(int argc, char *argv[])
{
    std::cout << "========================================\n";
    std::cout << "AUTONOMOUS ROVER (ArUco -> Robot Bridge)\n";
    std::cout << "========================================\n\n";

    // Load config
    auto cfg = loadConfig({ "../config/rover_config.yaml", "config/rover_config.yaml" });
    std::string ipCameraURL = cfgStr(cfg, "camera.url", "http://192.168.0.10:8080/video");
    std::string serialPort = cfgStr(cfg, "robot.port", "/dev/ttyUSB0");
    int baud = cfgInt(cfg, "robot.baud", 115200);

    int speedForward = cfgInt(cfg, "speed.forward", 150);
    int speedBackward = cfgInt(cfg, "speed.back", 150);
    int speedTurn = cfgInt(cfg, "speed.turn", 130);
    int msPerCm = cfgInt(cfg, "calibration.ms_per_cm", 80);
    int msPerDeg = cfgInt(cfg, "calibration.ms_per_deg", 12);
    std::string protocolMode = cfgStr(cfg, "protocol.mode", "standard");

    // Camera override from argv, if present
    if (argc > 1) ipCameraURL = argv[1];

    // Setup camera
    cv::VideoCapture cam;
    std::cout << "Connecting to camera: " << ipCameraURL << "\n";
    cam.open(ipCameraURL, cv::CAP_FFMPEG);
    if (!cam.isOpened())
    {
        std::cout << "ERROR: Cannot open camera!\n";
        return -1;
    }
    std::cout << "✓ Camera connected.\n";

    // Setup ArUco
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_50);
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();

    // Navigator
    AutonomousNavigator navigator;

    // Visited memory
    const std::string visitedPath = "visited_markers.json";
    std::set<int> visited = loadVisited(visitedPath);
    navigator.preloadVisited(visited);
    std::cout << "Visited markers loaded: " << visited.size() << "\n";

    // Serial transport
    SerialTransport serial;
    if (!serial.open(serialPort, baud))
    {
        std::cout << "WARN: Serial open failed for " << serialPort << " (baud " << baud << "). Continue without robot.\n";
    }
    else
    {
        std::cout << "✓ Serial connected: " << serialPort << " @" << baud << "\n";
        // optional: push calibration
        serial.sendLine(rovercmd::buildCalSetMsPerCm(msPerCm));
        serial.sendLine(rovercmd::buildCalSetMsPerDeg(msPerDeg));
    }

    // Bridge
    RoverConfig rcfg;
    rcfg.speedForward = speedForward;
    rcfg.speedBackward = speedBackward;
    rcfg.speedTurn = speedTurn;
    rcfg.msPerCm = msPerCm;
    rcfg.msPerDeg = msPerDeg;
    rcfg.useStm32Protocol = (protocolMode == "stm32");
    RobotBridge bridge(rcfg);

    cv::Mat frame;
    std::string lastInstruction;
    uint64_t lastSentMs = 0;
    auto nowMs = []() -> uint64_t {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (uint64_t)ts.tv_sec * 1000ULL + (uint64_t)ts.tv_nsec / 1000000ULL;
    };

    while (true)
    {
        cam >> frame;
        if (frame.empty())
        {
            std::cout << "ERROR: Empty frame\n";
            break;
        }

        navigator.setFrameSize(frame.cols, frame.rows);

        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
        cv::aruco::detectMarkers(frame, dictionary, markerCorners, markerIds, detectorParams, rejectedCandidates);
        navigator.updateMarkers(markerIds, markerCorners);

        std::string instruction = navigator.getNavigationInstruction();

        // High-priority: Yellow boundary avoidance
        double yellowRatio = 0.0;
        bool boundary = detectYellowBoundaryAhead(frame, yellowRatio);
        if (boundary)
        {
            instruction = "STOP";
            // Optional escape if ratio is strong
            if (yellowRatio > 0.06) instruction = "MUNDUR 20 cm";
        }

        // If navigator locks, mark visited
        if (instruction.find("LOCKED") != std::string::npos)
        {
            int tid = navigator.getTargetId();
            if (tid >= 0 && visited.find(tid) == visited.end())
            {
                visited.insert(tid);
                saveVisited(visitedPath, visited);
                std::cout << "[" << nowString() << "] Marked visited ID=" << tid << "\n";
            }
        }

        // Rate-limit sending to robot to avoid spamming
        bool changed = (instruction != lastInstruction);
        uint64_t now = nowMs();
        bool timeout = (now - lastSentMs) > 600; // ms
        if (changed || timeout)
        {
            auto commands = bridge.mapInstruction(instruction);
            if (!commands.empty() && serial.isOpen())
            {
                for (const auto &cmd : commands)
                {
                    serial.sendLine(cmd);
                }
            }
            lastInstruction = instruction;
            lastSentMs = now;
        }

        // Show small status on frame
        cv::putText(frame, "Instruksi: " + instruction, cv::Point(10, 30),
                    cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
        // Visual for yellow boundary
        double ycov = 0.0;
        bool b = detectYellowBoundaryAhead(frame, ycov);
        if (b)
        {
            int h = frame.rows;
            cv::rectangle(frame, cv::Point(0, int(h*0.80)), cv::Point(frame.cols, h),
                          cv::Scalar(0, 255, 255), 2);
            std::string warn = "YELLOW BOUNDARY! ratio=" + std::to_string((int)(ycov*100)) + "%";
            cv::putText(frame, warn, cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                        cv::Scalar(0, 255, 255), 2);
        }
        cv::imshow("Rover Camera", frame);
        int key = cv::waitKey(1);
        if (key == 'q' || key == 'Q' || key == 27) break;
    }

    cam.release();
    cv::destroyAllWindows();
    if (serial.isOpen()) serial.close();
    std::cout << "Shutdown.\n";
    return 0;
}


