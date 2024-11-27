#pragma once
#include <GL/glew.h>
#include <string>

class Clock {
public:
    Clock(); // Constructor with the texture path
    ~Clock();
    void render(); // Render the background

private:
    unsigned int VAO, VBO, textureID, shaderProgram; // OpenGL objects
    void loadTexture(const std::string& texturePath); // Load texture from file
};