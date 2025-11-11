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

// Fungsi untuk mendapatkan timestamp
std::string getCurrentTimestamp()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf);
}

// Fungsi untuk mendapatkan timestamp untuk filename
std::string getFilenameTimestamp()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tstruct);
    return std::string(buf);
}

// Fungsi untuk log deteksi ArUco ke file
void logArucoDetection(int markerID, cv::Point2f center, float size, const std::string &filename = "aruco_detection_log.txt")
{
    std::ofstream logFile;
    logFile.open(filename, std::ios::app); // Append mode

    if (logFile.is_open())
    {
        logFile << "================================================\n";
        logFile << "Timestamp      : " << getCurrentTimestamp() << "\n";
        logFile << "Marker ID      : " << markerID << "\n";
        logFile << "Center Position: (" << (int)center.x << ", " << (int)center.y << ")\n";
        logFile << "Size (pixels)  : " << (int)size << " px\n";
        logFile << "Status         : DETECTED\n";
        logFile << "================================================\n\n";
        logFile.close();
    }
}

// Fungsi untuk membuat modal box notifikasi
void drawNotificationModal(cv::Mat &frame, int markerID, cv::Point2f center, float size, int displayTime)
{
    int frameWidth = frame.cols;
    int frameHeight = frame.rows;

    // Ukuran modal box
    int modalWidth = 450;
    int modalHeight = 220;
    int modalX = (frameWidth - modalWidth) / 2;
    int modalY = 50; // Dari atas

    // Buat overlay semi-transparan
    cv::Mat overlay = frame.clone();

    // Background modal (putih dengan border)
    cv::rectangle(overlay,
                  cv::Point(modalX, modalY),
                  cv::Point(modalX + modalWidth, modalY + modalHeight),
                  cv::Scalar(255, 255, 255), -1);

    // Border hijau
    cv::rectangle(overlay,
                  cv::Point(modalX, modalY),
                  cv::Point(modalX + modalWidth, modalY + modalHeight),
                  cv::Scalar(0, 255, 0), 4);

    // Header hijau
    cv::rectangle(overlay,
                  cv::Point(modalX, modalY),
                  cv::Point(modalX + modalWidth, modalY + 50),
                  cv::Scalar(0, 200, 0), -1);

    // Blend dengan frame asli (transparansi)
    double alpha = 0.9; // Opacity
    cv::addWeighted(overlay, alpha, frame, 1 - alpha, 0, frame);

    // Teks header
    cv::putText(frame, "ARUCO DETECTED!",
                cv::Point(modalX + 100, modalY + 35),
                cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255, 255, 255), 2);

    // Icon checkmark (simulasi dengan circle)
    cv::circle(frame, cv::Point(modalX + 50, modalY + 25), 15, cv::Scalar(255, 255, 255), -1);
    cv::putText(frame, "✓", cv::Point(modalX + 40, modalY + 35),
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 200, 0), 3);

    // Informasi detail
    int textY = modalY + 80;
    int lineSpacing = 35;

    // Marker ID
    std::string idText = "Marker ID: " + std::to_string(markerID);
    cv::putText(frame, idText,
                cv::Point(modalX + 30, textY),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);

    // Position
    textY += lineSpacing;
    std::string posText = "Position: (" + std::to_string((int)center.x) + ", " + std::to_string((int)center.y) + ")";
    cv::putText(frame, posText,
                cv::Point(modalX + 30, textY),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(50, 50, 50), 2);

    // Size
    textY += lineSpacing;
    std::string sizeText = "Size: " + std::to_string((int)size) + " pixels";
    cv::putText(frame, sizeText,
                cv::Point(modalX + 30, textY),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(50, 50, 50), 2);

    // Timestamp
    textY += lineSpacing;
    std::string timeText = "Time: " + getCurrentTimestamp();
    cv::putText(frame, timeText,
                cv::Point(modalX + 30, textY),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(100, 100, 100), 1);
}

