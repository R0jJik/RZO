#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
const double PI = 3.14159265358979323846;

printImage(Mat& image, string name)
{
    imshow(name, image);
    string link =name + ".jpg";
    imwrite(link, image);
}

vector<Point> creatFigure(Point center, int size, int startAng, int countAngels)
{
    vector<cv::Point> points;
    double startAngle = startAng * PI / 180.0;
    double angleStep = 2 * PI / countAngels;
    for (int i = 0; i < countAngels; i++) {
        double angle = startAngle + i * angleStep;
        int x = center.x + static_cast<int>(size * cos(angle));
        int y = center.y + static_cast<int>(size * sin(angle));
            points.push_back(cv::Point(x, y));             
    }  
        return points;
}

int main()
{
    setlocale(LC_ALL, "ru");
    Mat canvas(600, 1000, CV_8UC3, Scalar(60, 0, 40));
    for (size_t i = 1; i < 10; i++)
    {
        for (size_t j = 1; j < 10; j++)
        {
            int x = 1000 - i * 100;
            int y = 600 - j * 60;
            polylines(canvas, creatFigure(Point(x,y), 10, i+j*5, 3), true, Scalar(255, 255, 255), 1);
        }
    }
    
    line(canvas, Point(0, 550), Point(999, 550), Scalar(255, 0, 0), 6);
    rectangle(canvas, Point(0, 550), Point(520, 599), Scalar(0, 0, 255), 2);
    ellipse(canvas, Point(500, 300), Size(200, 70), 30, 0, 360, Scalar(157, 202, 216), 5);
    ellipse(canvas, Point(500, 300), Size(250, 87), 30, 0, 360, Scalar(246, 243, 235), 20);
    ellipse(canvas, Point(500, 300), Size(300, 135), 30, 0, 360, Scalar(134, 145, 134), 40);
    circle(canvas, Point(500, 300), 150, Scalar(57, 144, 201), -1);
    ellipse(canvas, Point(500, 300), Size(200, 70), 30, 0, 200, Scalar(157, 202, 216), 5);
    ellipse(canvas, Point(500, 300), Size(250, 87), 30, 0, 200, Scalar(246, 243, 235), 20);
    ellipse(canvas, Point(500, 300), Size(300, 135), 30, 0, 200, Scalar(134, 145, 134), 40);

    string text = "Palchuch D.A. Group 22-AM-1";
    int textWidth = text.length() * 18;
    int direction = 1;
    int xPos = 20; 
    int yPos = 580;
    int speed = 6;
    namedWindow("Анимация");
    while (true) {
        Mat frame = canvas.clone();
        putText(frame, text, Point(xPos, yPos), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255, 255), 2);
        xPos += direction * speed;
        if (xPos > 1000 - textWidth) {
            direction = -1;
        }
        if (xPos < 20) {
            direction = 1;
        }
        imshow("Анимация", frame);
        int key = waitKey(30);
        if (key == 27) { break; }
    }
    putText(canvas, text, Point(20, 580), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255, 255), 2);
    printImage(canvas, "Итог");
    waitKey(0);
    destroyAllWindows();
    return 0;
}
