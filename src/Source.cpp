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


#include"stb_image.h"

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

std::vector<glm::vec3> verticesC; 

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
        verticesC.push_back(temp[0]);
        verticesC.push_back(temp[i + 1]);
        verticesC.push_back(temp[i + 2]);
    }
}


bool isEnemyCollide(glm::vec3 player,glm::vec3 object)
{

        glm::vec3 box1right = glm::vec3(1.0f);
        glm::vec3 box1Left = glm::vec3(1.0f);

        glm::vec3 box2right  = glm::vec3(1.0f);
        glm::vec3 box2Left = glm::vec3(1.0f);

        box1right.x = player.x;
        box1right.y = player.y;

        box1Left.x = player.x + 0.1f;
        box1Left.y = player.y + 0.1f;

        box2right.x = object.x;
        box2right.y = object.y;

        box2Left.x = 1.f + object.x;
        box2Left.y = 1.2f + object.y;

        if (box2right.x <= box1Left.x &&
            box2Left.x >= box1right.x &&
            box2right.y <= box1Left.y &&
            box2Left.y >= box1right.y)
        {
            return true;
        }
    
    return false;
}

bool isObjectCollide(glm::vec3 player, glm::vec3 object)
{

    glm::vec3 box1right = glm::vec3(1.0f);
    glm::vec3 box1Left = glm::vec3(1.0f);

    glm::vec3 box2right = glm::vec3(1.0f);
    glm::vec3 box2Left = glm::vec3(1.0f);

    box1right.x = player.x;
    box1right.y = player.y;

    box1Left.x = player.x + 0.1f;
    box1Left.y = player.y + 0.1f;

    box2right.x = object.x;
    box2right.y = object.y;

    box2Left.x = 1.f + object.x;
    box2Left.y = 0.2f + object.y;

    if (box2right.x <= box1Left.x &&
        box2Left.x >= box1right.x &&
        box2right.y <= box1Left.y &&
        box2Left.y >= box1right.y)
    {
        return true;
    }

    return false;
}



