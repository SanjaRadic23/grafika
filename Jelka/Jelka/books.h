#pragma once
#include <GL/glew.h>
#include <string>

class Books {
public:
    Books(); 
    ~Books();
    void render(); 

private:
    unsigned int VAO, VBO, textureID, shaderProgram;
};