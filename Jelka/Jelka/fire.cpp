#include "Fire.h"
#include "helpers.h"
#include <iostream>

// Constructor
Fire::Fire() {
    float vertices[] = {
        // Pozicije       // Teksturne koordinate
        -0.67, -0.1,     0.0, 1.0,
        -0.67, -0.38,     0.0, 0.0,
        -0.32,  -0.38,     1.0, 0.0,
        -0.32,  -0.1,     1.0, 1.0,
    };

    shaderProgram = createShader("fire.vert", "fire.frag");

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

    // Učitavanje tri teksture vatre
    textureID1 = loadImageToTexture("fire1.png");
    textureID2 = loadImageToTexture("fire4.png");
    textureID3 = loadImageToTexture("fire3.png");

    // Podešavanje tekstura
    GLuint textures[3] = { textureID1, textureID2, textureID3 };
    for (int i = 0; i < 3; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Postavljanje uniform lokacija za teksture
    glUseProgram(shaderProgram);

    GLint uTex1Loc = glGetUniformLocation(shaderProgram, "uTex1");
    GLint uTex2Loc = glGetUniformLocation(shaderProgram, "uTex2");
    GLint uTex3Loc = glGetUniformLocation(shaderProgram, "uTex3");

    glUniform1i(uTex1Loc, 0);  // Tekstura 1 na GL_TEXTURE0
    glUniform1i(uTex2Loc, 1);  // Tekstura 2 na GL_TEXTURE1
    glUniform1i(uTex3Loc, 2);  // Tekstura 3 na GL_TEXTURE2

    glUseProgram(0);
}

// Destructor
Fire::~Fire() {
    glDeleteTextures(1, &textureID1);
    glDeleteTextures(1, &textureID2);
    glDeleteTextures(1, &textureID3);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}

// Render the background
void Fire::render(float time, float fireX, float fireY) {
    glUseProgram(shaderProgram);

    // Prosleđivanje uniform promenljivih
    GLint uTimeLoc = glGetUniformLocation(shaderProgram, "uTime");
    glUniform1f(uTimeLoc, time);

    GLint firePosLoc = glGetUniformLocation(shaderProgram, "firePos");
    glUniform2f(firePosLoc, fireX, fireY);

    GLint fireRadiusLoc = glGetUniformLocation(shaderProgram, "fireRadius");
    glUniform1f(fireRadiusLoc, 0.2);

    // Aktiviranje i bind-ovanje tekstura
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureID3);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);
}