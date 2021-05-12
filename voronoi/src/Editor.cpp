#include "Editor.h"

void Editor::init(void) {
	view = MatrixXd::Identity(4, 4);
	p0 = Vector2d(0,0);
	p1 = Vector2d(0,0);
	cur_col = Vector3d(191,51,46);
	quit = 0;
}


void Editor::update_camera(void) {
	double alpha = 0.0;
	double beta = 0.0;
	double radius = 9.0;
	Eigen::Vector3d cam_pos = radius * Vector3d(cos(alpha)*cos(beta), sin(beta), sin(alpha)*cos(beta));

	Eigen::Matrix4f camera_d, camera_pos;
	camera_d << cos(M_PI/2.0-alpha),   0, -sin(M_PI/2.0-alpha), 0,
				-cos(alpha)*sin(beta), cos(beta),  -sin(alpha)*sin(beta),   0,
				cos(alpha)*cos(beta),  sin(beta),  sin(alpha)*cos(beta),    0,
				0,                     0, 0,                      1;
	camera_pos << 1, 0, 0, -cam_pos(0), 
				0, 1, 0, -cam_pos(1), 
				0, 0, 1, -cam_pos(2), 
				0, 0, 0, 1;
	// camera = camera_d * camera_pos;
}

void Editor::update_view(GLFWwindow* window) {
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	width = double(w);
	height = double(h);
	double aspect_ratio = double(height)/double(width);
	if (view(0,0)/view(1,1) != aspect_ratio) {
		view(0,0) = aspect_ratio * view(1,1);
	}
}
