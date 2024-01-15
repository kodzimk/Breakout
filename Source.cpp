#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Camera.h"

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

    float vertices[] =
    {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int vbo;
    unsigned int vao;

    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3* sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));


    glfwSwapInterval(1);


    ShaderProgramSource source = ParseShaders("res/Vertex_Shader.vertex", "res/Frag_Shader.frag");
    ShaderProgramSource source2 = ParseShaders("res/lamp.vs", "res/lamp.frag");

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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(program);

    unsigned int lamp = glCreateProgram();

    unsigned int vertex_shader2, fragment_shader2;

    const GLchar* a2 = source2.VertexSource.c_str();
    const GLchar* b2 = source2.FragmentSource.c_str();

    vertex_shader2 = glCreateShader(GL_VERTEX_SHADER);
    GLCall(glShaderSource(vertex_shader2, 1, &a2, nullptr));
    GLCall(glCompileShader(vertex_shader2));

    fragment_shader2 = glCreateShader(GL_FRAGMENT_SHADER);
    GLCall(glShaderSource(fragment_shader2, 1, &b2, nullptr));
    GLCall(glCompileShader(fragment_shader2));

    glAttachShader(lamp, vertex_shader2);
    glAttachShader(lamp, fragment_shader2);

    glLinkProgram(lamp);

    glDeleteShader(vertex_shader2);
    glDeleteShader(fragment_shader2);

    Camera camera(800.f,600.f,{0.0f,0.0f,2.0f});
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, { 0.f,0.5f,0.f });

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model, { 2.f,0.5f,1.f });
    

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

   GLCall(glUniform3fv(glGetUniformLocation(program, "lightColor"),1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f))));
   GLCall(glUniform3fv(glGetUniformLocation(program, "objectColor"), 1,glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f))));

   GLCall(glUniform3fv(glGetUniformLocation(program, "lightPos"), 1,glm::value_ptr(glm::vec3(lightPos.x,lightPos.y,lightPos.z))));
   GLCall(glUniform3fv(glGetUniformLocation(program, "viewPos"),1,glm::value_ptr(glm::vec3(camera.Position.x,camera.Position.y,camera.Position.z))));

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        camera.Inputs(window);
        camera.Matrix(45.f,0.1f,100.f,program,"camera");
      

        glBindVertexArray(vao);
        GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model)));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));


        glUseProgram(lamp);
        camera.Matrix(45.f, 0.1f, 100.f, program, "camera");
        GLCall(glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model2)));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));




        glfwSwapBuffers(window);


        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;

}

