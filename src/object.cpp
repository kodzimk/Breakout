#include "object.h"

Object::Object()
{
}

Object::Object(glm::vec3 startPosition, unsigned int index, unsigned int program)
{
  

    model = glm::mat4(1.0f);
    model = glm::translate(model, startPosition);
   
}

Object::~Object()
{
    glDeleteBuffers(1, &buffer);
}

bool Object::isCollide()
{
    return false;
}
