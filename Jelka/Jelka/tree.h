#pragma once
#include <GL/glew.h>
#include <string>

class Tree {
public:
    Tree(); 
    ~Tree();
    void render();

private:
    unsigned int VAO, VBO, textureID, shaderProgram;
};
