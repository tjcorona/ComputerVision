#ifndef CVIMAGE_HH
#define CVIMAGE_HH

#include "Image.hh"
#include <opencv2/opencv.hpp>
#include <limits>

class CVImage : public Image, public cv::Mat
{
public:
  CVImage(const cv::Mat& image) : Image(), cv::Mat(image) {}

  virtual ~CVImage() {}

  virtual double operator () (int i,int j) const
  { return this->at<uchar>(i,j); }
  virtual int Length() const { return rows; }
  virtual int Width() const { return cols; }
  int Area() const { return Length()*Width(); }
};

#endif /* CVIMAGE_HH */
