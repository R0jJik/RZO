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
int main {
    Mat img = imread("image.png");

    if (img.empty()) {
        cout << "Ошибка: не удалось загрузить изображение!" << endl;
        return 0;
    }
    Mat result;
    result = img.clone();


    Mat gray;
    cvtColor(result, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, Size(5, 5), 0);

    Mat binary;
    threshold(gray, binary, 127, 255, THRESH_BINARY_INV);

    vector<vector<Point>> contours;
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        double perimeter = arcLength(contour, true);
        double area = contourArea(contour);

        Moments m = moments(contour);
        Point2f center(m.m10 / m.m00, m.m01 / m.m00);

        vector<Point> approx;
        double epsilon = 0.02 * perimeter;
        approxPolyDP(contour, approx, epsilon, true);

        string shapeName;

        if (approx.size() == 3) {
            shapeName = "Triangle";
        }
        else if (approx.size() == 4) {
            shapeName = "Rectangle";
        }
        else if (approx.size() == 5) {
            shapeName = "Pentagon";
        }
        else if (approx.size() == 6) {
            shapeName = "Hexagon";
        }
        else {
            double circularity = 4 * CV_PI * area / (perimeter * perimeter);
            if (circularity > 0.88) {
                shapeName = "Circle";
            }
            else if (circularity > 0.7) {
                shapeName = "Oval";
            }
            else {
                shapeName = "ERR";
            }
        }

        drawContours(result, vector<vector<Point>>{contour},
            -1, Scalar(0, 255, 0), -1);
        string text = shapeName + " P=" + to_string((int)perimeter);
        putText(result, text, Point(center.x - 40, center.y + 5),
            FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 2);
    }
    printImage(result, "Итог");
    waitKey(0);
    return 0;
}