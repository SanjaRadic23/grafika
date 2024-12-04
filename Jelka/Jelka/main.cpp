// Autor: Sanja Radić, RA 14/2021

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include "helpers.h"
#include "background.h"
#include "tree.h"
#include "fireplace.h"
#include "shelf.h"
#include "clock.h"
#include "textrenderer.h"
#include "fire.h"
#include "firelight.h"
#include <vector>
#include <ctime>  // Za rad sa vremenom

// Globalne promenljive koje ćemo koristiti za svetlost (ili bilo koju drugu vrednost)
float brightness = 1.0f;  // Početna svetlost (vrednost može biti između 0.0 i 1.0)
bool showText = false; // Flag to toggle text visibility
bool showTimeText = false;
void handleInput(GLFWwindow* window);
void handleInputClock(GLFWwindow* window, float clockX, float clockY, float clockWidth, float clockHeight);
bool isMouseOverClock(float mouseX, float mouseY, float clockX, float clockY, float clockWidth, float clockHeight);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// Funkcija za dobavljanje trenutnog vremena u formatu HH:MM:SS
std::string getCurrentTime() {
    // Dobijanje trenutnog vremena
    std::time_t now = std::time(0);
    std::tm* localTime = std::localtime(&now);

    // Formatiranje vremena u HH:MM:SS format
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);

    return std::string(buffer);
}

int main(void)
{
    
    if (!glfwInit())
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 1500;
    unsigned int wHeight = 800;
    const char wTitle[] = "[Christmas tree]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    float clockX = wWidth / 2 + 10;        // X pozicija (početak kvadrata)
    float clockY = wHeight / 2 + 247;       // Y pozicija (početak kvadrata)
    float clockWidth = 100;   // Širina kvadrata
    float clockHeight = 50;  // Visina kvadrata
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
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
    TextRenderer textrenderer("CENTURY.TTF", 1500, 800);

    while (!glfwWindowShouldClose(window))
    {
        handleInput(window);
        handleInputClock(window, clockX, clockY, clockWidth, clockHeight);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        background.render();
        tree.render();
        fireplace.render();
        shelf.render();
        clock.render();
        float currentTime = glfwGetTime();
        // Pozicija vatre (prilagodite prema potrebi)
        float fireX = -0.5f;  // X koordinata
        float fireY = -0.25f; // Y koordinata
        // Renderovanje vatre
        fire.render(currentTime, fireX, fireY);
        firelight.render();

        if (showTimeText) {
            glfwSetScrollCallback(window, scroll_callback);
            // Render current time
            std::string currentTimeStr = getCurrentTime();
            glm::vec3 textColor = glm::vec3(brightness, brightness, brightness);

            textrenderer.renderText(currentTimeStr, wWidth / 2 + 13, wHeight / 2 + 257, 0.46f, textColor);

        }

        if (showText) {
            std::vector<std::string> lines = {
                "Controls:",
                "Key 1: Switch to slow blinking mode for tree lights.",
                "Key 2: Switch to fast blinking mode for tree lights.",
                "Mouse Click on Clock: Toggle the clock display on / off.",
                "Mouse Scroll Wheel: Adjust the brightness of the clock display."
            };

            // Set starting position for text rendering
            float startX = wWidth / 2 - 500;  // Center text horizontally
            float startY = wHeight / 2 - 250; // Start Y position

            // Set line height (spacing between lines)
            float lineHeight = 30.0f;

            // Render each line of text
            for (int i = 0; i < lines.size(); ++i) {
                textrenderer.renderText(lines[i], startX, startY - i * lineHeight, 0.7f, glm::vec3(1.0f, 0.5f, 0.5f));
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void handleInput(GLFWwindow* window) {
    static bool keyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        if (!keyPressed) {
            showText = !showText; // Toggle text display
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

    glfwGetCursorPos(window, &mouseX, &mouseY); // Dobavi poziciju miša
    mouseY = 800 - mouseY; // Obrni Y koordinatu, jer GLFW koristi drugačiji sistem

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!keyPressed) {
            if (isMouseOverClock(mouseX, mouseY, clockX, clockY, clockWidth, clockHeight)) {
                showTimeText = !showTimeText; // Toggle prikaz vremena
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
    std::cout << "Scroll detected: " << yoffset << std::endl;  // Ispis u konzoli

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);  // Dobavljanje pozicije miša

    mouseY = 800 - mouseY; // Obrni Y koordinatu, jer GLFW koristi drugačiji sistem (y = 0 na dnu)

    float clockX = 1500 / 2 + 10;        // X pozicija (početak kvadrata)
    float clockY = 800 / 2 + 247;       // Y pozicija (početak kvadrata)
    float clockWidth = 100;   // Širina kvadrata
    float clockHeight = 50;  // Visina kvadrata

    // Proveravamo da li je miš unutar okvira sata
    if (mouseX >= clockX && mouseX <= clockX + clockWidth &&
        mouseY >= clockY && mouseY <= clockY + clockHeight) {
        // Ako jeste, menjamo svetlost u zavisnosti od pomeranja skrola
        if (yoffset > 0) {
            brightness += 0.05f;  // Povećavamo svetlost
        }
        else if (yoffset < 0) {
            brightness -= 0.05f;  // Smanjujemo svetlost
        }

        // Ograničavamo svetlost da bude između 0.0 i 1.0
        if (brightness > 1.0f) {
            brightness = 1.0f;
        }
        else if (brightness < 0.15f) {
            brightness = 0.15f;
        }

        std::cout << "Brightness: " << brightness << std::endl;  // Ispis trenutne svetlosti
    }
}

