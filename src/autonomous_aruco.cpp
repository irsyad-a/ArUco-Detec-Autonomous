#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/aruco.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <map>
#include <set>
#include <cmath>
#include <deque>

// ============================================
// ENUMERASI STATUS ROBOT
// ============================================
enum RobotState
{
    SCANNING,     // Mencari marker
    TRACKING,     // Mengikuti marker
    LOCKED,       // Locked on target, siap scan
    APPROACHING,  // Mendekati marker
    SCAN_COMPLETE // Scan selesai
};

// ============================================
// STRUKTUR DATA MARKER
// ============================================
struct MarkerInfo
{
    int id;
    cv::Point2f center;
    float size;
    float distance; // Estimasi jarak (berdasarkan ukuran)
    float angle;    // Sudut dari center frame
    time_t lastSeen;
    bool isScanned;
};

// ============================================
// KELAS AUTONOMOUS NAVIGATION SYSTEM
// ============================================
class AutonomousNavigator
{
private:
    RobotState currentState;
    std::map<int, MarkerInfo> detectedMarkers;
    std::map<int, bool> scannedMarkers;
    int targetMarkerId;

    // Statistik
    int totalMarkersFound;
    int totalMarkersScanned;
    time_t sessionStartTime;

    // History untuk smoothing
    std::deque<std::string> navigationHistory;
    int maxHistorySize;

    // Threshold parameters
    float centerThreshold;  // Threshold untuk "centered"
    float approachDistance; // Jarak optimal untuk scan
    float minMarkerSize;    // Minimum size untuk scan
    float farMarkerSize;    // Size threshold untuk marker jauh (perlu maju)

    // Frame dimensions
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
          centerThreshold(120.0f), // Lebih longgar untuk 3 meter
          approachDistance(180.0f),
          minMarkerSize(150.0f), // Size untuk LOCKED (dekat)
          farMarkerSize(50.0f),  // Size marker jauh - perlu MAJU agresif!
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

