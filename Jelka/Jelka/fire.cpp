#include "Fire.h"
#include "helpers.h"
#include <iostream>

Fire::Fire() {
    float vertices[] = {
        // Pozicije       // Teksturne koordinate
        -0.67, -0.1,     0.0, 1.0,
        -0.67, -0.36,     0.0, 0.0,
        -0.32,  -0.36,     1.0, 0.0,
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

    //ucitavanje tekstura
    textureID1 = loadImageToTexture("fire1.png");
    textureID2 = loadImageToTexture("fire4.png");
    textureID3 = loadImageToTexture("fire3.png");

    
    unsigned textures[3] = { textureID1, textureID2, textureID3 };
    for (int i = 0; i < 3; i++) {
        //sve naredno je vezano za teksturu textures[i]
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        //skup manjih verzija teksture, ako je objekat predaleko da renderovanje bude dobro
        glGenerateMipmap(GL_TEXTURE_2D);
        //tekstura se uvija po s i po t, iz vertices druga dva parametra u jednom redu 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //ako koordinate izadju van opsega, tekstura
        //se ponavlja, GL_CLAMP_TO_EDGE razvuce ivicu...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //ako se tekstura smanjuje radi nearest tj koristi boje okolnih piksela da hendlujes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //isto ako se povecava
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    //ne vazi vise za tu teksturu
    glBindTexture(GL_TEXTURE_2D, 0);

    //ukljuci sejdere
    glUseProgram(shaderProgram);

    //prosledjujemo uniformne promenljive
    unsigned uTex1Loc = glGetUniformLocation(shaderProgram, "uTex1");
    unsigned uTex2Loc = glGetUniformLocation(shaderProgram, "uTex2");
    unsigned uTex3Loc = glGetUniformLocation(shaderProgram, "uTex3");

    glUniform1i(uTex1Loc, 0);  // tekstura 1 na GL_TEXTURE0
    glUniform1i(uTex2Loc, 1);  // tekstura 2 na GL_TEXTURE1
    glUniform1i(uTex3Loc, 2);  // tekstura 3 na GL_TEXTURE2

    glUseProgram(0);
}

Fire::~Fire() {
    glDeleteTextures(1, &textureID1);
    glDeleteTextures(1, &textureID2);
    glDeleteTextures(1, &textureID3);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}

void Fire::render(float time, float fireX, float fireY) {
    glUseProgram(shaderProgram);

    //ako ne pronadje uTime vraca -1
    unsigned uTimeLoc = glGetUniformLocation(shaderProgram, "uTime");
    //1f znaci uTimeLoc dobija 1 float vrednosr
    glUniform1f(uTimeLoc, time);

    unsigned firePosLoc = glGetUniformLocation(shaderProgram, "firePos");
    //ovde se prosledjuju dve float vrednosti, tj firePosLoc ima x i y koordinatu, u frag je to vec2
    glUniform2f(firePosLoc, fireX, fireY);

    unsigned fireRadiusLoc = glGetUniformLocation(shaderProgram, "fireRadius");
    glUniform1f(fireRadiusLoc, 0.2);

    //aktivira se teksturna jedinica
    glActiveTexture(GL_TEXTURE0);
    //vezuje se tekstura za nju i sad vezemo svaku za zasebnu i mozemo da prikazujemo u istom sejderu vise njih
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