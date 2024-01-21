#fragment
#version 330 core

out vec4 FragColor;
in vec2 texCoord;

uniform vec4 color;
uniform sampler2D texture_v;

void main()
{
   FragColor =  color *texture(texture_v,texCoord);
 };