int main()
{
  
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
         0.5f,  0.5f, 0.0f,  
         0.5f, -0.5f, 0.0f,  
        -0.5f, -0.5f, 0.0f,  
        -0.5f,  0.5f, 0.0f   
   };

    float texCoords[] = {
     1.0f, 1.1f, 
     1.0f, 0.0f, 
     0.0f, 0.0f,
     0.0f,1.0f   
    };

    unsigned int indices[] = {  
        0, 1, 3,  
        1, 2, 3   
    };

    unsigned int VBO, VAO, EBO,TEX;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &TEX);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, TEX);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    buildCircle(1, 120);

    unsigned int VAO2,VBO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) *verticesC.size(), verticesC.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    Object blocks[57];

    unsigned int texture1;
   
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));	
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
   
    unsigned char* data = stbi_load("res/textures/block.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    
        blocks[0].isAlive = true;
        blocks[0].position = { -9.5f,4.5f,0.0f };
        blocks[0].model = glm::mat4(1.f);
        blocks[0].model = glm::scale(blocks[0].model, glm::vec3(0.1f, 0.2f, 1.0f));
        blocks[0].model = glm::translate(blocks[0].model, blocks[0].position);
        blocks[0].color = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
        for (int i = 1; i < 19; i++)
        {
            blocks[i].isAlive = true;
            blocks[i].position = { blocks[i - 1].position.x + 1.f,blocks[i - 1].position.y,blocks[i - 1].position.z };
            blocks[i].model = glm::mat4(1.f);
            blocks[i].model = glm::scale(blocks[i].model, glm::vec3(0.1f, 0.2f, 1.0f));
            blocks[i].model = glm::translate(blocks[i].model, blocks[i].position);
            blocks[i].color = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
        }
        blocks[19].isAlive = true;
        blocks[19].position = { -9.5f,3.5f,0.0f };
        blocks[19].model = glm::mat4(1.f);
        blocks[19].model = glm::scale(blocks[19].model, glm::vec3(0.1f, 0.2f, 1.0f));
        blocks[19].model = glm::translate(blocks[19].model, blocks[19].position);
        blocks[19].color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);

        for (int i = 20; i < 38; i++)
        {
            blocks[i].isAlive = true;
            blocks[i].position = { blocks[i - 1].position.x + 1.f,blocks[i - 1].position.y,blocks[i - 1].position.z };
            blocks[i].model = glm::mat4(1.f);
            blocks[i].model = glm::scale(blocks[i].model, glm::vec3(0.1f, 0.2f, 1.0f));
            blocks[i].model = glm::translate(blocks[i].model, blocks[i].position);
            blocks[i].color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
        blocks[38].isAlive = true;
        blocks[38].position = { -9.5f,2.5f,0.0f };
        blocks[38].model = glm::mat4(1.f);
        blocks[38].model = glm::scale(blocks[38].model, glm::vec3(0.1f, 0.2f, 1.0f));
        blocks[38].model = glm::translate(blocks[38].model, blocks[38].position);
        blocks[38].color = glm::vec4(0.0f, 0.3f, 0.0f, 1.0f);
        for (int i = 39; i < 57; i++)
        {
            blocks[i].isAlive = true;
            blocks[i].position = { blocks[i - 1].position.x + 1.f,blocks[i - 1].position.y,blocks[i - 1].position.z };
            blocks[i].model = glm::mat4(1.f);
            blocks[i].model = glm::scale(blocks[i].model, glm::vec3(0.1f, 0.2f, 1.0f));
            blocks[i].model = glm::translate(blocks[i].model, blocks[i].position);

            blocks[i].color = glm::vec4(0.0f, 0.3f, 0.0f, 1.0f);
        }



        Object object;
        object.color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
        object.isAlive = true;
        object.position = glm::vec3(0.0f, -17.0f, 0.0f);
        object.model = glm::mat4(1.0f);
        object.model = glm::scale(object.model, glm::vec3(0.2f, 0.05, 1.0f));
        object.model = glm::translate(object.model, object.position);

        glm::vec3 playerActualPosition = glm::vec3(1.0f);
        glm::vec3 objectActualPosition = glm::vec3(1.0f);

      
        glUniform1i(glGetUniformLocation(shaderProgram, "texture_v"), texture1);
     

        Object player;

        player.color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
        player.isAlive = true;
        player.position = glm::vec3(0.0f, -15.5f, 0.0f);
        player.model = glm::mat4(1.0f);
        player.model = glm::scale(player.model, glm::vec3(0.05f, 0.05f, 1.0f));
        player.model = glm::translate(player.model, player.position);

        glm::vec3  playerPos = { 0.0f,0.0f,0.0f };

        bool isPressed = false;
        float speedY = 0.0f;
        float speedX = 0.0f;
        bool collide = false;
        
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

     
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); 
 
        for (int i = 0; i < 57; i++)
        {
            GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(blocks[i].model)));
            GLCall(glUniform4f(glGetUniformLocation(shaderProgram, "color"),blocks[i].color.x,blocks[i].color.y,blocks[i].color.z,blocks[i].color.a));
           
            if(blocks[i].isAlive)
              glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            if (isEnemyCollide(playerActualPosition, blocks[i].position)&&blocks[i].isAlive)
            {
                blocks[i].isAlive = false;
                speedY = -0.00003f;
                playerPos.y = -0.002f;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) > 0 && !isPressed)
        {
            isPressed = true;
            speedY = 0.00003f;
        }


        if (speedY > 0.0f || speedY < 0.0f) {

            playerPos.y = playerPos.y + speedY;

            player.model = glm::translate(player.model, playerPos);
            player.position += playerPos;
        }

        if (isObjectCollide(playerActualPosition, objectActualPosition))
        {
            playerPos.y = 0.001f;
            speedY = 0.00003f;
        }
        

        GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(object.model)));
        GLCall(glUniform4f(glGetUniformLocation(shaderProgram, "color"), object.color.x, object.color.y, object.color.z, object.color.a));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        playerActualPosition = glm::vec3(player.position.x / 2, player.position.y / 4, player.position.z);
        objectActualPosition = glm::vec3(object.position.x, object.position.y / 4, object.position.z);

        glBindVertexArray(VAO2);



      
        GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(player.model)));
        GLCall(glUniform4f(glGetUniformLocation(shaderProgram, "color"), player.color.x, player.color.y, player.color.z, player.color.a));
        glDrawArrays(GL_TRIANGLES, 0, verticesC.size());

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
