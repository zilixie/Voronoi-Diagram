#version 150 core
#define PI 3.1415926538

in vec4 position;
out vec3 f_color;
uniform vec3 line_col;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = view * model * vec4(position[0], position[1], position[2], 1.0);
	f_color = line_col;
}