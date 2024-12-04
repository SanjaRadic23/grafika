#pragma once
#include <GL/glew.h>
#include <string>

class Fireplace {
public:
    Fireplace(); 
    ~Fireplace();
    void render(); 

private:
    unsigned int VAO, VBO, textureID, shaderProgram; 
};