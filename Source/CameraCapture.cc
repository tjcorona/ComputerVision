#include "cv.h"
#include "highgui.h"

using namespace cv;

int main(int, char**)
{
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
    return -1;

  namedWindow("camera",1);

  Mat frame;

  while (true)
  {
    cap >> frame; // get a new frame from camera
    imshow("frame", frame);

    if(waitKey(30) >= 0) break;
  }    
    imwrite("cameraCapture.png", frame);        // save the image
    // if( (char)cvWaitKey(30) == 27 ) break;
  
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
