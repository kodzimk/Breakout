#fragment
#version 330 core

out vec4 color;

in vec2 tex_Coord;

uniform sampler2D u_Texture;

void main()
{
vec4 tex_color = texture(u_Texture,tex_Coord);
color = tex_color;
};