#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
using namespace cv;
using namespace std;

int main()
{
    cv::VideoCapture cap("face.mp4");

    if (!cap.isOpened()) {
        std::cerr << "ОШИБКА: Не удалось открыть видеофайл 'face.mp4'!" << std::endl;
        system("pause");
        return -1;
    }
    int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);
    VideoWriter out("face1.mkv", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(frame_width, frame_height));
    if (!out.isOpened()) {
        cerr << "Ошибка открытия VideoWriter\n";
        return -1;
    }
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyeCascade;
    cv::CascadeClassifier smileCascade;

    faceCascade.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml");
    eyeCascade.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml");
    smileCascade.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_smile.xml");

    cv::Mat frame, gray;

    while (true) {
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(gray, faces, 1.3, 5, 0, cv::Size(100, 100));
        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);


            cv::Mat faceROI = gray(face);

            std::vector<cv::Rect> eyes;
            eyeCascade.detectMultiScale(faceROI, eyes, 1.3, 6, 0, cv::Size(40, 40));

            for (const auto& eye : eyes) {
                cv::Rect eyeOnFrame(face.x + eye.x, face.y + eye.y, eye.width, eye.height);
                cv::rectangle(frame, eyeOnFrame, cv::Scalar(0, 0, 255), 2);
            }

            cv::Rect smileROIrect(0, face.height * 0.6, face.width, face.height * 0.4);
            cv::Mat smileROI = faceROI(smileROIrect);

            std::vector<cv::Rect> smiles;
            smileCascade.detectMultiScale(smileROI, smiles, 1.2, 20, 0, cv::Size(40, 20));

            for (const auto& smile : smiles) {
                cv::Rect smileOnFrame(face.x + smileROIrect.x + smile.x,
                    face.y + smileROIrect.y + smile.y,
                    smile.width, smile.height);
                cv::rectangle(frame, smileOnFrame, cv::Scalar(255, 0, 0), 2);
            }
        }
        out.write(frame);
        cv::imshow("Детекция лиц, глаз и улыбок", frame);

        if (cv::waitKey(30) == 27) {
            break;
        }
    }
    out.release();
    cap.release();
    cv::destroyAllWindows();

    return 0;
}