    // Update deteksi marker
    void updateMarkers(const std::vector<int> &markerIds,
                       const std::vector<std::vector<cv::Point2f>> &markerCorners)
    {
        time_t currentTime = time(0);
        std::set<int> currentFrameMarkers;

        for (size_t i = 0; i < markerIds.size(); i++)
        {
            int id = markerIds[i];
            currentFrameMarkers.insert(id);

            // Hitung center
            cv::Point2f center(0, 0);
            for (int j = 0; j < 4; j++)
            {
                center.x += markerCorners[i][j].x;
                center.y += markerCorners[i][j].y;
            }
            center.x /= 4.0f;
            center.y /= 4.0f;

            // Hitung size (diagonal)
            float dx = markerCorners[i][0].x - markerCorners[i][2].x;
            float dy = markerCorners[i][0].y - markerCorners[i][2].y;
            float size = std::sqrt(dx * dx + dy * dy);

            // Hitung sudut dari center frame
            float angle = std::atan2(center.y - frameCenter.y,
                                     center.x - frameCenter.x) *
                          180.0f / M_PI;

            // Estimasi jarak (inverse proportional to size)
            float distance = 1000.0f / (size + 1.0f); // Simple estimation

            // Update atau tambah marker info
            if (detectedMarkers.find(id) == detectedMarkers.end())
            {
                // Marker baru ditemukan
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

    // Pilih target marker berdasarkan prioritas
    int selectTargetMarker()
    {
        if (detectedMarkers.empty())
            return -1;

        // Prioritas: marker belum di-scan, paling dekat ke center
        int bestTarget = -1;
        float bestScore = FLT_MAX;

        for (const auto &pair : detectedMarkers)
        {
            const MarkerInfo &info = pair.second;

            // Skip jika sudah di-scan
            if (info.isScanned)
                continue;

            // Hitung jarak dari center frame
            float dx = info.center.x - frameCenter.x;
            float dy = info.center.y - frameCenter.y;
            float distFromCenter = std::sqrt(dx * dx + dy * dy);

            // Score: kombinasi jarak dari center dan ukuran marker
            float score = distFromCenter - info.size * 0.5f;

            if (score < bestScore)
            {
                bestScore = score;
                bestTarget = info.id;
            }
        }

        return bestTarget;
    }

    // Dapatkan instruksi navigasi
    std::string getNavigationInstruction()
    {
        if (detectedMarkers.empty())
        {
            currentState = SCANNING;
            return "PUTAR KIRI - Scan lingkungan";
        }

        // Pilih atau update target
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

        // Dapatkan info target
        const MarkerInfo &target = detectedMarkers[targetMarkerId];

        // Hitung offset dari center
        float offsetX = target.center.x - frameCenter.x;
        float offsetY = target.center.y - frameCenter.y;
        float distFromCenter = std::sqrt(offsetX * offsetX + offsetY * offsetY);

        // Decision making
        std::string instruction;

        // Check status marker
        bool isCentered = distFromCenter < centerThreshold;
        bool isCloseEnough = target.size > minMarkerSize;
        bool isFar = target.size < farMarkerSize; // Marker jauh (dari 3 meter)
        bool isModeratelyCentered = distFromCenter < (centerThreshold * 1.5f);

        // NEW STRATEGY: Prioritaskan MAJU untuk marker jauh!

        if (isCentered && isCloseEnough)
        {
            // LOCKED - siap untuk scan
            currentState = LOCKED;
            scannedMarkers[targetMarkerId] = true;
            totalMarkersScanned++;
            return "LOCKED! - Tekan 'SCAN' (S)";
        }
        else if (isFar)
        {
            // MARKER JAUH (< 50px) - LANGSUNG MAJU AGRESIF!
            currentState = APPROACHING;

            // Kasih instruksi MAJU dengan hint arah
            std::string majuInstr = "MAJU 100 cm";

            // Tambah hint kiri/kanan jika perlu (tapi tetap MAJU dulu!)
            if (!isModeratelyCentered)
            {
                if (offsetX > centerThreshold * 0.5f)
                    majuInstr += " + Sedikit KANAN";
                else if (offsetX < -centerThreshold * 0.5f)
                    majuInstr += " + Sedikit KIRI";
            }

            return majuInstr;
        }
        else if (target.size < minMarkerSize)
        {
            // MARKER MEDIUM (50-150px) - Perlu MAJU
            currentState = APPROACHING;

            // Jika cukup centered, full MAJU
            if (isModeratelyCentered)
            {
                float distance = (minMarkerSize - target.size) * 0.8f;
                return "MAJU " + std::to_string((int)distance) + " cm";
            }
            else
            {
                // Belum cukup centered - hybrid approach
                std::string direction = (offsetX > 0) ? "KANAN" : "KIRI";
                float distance = (minMarkerSize - target.size) * 0.5f;
                return "MAJU " + std::to_string((int)distance) + " cm + PUTAR " + direction;
            }
        }
        else
        {
            // MARKER DEKAT tapi belum centered - Fine tuning
            currentState = TRACKING;

            // Tentukan arah
            if (std::abs(offsetX) > std::abs(offsetY))
            {
                // Horizontal adjustment
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
                    // Hampir centered, maju sedikit
                    instruction = "MAJU 20 cm";
                }
            }
            else
            {
                // Vertical adjustment
                if (offsetY > centerThreshold)
                {
                    instruction = "TURUNKAN KAMERA";
                }
                else if (offsetY < -centerThreshold)
                {
                    instruction = "NAIKKAN KAMERA";
                }
                else
                {
                    // Centered, tinggal fine tuning
                    instruction = "MAJU 15 cm";
                }
            }
        }

        // Add to history untuk smoothing
        navigationHistory.push_back(instruction);
        if (navigationHistory.size() > maxHistorySize)
            navigationHistory.pop_front();

        return instruction;
    }

    // Dapatkan state string
    std::string getStateString() const
    {
        switch (currentState)
        {
        case SCANNING:
            return "SCANNING";
        case TRACKING:
            return "TRACKING";
        case LOCKED:
            return "LOCKED";
        case APPROACHING:
            return "APPROACHING";
        case SCAN_COMPLETE:
            return "COMPLETE";
        default:
            return "UNKNOWN";
        }
    }

    // Dapatkan warna state
    cv::Scalar getStateColor() const
    {
        switch (currentState)
        {
        case SCANNING:
            return cv::Scalar(0, 165, 255); // Orange
        case TRACKING:
            return cv::Scalar(0, 255, 255); // Yellow
        case LOCKED:
            return cv::Scalar(0, 255, 0); // Green
        case APPROACHING:
            return cv::Scalar(255, 255, 0); // Cyan
        case SCAN_COMPLETE:
            return cv::Scalar(255, 0, 255); // Magenta
        default:
            return cv::Scalar(128, 128, 128);
        }
    }

    RobotState getState() const { return currentState; }
    int getTargetId() const { return targetMarkerId; }
    int getTotalFound() const { return totalMarkersFound; }
    int getTotalScanned() const { return totalMarkersScanned; }
    const std::map<int, MarkerInfo> &getMarkers() const { return detectedMarkers; }
};

// ============================================
// FUNGSI UNTUK MENGGAMBAR NAVIGATION PANEL
// ============================================
void drawNavigationPanel(cv::Mat &panel,
                         const std::string &instruction,
                         const std::string &state,
                         const cv::Scalar &stateColor,
                         int targetId,
                         int found,
                         int scanned)
{
    // Background
    panel.setTo(cv::Scalar(30, 30, 30)); // Dark gray

    // Header
    cv::rectangle(panel, cv::Point(0, 0), cv::Point(panel.cols, 70),
                  cv::Scalar(50, 50, 50), -1);

    cv::putText(panel, "AUTONOMOUS NAVIGATION SYSTEM",
                cv::Point(20, 45),
                cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(255, 255, 255), 2);

    // State indicator
    int stateY = 120;
    cv::putText(panel, "STATUS:", cv::Point(30, stateY),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(200, 200, 200), 2);

    cv::rectangle(panel, cv::Point(200, stateY - 30), cv::Point(450, stateY + 10),
                  stateColor, -1);
    cv::rectangle(panel, cv::Point(200, stateY - 30), cv::Point(450, stateY + 10),
                  cv::Scalar(255, 255, 255), 2);

    cv::putText(panel, state, cv::Point(220, stateY - 5),
                cv::FONT_HERSHEY_DUPLEX, 0.9, cv::Scalar(255, 255, 255), 2);

    // Main instruction (BIG)
    int instrY = 220;
    cv::putText(panel, "INSTRUKSI:", cv::Point(30, instrY),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(200, 200, 200), 1);

    // Instruction box
    cv::rectangle(panel, cv::Point(20, instrY + 20), cv::Point(panel.cols - 20, instrY + 120),
                  cv::Scalar(0, 120, 255), -1); // Orange box
    cv::rectangle(panel, cv::Point(20, instrY + 20), cv::Point(panel.cols - 20, instrY + 120),
                  cv::Scalar(255, 255, 255), 3);

    // Split instruction if too long
    std::string displayInstr = instruction;
    if (displayInstr.length() > 20)
    {
        // Word wrap
        size_t spacePos = displayInstr.find(' ', 15);
        if (spacePos != std::string::npos && spacePos < displayInstr.length())
        {
            std::string line1 = displayInstr.substr(0, spacePos);
            std::string line2 = displayInstr.substr(spacePos + 1);

            cv::putText(panel, line1, cv::Point(40, instrY + 65),
                        cv::FONT_HERSHEY_DUPLEX, 1.3, cv::Scalar(255, 255, 255), 3);
            cv::putText(panel, line2, cv::Point(40, instrY + 100),
                        cv::FONT_HERSHEY_DUPLEX, 1.1, cv::Scalar(255, 255, 255), 2);
        }
        else
        {
            cv::putText(panel, displayInstr, cv::Point(40, instrY + 80),
                        cv::FONT_HERSHEY_DUPLEX, 1.1, cv::Scalar(255, 255, 255), 3);
        }
    }
    else
    {
        cv::putText(panel, displayInstr, cv::Point(40, instrY + 80),
                    cv::FONT_HERSHEY_DUPLEX, 1.5, cv::Scalar(255, 255, 255), 3);
    }

    // Statistics
    int statsY = 400;
    cv::putText(panel, "STATISTIK:", cv::Point(30, statsY),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(200, 200, 200), 2);

    std::string targetText = "Target ID: " + (targetId >= 0 ? std::to_string(targetId) : "NONE");
    std::string foundText = "Markers Found: " + std::to_string(found);
    std::string scannedText = "Markers Scanned: " + std::to_string(scanned);

    cv::putText(panel, targetText, cv::Point(30, statsY + 40),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
    cv::putText(panel, foundText, cv::Point(30, statsY + 75),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
    cv::putText(panel, scannedText, cv::Point(30, statsY + 110),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);

    // Controls
    int ctrlY = 570;
    cv::rectangle(panel, cv::Point(0, ctrlY - 10), cv::Point(panel.cols, ctrlY + 110),
                  cv::Scalar(50, 50, 50), -1);

    cv::putText(panel, "KONTROL:", cv::Point(30, ctrlY + 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(200, 200, 200), 1);

    cv::putText(panel, "[S] Scan Marker", cv::Point(30, ctrlY + 50),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
    cv::putText(panel, "[R] Reset Target", cv::Point(30, ctrlY + 75),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 255), 2);
    cv::putText(panel, "[Q] Keluar", cv::Point(30, ctrlY + 100),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 2);
}

// ============================================
// FUNGSI UNTUK MENGGAMBAR OVERLAY PADA CAMERA
// ============================================
void drawCameraOverlay(cv::Mat &frame,
                       const std::vector<int> &markerIds,
                       const std::vector<std::vector<cv::Point2f>> &markerCorners,
                       const AutonomousNavigator &navigator)
{
    int frameWidth = frame.cols;
    int frameHeight = frame.rows;
    cv::Point2f center(frameWidth / 2.0f, frameHeight / 2.0f);

    // Draw center crosshair
    int crossSize = 30;
    cv::Scalar crossColor = navigator.getStateColor();

    cv::line(frame, cv::Point(center.x - crossSize, center.y),
             cv::Point(center.x + crossSize, center.y), crossColor, 2);
    cv::line(frame, cv::Point(center.x, center.y - crossSize),
             cv::Point(center.x, center.y + crossSize), crossColor, 2);
    cv::circle(frame, center, crossSize + 10, crossColor, 2);

    // Draw center zone (threshold area)
    float threshold = 80.0f;
    cv::circle(frame, center, threshold, cv::Scalar(100, 100, 100), 1);

    // Draw detected markers dengan info tambahan
    if (!markerIds.empty())
    {
        const auto &markers = navigator.getMarkers();
        int targetId = navigator.getTargetId();

        for (size_t i = 0; i < markerIds.size(); i++)
        {
            int id = markerIds[i];
            bool isTarget = (id == targetId);

            // Warna berbeda untuk target
            cv::Scalar markerColor = isTarget ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 165, 255);

            // Draw marker outline
            for (int j = 0; j < 4; j++)
            {
                cv::line(frame, markerCorners[i][j],
                         markerCorners[i][(j + 1) % 4],
                         markerColor, isTarget ? 3 : 2);
            }

            // Get marker info
            if (markers.find(id) != markers.end())
            {
                const MarkerInfo &info = markers.at(id);

                // Draw center point
                cv::circle(frame, info.center, 5, markerColor, -1);

                // Draw line to center if target
                if (isTarget)
                {
                    cv::line(frame, info.center, center, cv::Scalar(0, 255, 0), 2);
                }

                // Info text
                std::string label = "ID:" + std::to_string(id);
                if (isTarget)
                    label += " [TARGET]";

                cv::putText(frame, label,
                            cv::Point(info.center.x - 40, info.center.y - 20),
                            cv::FONT_HERSHEY_SIMPLEX, 0.6, markerColor, 2);

                // Size info
                std::string sizeText = std::to_string((int)info.size) + "px";
                cv::putText(frame, sizeText,
                            cv::Point(info.center.x - 30, info.center.y + 30),
                            cv::FONT_HERSHEY_SIMPLEX, 0.5, markerColor, 1);
            }
        }
    }

    // Status bar di atas
    cv::rectangle(frame, cv::Point(0, 0), cv::Point(frameWidth, 40),
                  cv::Scalar(0, 0, 0), -1);

    std::string statusText = "Markers: " + std::to_string(markerIds.size()) +
                             " | State: " + navigator.getStateString();
    cv::putText(frame, statusText, cv::Point(10, 25),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
}

// ============================================
// FUNGSI LOGGING
// ============================================
std::string getCurrentTimestamp()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf);
}

void logScanEvent(int markerId, const MarkerInfo &info, const std::string &logFile)
{
    std::ofstream log;
    log.open(logFile, std::ios::app);

    if (log.is_open())
    {
        log << "========================================\n";
        log << "SCAN EVENT\n";
        log << "Timestamp: " << getCurrentTimestamp() << "\n";
        log << "Marker ID: " << markerId << "\n";
        log << "Position: (" << (int)info.center.x << ", " << (int)info.center.y << ")\n";
        log << "Size: " << (int)info.size << " px\n";
        log << "Status: SCANNED\n";
        log << "========================================\n\n";
        log.close();
    }
}

// ============================================
// MAIN FUNCTION
// ============================================
int main(int argc, char *argv[])
{
    std::cout << "========================================\n";
    std::cout << "SEMI-AUTONOMOUS ROBOT NAVIGATION SYSTEM\n";
    std::cout << "========================================\n\n";

    // Setup camera
    cv::VideoCapture cam;
    std::string ipCameraURL = "http://10.237.86.79:8080/video";

    if (argc > 1)
    {
        ipCameraURL = argv[1];
    }

    std::cout << "Connecting to camera: " << ipCameraURL << std::endl;
    cam.open(ipCameraURL, cv::CAP_FFMPEG);

    if (!cam.isOpened())
    {
        std::cout << "ERROR: Cannot open camera!\n";
        std::cout << "Usage: " << argv[0] << " [IP_CAMERA_URL]\n";
        std::cout << "Example: " << argv[0] << " http://192.168.1.100:8080/video\n";
        return -1;
    }

    std::cout << "✓ Camera connected successfully!\n\n";

    // Setup ArUco
    cv::Ptr<cv::aruco::Dictionary> dictionary =
        cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_50);
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams =
        cv::aruco::DetectorParameters::create();

    // Initialize navigator
    AutonomousNavigator navigator;

    // Log file
    std::string logFile = "autonomous_navigation_log.txt";
    std::ofstream log(logFile, std::ios::app);
    if (log.is_open())
    {
        log << "\n\n=== NEW SESSION ===\n";
        log << "Start Time: " << getCurrentTimestamp() << "\n";
        log << "Camera: " << ipCameraURL << "\n\n";
        log.close();
    }

    std::cout << "✓ Log file: " << logFile << "\n";
    std::cout << "\n========================================\n";
    std::cout << "SYSTEM READY!\n";
    std::cout << "========================================\n";

    // Create windows
    cv::namedWindow("Camera View", cv::WINDOW_NORMAL);
    cv::namedWindow("Navigation Panel", cv::WINDOW_NORMAL);

    // Resize windows
    cv::resizeWindow("Camera View", 800, 600);
    cv::resizeWindow("Navigation Panel", 500, 700);

    cv::Mat frame;
    cv::Mat navPanel(700, 500, CV_8UC3);

    bool running = true;

    while (running)
    {
        cam >> frame;

        if (frame.empty())
        {
            std::cout << "ERROR: Empty frame!\n";
            break;
        }

        // Update frame size untuk navigator
        navigator.setFrameSize(frame.cols, frame.rows);

        // Detect ArUco markers
        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;

        cv::aruco::detectMarkers(frame, dictionary, markerCorners, markerIds,
                                 detectorParams, rejectedCandidates);

        // Update navigator dengan deteksi terbaru
        navigator.updateMarkers(markerIds, markerCorners);

        // Get navigation instruction
        std::string instruction = navigator.getNavigationInstruction();

        // Draw overlays
        drawCameraOverlay(frame, markerIds, markerCorners, navigator);
        drawNavigationPanel(navPanel,
                            instruction,
                            navigator.getStateString(),
                            navigator.getStateColor(),
                            navigator.getTargetId(),
                            navigator.getTotalFound(),
                            navigator.getTotalScanned());

        // Show windows
        cv::imshow("Camera View", frame);
        cv::imshow("Navigation Panel", navPanel);

        // Keyboard control
        int key = cv::waitKey(1);

        if (key == 'q' || key == 'Q' || key == 27) // ESC
        {
            running = false;
            std::cout << "\nShutting down...\n";
        }
        else if (key == 's' || key == 'S')
        {
            // Manual scan
            int targetId = navigator.getTargetId();
            if (targetId >= 0)
            {
                const auto &markers = navigator.getMarkers();
                if (markers.find(targetId) != markers.end())
                {
                    logScanEvent(targetId, markers.at(targetId), logFile);
                    std::cout << "✓ Marker " << targetId << " SCANNED!\n";
                }
            }
        }
        else if (key == 'r' || key == 'R')
        {
            // Reset (akan auto-select target baru)
            std::cout << "Target reset - searching for new target...\n";
        }
    }

    // Cleanup
    cam.release();
    cv::destroyAllWindows();

    // Final log
    log.open(logFile, std::ios::app);
    if (log.is_open())
    {
        log << "\n=== SESSION END ===\n";
        log << "End Time: " << getCurrentTimestamp() << "\n";
        log << "Total Markers Found: " << navigator.getTotalFound() << "\n";
        log << "Total Markers Scanned: " << navigator.getTotalScanned() << "\n\n";
        log.close();
    }

    std::cout << "\n========================================\n";
    std::cout << "SESSION SUMMARY\n";
    std::cout << "========================================\n";
    std::cout << "Markers Found: " << navigator.getTotalFound() << "\n";
    std::cout << "Markers Scanned: " << navigator.getTotalScanned() << "\n";
    std::cout << "Log file: " << logFile << "\n";
    std::cout << "========================================\n";
    std::cout << "Thank you for using Autonomous Navigation System!\n\n";

    return 0;
}
