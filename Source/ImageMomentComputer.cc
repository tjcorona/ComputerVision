#include <iostream>
#include "CVImage.hh"
#include "ImageMoment.hh"

int main(int argc, char** argv )
{
  if ( argc != 2 )
  {
    std::cout<<"usage: ImageMomentComputer <Image_Path>"<<std::endl;
    return -1;
  }

  cv::Mat input,gray;
  input = cv::imread( argv[1], 1 );
  if ( !input.data )
  {
    std::cout<<"No image data"<<std::endl;
    return -1;
  }
  cvtColor( input, gray, cv::COLOR_BGR2GRAY );
  blur( gray, gray, cv::Size(3,3) );
 
  CVImage image(gray);
  
  cv::Moments mts = cv::moments(image,false);
  double hu[7];
  HuMoments(mts,hu);

  ImageMoment imageMoment(&image);

  std::cout<<"M[0][0]: "<<mts.m00<<" "<<imageMoment.M(0,0)<<std::endl;
  std::cout<<"M[1][0]: "<<mts.m10<<" "<<imageMoment.M(1,0)<<std::endl;
  std::cout<<"M[0][1]: "<<mts.m01<<" "<<imageMoment.M(0,1)<<std::endl;
  std::cout<<"M[2][0]: "<<mts.m20<<" "<<imageMoment.M(2,0)<<std::endl;
  std::cout<<"M[1][1]: "<<mts.m11<<" "<<imageMoment.M(1,1)<<std::endl;
  std::cout<<"M[0][2]: "<<mts.m02<<" "<<imageMoment.M(0,2)<<std::endl;
  std::cout<<"M[3][0]: "<<mts.m30<<" "<<imageMoment.M(3,0)<<std::endl;
  std::cout<<"M[2][1]: "<<mts.m21<<" "<<imageMoment.M(2,1)<<std::endl;
  std::cout<<"M[1][2]: "<<mts.m12<<" "<<imageMoment.M(1,2)<<std::endl;
  std::cout<<"M[0][3]: "<<mts.m03<<" "<<imageMoment.M(0,3)<<std::endl;

  std::cout<<"DM[0][0]: "<<(mts.m00-imageMoment.M(0,0))/mts.m00<<std::endl;
  std::cout<<"DM[1][0]: "<<(mts.m10-imageMoment.M(1,0))/mts.m10<<std::endl;
  std::cout<<"DM[0][1]: "<<(mts.m01-imageMoment.M(0,1))/mts.m01<<std::endl;
  std::cout<<"DM[2][0]: "<<(mts.m20-imageMoment.M(2,0))/mts.m20<<std::endl;
  std::cout<<"DM[1][1]: "<<(mts.m11-imageMoment.M(1,1))/mts.m11<<std::endl;
  std::cout<<"DM[0][2]: "<<(mts.m02-imageMoment.M(0,2))/mts.m02<<std::endl;
  std::cout<<"DM[3][0]: "<<(mts.m30-imageMoment.M(3,0))/mts.m30<<std::endl;
  std::cout<<"DM[2][1]: "<<(mts.m21-imageMoment.M(2,1))/mts.m21<<std::endl;
  std::cout<<"DM[1][2]: "<<(mts.m12-imageMoment.M(1,2))/mts.m12<<std::endl;
  std::cout<<"DM[0][3]: "<<(mts.m03-imageMoment.M(0,3))/mts.m03<<std::endl;

  std::cout<<"mu[2][0]: "<<mts.mu20<<" "<<imageMoment.mu(2,0)<<std::endl;
  std::cout<<"mu[1][1]: "<<mts.mu11<<" "<<imageMoment.mu(1,1)<<std::endl;
  std::cout<<"mu[0][2]: "<<mts.mu02<<" "<<imageMoment.mu(0,2)<<std::endl;
  std::cout<<"mu[3][0]: "<<mts.mu30<<" "<<imageMoment.mu(3,0)<<std::endl;
  std::cout<<"mu[2][1]: "<<mts.mu21<<" "<<imageMoment.mu(2,1)<<std::endl;
  std::cout<<"mu[1][2]: "<<mts.mu12<<" "<<imageMoment.mu(1,2)<<std::endl;
  std::cout<<"mu[0][3]: "<<mts.mu03<<" "<<imageMoment.mu(0,3)<<std::endl;

  std::cout<<"Dmu[2][0]: "<<(mts.mu20-imageMoment.mu(2,0))/mts.mu20<<std::endl;
  std::cout<<"Dmu[1][1]: "<<(mts.mu11-imageMoment.mu(1,1))/mts.mu11<<std::endl;
  std::cout<<"Dmu[0][2]: "<<(mts.mu02-imageMoment.mu(0,2))/mts.mu02<<std::endl;
  std::cout<<"Dmu[3][0]: "<<(mts.mu30-imageMoment.mu(3,0))/mts.mu30<<std::endl;
  std::cout<<"Dmu[2][1]: "<<(mts.mu21-imageMoment.mu(2,1))/mts.mu21<<std::endl;
  std::cout<<"Dmu[1][2]: "<<(mts.mu12-imageMoment.mu(1,2))/mts.mu12<<std::endl;
  std::cout<<"Dmu[0][3]: "<<(mts.mu03-imageMoment.mu(0,3))/mts.mu03<<std::endl;

  // for (unsigned i=0;i<4;i++)
  //   for (unsigned j=0;j<4;j++)
  //     std::cout<<"mu["<<i<<","<<j<<"]: "<<imageMoment.mu(i,j)<<std::endl;

  // for (unsigned i=0;i<4;i++)
  //   for (unsigned j=0;j<4;j++)
  //     std::cout<<"nu["<<i<<","<<j<<"]: "<<imageMoment.nu(i,j)<<std::endl;

  for (unsigned i=0;i<7;i++)
    std::cout<<"I["<<i+1<<"]: "<<hu[i]<<" "<<imageMoment.I(i)<<std::endl;
    
  for (unsigned i=0;i<7;i++)
    std::cout<<"DI["<<i+1<<"]: "<<(hu[i]-imageMoment.I(i))/hu[i]<<std::endl;

  // cv::namedWindow("Display Image", CV_WINDOW_AUTOSIZE );
  // cv::imshow("Display Image", image);

  // cv::waitKey(0);

  return 0;
}
