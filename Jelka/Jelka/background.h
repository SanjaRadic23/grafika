#pragma once
#include <GL/glew.h>
#include <string>

class Background {
public:
    Background(); 
    ~Background();
    void render(); 
private:
    unsigned int VAO, VBO, textureID, shaderProgram; 
};
