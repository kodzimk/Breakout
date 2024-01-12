#include<math.h>

struct mat4 {
	float entries[16];
};

struct vector3 {
	float entries[3];
};

mat4 matrix_transform_translate(vector3 position);


mat4 matrix_tranform_rotation(vector3 position,float gradus);