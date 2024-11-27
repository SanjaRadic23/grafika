#pragma once
#include <GL/glew.h>
#include <string>

class Tree {
public:
    Tree(); // Constructor with the texture path
    ~Tree();
    void render(); // Render the background

private:
    unsigned int VAO, VBO, textureID, shaderProgram; // OpenGL objects
    void loadTexture(const std::string& texturePath); // Load texture from file
};
