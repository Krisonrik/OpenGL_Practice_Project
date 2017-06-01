// create_a_window.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


void key_callback(GLFWwindow* window, int key, int, int action, int)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true,
    // closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


int main()
{
     //Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    ////glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

//        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    float r = 0.1f;
    float g = 0.5f;
    float b = 0.3f;
    float a = 1.0f;
    bool goUp = true;
    // Program loop
    while (!glfwWindowShouldClose(window))
    {
        // Check and call events
        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);

        // Rendering commands here

            glClearColor(r, g, b, a);
            glClear(GL_COLOR_BUFFER_BIT);

            if (r >= 1.0 || g >= 1.0 || b >= 1.0) {
                goUp = false;
            } else if (r <= 0 || g <= 0 || b <= 0) {
                goUp = true;
            }
            if (goUp) {
                r += 0.01f;
                g += 0.01f;
                b += 0.01f;
            }
            else {
                    r -= 0.01f;
                    g -= 0.01f;
                    b -= 0.01f;
            }

        // Swap the buffers
        glfwSwapBuffers(window);


    }



    glfwTerminate();
    return 0;
}

