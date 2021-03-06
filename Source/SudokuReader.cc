#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <getopt.h>
#include <iostream>
#include <string>

#include "CVImage.hh"
#include "ImageMoment.hh"

using namespace cv;

int main( int argc, char** argv )
{
  std::string usage =
    "\n"
    "Usage: SudokuReader <options> <image>\n"
    "\n"
    "This program reads Sudoku puzzles.\n"
    "\n"
    "\tAvailable options:\n"
    "\t -h, --help             (shows this message and exits)\n"
    "\t -c, --compare          (bool; display original image for comparison)\n"
    "\t -p, --plot-statistics  (bool; plot statistics)\n"
    "\t -s, --show-statistics  (bool; display statistics)\n"
    "\t -v, --verify           (bool; verify each entry)\n"
    ;

  static struct option longOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"compare", no_argument, 0, 'c'},
    {"plot-statistics", no_argument, 0, 'p'},
    {"show-statistics", no_argument, 0, 's'},
    {"verify", no_argument, 0, 'v'},
  };

  static const char *optString = "hcpsv";

  bool compare = false;
  bool verify = false;
  bool plot_statistics = false;
  bool show_statistics = false;
  
  while(1) {
    char optId = getopt_long(argc, argv,optString, longOptions, NULL);
    if(optId == -1) break;
    switch(optId) {
    case('h'): // help
      std::cout<<usage<<std::endl;
      return 0;
    case('c'):
      compare = true;
      break;
    case('p'):
      plot_statistics = true;
      break;
    case('s'):
      show_statistics = true;
      break;
    case('v'):
      verify = true;
      break;
    default: // unrecognized option
      std::cout<<usage<<std::endl;
      return 1;
    }
  }

  std::string image = "Data/sudoku.png";

  if (optind != argc)
    image = argv[optind];
    
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

  Rect boundingRec = boundingRect(boundingContour);

  if (verify)
  {
    namedWindow( "puzzle", CV_WINDOW_AUTOSIZE );
    rectangle(src,Point(boundingRec.x,boundingRec.y),Point(boundingRec.x + boundingRec.width,boundingRec.y + boundingRec.height),Scalar(0, 0, 255), 2);
    imshow( "puzzle", src );
  }

  double x_box = (boundingRec.width)/9.;
  double y_box = (boundingRec.height)/9.;

  tesseract::TessBaseAPI *myOCR = 
    new tesseract::TessBaseAPI();

  myOCR->SetVariable("tessedit_char_whitelist"," 123456789");

  if (myOCR->Init(NULL, "eng")) {
    std::cerr<<"Could not initialize tesseract."<<std::endl;
    return 1;
  }

  std::vector<int> cells;

  std::vector< std::vector<std::vector<double> > > moments(10);

  for (int i=0;i<81;i++)
  {
    double x_start = boundingRec.x + (i%9)*x_box;
    double y_start = boundingRec.y + (i/9)*y_box;

    cv::Mat subset;
    cv::Rect rec(x_start + x_box/10,
    		 y_start + y_box/10,
    		 x_box - 2.*x_box/10,
    		 y_box - 2.*y_box/10);
    gray(rec).copyTo(subset);

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

    std::vector<double> mts;
    
    {
      /// compute Hu moments
      CVImage image(subset);
      ImageMoment imageMoment(&image);
      for (unsigned i=0;i<8;i++)
	mts.push_back(imageMoment.I(i));
    }

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
      rectangle(src, Point(rec.x, rec.y),
		Point(rec.x + rec.width, rec.y + rec.height),
		Scalar(0, 0, 255), 2);

      imshow( "puzzle", src );

      std::cout<<"Identify value (0 for blank). Press any other key for "<<value<<".\r";
      std::cout.flush();
      
      int userValue = waitKey(0);
      /// ASCII offset
      userValue -= 48;
      if (userValue >= 0 && userValue < 10)
	value = userValue;
    }
    
    if (value >= 0 && value < 10)
      moments.at(value).push_back(mts);

    cells.push_back(value);
  }

  myOCR->End();

  if (plot_statistics)
  {
    TApplication* application = new TApplication("momentHistograms",&argc,argv);
    TCanvas* canvas = new TCanvas("C","Canvas",5,5,1600,800);
    canvas->Divide(4,2);

    std::vector<std::vector<TH1F*> > momentHistograms(10,std::vector<TH1F*>(8));
    for (unsigned i=0;i<10;i++)
    {
      for (unsigned j=0;j<8;j++)
      {
	std::stringstream s; s << "moment_" << i << "_" << j;
	TH1F* momentHistogram = new TH1F(s.str().c_str(),"",100,-50,-6);
	momentHistograms[i][j] = momentHistogram;
      }
    }

    // loop over digits
    for (unsigned i=0;i<moments.size();i++)
    {
      // std::cout<<"digit "<<i<<std::endl;
      // std::cout<<moments.at(i).size()<<" instances"<<std::endl;
      // std::cout<<moments.at(i).at(0).size()<<" moments"<<std::endl;
      // std::cout<<std::endl;

      // loop over instances
      for (unsigned j=0;j<moments.at(i).size();j++)
      {
	// loop over moments
	for (unsigned k=0;k<moments.at(i).at(j).size();k++)
	{
	  std::cout<<"histogram "<<i<<","<<k<<": "<<log(moments.at(i).at(j).at(k))<<std::endl;
	  double val = log(moments.at(i).at(j).at(k));
	  // if (val > -60)
	    momentHistograms[i][k]->Fill(val);
	}
      }
    }

    for (unsigned i=0;i<10;i++)
    {
      for (unsigned j=0;j<8;j++)
      {
	canvas->cd(j+1);
	// momentHistograms[i][j]->SetLineColor(i+1);
	momentHistograms[i][j]->SetFillColor((i<9 ? i+1 : 11));
	if (i == 0 && j==0)
	  momentHistograms[i][j]->Draw();
	else
	  momentHistograms[i][j]->Draw("same");
      }
    }

    application->Run();

    for (unsigned i=0;i<10;i++)
      for (unsigned j=0;j<8;j++)
	delete momentHistograms[i][j];
    delete canvas;
  }

  if (show_statistics)
  {
    for (unsigned i=0;i<moments.size();i++)
    {
      std::cout<<"Hu moments for digit "<<i<<":"<<std::endl;
      std::vector<double> av(8,0.);
      std::vector<double> av2(8,0.);
      for (unsigned j=0;j<moments.at(i).size();j++)
      {
	for (unsigned k=0;k<moments.at(i).at(j).size();k++)
	{
	  av.at(k) += moments.at(i).at(j).at(k);
	  av2.at(k) += (moments.at(i).at(j).at(k)*moments.at(i).at(j).at(k));
	}
      }

      std::cout<<std::scientific;
    
      for (unsigned j=0;j<8;j++)
      {
	av.at(j)/=moments.at(i).size();
	av2.at(j)/=moments.at(i).size();
	std::cout<<"Descriptor "<<j+1<<" mean, sigma: "<<av.at(j)<<" "<<sqrt(av2.at(j)-(av.at(j)*av.at(j)))<<std::endl;
      }
      std::cout<<std::endl;
    }
  }
  
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
  
  if (compare)
  {
    if (!verify)
      namedWindow( "puzzle", CV_WINDOW_AUTOSIZE );
    imshow( "puzzle", src );
    waitKey(0);
  }

  return 0;
}
