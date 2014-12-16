#ifndef IMAGEMOMENT_HH
#define IMAGEMOMENT_HH

#include "Image.hh"

class ImageMoment
{
public:
  ImageMoment() {}
  ImageMoment(const Image* image) : fImage(image) { ComputeMoments(); }

  void SetImage(const Image* image) { fImage = image; ComputeMoments(); }
  
  virtual ~ImageMoment() {}

  double M(int i,int j)  const { return (i < 4 && j < 4 ? fM[i][j] : 0); }
  double mu(int i,int j) const { return (i < 4 && j < 4 ? fmu[i][j] : 0); }
  double nu(int i,int j) const { return (i < 4 && j < 4 ? fnu[i][j] : 0); }
  double I(int i)        const { return (i < 8 ? fI[i] : 0); }
  
protected:
  void ComputeMoments();

  const Image* fImage;

  /// Raw Moments
  double fM[4][4];
  /// Central Moments
  double fmu[4][4];
  /// Scale-invariant Moments
  double fnu[4][4];
  /// Rotation-invariant (Hu) Moments
  double fI[8];
};

#endif /* IMAGEMOMENT_HH */
