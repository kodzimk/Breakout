#include"linear_algebros.h"

mat4 matrix_transform_translate(vector3 position)
{
	mat4 matrix;

	matrix.entries[0] = 1;
	matrix.entries[1] = 0;
	matrix.entries[2] = 0;
	matrix.entries[3] = 0;

	matrix.entries[4] = 0;
	matrix.entries[5] = 1;
	matrix.entries[6] = 0;
	matrix.entries[7] = 0;


	matrix.entries[8] = 0;
	matrix.entries[9] = 0;
	matrix.entries[10] = 1;
	matrix.entries[11] = 0;

	matrix.entries[12] = position.entries[0];
	matrix.entries[13] = position.entries[1];
	matrix.entries[14] = position.entries[2];
	matrix.entries[15] = 1;

	return matrix;
}

mat4 matrix_tranform_rotation(vector3 position ,float a)
{
	mat4 matrix;

	float gradus = a * 180 / 3.14;


	matrix.entries[0] = cos(gradus);
	matrix.entries[1] = sin(gradus);
	matrix.entries[2] = 0;
	matrix.entries[3] = 0;

	matrix.entries[4] = -sin(gradus);
	matrix.entries[5] = cos(gradus);
	matrix.entries[6] = 0;
	matrix.entries[7] = 0;


	matrix.entries[8] = 0;
	matrix.entries[9] = 0;
	matrix.entries[10] = 1;
	matrix.entries[11] = 0;

	matrix.entries[12] =  position.entries[0];
	matrix.entries[13] =  position.entries[1];
	matrix.entries[14] =  position.entries[2];
	matrix.entries[15] = 1;

	return matrix;
}

mat4 look_at(vector3 from, vector3 to)
{
	vector3 global_up = { 0.0f,0.0f,1.0f };

	vector3 f = {
		to.entries[0] - from.entries[0],
		to.entries[1] - from.entries[1],
		to.entries[2] - from.entries[2]
	};
	f = normalize(f);

	vector3 r = normalize(cross(f, global_up));
	vector3 u = normalize(cross(r, f));

	mat4 matrix;


	matrix.entries[0] = r.entries[0];
	matrix.entries[1] = u.entries[0];
	matrix.entries[2] = -f.entries[0];
	matrix.entries[3] = 0;

	matrix.entries[4] = r.entries[1];
	matrix.entries[5] = u.entries[1];
	matrix.entries[6] = -f.entries[1];
	matrix.entries[7] = 0;


	matrix.entries[8] = r.entries[2];
	matrix.entries[9] = u.entries[2];
	matrix.entries[10] = -f.entries[2];
	matrix.entries[11] = 0;

	matrix.entries[12] = -dot(r, from);
	matrix.entries[13] = -dot(u,from);
	matrix.entries[14] = dot(f,from);
	matrix.entries[15] = 1.0f;

	return matrix;
}

float dot(vector3 u, vector3 v)
{
	return u.entries[0] * v.entries[0] + u.entries[1] * v.entries[1] + u.entries[2] * v.entries[2];
}

vector3 normalize(vector3 v)
{
	vector3 u;

	float magnitude = sqrtf(dot(v, v));

	u.entries[0] = v.entries[0] / magnitude;
	u.entries[1] = v.entries[1] / magnitude;
	u.entries[2] = v.entries[2] / magnitude;

	return u;
}

vector3 cross(vector3 u, vector3 v) {
	vector3 w;

	w.entries[0] = u.entries[1] * v.entries[2] - u.entries[2] * v.entries[1];
	w.entries[1] = -(u.entries[0] * v.entries[2] - u.entries[2] * v.entries[0]);
	w.entries[2] = u.entries[0] * v.entries[1] - u.entries[1] * v.entries[0];

	return w;
}

mat4 projection_create(float fovy, float aspect, float near, float fear) {
	fovy = fovy * 3.14 / 360.f;

	float t = tanf(fovy);
	float n = -near;
	float f = -fear;
	
	mat4 matrix;

	for (int i = 0; i < 16; ++i)
	{
		matrix.entries[i] = 0.0f;
	}

	matrix.entries[0] = 1.0f / (aspect * t);
	matrix.entries[5] = 1.0f / t;
	matrix.entries[10] = -(n + f) / (n - f);
	matrix.entries[11] = -1.0f;
	matrix.entries[14] = 2 * n * f / (n - f);

	return matrix;
}