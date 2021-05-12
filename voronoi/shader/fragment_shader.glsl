#version 150 core

out vec4 outColor;
in vec3 f_color;
uniform bool click;

void main()
{	
	if (click) {
		outColor = vec4(f_color + vec3(0.03, 0.03, 0.03), 1.0);
	} else {
		outColor = vec4(f_color, 1.0);
	}
}
