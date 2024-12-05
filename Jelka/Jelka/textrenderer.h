#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

// Struktura za karaktere fonta
struct Character {
    GLuint TextureID;  // ID teksture za karakter
    glm::ivec2 Size;   // velicina karaktera
    glm::ivec2 Bearing; // udaljenost od pocetne tacke do gornjeg levog ugla karaktera
    GLuint Advance;    // razmak do sledeceg karaktera
};

class TextRenderer {
public:
    TextRenderer(const std::string& fontPath, GLuint screenWidth, GLuint screenHeight);
    ~TextRenderer();
    void renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
    std::map<char, Character> Characters; 
    GLuint VAO, VBO, shaderProgram;   
    GLuint screenWidth, screenHeight;

    void loadFont(const std::string& fontPath); 
    void initRenderData();                  
};

