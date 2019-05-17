//__________________________
//author:beijinlin2016012070
//date:2019-05-17
//--------------------------
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShader1Source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 ourColor;\n"
    "uniform mat4 transform;\n"
    "void main(){\n"
    "    gl_Position = transform * vec4(aPos, 1.0);\n"
    "    ourColor = vec4(gl_Position.x + 0.5, gl_Position.y + 0.5, gl_Position.z + 0.5, gl_Position.x + gl_Position.y + gl_Position.z);\n"
    "}\n\0";
const char *vertexShader2Source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 ourColor;\n"
    "uniform float Color;\n"
    "uniform mat4 transform;\n"
    "void main(){\n"
    "    gl_Position = transform * vec4(aPos, 1.0);\n"
    "    ourColor = vec4(gl_Position.x + Color, Color, 0.0, 1.0 - Color);\n"
   // "    ourColor = Color;\n"
    "}\n\0";
const char *fragmentShader1Source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 ourColor;\n"
    "void main(){\n"
    "    FragColor = ourColor;\n"
    "}\n\0";
const char *fragmentShader2Source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 ourColor;\n"
    "void main(){\n"
    "    FragColor = ourColor;\n"
    "}\n\0";

//input about
//-----------
double rate = 0;
int i = 1;
float trans_x = 0.0f;
float trans_y = 0.0f;
float last_x = 0.0f;
float last_y = 0.0f;

int main(){
    //glfw: initialize and configure
    //-----------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw window creation
    //--------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My Work 2016012070", NULL, NULL);
    if (window == NULL){
       std::cout << "Failed to create GLFW window" << std::endl;
       glfwTerminate();
       return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad: load all OpenGL function pointers
    //---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //build and compile shader programs
    //---------------------------------
    unsigned int vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
    unsigned int vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgram1 = glCreateProgram();
    unsigned int shaderProgram2 = glCreateProgram();
    glShaderSource(vertexShader1, 1, &vertexShader1Source, NULL);
    glShaderSource(vertexShader2, 1, &vertexShader2Source, NULL);
    glShaderSource(fragmentShader1, 1, &fragmentShader1Source, NULL);
    glShaderSource(fragmentShader2, 1, &fragmentShader2Source, NULL);
    glCompileShader(vertexShader1);
    glCompileShader(vertexShader2);
    glCompileShader(fragmentShader1);
    glCompileShader(fragmentShader2);
    //link the two program objects
    //----------------------------
    glAttachShader(shaderProgram1, vertexShader1);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    glAttachShader(shaderProgram2, vertexShader2);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);

    //set up vertex data, buffers and configure vertex attributes
    //-----------------------------------------------------------
    float firstTriangle[] = {
        0.0, 2.0/3, 0.0f,
        -sqrt(3.0)/3, -1.0/3, 0.0f,
        sqrt(3.0)/3, -1.0/3, 0.0f,
        0.0, -2.0/3, 0.0f,
        -sqrt(3)/3, 1.0/3, 0.0f,
        sqrt(3)/3, 1.0/3, 0.0f,
    };
    float secondTriangle[] = {
        0.0, 2.0/9, 0.0f,
        -sqrt(3)/9, -1.0/9, 0.0f,
        sqrt(3)/9, -1.0/9, 0.0f,
        0.0, -2.0/9, 0.0f,
        -sqrt(3)/9, 1.0/9, 0.0f,
        sqrt(3)/9, 1.0/9, 0.0f,
    };
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    //the first triangle setting
    //--------------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //the second triangle setting
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //render loop
    //-----------
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)){
        //record last Time
        //----------------
        double interval = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        //input
        //-----
        processInput(window);

        //trans matrix
        //------------
        glm::mat4 trans1 = glm::mat4(1.0f);
        glm::mat4 trans2 = glm::mat4(1.0f);

        //render
        //------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw the first triangle
        //-----------------------
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOs[0]);
        //trans1 = glm::translate(trans1, glm::vec3(0.5f, -0.5f, 0.0f));
        //trans1 = glm::rotate(trans1, glm::radians((float)(interval) * 10), glm::vec3(0.0f, 0.0f, 1.0f));
        trans1 = glm::translate(trans1, glm::vec3(last_x, last_y, 0.0f));
        trans1 = glm::rotate(trans1, glm::radians((float)(lastTime*rate)), glm::vec3(0.0f, 0.0f, 1.0f));
        trans1 = glm::translate(trans1, glm::vec3(-1 * last_x, -1 * last_y, 0.0f));
        float scaleRate = 0.5 + 0.5 * abs(sin(lastTime));
        trans1 = glm::translate(trans1, glm::vec3(trans_x, trans_y, 0.0f));
        trans1 = glm::scale(trans1, glm::vec3(scaleRate, scaleRate, scaleRate));
        unsigned int transformLoc1 = glGetUniformLocation(shaderProgram1, "transform");
        glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(trans1));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //draw the second triangle
        //------------------------
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAOs[1]);
        float colorValue = scaleRate;
        unsigned int colorLoc = glGetUniformLocation(shaderProgram2, "Color");
        glUniform1f(colorLoc, colorValue);

        trans2 = glm::translate(trans2, glm::vec3(trans_x, trans_y, 0.0f));
        unsigned int transformLoc2 = glGetUniformLocation(shaderProgram1, "transform");
        glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(trans2));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //glfw:swap buffers and poll IO events
        //------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    //glfw: terminate
    //---------------
    glfwTerminate();
    return 0;
}

//process all input
//-----------------
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    //input rotate rate
    //-----------------
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        rate += 1.0f;
        std::cout << rate << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        rate -= 1.0f;
        std::cout << rate << std::endl;
    }
    //stop rotate
    //-----------
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
        rate = 0.0;
        std::cout << rate << std::endl;
    }

    last_x = trans_x;
    last_y = trans_y;
    //input translate rate
    //--------------------
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        trans_y += 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        trans_y -= 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        trans_x -= 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        trans_x += 0.001f;
    }

}

//glfw: whenever the window size changed
//--------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    int minLeng = width > height ? height : width;
    glViewport(0, 0, minLeng, minLeng);
}
