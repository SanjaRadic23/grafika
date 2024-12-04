#include "Present3.h"
#include "helpers.h"
#include <iostream>

// Constructor
Present3::Present3() :textureID(0) {
    float vertices1[] = {
        // Positions        // Texture Coordinates
         0.5,  -0.4,    0.0, 1.0, // Top-left
         0.5,  -0.7,    0.0, 0.0, // Bottom-left
         0.6, -0.7,    1.0, 0.0, // Bottom-right
         0.6,  -0.4,    1.0, 1.0  // Top-right
    };

    float vertices2[] = {
        // Positions        // Texture Coordinates
         0.65,  -0.4,    0.0, 1.0, // Top-left
         0.65,  -0.7,    0.0, 0.0, // Bottom-left
         0.75, -0.7,    1.0, 0.0, // Bottom-right
         0.75,  -0.4,    1.0, 1.0  // Top-right
    };

    float vertices3[] = {
        // Positions        // Texture Coordinates
         0.78,  -0.3,    0.0, 1.0, // Top-left
         0.78,  -0.6,    0.0, 0.0, // Bottom-left
         0.88, -0.6,    1.0, 0.0, // Bottom-right
         0.88,  -0.3,    1.0, 1.0  // Top-right
    };

    float vertices4[] = {
        // Positions        // Texture Coordinates
         0.55,  -0.4,    0.0, 1.0, // Top-left
         0.55,  -0.7,    0.0, 0.0, // Bottom-left
         0.45, -0.7,    1.0, 0.0, // Bottom-right
         0.,  -0.4,    1.0, 1.0  // Top-right
    };


    shaderProgram = createShader("present.vert", "present.frag");
    unsigned int stride = (2 + 2) * sizeof(float);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Tekstura
    textureID = loadImageToTexture("present3.png");
    glBindTexture(GL_TEXTURE_2D, textureID);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(shaderProgram);
    unsigned uTexLoc = glGetUniformLocation(shaderProgram, "uTex");
    glUniform1i(uTexLoc, 0);
    glUseProgram(0);
}

// Destructor
Present3::~Present3() {
    glDeleteTextures(1, &textureID);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}
// Render the background
void Present3::render() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}