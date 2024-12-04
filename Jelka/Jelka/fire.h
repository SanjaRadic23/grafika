#pragma once
#include <GL/glew.h>
#include <string>

class Fire {
public:
    Fire(); 
    ~Fire();
    void render(float time, float fireX, float fireY);

private:
    unsigned int VAO, VBO, textureID1, textureID2, textureID3, shaderProgram; // smenjuju se teksture tokom vremena (ima ih 3)
};
