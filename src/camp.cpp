#include <opencv4/opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::VideoCapture cam(0);
    if (!cam.isOpened())
    {
        std::cerr << "Error: Tidak dapat membuka kamera." << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true)
    {
        cam >> frame;
        if (frame.empty())
        {
            std::cerr << "Error: Gambar kosong." << std::endl;
            break;
        }

        cv::imshow("Kamera", frame);
        if (cv::waitKey(30) >= 0)
            break;
    }

    return 0;
}