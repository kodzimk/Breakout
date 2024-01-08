#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>


#include"res/stb_image.h"

#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
   x;\
   ASSERT(GlLogCall(#x,__FILE__,__LINE__))

struct TexturePropr {
    int height, width,pp;
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


static ShaderProgramSource ParseShaders(const std::string&filepath,const std::string& filepathFrag)
{
    std::ifstream stream(filepath);


    enum class ShaderType {
        NONE = -1,VERTEX = 0,FRAGMENT = 1
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

    return { ss[0].str(),ss[1].str()};

}


int main()
{
    GLFWwindow* window;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    if (!glfwInit())
        return -1;


    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

  
    glfwMakeContextCurrent(window);

    if (!glewInit() == GLEW_OK)
        return -1;

    float block[] = {
        -0.5f,-0.5f,
        0.5f,-0.5f,
        0.5f,0.5f,
        -0.5f,0.5f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };


    float texture_coord[] = {
    0.0f,1.0f,
    1.0f,1.0f,
    1.0f,0.0f,
    0.0f,0.0f
    };


    unsigned int vbo;
    unsigned int vbo_color;
    unsigned int vao;

    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, block, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 4, texture_coord, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(1);
  

    const char* vertex_source =
        "#version 330 core\n"
        "layout(location = 0) in vec4 pos;\n"
        "layout(location = 1)in vec4 color;\n"
        "out vec4 u_Color;\n"
        "void main()\n"
        "{\n"
        "gl_Position = pos;\n"
        "u_Color = color;\n"
        "}\n";


    const char* fragment_source =
        "#version 330 core\n"
        "out vec4 FragColor;"
        "in vec4 u_Color;\n"
        "void main()\n"
        "{\n"
        "FragColor = u_Color;\n"
        "}\n";

    glfwSwapInterval(1);



    ShaderProgramSource source = ParseShaders("res/Vertex_Shader.vertex", "res/Frag_Shader.frag");

    unsigned int program = glCreateProgram();

    unsigned int vertex_shader, fragment_shader;

    const GLchar* a = source.VertexSource.c_str();
    const GLchar* b = source.FragmentSource.c_str();

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1,&a, nullptr);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1,&b, nullptr);
    glCompileShader(fragment_shader);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(program);

    stbi_set_flip_vertically_on_load(0);

    

    int width=0, height=0, pp = 0;
    unsigned char* buffer= stbi_load("niger.png", &width, &height, &pp, 0);

    unsigned int id = 0;

    GLCall(glGenTextures(1, &id));
    GLCall(glBindTexture(GL_TEXTURE_2D, id));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    glGenerateMipmap(GL_TEXTURE_2D);
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));


    stbi_image_free(buffer);

  
    int location = glGetUniformLocation(program, "u_Texture");
    GLCall(glActiveTexture(GL_TEXTURE0));
   
    GLCall(glUniform1i(location, 0));

   
    
    while (!glfwWindowShouldClose(window))
    {       
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBindTexture(GL_TEXTURE_2D,id);

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
       
 
        glfwSwapBuffers(window);

    
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}