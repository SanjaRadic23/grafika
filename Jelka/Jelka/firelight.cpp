#include "FireLight.h"
#include "helpers.h"
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
#define CRES 30 //broj tacaka koje se koriste za crtanje kruga, mozda dovoljno za lep i gladak prikaz

FireLight::FireLight() {
    //svih 30 segmenata plus ukljucujuci srediste i prvu i poslednju tacku koje se spajaju u jednu da se oformi krug
    //puta 2 jer ce svaki taj segment imat x,y koordinatu
    float circle[(CRES + 2) * 2];
    float r = 0.35; // radius
    float centerX = -0.5; 
    float centerY = -0.2;

    circle[0] = centerX; //dodamo u niz centar
    circle[1] = centerY;

    for (int i = 0; i <= CRES; i++)
    {
        //tacke na ivici kruga
        //matematicke funkcije rade sa radijanima
        float angle = (3.141592 / 180) * (i * 360 / CRES);
        circle[2 + 2 * i] = centerX + r * cos(angle);   
        circle[2 + 2 * i + 1] = centerY + r * sin(angle); 
    }

    shaderProgram = createShader("firelight.vert", "firelight.frag");
    //kreiramo jedan vao i smestamo ga u prom vao
    glGenVertexArrays(1, &VAO);
    //ovo znaci da je ovaj vao sad aktivan i svaki definisan vbo nakon ovoga pripada njemu
    glBindVertexArray(VAO);

    //jedan vbo i smestamo u prom vbo
    glGenBuffers(1, &VBO);
    //vbo je vezan sad za buffer, nadalje je sve povezano sa ovim vbo i puni se ovaj buffer 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //punimo taj buffer, zauzimamo koliko zauzimaju 32 tacke, prosledjujemo sve tacke i kazemo da se podaci retko menjaju
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);

    //e sad odredjujemo kako se interpretiraju vbo podaci 
    //0-pozicija, 2-svaki atribut ima x i y, tip je float, ne treba da normalizujemo podatke, preskacemo 2 
    //floata da dodjemo do sledece tacke to nam je korak,podaci se citaju od pocetka niza i uzima x i y
    //zbog 2 na pocetku
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //omogucava upotrebu atributa 0 tj. pozicije tokom renderovanja
    glEnableVertexAttribArray(0);

    //odjavljujemo trenutan vao i vbo, vise se ne odnosi na njih
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

FireLight::~FireLight() {
    glDeleteBuffers(1, &VBO);//1 vbo na poziciji &vbo obrisi
    glDeleteVertexArrays(1, &VAO);//isto za vao
    glDeleteProgram(shaderProgram);//obrisi program za ucitane sejdere u ovom fajlu
}

void FireLight::render(float currentTime) {
    glUseProgram(shaderProgram); //koristi program ucitan sejderima iz fajla

    unsigned fireCenterLoc = glGetUniformLocation(shaderProgram, "fireCenter");
    glUniform2f(fireCenterLoc, -0.5, -0.25); 

    unsigned maxDistanceLoc = glGetUniformLocation(shaderProgram, "maxDistance");
    glUniform1f(maxDistanceLoc, 0.25); 

    unsigned minAlphaLoc = glGetUniformLocation(shaderProgram, "minAlpha");
    glUniform1f(minAlphaLoc, 0.1);

    unsigned time = glGetUniformLocation(shaderProgram, "time");
    glUniform1f(time, currentTime);

    //aktivira vao koji ima sve informacije o koordinatama i atributima kruga (maks 16)
    glBindVertexArray(VAO);
    //pristupa vbo i gleda kako da nacrta, spajamo fenom (ventilatorom), 0 pocetni indeks u vbo znaci od prvog x i y, 
    //koliko tacaka imamo cres 30 i 1 centar 1 koja zatvara krug, i pocetak i kraj
    glDrawArrays(GL_TRIANGLE_FAN, 0, CRES + 2);
    //iskljucuje trenutni vao
    glBindVertexArray(0);
    //iskljucuje trenutni shaderProgram
    glUseProgram(0);
}