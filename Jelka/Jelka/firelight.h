#pragma once
#include <GL/glew.h>
#include <string>

class FireLight {
public:
    FireLight(); 
    ~FireLight();
    void render(float currentTime); 

private:
    unsigned int VAO, VBO, shaderProgram;
};
