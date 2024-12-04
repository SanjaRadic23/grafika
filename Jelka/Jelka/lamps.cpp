#include "Lamps.h"
#include "helpers.h"
#include <random>

Lamps::Lamps() {
    float vertices[] = {
        // Positions         // Colors
        0.6, -0.20,  1.0, 0.0, 0.0, 
        0.8, -0.15,   0.0, 1.0, 0.0, 
        0.7, 0.2,    0.0, 0.0, 1.0, 
        0.75, 0.5,    1.0, 1.0, 0.0,
        0.7, -0.05,    1.0, 0.0, 1.0,
        0.82, 0.1,      0.0, 1.0, 1.0,
        0.65, 0.4,      1.0, 0.0, 1.0,
        0.58, 0.0,    1.0, 1.0, 0.0,
        0.89, -0.1,  1.0, 0.0, 0.0,
    };

    shaderProgram = createShader("lamps.vert", "lamps.frag");

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Lamps::~Lamps() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}

void Lamps::render(float time, int mode, float lampRadius) {
    glUseProgram(shaderProgram);

    unsigned int timeLoc = glGetUniformLocation(shaderProgram, "time");
    glUniform1f(timeLoc, time);

    unsigned int modeLoc = glGetUniformLocation(shaderProgram, "mode");
    glUniform1i(modeLoc, mode);

    unsigned int radiusLoc = glGetUniformLocation(shaderProgram, "lampRadius");
    glUniform1f(radiusLoc, lampRadius);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 9);

    glBindVertexArray(0);
    glUseProgram(0);
}