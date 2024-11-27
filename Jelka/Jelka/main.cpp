//Autor: Sanja Radić, RA 14/2021


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
#include <vector>


bool showText = false; // Flag to toggle text visibility
void handleInput(GLFWwindow* window);

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
    TextRenderer textrenderer("CENTURY.TTF", 1500, 800);

    while (!glfwWindowShouldClose(window))
    {
        handleInput(window);
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
                textrenderer.renderText(lines[i], startX, startY - i * lineHeight, 0.7f, glm::vec3(1.0f, 0.5f, 0.5f)
                );
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