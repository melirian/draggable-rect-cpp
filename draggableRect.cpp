#include <opencv2\opencv.hpp>
#include <opencv\highui.hpp>

#define RECT_HEIGHT 100
#define RECT_WIDTH 100
#define THIKNESS 3
//#define COLOR (0,255,0)

#define WINDOW_CLOSED -1

cv::Rect roiRect;
//ROI image
cv::Mat roiImg

//Original layer
cv::Mat img;
//Layer for refreshing the window
cv::Mat3b layer;
//Layer with rectangle
cv::Mat3b working;

//Check if mouse coordinates get out of the window to prevent an attempt to draw the rectangle outside
int CheckCoordinates(int x, int y) {
	if ((x < 0) || (y < 0) || (x + roiRect.width > workingFrame.cols) || (y + roiRect.height > workingFrame.rows)) { return false; }
	return true;
}

void SetRectangleSize(int frameHeight, int frameWidth)
{
    //Start in the middle of the screen
    roiRect.x = frameHeight / 2;
    roiRect.y = frameWidth / 2;
    //With fixed size
    roiRect.height = RECT_HEIGHT;
    roiRect.width = RECT_WIDTH;
}

void SelectROI()
{
    roiImg = img(cv::Range(roiRect.x, RECT_HEIGHT), cv::Range(roiRect.y, RECT_WIDTH));
}

void onRectangle(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_MOUSEMOVE && flags == cv::EVENT_FLAG_LBUTTON)
    {
		if (CheckCoordinates(x, y) == true) {
        //Set coordinates of starting points of a Mouse
        roiRect.x = x;
        roiRect.y = y;
		|
        //Refresh the window
        working = layer.clone();
        rectangle(working, roiRect, Scalar(0, 255, 0), THIKNESS);
        imshow("Webcam", working);
    }
}

void CamCapture()
{
    cv::VideoCapture cam(0);
    SetRectangleSize(cam.get(cv::CAP_PROP_FRAME_HEIGHT), cam.get(cv::CAP_PROP_FRAME_WIDTH));
    cv::namedWindow("Webcam", WindowFlags::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Webcam", onRectangle, NULL);
    while(true)
    {
        cam.read(img);
        layer = img.clone();
        working = img.clone();

        cv::rectangle(working, roiRect, Scalar(0,255,0), THIKNESS);
        cv::imshow("Webcam", working);
        if (roiImg != NULL)
            cv::imshow("ROI", roiImg);
        
        if ((waitKey(10) == 27) || (getWindowProperty("Webcam", WINDOW_AUTOSIZE) == WINDOW_CLOSED))
            break;
    }
    cv::destroyAllWindows();
    cam.release();
}

int main(int argc, char** argv)
{
    CamCapture();
    return 0;
}