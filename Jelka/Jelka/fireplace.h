#pragma once
#include <GL/glew.h>
#include <string>

class Fireplace {
public:
    Fireplace(); // Constructor with the texture path
    ~Fireplace();
    void render(); // Render the background

private:
    unsigned int VAO, VBO, textureID, shaderProgram; // OpenGL objects
    void loadTexture(const std::string& texturePath); // Load texture from file
};