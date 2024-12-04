#pragma once
#include <GL/glew.h>
#include <string>

class Present3 {
public:
    Present3(); 
    ~Present3();
    void render();

private:
    unsigned int VAO, VBO, textureID, shaderProgram;
};