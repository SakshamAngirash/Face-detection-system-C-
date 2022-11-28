#include "C:\opencv\build\include\opencv2\objdetect.hpp"
#include "C:\opencv\build\include\opencv2\highgui.hpp"
#include "C:\opencv\build\include\opencv2\imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
// Function Declaration to detect face
void detectFace(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale);
string cascadeName, nestedCascadeName;
int main(int argc, const char** argv)
{
    // Class for video capturing from video files & provides C++ API for capturing video from cameras or reading video files.
    VideoCapture capture;
    Mat frame, image;
    // PreDefined trained XML classifiers with facial features
    CascadeClassifier cascade, nestedCascade;
    double scale = 1;
    // Loads a classifiers from "opencv\sources\data\haarcascades" directory
    nestedCascade.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml");
    // Before execution, change the paths according to your openCV location
    cascade.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalcatface.xml");
    // To capture Video from WebCam replace the path with 0, else edit the path
    capture.open("C:\\Users\\Saksham\\Downloads\\WhatsApp Video 2022 - 11 - 27 at 6.34.00 PM.mp4");
    if (capture.isOpened())
    {
        // Capturing frames from the video
        cout << "Face Detection is started...." << endl;
        while (1)
        {
            capture >> frame;
            if (frame.empty())
                break;
            Mat frame1 = frame.clone();
            // Calling the detect face function
            detectFace(frame1, cascade, nestedCascade, scale);
            char c = (char)waitKey(10);
            // Press q to exit from the window
            if (c == 27 || c == 'q' || c == 'Q')
                break;
        }
    }
    else
        cout << "Could not Open Video/Camera! ";
    return 0;
}
// Defination of Detect face function
void detectFace(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale)
{
    vector<Rect> faces, faces2;
    Mat gray, smallImg;
    // To convert the frames to gray color
    cvtColor(img, gray, COLOR_BGR2GRAY);
    double fx = 1 / scale;
    // To resize the frames
    resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
    equalizeHist(smallImg, smallImg);
    // To detect faces of different sizes using cascade classifier
    cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    // To draw circles around the faces
    for (size_t i = 0; i < faces.size(); i++)
    {
        Rect r = faces[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = Scalar(255, 0, 0);
        int radius;
        double aspect_ratio = (double)r.width / r.height;
        if (0.75 < aspect_ratio && aspect_ratio < 1.3)
        {
            center.x = cvRound((r.x + r.width * 0.5) * scale);
            center.y = cvRound((r.y + r.height * 0.5) * scale);
            radius = cvRound((r.width + r.height) * 0.25 * scale);
            // To draw circle
            circle(img, center, radius, color, 3, 8, 0);
        }
        else
            rectangle(img, Point(cvRound(r.x * scale), cvRound(r.y * scale)),
                Point(cvRound((r.x + r.width - 1) * scale),
                    cvRound((r.y + r.height - 1) * scale)), color, 3, 8, 0);
        if (nestedCascade.empty())
            continue;
        smallImgROI = smallImg(r);
    }
    // To display the detected face
    imshow("Face Detection", img);
}