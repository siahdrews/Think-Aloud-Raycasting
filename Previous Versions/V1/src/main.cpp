/**
 * Name: main.cpp
 * Author: Josiah Andrews
 * Created: 2/1/24
 * Purpose: basic OpenGL program
*/

// includes header files
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// declares standard namespace
using namespace std; 

// callback function to set the viewport size whenever framebuffer size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// main function
int main()
{
    // init glfw, if fails, print error messoge
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    // sets GLFW window hints
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // creates window with specified size and title
    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "itfinallyworks", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        return -1;
    }

    // sets openGL context to newly created window and init GLAD
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // sets initial viewport and specifies framebuffer size
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // mainloop continuously swaps the front and back buffers and polls events until the window should close
    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // glfw is terminated, and program returns 0
    glfwTerminate();
    return 0;
}