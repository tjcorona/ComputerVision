#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <getopt.h>
#include <iostream>
#include <string>

using namespace cv;

int main( int argc, char** argv )
{
  std::string usage =
    "\n"
    "Usage: SudokuReader <options>\n"
    "\n"
    "This program reads Sudoku puzzles.\n"
    "\n"
    "\tAvailable options:\n"
    "\t -h, --help              (shows this message and exits)\n"
    "\t -i, --input             (input file)\n"
    "\t -v, --verify            (bool; verify each entry)\n"
    ;

  static struct option longOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"input", required_argument, 0, 'i'},
    {"verify", no_argument, 0, 'v'},
  };

  static const char *optString = "hi:v";

  std::string image = "Data/sudoku.png";
  bool verify = false;
  
  while(1) {
    char optId = getopt_long(argc, argv,optString, longOptions, NULL);
    if(optId == -1) break;
    switch(optId) {
    case('h'): // help
      std::cout<<usage<<std::endl;
      return 0;
    case('i'):
      image = optarg;
      break;
    case('v'):
      verify = true;
      break;
    default: // unrecognized option
      std::cout<<usage<<std::endl;
      return 1;
    }
  }

  /// Load an image
  Mat src = imread( image.c_str() );

  if( !src.data )
  { return -1; }

  /// Filter and find bounding contour
  Mat gray, blur, thresh;

  std::vector < std::vector<Point> > contours;

  cvtColor(src, gray, COLOR_BGR2GRAY);
  GaussianBlur(gray, blur, Size(5, 5), 2, 2);
  adaptiveThreshold(blur, thresh, 255, 1, 1, 11, 2);
  findContours(thresh, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

  int area = 0;
  std::vector < Point > boundingContour;
  Mat filteredImage;
  for (size_t i = 0; i < contours.size(); i++)
  {
    std::vector < Point > cnt = contours[i];
    if (contourArea(cnt) > area)
    {
      area = contourArea(cnt);
      boundingContour = cnt;
      Rect rec = boundingRect(cnt);
      filteredImage = gray(rec);
    }
  }

  /// Compute the centroid of the bounding contour
  Point centroid(0,0);
  for (int i = 0; i < boundingContour.size(); i++)
    centroid += boundingContour[i];
  centroid.x /= boundingContour.size();
  centroid.y /= boundingContour.size();

  /// Using the centroid, compute the corners
  std::vector< std::vector<Point> > sortedPoints(4);
  for (int i = 0; i < boundingContour.size(); i++)
  {
    if (boundingContour[i].x < centroid.x &&
	boundingContour[i].y < centroid.y)
      sortedPoints[0].push_back(boundingContour[i]);
    else if (boundingContour[i].x > centroid.x &&
	     boundingContour[i].y < centroid.y)
      sortedPoints[1].push_back(boundingContour[i]);
    else if (boundingContour[i].x < centroid.x &&
	     boundingContour[i].y > centroid.y)
      sortedPoints[2].push_back(boundingContour[i]);
    else
      sortedPoints[3].push_back(boundingContour[i]);
  }

  std::vector<Point> corner(4,Point(0,0));
  for (int i=0;i<4;i++)
  {
    for (int j=0;j<sortedPoints[i].size();j++)
      corner[i] += sortedPoints[i][j];
    corner[i].x /= sortedPoints[i].size();
    corner[i].y /= sortedPoints[i].size();
  }
    
  int x_box = (corner[1].x - corner[0].x)/9;
  int y_box = (corner[2].y - corner[0].y)/9;

  tesseract::TessBaseAPI *myOCR = 
    new tesseract::TessBaseAPI();

  myOCR->SetVariable("tessedit_char_whitelist"," 123456789");

  if (myOCR->Init(NULL, "eng")) {
    std::cerr<<"Could not initialize tesseract."<<std::endl;
    return 1;
  }

  std::vector<int> cells;
  
  if (verify)
  {
    namedWindow( "cell", CV_WINDOW_AUTOSIZE );
    std::cout<<"Change window focus to the cell display."<<std::endl;
  }

  for (int i=0;i<81;i++)
  {
    int x_start = corner[0].x + (i%9)*x_box;
    int y_start = corner[0].y + (i/9)*y_box;

    cv::Mat subset;
    gray(cv::Rect(x_start + x_box/4,
		  y_start + y_box/5,
		  x_box - x_box/4,
		  y_box - y_box/5)).copyTo(subset);

    std::string img_name;
    {
      std::stringstream s;
      if (i<10)
	s << "tmp_0" << i<<".png";
      else
	s << "tmp_" << i<<".png";
      img_name = s.str();
    }
    imwrite(img_name.c_str(), subset);

    Pix *pix = pixRead(img_name.c_str());
    myOCR->SetImage(pix);

    std::string outText = myOCR->GetUTF8Text();

    int value = 0;

    {
      std::stringstream s; s << outText; s >> value;
    }
    
    myOCR->Clear();
    pixDestroy(&pix);
    std::remove(img_name.c_str());

    if (verify)
    {
      imshow( "cell", subset );

      std::cout<<"Identify value, or 0 for blank (default = "<<value<<"): \r";
      std::cout.flush();
      
      int userValue = waitKey(0);
      /// ASCII offset
      userValue -= 48;
      if (userValue >= 0 && userValue < 10)
	value = userValue;
    }

    cells.push_back(value);
  }

  myOCR->End();

  {
    const int n = 9;
    int counter = 0;

    for (unsigned int i=0;i<n;i++)
    {
      std::cout<<std::endl;
      for (unsigned int j=0;j<n;j++)
	std::cout<<" ---";
      std::cout<<std::endl<<"|";
      for (unsigned int j=0;j<n;j++)
      {
	std::cout<<" ";
	if (cells[counter] != 0)
	  std::cout<<cells[counter];
	else
	  std::cout<<" ";
	std::cout<<" |";
	counter++;
      }
    }
    std::cout<<std::endl<<" ---";
    for (unsigned int j=0;j<n-1;j++)
      std::cout<<" ---";
    std::cout<<std::endl;
    std::cout<<std::endl;
  }
  
  return 0;
}
