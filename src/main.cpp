// system includes
#include <stdint.h>
#include <iostream>
#include <chrono>

// gl includes
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

int main()
{
    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL-Renderer", nullptr, nullptr);
    if(window == nullptr) {
        std::cout << "Failed to create window" << "\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failed to init glad" << "\n";
        return -1;
    }

    // criando um vertex shader
    const char* vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 pos; \n"
    "void main() { \n"
    "   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
    "}\0";

    // criando o id do vertexShader
    uint32_t vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // compilando o vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // checkando a compilação
    int vertexStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexStatus);

    // se falhar, joga no log
    if(vertexStatus == 0) {
        char log[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, log);
        std::cout << "ERROR: falied to compile the vertex shader, here's the log: \n" << log;
        return -1;
    }

    std::cout << "INFO: vertex shader compiled with sucess\n";

    // criando o pixel/fragment shader
    const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\0";

    // compilando o shader (de novo)
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // verificando de novo, por via das dúvidas
    int fragmentStatus;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentStatus);

    // se falhar, joga no log
    if(fragmentStatus == 0) {
        char log[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
        std::cout << "ERROR: falied to compile the fragment shader, here's the log: \n" << log;
        return -1;
    }

    std::cout << "INFO: fragment shader compiled with sucess\n";

    // criando o shader program
    uint32_t shaderProgram;
    shaderProgram = glCreateProgram();

    // linkando os shaders num só programa
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    std::cout << "INFO: shader program linked with sucess\n";

    // checkando se não deu merda na linkagem
    int linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);

    if(linkStatus == 0) {
        char log[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, log);
        std::cout << "ERROR: falied to link the shaders into a shader program, here's the log: \n" << log;
        return -1;
    }
    // deletando os shaders objects, já que não vão ser mais usados daqui pra frente
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // as coordernadas estão no sentido anti-horário
    // e todas estão normalizadas
    const float vertexData[] = {
        -0.5f,-0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f,  0.0f 
    };

    // criando o VAO (vertex array object), ele deve vir ANTES do bind no VBO
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);

    // criando um id pra o VBO (vertex buffer object)
    uint32_t VBO;
    glGenBuffers(1, &VBO);

    // dando bind no VAO, pra todos os vertex attribs serem ligados a ele
    glBindVertexArray(VAO);
    
    // vinculando o vbo no GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // copiando os dados para o buffer, da CPU para a GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // instruindo o opengl a como utilizar nosso vertex data nos vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // depois de configurar tudo, podemos desplugar o VAO
    glBindVertexArray(0);

    // main render loop
    while(!glfwWindowShouldClose(window)) {

        if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // desenha!


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}