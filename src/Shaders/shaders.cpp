#include "shaders.h"



shaders::shaders()
{

}

shaders::~shaders()
{
    glDeleteProgram(this->program);
}

void shaders::useProgram()
{
    glUseProgram(this->program);
}
