// Autor: Sanja Radić, RA 14/2021

#define _CRT_SECURE_NO_WARNINGS //ignorise warninge rizicnih funkcija
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h> //za OpenGL naredbe
#include <GLFW/glfw3.h> //za pravljenje i otvaranje prozora
#include "helpers.h"
#include "background.h"
#include "tree.h"
#include "fireplace.h"
#include "shelf.h"
#include "clock.h"
#include "textrenderer.h"
#include "fire.h"
#include "firelight.h"
#include "lamps.h"
#include "present.h"
#include "present2.h"
#include "present3.h"
#include "present4.h"
#include "books.h"
#include <vector>
#include <ctime> 

float brightness = 1.0f; 
bool showText = false; 
bool showTimeText = false;
int mode = 0;
void handleInput(GLFWwindow* window);
void handleInputClock(GLFWwindow* window, float clockX, float clockY, float clockWidth, float clockHeight);
bool isMouseOverClock(float mouseX, float mouseY, float clockX, float clockY, float clockWidth, float clockHeight);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void handleInputNumber1(GLFWwindow* window);
void handleInputNumber2(GLFWwindow* window);


std::string getCurrentTime() {

    std::time_t now = std::time(0);
    std::tm* localTime = std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);

    return std::string(buffer);
}

int main(void)
{
    float lastTime = glfwGetTime();  // pocetak rada aplikacije
    float deltaTime = 0.0f;          // razlika izmedju dva frejma
    float frameTime = 1.0f / 60.0f;  // oko 16.666667 ms

    // Pokretanje GLFW biblioteke
    // Nju koristimo za stvaranje okvira prozora
    if (!glfwInit()) // !0 == 1; glfwInit inicijalizuje GLFW i vrati 1 ako je inicijalizovana uspjesno, a 0 ako nije
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // OpenGL 3.3 i to je programabilni pipeline, unified shaders
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window; //Mjesto u memoriji za prozor
    unsigned int wWidth = 1500;
    unsigned int wHeight = 800;
    const char wTitle[] = "[Christmas tree]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    // glfwCreateWindow( sirina, visina, naslov, monitor na koji ovaj prozor ide preko citavog ekrana (u tom slucaju umjesto NULL ide glfwGetPrimaryMonitor() ), i prozori sa kojima ce dijeliti resurse )
    float clockX = wWidth / 2 + 10;
    float clockY = wHeight / 2 + 247;
    float clockWidth = 100;
    float clockHeight = 50;
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    //prozor je aktivan
    glfwMakeContextCurrent(window);
    //dozvoljava setovanje velicine pointa u vertex shaderu
    glEnable(GL_PROGRAM_POINT_SIZE);

    if (glewInit() != GLEW_OK)  //Slicno kao glfwInit. GLEW_OK je predefinisani izlazni kod za uspjesnu inicijalizaciju sadrzan unutar biblioteke
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    Background background;
    Tree tree;
    Fireplace fireplace;
    Shelf shelf;
    Clock clock;
    Fire fire;
    FireLight firelight;
    Lamps lamps;
    Present present;
    Present2 present2;
    Present3 present3;
    Present4 present4;
    Books books;
    TextRenderer textrenderer("CENTURY.TTF", 1500, 800);

    while (!glfwWindowShouldClose(window)) //kad ubijemo prozor zavrsava se beskonacna petlja
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        if (deltaTime >= frameTime)
        {
            handleInput(window);
            handleInputClock(window, clockX, clockY, clockWidth, clockHeight);
            handleInputNumber1(window);
            handleInputNumber2(window);

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            //ciscenje pozadine (boji u sivo)
            glClearColor(0.5, 0.5, 0.5, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            background.render();
            tree.render();
            fireplace.render();
            shelf.render();
            clock.render();
            present.render();
            present3.render();
            present4.render();
            books.render();

            float currentTime = glfwGetTime();
            float fireX = -0.5f; 
            float fireY = -0.25f; 
            fire.render(currentTime, fireX, fireY);

            firelight.render(currentTime);

            if(mode == 1)
                lamps.render(currentTime, 1, 1.0);
            else if(mode == 2)
                lamps.render(currentTime, 2, 1.0);
            else
                lamps.render(currentTime, 0, 1.0);

            if (showTimeText) {
                glfwSetScrollCallback(window, scroll_callback);

                std::string currentTimeStr = getCurrentTime();
                //glm za rad sa matematikom u grafici, vektor sa tri parametra
                glm::vec3 textColor = glm::vec3(brightness, brightness, brightness);

                textrenderer.renderText(currentTimeStr, wWidth / 2 + 13, wHeight / 2 + 257, 0.46f, textColor);

            }

            if (showText) {
                std::vector<std::string> lines = {
                    "Controls:",
                    "Key 1: Switch to first blinking mode for tree lights.",
                    "Key 2: Switch to second blinking mode for tree lights.",
                    "Mouse Click on Clock: Toggle the clock display on / off.",
                    "Mouse Scroll Wheel: Adjust the brightness of the clock display."
                };

                float startX = wWidth / 2 - 500; 
                float startY = wHeight / 2 - 250; 

                float lineHeight = 30.0f;
                //startY - i * lineHeight svaka sledeca linija ide ispod za 30 i mnozi se sa i jer ako je 3. linija ici ce 3*30 dole
                for (int i = 0; i < lines.size(); ++i) {
                    textrenderer.renderText(lines[i], startX, startY - i * lineHeight, 0.7f, glm::vec3(1.0f, 0.5f, 0.5f));
                }
            }

            //std::cout << frameTime << std::endl;
            lastTime = currentTime;
        }
        else {
            //std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;
        }
        //Zamjena vidljivog bafera sa pozadinskim
        //sve se pise u pozadinskom baferu i kad se zavrsi se prikaze
        glfwSwapBuffers(window);
        //Hvatanje dogadjaja koji se ticu okvira prozora (promjena velicine, pomjeranje itd)
        //red cekanja za dogadjaje
        glfwPollEvents();
    }
    //Sve OK - batali program
    glfwTerminate();
    return 0;
}

