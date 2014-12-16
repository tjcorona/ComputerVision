#ifndef IMAGE_HH
#define IMAGE_HH

class Image
{
public:
  virtual ~Image() {}

  virtual double operator () (int,int) const = 0;
  virtual int Length() const = 0;
  virtual int Width() const = 0;
  int Area() const { return Length()*Width(); }
};

#endif /* IMAGE_HH */
