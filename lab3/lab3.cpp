#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>


printImage(Mat& image, string name)
{
    imshow(name, image);
    string link = name + ".jpg";
    imwrite(link, image);
}

int main()
{
    Mat image = cv::imread("test.png");
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);
    vector<Mat> channels;
    Mat Mask,Mask1, Mask2;
    inRange(hsv, Scalar(0, 100, 100), Scalar(20, 255, 255), Mask1);
    inRange(hsv, Scalar(160, 100, 100), Scalar(180, 255, 255), Mask2);
    Mask = Mask1 | Mask2;
    printImage(image, "Исходник");
    printImage(Mask1,"Кусок_1");
    printImage(Mask2, "Кусок_2");
    printImage(Mask, "Розовый2");
    waitKey(0);
    return 0;
}
