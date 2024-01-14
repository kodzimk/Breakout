#fragment
#version 330 core

out vec4 color;


uniform vec3 firstLight;
uniform vec3 secondLight;

void main()
{
 float ambientStrength = 0.1;
 vec3 ambient = ambientStrength * firstLight;

 vec3 result = ambient *secondLight;
color = vec4(result,1.0f);
};