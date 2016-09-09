#version 330 core

// input data
smooth in vec3 smoothColor;

// Ouput data
out vec4 color;

void main()
{
	// Output
	color = vec4(smoothColor, 1.0f);
}
