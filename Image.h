#pragma once

class Image
{
   unsigned char *width = nullptr;
   unsigned char *height = nullptr;
   unsigned char *pixelData = nullptr;
   unsigned char *imageData = nullptr;
public:
   Image(unsigned char *imageData);

   unsigned char* getImageData();
   unsigned char* getPixelData();
   unsigned char getWidth();
   unsigned char getHeight();

   void setImageData(unsigned char *newData);

};