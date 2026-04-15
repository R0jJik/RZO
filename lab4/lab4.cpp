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
int main() {
    Mat img = imread("image.png");

    if (img.empty()) {
        cout << "Îøèáêà: íå óäàëîñü çàãðóçèòü èçîáðàæåíèå!" << endl;
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
            vector<double> sides;
            for (size_t i = 0; i < approx.size(); i++) {
                Point p1 = approx[i];
                Point p2 = approx[(i + 1) % approx.size()];
                double side = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
                sides.push_back(side);
            }

            double avgSide = (sides[0] + sides[1] + sides[2] + sides[3]) / 4;
            bool sidesEqual = true;
            for (double side : sides) {
                if (abs(side - avgSide) > avgSide * 0.1) {
                    sidesEqual = false;
                    break;
                }
            }

            RotatedRect minRect = minAreaRect(approx);
            double rectArea = minRect.size.width * minRect.size.height;
            double areaRatio = area / rectArea;

            bool isRectangular = (areaRatio > 0.85); 
            if (sidesEqual && isRectangular) {
                shapeName = "Square";
            }
            else if (sidesEqual && !isRectangular) {
                shapeName = "Rhombus";
            }
            else if (!sidesEqual && isRectangular) {
                shapeName = "Rectangle";
            }
            else {
                shapeName = "Quadrilateral";
            }
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
                shapeName = "Error";
            }
        }

        drawContours(result, vector<vector<Point>>{contour},
            -1, Scalar(0, 255, 0), -1);
        string text = shapeName + " P=" + to_string((int)perimeter);
        putText(result, text, Point(center.x - 40, center.y + 5),
            FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 2);
    }
    printImage(result, "Èòîã");
    waitKey(0);
    return 0;
}
