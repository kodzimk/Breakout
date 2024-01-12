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