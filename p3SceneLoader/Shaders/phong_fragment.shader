#version 330 core
uniform vec3 u_color;
out vec4 FragColor;
in vec3 FragPos;
in vec2 UV;
in vec3 Normal;
uniform sampler2D texture_diffuse;
uniform sampler2D texture2_diffuse;

uniform vec3 u_light_pos;
uniform vec3 u_camera_pos;
uniform vec3 u_ambient_color;
uniform bool multi;


void main()
{

	vec3 lightVector = normalize(u_light_pos - FragPos);

	float specularStrength = 1.0f;
	vec3 viewDir = normalize(u_camera_pos - FragPos);
	vec3 reflectDir = reflect(-lightVector, Normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 16);

	vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);

	vec3 diffuse = vec3(max(dot(Normal, lightVector), 0.0));

	if (multi == true) {
		FragColor = texture(texture_diffuse, UV) * vec4(u_ambient_color + specular + diffuse, 1.0) * texture(texture2_diffuse, UV);
	}
	else
		FragColor = texture(texture_diffuse, UV) * vec4(u_ambient_color + specular + diffuse, 1.0);

	//FragColor = texture(texture_diffuse, UV);


}