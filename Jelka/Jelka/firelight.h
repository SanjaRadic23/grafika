#pragma once
#include <GL/glew.h>
#include <string>

class FireLight {
public:
    FireLight(); // Constructor with the texture path
    ~FireLight();
    void render(float currentTime); // Render the background

private:
    unsigned int VAO, VBO, shaderProgram;
};
