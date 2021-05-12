#ifndef EDITOR_H
#define EDITOR_H

#include "Helpers.h"

#ifdef __APPLE__
	#define GL_SILENCE_DEPRECATION
	#include <GLFW/glfw3.h>
#else
	#include <GLFW/glfw3.h>
#endif

#include <Eigen/Core>
#include <Eigen/Dense>
#include <chrono>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <math.h>
#include <cmath>

using namespace std;
using namespace Eigen;

class Editor {
	public:	
		double width;
		double height;
		int quit;

		Eigen::Matrix4d view;
		Vector2d p0;
		Vector2d p1;
		Vector3d cur_col;

		void init(void);
		void update_camera(void);
		void update_view(GLFWwindow* window);
};

#endif

