#include"Shaders/config.h"

class Object
{
public:
	Object();
	Object(glm::vec3 startPosition,unsigned int index,unsigned int program);
	~Object();

	bool isCollide();

public:
	unsigned int id;
	unsigned int buffer;
	bool inGame;
	glm::mat4 model;
	glm::vec3 color;
};

