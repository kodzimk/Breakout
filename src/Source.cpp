#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Object {
    bool isAlive;
    glm::mat4 model;
    glm::vec3 position;
    glm::vec4 color;
};


#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
   x;\
   ASSERT(GlLogCall(#x,__FILE__,__LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);

}

static bool GlLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenglError] (" << error << ")" << function << " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}

#define HEIGHT 800
#define WIDTH  1000


struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};
static ShaderProgramSource ParseShaders(const std::string& filepath, const std::string& filepathFrag)
{
    std::ifstream stream(filepath);


    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("vertex") != std::string::npos)
        {
            type = ShaderType::VERTEX;

        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    stream.close();

    stream.open(filepathFrag);

    while (getline(stream, line))
    {
        if (line.find("#fragment") != std::string::npos)
        {
            type = ShaderType::FRAGMENT;

        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    const char* vertexS = ss[0].str().c_str();

    const char* vertexSS = ss[1].str().c_str();

    return { ss[0].str(),ss[1].str() };

}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    ShaderProgramSource src = ParseShaders("src/Vertex_Shader.vertex", "src/Frag_Shader.frag");
    const GLchar* vertexShaderSource = src.VertexSource.c_str();
    const GLchar* fragmentShaderSource = src.FragmentSource.c_str();
    std::cout << vertexShaderSource;
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    Object blocks[38];


        blocks[0].isAlive = true;
        blocks[0].position = { -4.5f,4.5f,0.0f };
        blocks[0].model = glm::mat4(1.f);
        blocks[0].model = glm::scale(blocks[0].model, glm::vec3(0.2f, 0.2f, 1.0f));
        blocks[0].model = glm::translate(blocks[0].model, blocks[0].position);
        blocks[0].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);


        for (int i = 1; i < 19; i++)
        {
            blocks[i].isAlive = true;
            blocks[i].position = { blocks[i-1].position.x + 0.5,blocks[i - 1].position.y,blocks[i - 1].position.z};
            blocks[i].model = glm::mat4(1.f);
            blocks[i].model = glm::scale(blocks[i].model, glm::vec3(0.2f, 0.2f, 1.0f));
            blocks[i].model = glm::translate(blocks[i].model, blocks[i].position);

            if(i % 2 == 0)
            blocks[i].color = glm::vec4( 0.0f, i*0.3f,  0.0f,1.0f);

            if (i % 3 == 0)
                blocks[i].color = glm::vec4(i* 0.1f,  0.0f,  0.0f, 1.0f);

            if (i % 5 == 0)
                blocks[i].color = glm::vec4( 0.0f, 0.0f,  i *0.4f, 1.0f);
        }

        blocks[19].isAlive = true;
        blocks[19].position = { -4.5f,3.5f,0.0f };
        blocks[19].model = glm::mat4(1.f);
        blocks[19].model = glm::scale(blocks[19].model, glm::vec3(0.2f, 0.2f, 1.0f));
        blocks[19].model = glm::translate(blocks[19].model, blocks[19].position);
        blocks[19].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

        for (int i = 20; i < 38; i++)
        {
            blocks[i].isAlive = true;
            blocks[i].position = { blocks[i - 1].position.x + 0.5f,blocks[i - 1].position.y,blocks[i - 1].position.z };
            blocks[i].model = glm::mat4(1.f);
            blocks[i].model = glm::scale(blocks[i].model, glm::vec3(0.2f, 0.2f, 1.0f));
            blocks[i].model = glm::translate(blocks[i].model, blocks[i].position);

            if (i % 2 == 0)
                blocks[i].color = glm::vec4(0.0f, i * 0.3f, 0.0f, 1.0f);

            if (i % 3 == 0)
                blocks[i].color = glm::vec4(i * 0.1f, 0.0f, 0.0f, 1.0f);

            if (i % 5 == 0)
                blocks[i].color = glm::vec4(0.0f, 0.0f, i * 0.4f, 1.0f);
        }

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

     
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); 

        for (int i = 0; i < 38; i++)
        {
            GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(blocks[i].model)));
            GLCall(glUniform4f(glGetUniformLocation(shaderProgram, "color"),blocks[i].color.x,blocks[i].color.y,blocks[i].color.z,blocks[i].color.a));
           
            if(blocks[i].isAlive)
              glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
      
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
