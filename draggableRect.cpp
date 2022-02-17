#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>

#define RECT_HEIGHT 100
#define RECT_WIDTH 100
#define THIKNESS 3
//#define COLOR (0,255,0)

#define WINDOW_CLOSED -1

cv::Rect roiRect;

//Original layer
cv::Mat source;
//Layer for refreshing the window
cv::Mat3b layer;
//Layer with rectangle
cv::Mat3b workingFrame;


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

cv::Mat SelectROI()
{
    //extract ROI using rectangle
    cv::Mat roiImg = source(roiRect);
    return roiImg;
}

//Mouse callback
void onRectangle(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_MOUSEMOVE && flags == cv::EVENT_FLAG_LBUTTON)
    {
        if (CheckCoordinates(x, y) == true) {
            //Set coordinates of starting points of a Mouse
            roiRect.x = x;
            roiRect.y = y;
        }
        //Refresh the window
        workingFrame = layer.clone();
        rectangle(workingFrame, roiRect, cv::Scalar(0, 255, 0), THIKNESS);
        cv::imshow("Webcam", workingFrame);
    }
}

void CamCapture()
{
    cv::VideoCapture cam(0);
    SetRectangleSize(cam.get(cv::CAP_PROP_FRAME_HEIGHT), cam.get(cv::CAP_PROP_FRAME_WIDTH));
    cv::namedWindow("Webcam", cv::WindowFlags::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Webcam", onRectangle, NULL);

    while(true)
    {
        cam.read(source);
        layer = source.clone();
        workingFrame = source.clone();

        cv::rectangle(workingFrame, roiRect, cv::Scalar(0,255,0), THIKNESS);

        cv::imshow("Webcam", workingFrame);
        cv::Mat roiImg = SelectROI();
        if (!roiImg.empty()) { 
            cv::imshow("ROI", roiImg);
        }
        //exit if esc or close button is pressed
        if ((cv::waitKey(10) == 27) || (cv::getWindowProperty("Webcam", cv::WINDOW_AUTOSIZE) == WINDOW_CLOSED))
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
