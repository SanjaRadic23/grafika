#include "FireLight.h"
#include "helpers.h"
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
#define CRES 30 // Circle Resolution

// Constructor
FireLight::FireLight() {
    float circle[(CRES + 2) * 2];
    float r = 0.35; // radius
    float centerX = -0.5; // Center X0
    float centerY = -0.2; // Center Y0

    circle[0] = centerX; // Center X
    circle[1] = centerY; // Center Y

    for (int i = 0; i <= CRES; i++)
    {
        // Convert degrees to radians for trigonometric functions
        float angle = (3.141592 / 180) * (i * 360 / CRES);
        circle[2 + 2 * i] = centerX + r * cos(angle);     // Xi 
        circle[2 + 2 * i + 1] = centerY + r * sin(angle); // Yi
    }

    // Create shader program
    shaderProgram = createShader("firelight.vert", "firelight.frag");

    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Destructor
FireLight::~FireLight() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}

// Render funkcija
void FireLight::render() {
    glUseProgram(shaderProgram);

    // Postavljanje uniformnih promenljivih za poziciju centra vatre
    GLint fireCenterLoc = glGetUniformLocation(shaderProgram, "fireCenter");
    glUniform2f(fireCenterLoc, -0.495f, -0.24f); // Na primer, centar u UV prostoru (0.5, 0.5)

    // Postavljanje uniformne promenljive za maksimalnu udaljenost
    GLint maxDistanceLoc = glGetUniformLocation(shaderProgram, "maxDistance");
    glUniform1f(maxDistanceLoc, 0.5f); // Podesite na odgovaraju?u vrednost za maksimalnu udaljenost

    // Postavljanje minimalne transparentnosti (pokušajte sa 0.01 za manju providnost)
    GLint minAlphaLoc = glGetUniformLocation(shaderProgram, "minAlpha");
    glUniform1f(minAlphaLoc, 0.3f); // Na primer, centar je gotovo providan

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, CRES + 2);
    glBindVertexArray(0);
    glUseProgram(0);
}