#version 330 core
layout(location = 0) in vec3 a_vertex;

uniform mat4 u_view;
uniform mat4 u_projection;
out vec3 TexCoords;

void main()
{
	TexCoords = a_vertex;
	gl_Position = u_projection * u_view * vec4(a_vertex, 1.0);
	
}