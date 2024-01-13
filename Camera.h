#include<math.h>
#include"res/Component/cameta_component.h"
#include"res/Component/transform_component.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>

class Camera
{
public:
	Camera(unsigned int program);
	~Camera();

public:
	unsigned int viewLocation;
	glm::vec3 global_up;
	bool update(GLFWwindow* window, glm::vec3& pos,CameraComponent& camera,glm::vec3& eulers);
};

