#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct CameraComponent {
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 forwards;
};