#include "Tree.h"
#include "helpers.h"
#include <iostream>

// Constructor
// Constructor
Tree::Tree() : textureID(0) {
    float vertices[] = {
        // Pozicije         // Teksturne koordinate
         0.45f, -0.52f,        0.0f, 0.0f,   // Donja leva
         1.0f, -0.52f,        1.0f, 0.0f,   // Donja desna
         0.45f,  0.2f,        0.0f, 0.6f,  // Srednja leva
         0.97f,  0.2f,        1.0f, 0.6f,  // Srednja desna
         0.72f, 0.9f,         0.5f, 1.0f    // Srednja gornja
    };

    // Shader setup
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
    // Texture setup
    textureID = loadImageToTexture("tree.png");
    glBindTexture(GL_TEXTURE_2D, textureID);
    glGenerateMipmap(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
Tree::~Tree() {
    glDeleteTextures(1, &textureID);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}

// Render the tree
void Tree::render() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
