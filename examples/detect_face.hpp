
#ifndef OPENCV___DETECT_FACE_HPP
#define OPENCV___DETECT_FACE_HPP

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <iostream>
#include <optional>

namespace example
{

std::optional<cv::Rect> detect_face(
    cv::Mat& img,
    cv::CascadeClassifier& cascade,
    std::optional<cv::CascadeClassifier> nestedCascade = std::nullopt)
{
    double t;
    std::vector<cv::Rect> faces;
    cv::Mat gray, smallImg;
    cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    resize(gray, smallImg, cv::Size(), 1, 1, cv::INTER_LINEAR_EXACT);
    equalizeHist(smallImg, smallImg);
    t = (double)cv::getTickCount();
    cascade.detectMultiScale(gray, faces,
        1.1, 2, 0
            //|CASCADE_FIND_BIGGEST_OBJECT
            //|CASCADE_DO_ROUGH_SEARCH
            | cv::CASCADE_SCALE_IMAGE,
        cv::Size(30, 30));
    t = (double)cv::getTickCount() - t;
    printf("detection time = %g ms\n", t * 1000 / cv::getTickFrequency());

    if (faces.empty()) {
        return std::nullopt;
    }

    cv::Rect r = faces[0];
    cv::Mat smallImgROI;
    std::vector<cv::Rect> nestedObjects;
    cv::Scalar color = cv::Scalar(255, 0, 0);
    int radius;
    rectangle(img, cv::Point(cvRound(r.x), cvRound(r.y)),
        cv::Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
        color, 3, 8, 0);
    if (nestedCascade && !nestedCascade->empty()) {
        smallImgROI = smallImg(r);
        nestedCascade->detectMultiScale(smallImgROI, nestedObjects,
            1.1, 2, 0
                //|CASCADE_FIND_BIGGEST_OBJECT
                //|CASCADE_DO_ROUGH_SEARCH
                //|CASCADE_DO_CANNY_PRUNING
                | cv::CASCADE_SCALE_IMAGE,
            cv::Size(30, 30));

        cv::Point center;
        for (const auto& nr : nestedObjects) {
            center.x = cvRound((r.x + nr.x + nr.width * 0.5));
            center.y = cvRound((r.y + nr.y + nr.height * 0.5));
            radius = cvRound((nr.width + nr.height) * 0.25);
            circle(img, center, radius, color, 3, 8, 0);
        }
    }
    return r;
}

} // namespace example

#endif //OPENCV___DETECT_FACE_HPP
