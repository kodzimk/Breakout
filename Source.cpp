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

#include"res/stb_image.h"
#include"res/linear_algebros.h"



#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
   x;\
   ASSERT(GlLogCall(#x,__FILE__,__LINE__))

struct TexturePropr {
    int height, width, pp;
    unsigned char* buffer;
    unsigned int id;
};

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

std::vector<glm::vec3> vertices;

void buildCircle(float radius, int vCount)
{
    float angle = 360.0f / vCount;

    int triangleCount = vCount - 2;

    std::vector<glm::vec3> temp;
    // positions
    for (int i = 0; i < vCount; i++)
    {
        float currentAngle = angle * i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * sin(glm::radians(currentAngle));
        float z = 0.0f;

        temp.push_back(glm::vec3(x, y, z));
    }

    for (int i = 0; i < triangleCount; i++)
    {
        vertices.push_back(temp[0]);
        vertices.push_back(temp[i + 1]);
        vertices.push_back(temp[i + 2]);
    }
}


int main()
{
    GLFWwindow* window;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit())
        return -1;


    window = glfwCreateWindow(1200, 1000, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);



    if (!glewInit() == GLEW_OK)
        return -1;

    float vertices2[] =
    {
       -1.f,1.f,
       -1.f,-1.0f,
       1.0f,-1.0f,
       -1.f,-1.0f,
       1.0f,-1.0f,
       1.0f,1.0f
    };

    unsigned int vbo;
    unsigned int vao;
    unsigned int vbo2;
    unsigned int vao2;



    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));


    GLCall(glGenBuffers(1, &vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW));
    

    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    GLCall(glEnableVertexAttribArray(0));

    buildCircle(0.1, 128);

    ShaderProgramSource source = ParseShaders("res/Vertex_Shader.vertex", "res/Frag_Shader.frag");

    unsigned int program = glCreateProgram();

    unsigned int vertex_shader, fragment_shader;

    const GLchar* a = source.VertexSource.c_str();
    const GLchar* b = source.FragmentSource.c_str();

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLCall(glShaderSource(vertex_shader, 1, &a, nullptr));
    GLCall(glCompileShader(vertex_shader));

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    GLCall(glShaderSource(fragment_shader, 1, &b, nullptr));
    GLCall(glCompileShader(fragment_shader));

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glUseProgram(program);


    GLCall(glGenVertexArrays(1, &vao2));
    GLCall(glBindVertexArray(vao2));

    GLCall(glGenBuffers(1, &vbo2));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo2));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW));


    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));
    GLCall(glEnableVertexAttribArray(0));


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
 
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

    
 
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        glUniform3fv(glGetUniformLocation(program, "Color"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
        GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model)));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 12));

  
        glBindVertexArray(vao2);
        glUniform3fv(glGetUniformLocation(program, "Color"), 1, glm::value_ptr(glm::vec3(1.f, 1.f, 0.0f)));
        GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model2)));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, vertices.size()));


        glfwSwapBuffers(window);


        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;

}

