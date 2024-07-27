#include "GameWindow.hpp"
#include <iostream>
#include <thread>

void GameWindow::clickLMB(int x, int y) const {
  INPUT Inputs[1] = {0};

  Inputs[0].type = INPUT_MOUSE;
  Inputs[0].mi.dx = ((x + left) * 65535) / screenWidth;
  Inputs[0].mi.dy = ((y + top) * 65535) / screenHeight;
  Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
  SendInput(1, Inputs, sizeof(INPUT));

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  Inputs[0].type = INPUT_MOUSE;
  Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  Inputs[0].mi.dx = 0;
  Inputs[0].mi.dy = 0;
  SendInput(1, Inputs, sizeof(INPUT));

  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  Inputs[0].type = INPUT_MOUSE;
  Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
  SendInput(1, Inputs, sizeof(INPUT));
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

Pixel GameWindow::getPixel(int x, int y) const {
  if(pixels == nullptr) {
    return Pixel(0, 0, 0);
  }
  int p = (height-y-1)*width+x; // upside down
  unsigned char r = pixels[p].rgbRed;
  unsigned char g = pixels[p].rgbGreen;
  unsigned char b = pixels[p].rgbBlue;
  return Pixel(r, g, b);
}

void GameWindow::refresh() {
  HWND windowHandle = FindWindow(nullptr, "MOLEK-SYNTEZ"); // GetDesktopWindow();
  if(windowHandle == nullptr) {
    std::cout << "Couldn't find MOLEK-SYNTEZ window";
    return;
  }
  updateSize(windowHandle);

  HDC windowDC = GetDC(windowHandle);
  HDC hCaptureDC = CreateCompatibleDC(windowDC);
  HBITMAP hCaptureBitmap = CreateCompatibleBitmap(windowDC, width, height);
  SelectObject(hCaptureDC, hCaptureBitmap);

  BitBlt(hCaptureDC, 0, 0, width, height, windowDC, 0,0, SRCCOPY|CAPTUREBLT);

  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;


  delete[] pixels;
  pixels = new RGBQUAD[width * height];

  GetDIBits(
      hCaptureDC,
      hCaptureBitmap,
      0,
      height,
      pixels,
      &bmi,
      DIB_RGB_COLORS
  );

  ReleaseDC(windowHandle, windowDC);
  DeleteDC(hCaptureDC);
  DeleteObject(hCaptureBitmap);
}

void GameWindow::updateSize(HWND windowHandle) {
  RECT clientRect;
  RECT windowRect;
  GetClientRect(windowHandle, &clientRect);
  GetWindowRect(windowHandle, &windowRect);
  width = clientRect.right - clientRect.left;
  height = clientRect.bottom - clientRect.top;
  top = windowRect.bottom - clientRect.bottom;
  left = windowRect.right - clientRect.right;
}

GameWindow::GameWindow() {
  screenWidth = GetSystemMetrics(SM_CXSCREEN);
  screenHeight = GetSystemMetrics(SM_CYSCREEN);
}