void handleInput(GLFWwindow* window) {
    static bool keyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        if (!keyPressed) {
            showText = !showText;
            keyPressed = true;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
        keyPressed = false;
    }
}

void handleInputClock(GLFWwindow* window, float clockX, float clockY, float clockWidth, float clockHeight) {
    static bool keyPressed = false;
    double mouseX, mouseY;

    glfwGetCursorPos(window, &mouseX, &mouseY);
    mouseY = 800 - mouseY; //da bi se usladilo da donji levi ugao bude 0,0

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!keyPressed) {
            if (isMouseOverClock(mouseX, mouseY, clockX, clockY, clockWidth, clockHeight)) {
                showTimeText = !showTimeText;
                keyPressed = true;
            }
        }
    }
    else if (glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        keyPressed = false;
    }
}

bool isMouseOverClock(float mouseX, float mouseY, float clockX, float clockY, float clockWidth, float clockHeight) {
    return (mouseX >= clockX && mouseX <= clockX + clockWidth &&
        mouseY >= clockY && mouseY <= clockY + clockHeight);
}



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    std::cout << "Scroll detected: " << yoffset << std::endl;

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    mouseY = 800 - mouseY;

    float clockX = 1500 / 2 + 10; 
    float clockY = 800 / 2 + 247;
    float clockWidth = 100;
    float clockHeight = 50;

    
    if (mouseX >= clockX && mouseX <= clockX + clockWidth &&
        mouseY >= clockY && mouseY <= clockY + clockHeight) {
       
        if (yoffset > 0) {
            brightness += 0.05f;  
        }
        else if (yoffset < 0) {
            brightness -= 0.05f;
        }

        
        if (brightness > 1.0f) {
            brightness = 1.0f;
        }
        else if (brightness < 0.15f) {
            brightness = 0.15f;
        }

        std::cout << "Brightness: " << brightness << std::endl;  
    }
}

void handleInputNumber1(GLFWwindow* window) {
    static bool keyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (!keyPressed) {
            if (mode == 1) {
                mode = 0; 
            }
            else {
                mode = 1; 
            }
            keyPressed = true;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) {
        keyPressed = false;
    }
}

void handleInputNumber2(GLFWwindow* window) {
    static bool keyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (!keyPressed) {
            if (mode == 2) {
                mode = 0; 
            }
            else {
                mode = 2; 
            }
            keyPressed = true;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) {
        keyPressed = false;
    }
}

