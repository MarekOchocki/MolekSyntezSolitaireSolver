#pragma once
#include <windows.h>

class Pixel {
public:
  unsigned char r;
  unsigned char g;
  unsigned char b;

  explicit Pixel(unsigned char r, unsigned char g, unsigned char b): r(r), g(g), b(b) {}
};

class GameWindow {
  int top = 0;
  int left = 0;
  int width = 0;
  int height = 0;
  int screenWidth = 0;
  int screenHeight = 0;
  RGBQUAD* pixels = nullptr;

public:
  GameWindow();
  void clickLMB(int x, int y) const;
  Pixel getPixel(int x, int y) const;
  void refresh();

private:
  void updateSize(HWND windowHandle);
};


