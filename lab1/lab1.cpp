#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

printImage(Mat& image, string name)
{
    imshow(name, image);
    string link = name + ".jpg";
    imwrite(link, image);
}
int main() 
{
    Mat image = cv::imread("test.jpg");
    if (image.empty())
    {
        cout << "Ошибка: не удалось загрузить изображение!" << endl;
        return -1;
    }
    Point2f center(image.cols / 2.0, image.rows / 2.0);
    Mat gray, resize1, resize2, rotated1, rotated2, blur, canny;

    cvtColor(image, gray, COLOR_BGR2GRAY);

    resize(image, resize1, Size(), 0.5, 0.5);
    resize(image, resize2, Size(), 2, 2);

    Mat side = getRotationMatrix2D(center, 90, 1.0);
    Mat back = getRotationMatrix2D(center, 180, 1.0);
    warpAffine(image, rotated1, side, image.size());
    warpAffine(image, rotated2, back, image.size());

    GaussianBlur(image, blur, cv::Size(9, 9), 10);

    Canny(gray, canny, 50, 150);

    imshow("Исходная крепость", image);
    printImage(gray, "Чёрно-белая");
    printImage(resize1, "Маленькая");
    printImage(resize2, "Большая");
    printImage(rotated1, "Боковая");
    printImage(rotated2, "Перевернутая");
    printImage(blur, "Гладкая");
    printImage(canny, "Граничная");

    waitKey(0);
    destroyAllWindows();

    Mat binary, eroded, dilated;
    threshold(gray, binary, 127, 255, cv::THRESH_BINARY);
    Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    erode(binary, eroded, kernel);
    dilate(binary, dilated, kernel);
    printImage(binary, "Двоичная");
    printImage(eroded, "Двоично-черная");
    printImage(dilated, "Двоичная-белая");
    waitKey(0);
    destroyAllWindows();
    return 0;
}