#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
unsigned int loadImageToTexture(const char* filePath);

#endif // GLOBAL_FUNCTIONS_H