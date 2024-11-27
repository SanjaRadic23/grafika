#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

// Struktura za karaktere fonta
struct Character {
    GLuint TextureID;  // ID teksture za karakter
    glm::ivec2 Size;   // Veličina karaktera
    glm::ivec2 Bearing; // Offset od baze do gornjeg levog ugla
    GLuint Advance;    // Offset do sledećeg karaktera
};

class TextRenderer {
public:
    TextRenderer(const std::string& fontPath, GLuint screenWidth, GLuint screenHeight);
    ~TextRenderer();

    void renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
    std::map<char, Character> Characters; // Mapa karaktera
    GLuint VAO, VBO, shaderProgram;       // OpenGL objekti
    GLuint screenWidth, screenHeight;    // Dimenzije ekrana

    void loadFont(const std::string& fontPath); // Učitaj font
    void initRenderData();                      // Inicijalizuj OpenGL objekte
};

