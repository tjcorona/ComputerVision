#include "ImageMoment.hh"
#include <cmath>

void ImageMoment::ComputeMoments()
{
  double I = 0;

  for ( int i = 0; i < 4; i++ )
    for ( int j = 0; j < 4; j++ )
      fM[i][j] = 0.;

  /// First, we compute the raw moments
  for ( int i = 0; i < fImage->Width(); i++ )
  {
    for ( int j = 0; j < fImage->Length(); j++ )
    {
      I = fImage->operator () ( j, i );
      
      double x = i;
      double y = j;

      fM[0][0] += I;
      fM[1][0] += x*I;
      fM[0][1] += y*I;
      fM[2][0] += x*x*I;
      fM[1][1] += x*y*I;
      fM[0][2] += y*y*I;
      fM[3][0] += x*x*x*I;
      fM[2][1] += x*x*y*I;
      fM[1][2] += x*y*y*I;
      fM[0][3] += y*y*y*I;
    }
  }

  /// Then, we compute the central moments
  double xbar = fM[1][0]/fM[0][0];
  double ybar = fM[0][1]/fM[0][0];
  fmu[0][0] = fM[0][0];
  fmu[0][1] = fmu[1][0] = 0;
  fmu[1][1] = fM[1][1] - xbar * fM[0][1];
  fmu[2][0] = fM[2][0] - xbar * fM[1][0];
  fmu[0][2] = fM[0][2] - ybar * fM[0][1];
  fmu[2][1] = fM[2][1] - 2*xbar*fM[1][1] - ybar*fM[2][0] + 2*xbar*xbar*fM[0][1];
  fmu[1][2] = fM[1][2] - 2*ybar*fM[1][1] - xbar*fM[0][2] + 2*ybar*ybar*fM[1][0];
  fmu[3][0] = fM[3][0] - 3*xbar*fM[2][0] + 2*xbar*xbar*fM[1][0];
  fmu[0][3] = fM[0][3] - 3*ybar*fM[0][2] + 2*ybar*ybar*fM[0][1];
  
  /// Then, we compute the scale-invariant moments
  for (int i=0;i<4;i++)
    for (int j=0;j<4;j++)
      fnu[i][j] = ( i + j >= 2 ? fmu[i][j]/pow(fmu[0][0],1.+(i+j)/2.) : 0 );

  /// Finally, we compute the rotation-invariant moments
  fI[0] = fnu[2][0] + fnu[0][2];
  fI[1] = pow(fnu[2][0] - fnu[0][2],2) + 4.*fnu[1][1]*fnu[1][1];
  fI[2] = pow(fnu[3][0] - 3.*fnu[1][2],2) + pow(3.*fnu[2][1] - fnu[0][3],2);
  fI[3] = pow(fnu[3][0] + fnu[1][2],2) + pow(fnu[2][1] + fnu[0][3],2);
  fI[4] = (fnu[3][0]-3.*fnu[1][2])*(fnu[3][0]+fnu[1][2])*(pow(fnu[3][0]+fnu[1][2],2) - 3.*pow(fnu[2][1]+fnu[0][3],2)) + (3.*fnu[2][1]-fnu[0][3])*(fnu[2][1]+fnu[0][3])*(3.*pow(fnu[3][0]+fnu[1][2],2) - pow(fnu[2][1]+fnu[0][3],2));
  fI[5] = (fnu[2][0]-fnu[0][2])*(pow(fnu[3][0]+fnu[1][2],2) - pow(fnu[2][1]+fnu[0][3],2)) + 4.*fnu[1][1]*(fnu[3][0]+fnu[1][2])*(fnu[2][1]+fnu[0][3]);
  fI[6] = (3.*fnu[2][1]-fnu[0][3])*(fnu[3][0]+fnu[1][2])*(pow(fnu[3][0]+fnu[1][2],2) - 3.*pow(fnu[2][1]+fnu[0][3],2)) - (fnu[3][0]-3.*fnu[1][2])*(fnu[2][1]+fnu[0][3])*(3.*pow(fnu[3][0]+fnu[1][2],2) - pow(fnu[2][1]+fnu[0][3],2));
  fI[7] = fnu[1][1]*(pow(fnu[3][0]+fnu[1][2],2) - pow(fnu[0][3]+fnu[2][1],2)) - (fnu[2][0]-fnu[1][2])*(fnu[3][0]+fnu[1][2])*(fnu[0][3]+fnu[2][1]);
}
