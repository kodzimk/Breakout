#include<math.h>

struct mat4 {
	float entries[16];
};

struct vector3 {
	float entries[3];
};

mat4 matrix_transform_translate(vector3 position);


mat4 matrix_tranform_rotation(vector3 position,float gradus);

mat4 look_at(vector3 from,vector3 to);

float dot(vector3 u, vector3 v);

mat4 projection_create(float fovy, float aspect, float near, float fear);

vector3 normalize(vector3 v);

vector3 cross(vector3 u, vector3 v);