#include "Fireplace.h"
#include "helpers.h"
#include <iostream>

// Constructor
Fireplace::Fireplace() :textureID(0) {
    float vertices[] = {
        // Positions        // Texture Coordinates
        -1.0f,  0.4f,    0.0f, 1.0f, // Top-left
        -1.0f,  -0.6f,    0.0f, 0.0f, // Bottom-left
         0.0f, -0.6f,    1.0f, 0.0f, // Bottom-right
         0.0f,  0.4f,    1.0f, 1.0f  // Top-right
    };

    // Create the shader for rendering the background
    const char* vertexShaderSource = R"(
        #version 330 core 
        layout(location = 0) in vec2 inPos;
        layout(location = 1) in vec2 inTex; 
        out vec2 chTex;

        void main()
        {
	        gl_Position = vec4(inPos, 0.0, 1.0);
	        chTex = inTex;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec2 chTex;
        out vec4 outCol;
        uniform sampler2D uTex;
        void main()
        {
	        outCol = texture(uTex, chTex);
        }
    )";

    shaderProgram = createShader(vertexShaderSource, fragmentShaderSource);
    unsigned int stride = (2 + 2) * sizeof(float);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Tekstura
    textureID = loadImageToTexture("fireplace.png");
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
Fireplace::~Fireplace() {
    glDeleteTextures(1, &textureID);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}
// Render the background
void Fireplace::render() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}