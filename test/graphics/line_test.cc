#include "kuro/graphics/line.hh"

#include "kuro/img/tga_image.hh"

using namespace kuro;

int main()
{
  TgaImage image(100, 100);

  DrawLine0({ 13, 20 }, { 80, 40 }, image, TgaColor::white);
  
  TgaColor::white.Print();
  image.WriteTo("line_test.tga");
}
