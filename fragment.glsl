#version 330 core

out vec4 FragColor;
in vec3 normal;
in vec3 FragPos;  

uniform vec3 lightPos;  
uniform vec3 lightColor;
uniform vec3 color;
uniform vec3 viewPos;

float ambientStrength = 0.25f;
float specularStrength = 0.75f;
vec3 ambient = ambientStrength * lightColor;
vec3 norm = normalize(normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);  
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
vec3 specular = specularStrength * spec * lightColor;  
vec3 result = (ambient + diffuse + specular) * color;

void main() {
	FragColor = vec4(result, 1.0f);
}