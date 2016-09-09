#version 330 core

// input data
in vec2 uv;

// Ouput data
out vec4 color;
uniform sampler2D textureColor;

void main()
{
	// Output
	color = texture(textureColor, uv);
}
