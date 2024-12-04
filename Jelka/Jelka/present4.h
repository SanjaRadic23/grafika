#pragma once
#include <GL/glew.h>
#include <string>

class Present4 {
public:
    Present4(); // Constructor with the texture path
    ~Present4();
    void render(); // Render the background

private:
    unsigned int VAO, VBO, textureID, shaderProgram; // OpenGL objects
    void loadTexture(const std::string& texturePath); // Load texture from file
};