#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc,char** argv)
{
  if (argc != 2)
  {
    std::cout<<"usage: DisplayImage <Image_Path>"<<std::endl;
    return -1;
  }

  cv::Mat image;
  image = cv::imread(argv[1],1);

  if (!image.data)
  {
    std::cout<<"No image data"<<std::endl;
    return -1;
  }
  cv::namedWindow("Display Image",CV_WINDOW_AUTOSIZE);
  cv::imshow("Display Image",image);

  cv::waitKey(0);

  return 0;
}
