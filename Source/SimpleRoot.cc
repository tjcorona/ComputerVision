#include <iostream>
#include <TRandom3.h>
#include <TH1F.h>
#include <TApplication.h>

int main(int argc, char** argv)
{
  TApplication* application = new TApplication("example",&argc, argv);

  TRandom3* random = new TRandom3();

  TH1F* histogram = new TH1F("stats","",100,0,10);

  for(int i=0;i<10000;++i)
    histogram->Fill(random->Gaus(5,1));

  histogram->Draw();

  application->Run();

  return 0;
}
