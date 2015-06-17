/* loadppm.h */

#ifndef __LOADPPM_H
#define __LOADPPM_H

#if defined(_WIN32)
	#include <windows.h>
#endif
#include "gl.h"
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;

class PPMImage {
public:
  int sizeX, sizeY;
  GLubyte *data;
  PPMImage(const char *filename);
  ~PPMImage(){
    if (data)
      delete [] data;
  }
};

#endif
