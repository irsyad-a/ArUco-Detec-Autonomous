#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#include <ctime>

int main(int argc, char *argv[])
{
    cv::VideoCapture cam;
    std::string cameraSource;

    // Jika ada argument, gunakan sebagai path camera
    if (argc > 1)
    {
        cameraSource = argv[1];
        std::cout << "Mencoba membuka kamera dari path: " << cameraSource << std::endl;

        // Cek apakah input adalah angka (camera index) atau path
        if (cameraSource.find("/dev/video") != std::string::npos ||
            cameraSource.find("/dev/") != std::string::npos)
        {
            // Path langsung ke device camera (Linux)
            cam.open(cameraSource, cv::CAP_V4L2);
        }
        else
        {
            // Coba convert ke integer untuk camera index
            try
            {
                int cameraIndex = std::stoi(cameraSource);
                cam.open(cameraIndex);
                cameraSource = "Camera Index: " + cameraSource;
            }
            catch (...)
            {
                std::cout << "Error: Format input tidak valid!" << std::endl;
                std::cout << "Gunakan: ./face_blur_detection [camera_index] atau ./face_blur_detection [path]" << std::endl;
                return -1;
            }
        }
    }
    else
    {
        // Default: gunakan camera index 0
        std::cout << "Menggunakan kamera default (index 0)" << std::endl;
        cam.open(0);
        cameraSource = "Camera Index: 0";
    }

    if (!cam.isOpened())
    {
        std::cout << "Error: Tidak dapat membuka kamera dari: " << cameraSource << std::endl;
        return -1;
    }

    std::cout << "✓ Kamera berhasil dibuka: " << cameraSource << std::endl;

    // Test baca frame dulu untuk memastikan kamera benar-benar ready
    cv::Mat test_frame;
    cam >> test_frame;
    if (test_frame.empty())
    {
        std::cout << "Error: Kamera tidak dapat menghasilkan frame!" << std::endl;
        std::cout << "Coba gunakan kamera lain atau restart sistem." << std::endl;
        return -1;
    }
    std::cout << "✓ Test frame berhasil: " << test_frame.cols << "x" << test_frame.rows << std::endl;

    // Set resolusi kamera lebih besar (HD) - dengan fallback
    cam.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cam.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

        int actual_width = cam.get(cv::CAP_PROP_FRAME_WIDTH);
    int actual_height = cam.get(cv::CAP_PROP_FRAME_HEIGHT);

    if (actual_width == 0 || actual_height == 0)
    {
        std::cout << "⚠ Warning: Tidak dapat set resolusi HD, menggunakan default kamera" << std::endl;

        cam.release();
        if (argc > 1)
        {
            if (cameraSource.find("/dev/video") != std::string::npos)
                cam.open(cameraSource, cv::CAP_V4L2);
            else
                cam.open(std::stoi(argv[1]));
        }
        else
        {
            cam.open(0);
        }
        actual_width = cam.get(cv::CAP_PROP_FRAME_WIDTH);
        actual_height = cam.get(cv::CAP_PROP_FRAME_HEIGHT);
    }

    std::cout << "✓ Resolusi kamera: " << actual_width << "x" << actual_height << std::endl;

    // Load Haar Cascade untuk deteksi wajah (AI/ML Model)
    cv::CascadeClassifier face_cascade;
    std::string cascade_path = "/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml";

    if (!face_cascade.load(cascade_path))
    {
        std::cout << "Error: Tidak dapat load Haar Cascade model!" << std::endl;
        std::cout << "Coba path alternatif..." << std::endl;

        // Coba path alternatif
        cascade_path = "/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml";
        if (!face_cascade.load(cascade_path))
        {
            std::cout << "Error: Haar Cascade tidak ditemukan di sistem!" << std::endl;
            std::cout << "Install dengan: sudo apt install opencv-data" << std::endl;
            return -1;
        }
    }

    std::cout << "✓ Haar Cascade model berhasil dimuat" << std::endl;

    cv::Mat frame, hsv, mask, blur_hsv;

    cv::Scalar lower_black(0, 0, 0);
    cv::Scalar upper_black(180, 255, 40);

    // Buat window dengan ukuran yang dapat diatur
    cv::namedWindow("Face Blur + Black Object Detection", cv::WINDOW_NORMAL);
    cv::namedWindow("Mask - Deteksi Hitam", cv::WINDOW_NORMAL);

    // Set ukuran window lebih besar (bisa disesuaikan)
    cv::resizeWindow("Face Blur + Black Object Detection", 1280, 720);
    cv::resizeWindow("Mask - Deteksi Hitam", 640, 480);

    std::cout << "\nKontrol:" << std::endl;
    std::cout << "  Tekan 'q' atau ESC untuk keluar" << std::endl;
    std::cout << "  Tekan 's' untuk screenshot" << std::endl;
    std::cout << "  Tekan 'b' untuk toggle blur wajah" << std::endl;
    std::cout << "  Tekan 'f' untuk toggle fullscreen" << std::endl;

    bool blurFaces = true;
    bool isFullscreen = false;
    int frameCount = 0;
    time_t startTime = time(0);
    double fps = 0.0;

    while (true)
    {
        auto frame_start = cv::getTickCount();

        cam >> frame;

        if (frame.empty())
        {
            std::cout << "Error: Frame kosong!" << std::endl;
            break;
        }

        frameCount++;

        // ========== DETEKSI WAJAH DAN BLUR (AI/ML) ==========
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray); // Meningkatkan kontras untuk deteksi lebih baik

        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 5, 0, cv::Size(30, 30));

        int faceCount = 0;
        if (blurFaces)
        {
            for (size_t i = 0; i < faces.size(); i++)
            {
                faceCount++;
                // Perbesar area blur sedikit untuk coverage lebih baik
                int padding = 20;
                cv::Rect expandedFace(
                    std::max(0, faces[i].x - padding),
                    std::max(0, faces[i].y - padding),
                    std::min(frame.cols - faces[i].x + padding, faces[i].width + 2 * padding),
                    std::min(frame.rows - faces[i].y + padding, faces[i].height + 2 * padding));

                // Aplikasikan Gaussian Blur yang kuat pada area wajah
                cv::Mat faceROI = frame(expandedFace);
                cv::GaussianBlur(faceROI, faceROI, cv::Size(99, 99), 30);

                // Opsional: Gambar kotak di sekitar wajah yang di-blur
                cv::rectangle(frame, expandedFace, cv::Scalar(255, 0, 0), 2);
            }
        }

        // ========== DETEKSI OBJEK HITAM ==========
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
        cv::GaussianBlur(hsv, blur_hsv, cv::Size(5, 5), 0);
        cv::inRange(blur_hsv, lower_black, upper_black, mask);

        // Morphological operations
        cv::Mat kernel_open = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
        cv::Mat kernel_close = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9));
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel_open, cv::Point(-1, -1), 2);
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel_close, cv::Point(-1, -1), 2);

        // Cari kontur objek hitam
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int blackObjectCount = 0;
        double totalBlackArea = 0;

        for (size_t i = 0; i < contours.size(); i++)
        {
            double area = cv::contourArea(contours[i]);
            double perimeter = cv::arcLength(contours[i], true);

            if (area > 1200 && perimeter > 100)
            {
                double circularity = 4 * CV_PI * area / (perimeter * perimeter);
                cv::Rect boundingBox = cv::boundingRect(contours[i]);
                double aspectRatio = (double)boundingBox.width / boundingBox.height;

                if (circularity > 0.25 && aspectRatio > 0.3 && aspectRatio < 3.0)
                {
                    blackObjectCount++;
                    totalBlackArea += area;

                    // Gambar kotak hijau di sekitar objek hitam
                    cv::rectangle(frame, boundingBox, cv::Scalar(0, 255, 0), 2);

                    std::string label = "Objek #" + std::to_string(blackObjectCount);
                    cv::putText(frame, label,
                                cv::Point(boundingBox.x, boundingBox.y - 25),
                                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);

                    std::string info = "Area: " + std::to_string((int)area);
                    cv::putText(frame, info,
                                cv::Point(boundingBox.x, boundingBox.y - 5),
                                cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
                }
            }
        }

        // Hitung FPS
        auto frame_end = cv::getTickCount();
        fps = cv::getTickFrequency() / (frame_end - frame_start);

        // ========== PANEL INFORMASI DI POJOK KIRI ATAS ==========
        // Background semi-transparan untuk info panel
        cv::Mat overlay = frame.clone();
        cv::rectangle(overlay, cv::Point(5, 5), cv::Point(400, 230), cv::Scalar(0, 0, 0), -1);
        cv::addWeighted(overlay, 0.6, frame, 0.4, 0, frame);

        // Informasi Detail
        int yPos = 30;
        int lineHeight = 25;

        // Header
        cv::putText(frame, "=== SYSTEM INFO ===",
                    cv::Point(15, yPos), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 255), 2);
        yPos += lineHeight;

        // FPS
        std::string fpsText = "FPS: " + std::to_string((int)fps);
        cv::putText(frame, fpsText,
                    cv::Point(15, yPos), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
        yPos += lineHeight;

        // Resolution
        std::string resText = "Resolusi: " + std::to_string(frame.cols) + "x" + std::to_string(frame.rows);
        cv::putText(frame, resText,
                    cv::Point(15, yPos), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
        yPos += lineHeight;

        // Frame count
        std::string frameText = "Frame: " + std::to_string(frameCount);
        cv::putText(frame, frameText,
                    cv::Point(15, yPos), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
        yPos += lineHeight;

        // Wajah terdeteksi
        std::string faceText = "Wajah Terdeteksi: " + std::to_string(faceCount);
        cv::Scalar faceColor = faceCount > 0 ? cv::Scalar(0, 255, 255) : cv::Scalar(150, 150, 150);
        cv::putText(frame, faceText,
                    cv::Point(15, yPos), cv::FONT_HERSHEY_SIMPLEX, 0.5, faceColor, 1);
        yPos += lineHeight;

        // Status blur
        std::string blurStatus = "Blur Wajah: " + std::string(blurFaces ? "ON" : "OFF");
        cv::Scalar blurColor = blurFaces ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);
        cv::putText(frame, blurStatus,
                    cv::Point(15, yPos), cv::FONT_HERSHEY_SIMPLEX, 0.5, blurColor, 1);
        yPos += lineHeight;

        // Objek hitam terdeteksi
        std::string blackText = "Objek Hitam: " + std::to_string(blackObjectCount);
        cv::Scalar blackColor = blackObjectCount > 0 ? cv::Scalar(0, 255, 0) : cv::Scalar(150, 150, 150);
        cv::putText(frame, blackText,
                    cv::Point(15, yPos), cv::FONT_HERSHEY_SIMPLEX, 0.5, blackColor, 1);
        yPos += lineHeight;

        // Total area objek hitam
        if (blackObjectCount > 0)
        {
            std::string areaText = "Total Area: " + std::to_string((int)totalBlackArea) + " px";
            cv::putText(frame, areaText,
                        cv::Point(15, yPos), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(200, 200, 200), 1);
        }

        // Tampilkan hasil
        cv::imshow("Face Blur + Black Object Detection", frame);
        cv::imshow("Mask - Deteksi Hitam", mask);

        // Kontrol keyboard
        int key = cv::waitKey(1);
        if (key == 'q' || key == 27) // 'q' atau ESC
        {
            break;
        }
        else if (key == 's') // Screenshot
        {
            std::string filename = "screenshot_" + std::to_string(time(0)) + ".jpg";
            cv::imwrite(filename, frame);
            std::cout << "Screenshot disimpan: " << filename << std::endl;
        }
        else if (key == 'b') // Toggle blur
        {
            blurFaces = !blurFaces;
            std::cout << "Blur wajah: " << (blurFaces ? "ON" : "OFF") << std::endl;
        }
        else if (key == 'f') // Toggle fullscreen
        {
            isFullscreen = !isFullscreen;
            if (isFullscreen)
            {
                cv::setWindowProperty("Face Blur + Black Object Detection",
                                      cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
                std::cout << "Fullscreen: ON" << std::endl;
            }
            else
            {
                cv::setWindowProperty("Face Blur + Black Object Detection",
                                      cv::WND_PROP_FULLSCREEN, cv::WINDOW_NORMAL);
                std::cout << "Fullscreen: OFF" << std::endl;
            }
        }
    }

    // Statistik akhir
    double elapsedTime = difftime(time(0), startTime);
    double avgFPS = frameCount / elapsedTime;

    std::cout << "\n=== STATISTIK SESSION ===" << std::endl;
    std::cout << "Total Frame: " << frameCount << std::endl;
    std::cout << "Durasi: " << (int)elapsedTime << " detik" << std::endl;
    std::cout << "Rata-rata FPS: " << (int)avgFPS << std::endl;

    cam.release();
    cv::destroyAllWindows();

    return 0;
}
