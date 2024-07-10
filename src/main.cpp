// system includes
#include <cstdint>
#include <iostream>
#include <stdint.h>

// gl includes
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

// shaders
const static char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 pos; \n"
    "void main() { \n"
    "   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
    "}\0";

const static char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\0";

static uint32_t shaderProgram;

bool buildShaders() {
  // creating the obj
  uint32_t vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // compiling
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  // verifying the shader compilation
  int vertexStatus;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexStatus);

  // log if it fails
  if (vertexStatus == 0) {
    char log[512];
    glGetShaderInfoLog(vertexShader, 512, nullptr, log);
    std::cout
        << "ERROR: falied to compile the vertex shader, here's the log: \n"
        << log;
    return false;
  }

  std::cout << "INFO: vertex shader compiled with sucess\n";

  // same thing again
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  int fragmentStatus;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentStatus);

  if (fragmentStatus == 0) {
    char log[512];
    glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
    std::cout
        << "ERROR: falied to compile the fragment shader, here's the log: \n"
        << log;
    return false;
  }

  std::cout << "INFO: fragment shader compiled with sucess\n";

  // creating the main shader program
  shaderProgram = glCreateProgram();

  // linking all shaders
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  std::cout << "INFO: shader program linked with sucess\n";

  // verifying the process
  int linkStatus;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);

  if (linkStatus == 0) {
    char log[512];
    glGetProgramInfoLog(shaderProgram, 512, nullptr, log);
    std::cout << "ERROR: falied to link the shaders into a shader program, "
                 "here's the log: \n"
              << log;
    return false;
  }

  // deleting the shader objs
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return true;
}

// input data
const float dataA[] = {

  // first triangle
      -0.5f, 0.5f, 0.0f,
      -0.5f, 0.0f, 0.0f,
      0.0f, 0.5f, 0.0f,
};

const float dataB[] =  {
        
  // second triangle
      0.5f, -0.5f, 0.0f,
      0.5f, 0.0f, 0.0f,
      0.0f, -0.5f, 0.0f,
};

  // unsigned int indices[] = {
  //     0, 1, 3, // first triangle
  //     1, 2, 3  // second triangle
  // };

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() {
  // glfw init
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "OpenGL-Renderer", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create window"
              << "\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad init
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "failed to init glad"
              << "\n";
    return -1;
  }

  buildShaders();

  // "A" side:
  unsigned int aVAO, aVBO;

  glGenVertexArrays(1, &aVAO); // needs to be the first
  glGenBuffers(1, &aVBO);

  // binding the main VAO for the scene
  glBindVertexArray(aVAO);

  // bind and copy the vertices
  glBindBuffer(GL_ARRAY_BUFFER, aVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(dataA), dataA, GL_STATIC_DRAW);

  // setting the attrib layout
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // unbind the the VAO
  glBindVertexArray(0);

  // "B" side 
  unsigned int bVAO, bVBO;

  glGenVertexArrays(1, &bVAO); // needs to be the first
  glGenBuffers(1, &bVBO);

  // binding the main VAO for the scene
  glBindVertexArray(bVAO);

  // bind and copy the vertices
  glBindBuffer(GL_ARRAY_BUFFER, bVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(dataB), dataB, GL_STATIC_DRAW);

  // setting the attrib layout
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // unbind the the VAO
  glBindVertexArray(0);

  // main render loop
  while (!glfwWindowShouldClose(window)) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, true);
    }

    glClearColor(0.4f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(aVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(bVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &aVAO);
  glDeleteBuffers(1, &aVBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}