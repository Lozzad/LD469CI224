#version 330 core

// Input vertex data
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal_modelspace;

//Output data interpolated for each fragment
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 fragmentColor;

//constant values for whole mesh 
uniform mat4 MVP;	//mvp matrix
uniform mat4 V;		//view matrix?
uniform mat4 M;		//model matrix?
uniform vec3 LightPosition_worldspace;

void main() {
	//place the vertices
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

	fragmentColor = vertexColor;
}