// Hello_Triangle.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
// GLEW
//#define GLEW_STATIC
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



std::string readShader(std::string fileLocation) {
    std::ifstream inputSource;
    //    std::string fileLocation = "vertex_shader.glsl";
    inputSource.open(fileLocation);

    if (!inputSource) {
        std::cout << "open failed" << std::endl;
    }

    const std::string shaderInfo;
    //inputSource operator>> (std::string shaderInfo);

    return shaderInfo;
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


    float vertices1[] = {
        0.5f,  0.5f, 0.0f,
        0.75f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f
    };

    float vertices2[] = {
        -0.5f,  0.5f, 0.0f,
        -0.75f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f
    };
    // Vertex Indices array
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        3, 4, 5    // second triangle
    };

    // definition of vertex shader
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    // definition of fragment shader
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    const char *fragmentShaderSource_y = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "}\n\0";

    //  compiling the vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // compile the fragment shader

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // compile the fragment shader

    unsigned int fragmentShader_y;
    fragmentShader_y = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_y, 1, &fragmentShaderSource_y, NULL);
    glCompileShader(fragmentShader_y);

    // check if shader compiled successfully
    int  vert_success;
    int frag_success;
    char v_infoLog[512];
    char f_infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vert_success);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &frag_success);
    glGetShaderiv(fragmentShader_y, GL_COMPILE_STATUS, &frag_success);


    if (!vert_success || !frag_success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, v_infoLog);
        glGetShaderInfoLog(fragmentShader, 512, NULL, f_infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << v_infoLog << std::endl;
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << f_infoLog << std::endl;
    }
    else {
        std::cout << "SHADER::COMPILATION_SUCCEED\n" << std::endl;
    }

    unsigned int shaderProgram, shaderProgram_y;
    shaderProgram = glCreateProgram();
    shaderProgram_y = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glAttachShader(shaderProgram_y, vertexShader);
    glAttachShader(shaderProgram_y, fragmentShader_y);
    glLinkProgram(shaderProgram_y);


    int link_success;
    char link_infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &link_success);
    if (!link_success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, link_infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n" << link_infoLog << std::endl;
    }
    else {
        std::cout << "SHADER::LINKING_SUCCEED\n" << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader_y);


    // EBO setup
    unsigned int VBO1, VBO2, VAO1, VAO2;
    glGenVertexArrays(1, &VAO1);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO2);


    // ..:: Initialization code :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO1);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    // 3. copy our index array in a element buffer for OpenGL to use
    //glBindVertexArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    glBindVertexArray(VAO2);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    //glBindVertexArray(0);

    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    float r = 0.1f;
    float g = 0.5f;
    float b = 0.3f;
    float a = 1.0f;
    // Program loop
    while (!glfwWindowShouldClose(window))
    {
        // Check and call events
        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);

        // Rendering commands here
        // setup background color and clear color buffer
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        // use linked shader program and draw a triangle
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glBindVertexArray(0);

        glUseProgram(shaderProgram_y);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap the buffers
        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}

