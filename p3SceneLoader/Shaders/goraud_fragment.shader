#version 330 core
uniform vec3 u_color;
out vec4 FragColor;
in vec2 UV;
uniform sampler2D texture_diffuse;
in vec4 vertexColor;
in vec3 o_normal;
uniform bool sun;

void main()
{
	//FragColor = vec4(o_normal, 1.0);
	//FragColor = vertexColor;
	
	if (sun == true) {
		//FragColor = vertexColor;
		FragColor = texture(texture_diffuse, UV);
	}
	else {
		FragColor = texture(texture_diffuse, UV) * vertexColor;
	}
	//FragColor = vec3(u_color);
}