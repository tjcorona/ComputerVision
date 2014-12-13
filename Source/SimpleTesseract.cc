#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <string>

int main(int argc,char** argv)
{
  // [1]
  tesseract::TessBaseAPI *myOCR = 
    new tesseract::TessBaseAPI();

  // [2]
  std::cout<<"Tesseract-ocr version: "<<myOCR->Version()<<std::endl;
  std::cout<<"Leptonica version: "<<getLeptonicaVersion()<<std::endl;

  // [3]
  if (myOCR->Init(NULL, "eng")) {
    std::cerr<<"Could not initialize tesseract."<<std::endl;
    return 1;
  }

  std::string image;
  if (argc < 2)
    image = "Data/cleanText.png";
  else
    image = std::string(argv[1]);
    
  // [4]
  Pix *pix = pixRead(image.c_str());
  myOCR->SetImage(pix);

  // [5]
  std::string outText = myOCR->GetUTF8Text();
  std::cout<<"OCR output:"<<std::endl<<std::endl;
  std::cout<<outText<<std::endl;

  // [6]
  myOCR->Clear();
  myOCR->End();
  pixDestroy(&pix);
  return 0;
}
