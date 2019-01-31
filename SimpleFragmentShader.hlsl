#version 330 core

//values from vertex shader
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 fragmentColor;

//output data
out vec3 color;

//constant values
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;

void main() {
	color = fragmentColor;
}