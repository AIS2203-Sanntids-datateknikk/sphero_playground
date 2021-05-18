
#ifndef OPENCV___DETECT_FACE_HPP
#define OPENCV___DETECT_FACE_HPP

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include <iostream>

using namespace cv;

namespace {

    void detect(
            Mat &img,
            CascadeClassifier &cascade,
            CascadeClassifier &nestedCascade) {

        double t = 0;
        std::vector<Rect> faces, faces2;
        const static Scalar colors[] =
                {
                        Scalar(255, 0, 0),
                        Scalar(255, 128, 0),
                        Scalar(255, 255, 0),
                        Scalar(0, 255, 0),
                        Scalar(0, 128, 255),
                        Scalar(0, 255, 255),
                        Scalar(0, 0, 255),
                        Scalar(255, 0, 255)};
        Mat gray, smallImg;
        cvtColor(img, gray, COLOR_BGR2GRAY);
        resize(gray, smallImg, Size(), 1, 1, INTER_LINEAR_EXACT);
        equalizeHist(smallImg, smallImg);
        t = (double) getTickCount();
        cascade.detectMultiScale(smallImg, faces,
                                 1.1, 2, 0
                                                 //|CASCADE_FIND_BIGGEST_OBJECT
                                                 //|CASCADE_DO_ROUGH_SEARCH
                                                 | CASCADE_SCALE_IMAGE,
                                 Size(30, 30));
        t = (double) getTickCount() - t;
        printf("detection time = %g ms\n", t * 1000 / getTickFrequency());
        for (size_t i = 0; i < faces.size(); i++) {
            Rect r = faces[i];
            Mat smallImgROI;
            std::vector<Rect> nestedObjects;
            Point center;
            Scalar color = colors[i % 8];
            int radius;
            double aspect_ratio = (double) r.width / r.height;
            if (0.75 < aspect_ratio && aspect_ratio < 1.3) {
                center.x = cvRound((r.x + r.width * 0.5));
                center.y = cvRound((r.y + r.height * 0.5));
                radius = cvRound((r.width + r.height) * 0.25);
                circle(img, center, radius, color, 3, 8, 0);
            } else
                rectangle(img, Point(cvRound(r.x), cvRound(r.y)),
                          Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
                          color, 3, 8, 0);
            if (nestedCascade.empty())
                continue;
            smallImgROI = smallImg(r);
            nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
                                           1.1, 2, 0
                                                           //|CASCADE_FIND_BIGGEST_OBJECT
                                                           //|CASCADE_DO_ROUGH_SEARCH
                                                           //|CASCADE_DO_CANNY_PRUNING
                                                           | CASCADE_SCALE_IMAGE,
                                           Size(30, 30));

            for (const auto &nr : nestedObjects) {
                center.x = cvRound((r.x + nr.x + nr.width * 0.5));
                center.y = cvRound((r.y + nr.y + nr.height * 0.5));
                radius = cvRound((nr.width + nr.height) * 0.25);
                circle(img, center, radius, color, 3, 8, 0);
            }
        }
    }

}// namespace

#endif//OPENCV___DETECT_FACE_HPP
