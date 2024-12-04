#pragma once
#include <GL/glew.h>
#include <string>

class Fire {
public:
    Fire(); // Constructor with the texture path
    ~Fire();
    void render(float time, float fireX, float fireY); // Render the background

private:
    unsigned int VAO, VBO, textureID1, textureID2, textureID3, shaderProgram; // OpenGL objects
    void loadTexture(const std::string& texturePath); // Load texture from file
};
