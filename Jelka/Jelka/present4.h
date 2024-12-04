#pragma once
#include <GL/glew.h>
#include <string>

class Present4 {
public:
    Present4();
    ~Present4();
    void render();

private:
    unsigned int VAO, VBO, textureID, shaderProgram;
};