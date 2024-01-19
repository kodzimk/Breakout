#include"object.h"
#include"stb_image.h"


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
    GLFWwindow* window;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit())
        return -1;


    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);


    if (!glewInit() == GLEW_OK)
        return -1;

    float vertex[] = {
     1.0f,1.0f,
     -1.0f,1.0f,
     -1.0f,-1.0f
    };

    unsigned int objectVao = 0;
    unsigned int program = 0;
    unsigned int buffer = 0;

    glGenVertexArrays(1, &objectVao);
    glBindVertexArray(objectVao);

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertex, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    ShaderProgramSource source = ParseShaders("src/Shaders/Vertex_Shader.vertex", "src/Shaders/Frag_Shader.frag");

    const GLchar* vetrtex = source.VertexSource.c_str();
    const GLchar* fragment = source.FragmentSource.c_str();

    program = glCreateProgram();

    unsigned int vertex_shader, fragment_shader;

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLCall(glShaderSource(vertex_shader, 1, &vetrtex, nullptr));
    GLCall(glCompileShader(vertex_shader));

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    GLCall(glShaderSource(fragment_shader, 1, &fragment, nullptr));
    GLCall(glCompileShader(fragment_shader));

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glUseProgram(program);
        GLCall(glBindVertexArray(objectVao));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));


        glfwPollEvents();
    }

  
    glfwTerminate();
    return 0;

}