int main(int argc, char *argv[])
{
    cv::VideoCapture cam;
    std::string cameraSource;

    // ============================================
    // KONFIGURASI IP CAMERA
    // Ganti URL di bawah dengan URL IP Camera dari HP Anda
    // ============================================
    std::string ipCameraURL = "http://10.237.86.79:8080/video";

    // Jika ada argument, gunakan sebagai path camera
    if (argc > 1)
    {
        cameraSource = argv[1];
        std::cout << "Mencoba membuka kamera dari path: " << cameraSource << std::endl;

        // Cek apakah input adalah URL (IP Camera)
        if (cameraSource.find("http://") != std::string::npos ||
            cameraSource.find("https://") != std::string::npos ||
            cameraSource.find("rtsp://") != std::string::npos)
        {
            cam.open(cameraSource, cv::CAP_FFMPEG);
        }
        else if (cameraSource.find("/dev/video") != std::string::npos ||
                 cameraSource.find("/dev/") != std::string::npos)
        {
            cam.open(cameraSource, cv::CAP_V4L2);
        }
        else
        {
            try
            {
                int cameraIndex = std::stoi(cameraSource);
                cam.open(cameraIndex);
                cameraSource = "Camera Index: " + cameraSource;
            }
            catch (...)
            {
                std::cout << "Error: Format input tidak valid!" << std::endl;
                std::cout << "Contoh: ./aruco_detection http://192.168.1.100:8080/video" << std::endl;
                std::cout << "Contoh: ./aruco_detection 0" << std::endl;
                return -1;
            }
        }
    }
    else
    {
        std::cout << "==================================" << std::endl;
        std::cout << "Menggunakan IP Camera dari HP" << std::endl;
        std::cout << "URL: " << ipCameraURL << std::endl;
        std::cout << "==================================" << std::endl;

        cam.open(ipCameraURL, cv::CAP_FFMPEG);
        cameraSource = "IP Camera: " + ipCameraURL;
    }

    if (!cam.isOpened())
    {
        std::cout << "Error: Tidak dapat membuka kamera dari: " << cameraSource << std::endl;
        return -1;
    }

    std::cout << "✓ Kamera berhasil dibuka: " << cameraSource << std::endl;

    // ============================================
    // SETUP ArUco DETECTOR
    // ============================================
    // Pilih dictionary ArUco (5x5 dengan 50 markers berbeda)
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_50);

    // Buat detector parameters (default settings)
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();

    std::cout << "\n==================================" << std::endl;
    std::cout << "ArUco Marker Detection System" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Dictionary: DICT_5X5_50" << std::endl;
    std::cout << "Markers: ID 0-49 tersedia" << std::endl;
    std::cout << "\nKontrol:" << std::endl;
    std::cout << "  'q' atau '0' = Keluar" << std::endl;
    std::cout << "  's' = Screenshot" << std::endl;
    std::cout << "  'g' = Generate ArUco markers" << std::endl;
    std::cout << "  'l' = Lihat log file" << std::endl;
    std::cout << "==================================" << std::endl;

    // Setup logging
    std::string logFilename = "aruco_detection_log.txt";
    std::ofstream logFile(logFilename, std::ios::app);
    if (logFile.is_open())
    {
        logFile << "\n\n";
        logFile << "####################################################\n";
        logFile << "# NEW SESSION STARTED\n";
        logFile << "# Date/Time: " << getCurrentTimestamp() << "\n";
        logFile << "# Camera: " << cameraSource << "\n";
        logFile << "####################################################\n\n";
        logFile.close();
    }
    std::cout << "✓ Log file: " << logFilename << std::endl;
    std::cout << "==================================" << std::endl;

    cv::Mat frame;
    int frameCount = 0;

    // Tracking untuk deteksi baru (agar tidak spam log)
    std::map<int, time_t> lastDetectionTime;
    int detectionCooldown = 3; // Detik sebelum log ulang untuk marker yang sama

    // Untuk modal notification
    std::map<int, int> modalDisplayFrames; // Berapa frame lagi modal ditampilkan
    int modalDuration = 90;                // Frame (sekitar 3 detik di 30fps)

    while (true)
    {
        cam >> frame;

        if (frame.empty())
        {
            std::cout << "Error: Frame kosong!" << std::endl;
            break;
        }

        frameCount++;

        // Variabel untuk menyimpan hasil deteksi
        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;

        // Deteksi ArUco markers
        cv::aruco::detectMarkers(frame, dictionary, markerCorners, markerIds, detectorParams, rejectedCandidates);

        // Jika ada marker terdeteksi
        if (!markerIds.empty())
        {
            // Gambar marker yang terdeteksi (outline + ID)
            cv::aruco::drawDetectedMarkers(frame, markerCorners, markerIds);

            // Set untuk tracking marker yang masih terlihat di frame ini
            std::set<int> currentFrameMarkers;

            // Tampilkan informasi detail untuk setiap marker
            for (size_t i = 0; i < markerIds.size(); i++)
            {
                int markerId = markerIds[i];
                currentFrameMarkers.insert(markerId);

                // Hitung center point dari marker
                cv::Point2f center(0, 0);
                for (int j = 0; j < 4; j++)
                {
                    center.x += markerCorners[i][j].x;
                    center.y += markerCorners[i][j].y;
                }
                center.x /= 4.0;
                center.y /= 4.0;

                // Hitung ukuran marker (diagonal)
                float dx = markerCorners[i][0].x - markerCorners[i][2].x;
                float dy = markerCorners[i][0].y - markerCorners[i][2].y;
                float size = std::sqrt(dx * dx + dy * dy);

                // Cek apakah ini deteksi baru (atau sudah lewat cooldown)
                time_t currentTime = time(0);
                bool isNewDetection = false;

                if (lastDetectionTime.find(markerId) == lastDetectionTime.end())
                {
                    // Pertama kali terdeteksi
                    isNewDetection = true;
                }
                else if (difftime(currentTime, lastDetectionTime[markerId]) >= detectionCooldown)
                {
                    // Sudah lewat cooldown, anggap deteksi baru
                    isNewDetection = true;
                }

                if (isNewDetection)
                {
                    // LOG ke file
                    logArucoDetection(markerId, center, size, logFilename);

                    // Update waktu deteksi terakhir
                    lastDetectionTime[markerId] = currentTime;

                    // Aktifkan modal notification
                    modalDisplayFrames[markerId] = modalDuration;

                    // Console log
                    std::cout << "\n🎯 NEW DETECTION!" << std::endl;
                    std::cout << "   Marker ID: " << markerId << std::endl;
                    std::cout << "   Position: (" << (int)center.x << ", " << (int)center.y << ")" << std::endl;
                    std::cout << "   Size: " << (int)size << " px" << std::endl;
                    std::cout << "   Time: " << getCurrentTimestamp() << std::endl;
                    std::cout << "   ✓ Logged to file: " << logFilename << std::endl;
                }

                // Tampilkan info di atas marker
                std::string info = "ID: " + std::to_string(markerId) +
                                   " | Size: " + std::to_string((int)size) + "px";

                cv::putText(frame, info,
                            cv::Point(center.x - 60, center.y - 30),
                            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);

                // Gambar titik center
                cv::circle(frame, center, 5, cv::Scalar(0, 0, 255), -1);
            }

            // Tampilkan status di layar
            std::string status = "ArUco Terdeteksi: " + std::to_string(markerIds.size()) + " marker(s)";
            cv::putText(frame, status, cv::Point(10, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
        }
        else
        {
            // Tidak ada marker terdeteksi
            cv::putText(frame, "Tidak ada ArUco terdeteksi", cv::Point(10, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
        }

        // Tampilkan modal notifications untuk marker yang baru terdeteksi
        for (auto it = modalDisplayFrames.begin(); it != modalDisplayFrames.end();)
        {
            int markerId = it->first;
            int framesLeft = it->second;

            if (framesLeft > 0)
            {
                // Dapatkan info marker untuk modal
                // Cari marker di hasil deteksi saat ini
                for (size_t i = 0; i < markerIds.size(); i++)
                {
                    if (markerIds[i] == markerId)
                    {
                        // Hitung center dan size
                        cv::Point2f center(0, 0);
                        for (int j = 0; j < 4; j++)
                        {
                            center.x += markerCorners[i][j].x;
                            center.y += markerCorners[i][j].y;
                        }
                        center.x /= 4.0;
                        center.y /= 4.0;

                        float dx = markerCorners[i][0].x - markerCorners[i][2].x;
                        float dy = markerCorners[i][0].y - markerCorners[i][2].y;
                        float size = std::sqrt(dx * dx + dy * dy);

                        // Tampilkan modal
                        drawNotificationModal(frame, markerId, center, size, framesLeft);
                        break;
                    }
                }

                // Kurangi counter
                it->second--;
                ++it;
            }
            else
            {
                // Hapus dari map jika sudah habis
                it = modalDisplayFrames.erase(it);
            }
        }

        // Tampilkan informasi tambahan
        std::string info = "Dictionary: 4x4_50 | IDs: 0-49";
        cv::putText(frame, info, cv::Point(10, frame.rows - 15),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);

        // Tampilkan hasil
        cv::imshow("ArUco Marker Detection", frame);

        // Handle keyboard input
        int key = cv::waitKey(1);

        if (key == 48 || key == 'q' || key == 'Q')
        {
            // Keluar
            std::cout << "\nKeluar dari program..." << std::endl;
            break;
        }
        else if (key == 's' || key == 'S')
        {
            // Screenshot
            std::string filename = "aruco_screenshot_" + getFilenameTimestamp() + ".jpg";
            cv::imwrite(filename, frame);
            std::cout << "✓ Screenshot disimpan: " << filename << std::endl;
        }
        else if (key == 'l' || key == 'L')
        {
            // Tampilkan log file
            std::cout << "\n==================================" << std::endl;
            std::cout << "DETECTION LOG SUMMARY" << std::endl;
            std::cout << "==================================" << std::endl;
            std::cout << "Log file: " << logFilename << std::endl;
            std::cout << "Total markers tracked: " << lastDetectionTime.size() << std::endl;
            std::cout << "\nMarkers detected in this session:" << std::endl;
            for (const auto &pair : lastDetectionTime)
            {
                std::cout << "  - Marker ID " << pair.first
                          << " (last seen: " << ctime(&pair.second) << ")" << std::endl;
            }
            std::cout << "\nBuka file '" << logFilename << "' untuk detail lengkap" << std::endl;
            std::cout << "==================================" << std::endl;
        }
        else if (key == 'g' || key == 'G')
        {
            // Generate ArUco markers
            std::cout << "\n==================================" << std::endl;
            std::cout << "Generating ArUco Markers..." << std::endl;
            std::cout << "==================================" << std::endl;

            int markerSize = 200; // Ukuran marker dalam pixel
            int numMarkers = 10;  // Generate 10 markers pertama (ID 0-9)

            for (int id = 0; id < numMarkers; id++)
            {
                cv::Mat markerImage;
                cv::aruco::drawMarker(dictionary, id, markerSize, markerImage, 1);

                std::string filename = "aruco_marker_" + std::to_string(id) + ".png";
                cv::imwrite(filename, markerImage);
                std::cout << "✓ Generated: " << filename << std::endl;
            }
            std::cout << "\n✓ Selesai! " << numMarkers << " markers telah di-generate." << std::endl;
            std::cout << "Print markers tersebut dan gunakan untuk testing." << std::endl;
            std::cout << "==================================" << std::endl;
        }
    }

    cam.release();
    cv::destroyAllWindows();

    std::cout << "\n✓ Program selesai. Terima kasih!" << std::endl;
    return 0;
}
