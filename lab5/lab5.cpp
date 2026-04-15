#include "lab5.h"
#include <string>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
;

int main()
{
    VideoCapture cap("video.mkv");
    if (!cap.isOpened()) {
        cerr << "Ошибка открытия видео\n";
        return -1;
    }
    int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);
    VideoWriter out("video1.mkv", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(frame_width, frame_height));
    if (!out.isOpened()) {
        cerr << "Ошибка открытия VideoWriter\n";
        return -1;
    }

    Mat frame, gray, binary;
    vector<vector<Point>> contours;

    while (cap.read(frame))
    {
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        threshold(gray, binary, 240, 255, THRESH_BINARY_INV);
        findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours) {

            double perimeter = arcLength(contour, true);
            double area = contourArea(contour);
            if (area < 50) continue;

            vector<Point> approx;
            double epsilon = 0.02 * perimeter;
            approxPolyDP(contour, approx, epsilon, true);

            string shapeName;

            if (approx.size() == 3) {
                shapeName = "Triangle";
            }
            else if (approx.size() == 4) {
                shapeName = "Square";
            }
            else {
                double circularity = 4 * CV_PI * area / (perimeter * perimeter);
                if (circularity > 0.85) {
                    shapeName = "Circle";
                }
                else {
                    continue;
                }
            }

            Rect bbox = boundingRect(contour);
            rectangle(frame, bbox, Scalar(0, 255, 0), 1);

            Point textCenter(bbox.x + bbox.width / 2 - 30, bbox.y + bbox.height / 2 + 5);
            putText(frame, shapeName, textCenter,
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1);
        }
        imshow("Фигуры", frame);
        imshow("Бинарная", binary);
        out.write(frame);
        if (waitKey(30) == 27) break;
    }
    out.release();
    destroyAllWindows();
    return 0;
}