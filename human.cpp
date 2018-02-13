#include <iostream>
#include <opencv2/opencv.hpp>
 
using namespace std;
using namespace cv;
 
int main (int argc, const char * argv[])
{
    string filename = "test.mp4";
    VideoCapture cap(filename);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);    
    if (!cap.isOpened())
        return -1;
 
    Mat img;
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
 
    namedWindow("video capture", CV_WINDOW_AUTOSIZE);
    while (true)
    {
        cap >> img;
        if (!img.data)
            continue;
        int offset_x = 180;
        int offset_y = 80;

        cv::Rect roi;
        roi.x = offset_x ;
        roi.y = offset_y + 60;
        roi.width = img.size().width - (offset_x*2);
        roi.height = img.size().height - (offset_y*2);
        cv::Mat crop = img(roi);
        vector<Rect> found, found_filtered;

        hog.detectMultiScale(crop, found, 0, Size(8,24), Size(168, 168), 1.05, 2);

        size_t i, j;
        for (i=0; i<found.size(); i++)
        {
            Rect r = found[i];
            for (j=0; j<found.size(); j++)
                if (j!=i && (r & found[j])==r)
                    break;
            if (j==found.size())
                found_filtered.push_back(r);
        }
        for (i=0; i<found_filtered.size(); i++)
        {
	    Rect r = found_filtered[i];
            r.x += cvRound(r.width*0.1);
	    r.width = cvRound(r.width*0.8);
	    r.y += cvRound(r.height*0.06);
	    r.height = cvRound(r.height*0.9);
	    rectangle(crop, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
	}
        imshow("Test", img);
        if (waitKey(20) >= 0)
            break;
    }
    return 0;
}