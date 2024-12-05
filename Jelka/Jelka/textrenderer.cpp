#include "TextRenderer.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "helpers.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

TextRenderer::TextRenderer(const std::string& fontPath, GLuint screenWidth, GLuint screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight) {
    initRenderData(); //postavljanje bafera i sejdera
    loadFont(fontPath); //ucitavanje prosledjenog fonta
}
//destruktor
TextRenderer::~TextRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void TextRenderer::initRenderData() {
    shaderProgram = createShader("text.vert", "text.frag");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //radimo sa karakterima i dovoljno je 6 verteksa da bi mogao svaki karakter da se nacrta, svako 4 parametra
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //ortogonalna projekcija, tekst prikazujemo kao 2d pa nam znear i zfar nisu potrebni
    //gleda se samo visina i sirina ekrana
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight));
    //rezultat ovoga je postavljanje 0,0 u donji levi ugao i ide se do sirine i visine
    glUseProgram(shaderProgram);
    //Prosledjujemo matricu u sejder
    //1-uniformna promenljiva, 2-jednu matricu saljemo, 3-matrica nije transponovana, 4-pokazivac na prvi el. matrice
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
}
void TextRenderer::loadFont(const std::string& fontPath) {
    FT_Library ft;
    //inicijalizacija biblioteke i postavljanje instance na nju
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Failed to initialize FreeType library" << std::endl;
        return;
    }

    FT_Face face;
    //ucitavanje fonta
    //0-neki fontovi mogu sadrzati vise razlicith u jednom fajlu, mi uzimamo prvi 
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Done_FreeType(ft);
        return;
    }

    //velicina fonta, sirina 48, visina 0 jer ce se prilagoditi sirini
    FT_Set_Pixel_Sizes(face, 0, 48);

    //1 omogucava ucitavanje tekstura bilo koje sirine
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    //ascii
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load character " << c << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        //glyph karakter, gl_red jer je sve crno belo, nema mipmapa, bitmap.buffer podaci svakog piksela
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //razvlaci teksturu
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_LINEAR glatko skaliranje
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    for (char c : text) {
        Character ch = Characters[c];

        GLfloat xpos = x + ch.Bearing.x * scale; //donji levi ugao karaktera
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale; //sirina i visina karaktera
        GLfloat h = ch.Size.y * scale;

        float vertices[6][4] = { //dva trougla koja prave pravougaonik, 6 temena -> 2 trougla
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale; // pravimo razmak izmedju karaktera
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}