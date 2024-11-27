#pragma once
#include <GL/glew.h>
#include <string>

class Shelf {
public:
    Shelf(); // Constructor with the texture path
    ~Shelf();
    void render(); // Render the background

private:
    unsigned int VAO, VBO, textureID, shaderProgram; // OpenGL objects
    void loadTexture(const std::string& texturePath); // Load texture from file
};