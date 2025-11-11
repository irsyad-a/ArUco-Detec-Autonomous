// #include <opencv2/opencv.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/highgui.hpp>
// #include <iostream>

// cv::Mat Contrast(cv::Mat &img, float alpha)
// {
//     cv::Mat result;
//     img.convertTo(result, -1, alpha, 128 * (1 - alpha));
//     return result;
// }

// int main()
// {
//     cv::Mat src = cv::imread("../src/alam.jpg");

//     float kontras = -10;

//     cv::Mat result = Contrast(src, kontras);

//     cv::imshow("Original", src);
//     cv::imshow("Kontras", result);

//     cv::waitKey(0);

//     return 0;
// }

#include <opencv4/opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    cv::VideoCapture cam;
    std::string cameraSource;

    // ============================================
    // KONFIGURASI IP CAMERA
    // Ganti URL di bawah dengan URL IP Camera dari HP Anda
    // Contoh: "http://192.168.1.100:8080/video"
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
            // URL IP Camera atau RTSP stream
            cam.open(cameraSource, cv::CAP_FFMPEG);
        }
        // Cek apakah input adalah path device
        else if (cameraSource.find("/dev/video") != std::string::npos ||
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
                std::cout << "Gunakan: ./cobaAja_kj [URL/camera_index/path]" << std::endl;
                std::cout << "Contoh: ./cobaAja_kj http://192.168.1.100:8080/video" << std::endl;
                std::cout << "Contoh: ./cobaAja_kj 0" << std::endl;
                std::cout << "Contoh: ./cobaAja_kj /dev/video0" << std::endl;
                return -1;
            }
        }
    }
    else
    {
        // Default: gunakan IP Camera dari HP
        std::cout << "==================================" << std::endl;
        std::cout << "Menggunakan IP Camera dari HP" << std::endl;
        std::cout << "URL: " << ipCameraURL << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << "\nTips:" << std::endl;
        std::cout << "1. Pastikan HP dan komputer terhubung ke WiFi yang sama" << std::endl;
        std::cout << "2. Buka aplikasi IP Camera di HP Anda" << std::endl;
        std::cout << "3. Catat URL yang ditampilkan di aplikasi" << std::endl;
        std::cout << "4. Edit file cobaa.cpp, ubah variabel ipCameraURL" << std::endl;
        std::cout << "\nUntuk menggunakan kamera lain:" << std::endl;
        std::cout << "  ./cobaAja_kj [URL/camera_index/path]" << std::endl;
        std::cout << "==================================" << std::endl;

        cam.open(ipCameraURL, cv::CAP_FFMPEG);
        cameraSource = "IP Camera: " + ipCameraURL;
    }

    if (!cam.isOpened())
    {
        std::cout << "Error: Tidak dapat membuka kamera dari: " << cameraSource << std::endl;
        std::cout << "\nCoba periksa kamera yang tersedia:" << std::endl;
        std::cout << "  - ls /dev/video*" << std::endl;
        std::cout << "  - v4l2-ctl --list-devices" << std::endl;
        return -1;
    }

    std::cout << "✓ Kamera berhasil dibuka: " << cameraSource << std::endl;

    cv::Mat frame, hsv, mask, blur;

    // Range warna hitam dalam HSV - KETAT untuk objek hitam murni
    // Value 0-40: Hanya menangkap warna yang sangat gelap (hitam murni)
    // Saturation 0-255: Mencakup hitam dengan berbagai tingkat saturasi
    cv::Scalar lower_black(0, 0, 0);      // Batas bawah
    cv::Scalar upper_black(180, 255, 40); // Value 40 - Sangat ketat untuk hitam pekat

    std::cout << "\nKontrol:" << std::endl;
    std::cout << "  Tekan 'q' atau '0' untuk keluar" << std::endl;
    std::cout << "  Tekan 's' untuk screenshot" << std::endl;

    while (true)
    {
        cam >> frame;

        if (frame.empty())
        {
            std::cout << "Error: Frame kosong!" << std::endl;
            break;
        }

        // Konversi dari BGR ke HSV (lebih baik untuk deteksi warna)
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // Aplikasikan Gaussian Blur untuk mengurangi noise
        cv::GaussianBlur(hsv, blur, cv::Size(5, 5), 0);

        // Deteksi warna hitam menggunakan inRange
        cv::inRange(blur, lower_black, upper_black, mask);

        // Morphological operations - KETAT untuk menghilangkan noise maksimal
        cv::Mat kernel_open = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
        cv::Mat kernel_close = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9));

        // Erode lebih kuat untuk menghilangkan noise
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel_open, cv::Point(-1, -1), 2);
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel_close, cv::Point(-1, -1), 2);

        // Cari kontur dari objek yang terdeteksi
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Gambar bounding box untuk setiap objek yang terdeteksi
        int objectCount = 0;
        for (size_t i = 0; i < contours.size(); i++)
        {
            double area = cv::contourArea(contours[i]);
            double perimeter = cv::arcLength(contours[i], true);

            // Filter objek berdasarkan area dan perimeter - KETAT
            // Area > 1200: Objek harus cukup besar untuk dianggap valid
            // Perimeter > 100: Hindari titik-titik kecil atau fragmen
            if (area > 1200 && perimeter > 100)
            {
                // Hitung circularity untuk filter bayangan yang tidak beraturan
                double circularity = 4 * CV_PI * area / (perimeter * perimeter);

                // Dapatkan bounding rectangle
                cv::Rect boundingBox = cv::boundingRect(contours[i]);

                // Hitung aspect ratio untuk filter objek yang terlalu memanjang
                double aspectRatio = (double)boundingBox.width / boundingBox.height;

                // Filter berdasarkan bentuk - KETAT untuk objek solid
                // Circularity > 0.25: Harus cukup bulat/persegi (bukan bayangan tidak beraturan)
                // Aspect ratio 0.3-3.0: Hindari objek yang terlalu memanjang atau pipih
                if (circularity > 0.25 && aspectRatio > 0.3 && aspectRatio < 3.0)
                {
                    objectCount++;

                    // Gambar kotak hijau di sekitar objek
                    cv::rectangle(frame, boundingBox, cv::Scalar(0, 255, 0), 3);

                    // Tambahkan teks dengan informasi lengkap
                    std::string label = "Objek #" + std::to_string(objectCount);
                    cv::putText(frame, label,
                                cv::Point(boundingBox.x, boundingBox.y - 25),
                                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);

                    std::string info = "Area: " + std::to_string((int)area);
                    cv::putText(frame, info,
                                cv::Point(boundingBox.x, boundingBox.y - 5),
                                cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
                }
            }
        }

        // Tampilkan jumlah objek terdeteksi
        std::string status = "Objek Hitam Terdeteksi: " + std::to_string(objectCount);
        cv::putText(frame, status, cv::Point(10, 30),
                    cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);

        // Tampilkan hasil
        cv::imshow("Kamera - Deteksi Warna Hitam", frame);
        cv::imshow("Mask", mask);

        // Tekan '0' atau 'q' untuk keluar, 's' untuk screenshot
        int key = cv::waitKey(1);
        if (key == 48 || key == 'q')
        {
            break;
        }
        else if (key == 's')
        {
            std::string filename = "screenshot_" + std::to_string(time(0)) + ".jpg";
            cv::imwrite(filename, frame);
            std::cout << "Screenshot disimpan: " << filename << std::endl;
        }
    }

    cam.release();
    cv::destroyAllWindows();

    return 0;
}