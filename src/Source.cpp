#include"config.h"


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
      0.5f, -0.5f,   // bottom right
    -0.5f, -0.5f,  // bottom left
     0.0f,  0.5f
    };

    unsigned int VAO,VBO;
   

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    ShaderProgramSource source = ParseShaders("src/Vertex_Shader.vertex", "src/Frag_Shader.frag");
    std::cout << source.FragmentSource;

    const GLchar* vetrtex = source.VertexSource.c_str();
    const GLchar* fragment = source.FragmentSource.c_str();

    unsigned int program = glCreateProgram();

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


    glUseProgram(program);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);


        GLCall(glBindVertexArray(VAO));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}

