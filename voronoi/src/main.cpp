#define GL_SILENCE_DEPRECATION
#if defined(NANOGUI_GLAD)
    #if defined(NANOGUI_SHARED) && !defined(GLAD_GLAPI_EXPORT)
        #define GLAD_GLAPI_EXPORT
    #endif
    #include <glad/glad.h>
#else
    #if defined(__APPLE__)
        #define GLFW_INCLUDE_GLCOREARB
    #else
        #define GL_GLEXT_PROTOTYPES
    #endif
#endif

#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include "Helpers.h"
#include "Editor.h"
#include "GUI.h"
#include "Label.h"
#include "Voronoi.h"
#include "AVL.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

VertexBufferObject VBO; // VertexBufferObject wrapper
VertexArrayObject VAO;
Editor e;
Program program;

Voronoi diagram(20);
AVL t(&diagram);
GLFWwindow* window;
GUI ui;
std::vector<Event> pq_event;

std::vector< std::pair<int, std::vector<Eigen::Vector3i>> > tree_timeline;
std::vector<std::vector<Label>> beachline_tree_timeline;
std::vector<Eigen::MatrixXd> tree_edges_timeline;
std::vector< std::vector<std::string> > msg_timeline;
std::vector< std::vector<Label> > msg_label_timeline;


std::vector<Label> labels;
Label pq_vis;
// Label swpl_pos;
int next_event = 0;

// Callback Functions 
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// ui.screen->resizeCallbackEvent(width, height);
	glViewport(0, 0, width, height);
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	// ui.screen->scrollCallbackEvent(xoffset, yoffset);
	diagram.sweepline_y -= 0.0001 * yoffset;
	diagram.update_lines_param();
	diagram.update_lines_points();
	// swpl_pos.label_init(window, 1200, 100 - 800 * diagram.sweepline_y, "swpl_pos: " + std::to_string(diagram.sweepline_y));
	if (next_event > 1) {
		diagram.update_data(yoffset, tree_edges_timeline[next_event-2]);
	} else {
		diagram.update_data(yoffset, Eigen::MatrixXd(0,0));
	}
	Eigen::MatrixXf dataf = diagram.data.cast <float> ();
	VBO.update(dataf.transpose());
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
	// ui.screen->cursorPosCallbackEvent(xpos, ypos);
	// Get the size of the window.
	int width, height;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwGetWindowSize(window, &width, &height);
	Eigen::Vector2d pixel(xpos, height-1-ypos); // pixel position
	// Track the mouse positions
	if (e.p0 != Vector2d(0,0) && e.p1 != Vector2d(0,0)) {
		e.p0 = e.p1;
		e.p1 = pixel;
	} else {
		e.p0 = e.p1 = pixel;
	}
}

void mouse_click_callback(GLFWwindow* window, int button, int action, int mods) {
	// ui.screen->mouseButtonCallbackEvent(button, action, mods);
	// if (!ui.focus_check()) {
	// 	if (button == GLFW_MOUSE_BUTTON_LEFT) {
	// 	} else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
	// 	}
	// }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// ui.screen->keyCallbackEvent(key, scancode, action, mods);
	if (key == GLFW_KEY_MINUS) {e.view.topLeftCorner(2,2) = e.view.topLeftCorner(2,2) * 0.97;}
	if (key == GLFW_KEY_EQUAL) {e.view.topLeftCorner(2,2) = e.view.topLeftCorner(2,2) * 1.03;}
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
		e.quit = 1;
	}
}


void renderer_draw(Editor e, Program program, Voronoi diagram) {
	Matrix4f translation = MatrixXf::Identity(4, 4);
	int tree_edge_row = 0;
	if (next_event > 1) {
		tree_edge_row = tree_edges_timeline[next_event-2].rows();
	}

	Eigen::Matrix4f viewf = e.view.cast <float> ();
	glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, viewf.data());
	glUniform3f(program.uniform("line_col"), 0,0,0);

	glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, translation.data());
	glDrawArrays(GL_POINTS, 0, diagram.points.rows());

	glUniform3f(program.uniform("line_col"), 0.8,0.8,0.8);
	for (int i=0; i<diagram.lines.size(); i++) {
		glDrawArrays(GL_LINE_STRIP, diagram.points.rows() + diagram.vor_vtx.rows() + diagram.edges.size() * 2 
			+ 12 + 2 + 1000 + tree_edge_row + (diagram.sub_div + 1) *i, (diagram.sub_div + 1));
	}
	// draw beach line
	glUniform3f(program.uniform("line_col"), 1,0,0);
	glDrawArrays(GL_LINE_STRIP, diagram.points.rows() + diagram.vor_vtx.rows() + diagram.edges.size() * 2 
		+ 12 + 2 + 1000 + tree_edge_row + (diagram.sub_div + 1) *diagram.lines.size(), (diagram.sub_div + 1));

	// draw vor vtx
	glDrawArrays(GL_POINTS, diagram.points.rows(), diagram.vor_vtx.rows());

	// draw sweep line
	glUniform3f(program.uniform("line_col"), 0,0,0);
	glDrawArrays(GL_LINE_STRIP, diagram.points.rows() + diagram.vor_vtx.rows() + diagram.edges.size() * 2 + 12, 2);

	// draw edges
	glUniform3f(program.uniform("line_col"), 0,0,1);
	for (int i=0; i<diagram.edges.size(); i++) {
		glDrawArrays(GL_LINE_STRIP, diagram.points.rows() + diagram.vor_vtx.rows() + i*2,  2);
	}

	// draw circle
	glUniform3f(program.uniform("line_col"), 0,1,0);
	glDrawArrays(GL_LINE_LOOP, diagram.points.rows() + diagram.vor_vtx.rows() + diagram.edges.size() * 2 
		+ 12 + 2, 1000);

	// draw paddings
	glUniform3f(program.uniform("line_col"), 1,1,1);
	for (int i=0; i<4;i++) {
		glDrawArrays(GL_TRIANGLES, diagram.points.rows() + diagram.vor_vtx.rows() + diagram.edges.size() * 2 + i * 3, 3);
	}

	// draw tree edges
	glUniform3f(program.uniform("line_col"), 0,0,0);

	if (next_event > 1) {
		for (int i=0; i<tree_edge_row; i++) {
			glDrawArrays(GL_LINE_STRIP, diagram.points.rows() + diagram.vor_vtx.rows() + diagram.edges.size() * 2 
				+ 12 + 2 + 1000 + i * 2,  2);
		}
	}
}


int locate_event(double swpl_y) {
	int n = pq_event.size();
	if (swpl_y > pq_event[0].position(1)) {
		return 0;
	} else if (swpl_y < pq_event[n-1].position(1)) {
		return n-1;
	}
	for (int i=0; i<pq_event.size(); i++) {
		if (pq_event[i].position(1) > swpl_y && swpl_y > pq_event[i+1].position(1)) {
			return i+1;
		}
	}
	return pq_event.size();
}

Eigen::Vector2d compute_xy(int n, int h, double w) {
	double stride = w/(pow(2, h-1)-1);
	int i = floor(log2(n + 1));
	int j = n - (pow(2, i) - 1);
	double stride_i = stride * (h-i);
	double left_bound = -(pow(2, i)-1) * stride_i/2;
	double x = left_bound + j * stride_i;
	double y = 1-i*0.2;
	return Eigen::Vector2d(x, y);
}


void make_tree(int h, std::vector<Eigen::Vector3i> vec) {
	double w = 1.0;
	double stride = w/(pow(2, h-1)-1);
	std::vector< std::pair<Eigen::Vector2d, Eigen::Vector2i> > current_tree;
	Eigen::MatrixXd tree_edges;
	tree_edges.resize(2 * (vec.size()-1), 3);

	for (int n=0; n<vec.size(); n++) {
		int i = floor(log2(vec[n](2) + 1));
		int j = vec[n](2) - (pow(2, i) - 1);

		std::pair<Eigen::Vector2d, Eigen::Vector2i> pair;
		Eigen::Vector2d pos = compute_xy(vec[n](2), h, w);
		pair.first = pos;
		pair.second = Eigen::Vector2i(vec[n](0), vec[n](1));
		current_tree.push_back(pair);

		int parent = pow(2, i-1)-2 + floor(j/2)+1;
		Eigen::Vector2d parent_pos = compute_xy(parent, h, w);

		if (n>0) {
			tree_edges.row((n-1) * 2) = Eigen::RowVector3d(pos(0) + 1.23, pos(1) -0.05, 0); //compute_xy(parent, h, w)
			tree_edges.row((n-1) * 2 + 1) = Eigen::RowVector3d(parent_pos(0) + 1.23, parent_pos(1) -0.05, 0);
		}
	}
	tree_edges_timeline.push_back(tree_edges);
	std::vector<Label> tmp;
	for (int i=0; i<current_tree.size(); i++) {
		Label lab;
		Eigen::Vector2d node = current_tree[i].first;
		Eigen::Vector2i bp = current_tree[i].second;
		lab.label_init(window, 
			static_cast<int>(1180 + 400* node(0)), 
			static_cast<int>(400 - 400* node(1)), 
			"<" + std::to_string(bp(0)) + "," + std::to_string(bp(1)) + ">", Eigen::Vector4i(255, 255, 255, 255));
		tmp.push_back(lab);
	}
	beachline_tree_timeline.push_back(tmp);
}

void make_msg(std::vector<std::string> vec) {
	std::vector<Label> tmp;
	for (int i=0; i<vec.size(); i++) {
		Label lab;
		lab.label_init(window, 1050, 500 + i * 30, vec[i]);
		tmp.push_back(lab);
	}
	msg_label_timeline.push_back(tmp);
}


std::pair<Eigen::Vector2d, double> get_circle(Event e) {
	std::vector<int> v = e.associate;
	Eigen::Vector2d p = e.position;

	Eigen::Vector2d p1 = diagram.points.row(v[0]);
	Eigen::Vector2d p2 = diagram.points.row(v[1]);
	Eigen::Vector2d p3 = diagram.points.row(v[2]);

	double x12 = p1(0) - p2(0);
	double x13 = p1(0) - p3(0);
	double y12 = p1(1) - p2(1);
	double y13 = p1(1) - p3(1);
	double y31 = p3(1) - p1(1);
	double y21 = p2(1) - p1(1);
	double x31 = p3(0) - p1(0);
	double x21 = p2(0) - p1(0);
	double sx13 = pow(p1(0), 2) - pow(p3(0), 2);
	double sy13 = pow(p1(1), 2) - pow(p3(1), 2);
	double sx21 = pow(p2(0), 2) - pow(p1(0), 2);
	double sy21 = pow(p2(1), 2) - pow(p1(1), 2);

	double f = ((sx13) * (x12) + (sy13) * (x12) + (sx21) * (x13) + (sy21) * (x13))/ (2 * ((y31) * (x12) - (y21) * (x13)));
	double g = ((sx13) * (y12) + (sy13) * (y12) + (sx21) * (y13) + (sy21) * (y13))/ (2 * ((x31) * (y12) - (x21) * (y13)));
	double c = -pow(p1(0), 2) - pow(p1(1), 2) - 2 * g * p1(0) - 2 * f * p1(1);
	double h = -g;
	double k = -f;
	double sqr_of_r = h * h + k * k - c;
	double r = sqrt(sqr_of_r);

	std::pair<Eigen::Vector2d, double> pair;
	pair.first = Eigen::Vector2d(h,k);
	pair.second = r;
	return pair;
}

Eigen::MatrixXd gen_circle(Eigen::Vector2d c, double r) {
	Eigen::MatrixXd circle(1000,3);
	// double theta = 0;
	double inc = 2*M_PI/1000;
	for (int i=0; i<1000; i++) {
		double y = r * sin(i * inc);
		double x = r * cos(i * inc);
		circle.row(i) = Eigen::Vector3d (x + c(0), y + c(1), 0);
	}
	return circle;
}

int main(int argc, char **argv) {
	glfwInit();
	glfwSetTime(0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(1440, 800, "Voronoi Visualizer", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	e.init();
	e.update_view(window);

	node s1 = node(Eigen::Vector2i(0,1));
	node s2 = node(Eigen::Vector2i(1,0));
	t.root = &s1;
	t.root->right = &s2;

	t.report_tree(t.root, 0, 0);
	std::pair<int, std::vector<Eigen::Vector3i>> pair;	
	pair.first = 2; 
	pair.second = t.tree_rep;
	tree_timeline.push_back(pair);
	std::vector<std::string> tmp_msg;
	tmp_msg.push_back("CURRENT EVENT: SITE EVENT INSERT 0");
	msg_timeline.push_back(tmp_msg);

	std::vector<std::string> tmp_msg2;
	tmp_msg2.push_back("CURRENT EVENT: SITE EVENT INSERT 1");
	msg_timeline.push_back(tmp_msg2);

	printf("\nevents:\n");
	while (!diagram.event_queue.empty()) {
		Event tmp = diagram.event_queue.top();
		pq_event.push_back(tmp);
		if (tmp.type == 1) {printf("(%f, %f) event type: %d associate: %d\n", tmp.position(0), tmp.position(1), tmp.type, tmp.associate[0]);}
		if (tmp.type == 0) {printf("(%f, %f) event type: %d associate: (%d, %d, %d)\n", tmp.position(0), tmp.position(1), tmp.type, tmp.associate[0], tmp.associate[1], tmp.associate[2]);}
		diagram.event_queue.pop();
		if (tmp.type == 1) {
			if (tmp.associate[0] > 1) {
				std::vector<std::string> empty;
				t.msg = empty;
				t.insert(tmp, &diagram);
				printf("-------------\n");

				std::vector<Eigen::Vector3i> empty_tree;
				t.tree_rep = empty_tree;

				t.report_tree(t.root, 0, 0);
				std::pair<int, std::vector<Eigen::Vector3i>> pair;
				pair.first = t.height(t.root); 
				pair.second = t.tree_rep;

				tree_timeline.push_back(pair);
				msg_timeline.push_back(t.msg);

				printf("-------------\n");
			}
		} else { // circle event
			std::vector<std::string> empty;
			t.msg = empty;
			t.remove(tmp, &diagram);
			printf("-------------\n");
			std::vector<Eigen::Vector3i> empty_tree;
			t.tree_rep = empty_tree;

			t.report_tree(t.root, 0, 0);
			std::pair<int, std::vector<Eigen::Vector3i>> pair;
			pair.first = t.height(t.root); 
			pair.second = t.tree_rep;

			tree_timeline.push_back(pair);
			msg_timeline.push_back(t.msg);

			printf("-------------\n");
		}
	}
	printf("row: %ld, col: %ld\n", diagram.vor_vtx.rows(), diagram.vor_vtx.cols());
	printf("edges: %ld\n", diagram.edges.size());
	printf("open_edges\n");

	diagram.extend_open_edges();


	for (int i=0; i<tree_timeline.size(); i++) {
		make_tree(tree_timeline[i].first, tree_timeline[i].second);
	}
	for (int i=0; i<msg_timeline.size(); i++) {
		make_msg(msg_timeline[i]);
	}
	for (int j=0; j<beachline_tree_timeline.size(); j++) {
		for (int i=0; i<beachline_tree_timeline[j].size(); i++) {
			beachline_tree_timeline[j][i].screen->drawWidgets();
		}
	}
	for (int j=0; j<msg_label_timeline.size(); j++) {
		for (int i=0; i<msg_label_timeline[j].size(); i++) {
			msg_label_timeline[j][i].screen->drawWidgets();
		}
	}
	for (int i=0; i<diagram.points.rows(); i++) {
		printf("e.width: %f %f\n", e.width,e.width/2 + (e.width/2) * diagram.points(i,0));
		Label lab;
		lab.label_init(window, static_cast<int>(700 + 400 * diagram.points(i,0)), 
							static_cast<int>(400 - 400* diagram.points(i,1)), std::to_string(i));
		labels.push_back(lab);
	}
	pq_vis.label_init(window, 1050, 700, "NEXT EVENT: SITE EVENT INSERT 0");
	int cur_event = 0;


	printf("beachline_tree_timeline: %ld\n", beachline_tree_timeline.size());
	printf("tree_edges_timeline: %ld\n", tree_edges_timeline.size());
	printf("tree_timeline[0].second: %ld\n", tree_timeline[0].second.size());
	for (int i=0; i<tree_timeline[0].second.size(); i++) {
		printf("(%d, %d, %d)\n", tree_timeline[0].second[i](0), tree_timeline[0].second[i](1), tree_timeline[0].second[i](2));
	}
	printf("tree_timeline[1].second: %ld\n", tree_timeline[1].second.size());
	for (int i=0; i<tree_timeline[1].second.size(); i++) {
		printf("(%d, %d, %d)\n", tree_timeline[1].second[i](0), tree_timeline[1].second[i](1), tree_timeline[1].second[i](2));
	}
	printf("pq_event: %ld\n", pq_event.size());
	printf("msg_timeline: %ld\n", msg_timeline.size());



	Eigen::MatrixXd sweepline(2,3);
	Eigen::MatrixXd vor_edges(diagram.edges.size() * 2, 3);
	Eigen::MatrixXd padding(12,3);

	Eigen::MatrixXd circle(1000,3);
	circle = Eigen::MatrixXd::Zero(1000, 3);
	// circle = gen_circle(Eigen::Vector2d(0,0), 0.3);

	// printf("circle\n");
	// for (int i=0; i<1000; i++) {
	// 	printf("(%f, %f, %f)\n", circle(i,0), circle(i,1), circle(i,2));
	// }

	for (int i=0; i<diagram.edges.size(); i++) {
		vor_edges.row(i * 2) = Eigen::RowVector3d(diagram.edges[i](0,0), diagram.edges[i](1,0), 0);
		vor_edges.row(i * 2 + 1) = Eigen::RowVector3d(diagram.edges[i](0,1), diagram.edges[i](1,1), 0);
	}
	printf("vor_edges: (%ld, %ld)\n", vor_edges.rows(), vor_edges.cols());

	sweepline << -2, 0.95, 0, 0.8, 0.95, 0;
	padding << -2, 1, 0, -2, 1, 0, -2, 1, 0, -2, 1, 0, -2, 1, 0, -2, 1, 0, 
			   0.8, 1, 0, 0.8, -1, 0, 3, -1, 0, 0.8, 1, 0, 3, -1, 0, 3, 1, 0;
	diagram.data.resize(diagram.points.rows() + diagram.vor_vtx.rows() + vor_edges.rows() + 12 + 2 + circle.rows(), 3);
	diagram.data << diagram.points, diagram.vor_vtx, vor_edges, padding, sweepline, circle;

	printf("STRAT\n");

	VAO.init();
	VAO.bind(); 
	VBO.init();
	Eigen::MatrixXf dataf = diagram.data.cast <float> ();
	VBO.update(dataf.transpose());

	program.init("../shader/vertex_shader.glsl","../shader/fragment_shader.glsl","outColor");
	program.bind(); 
	program.bindVertexAttribArray("position",VBO);
	ui.nanogui_init(window, 15, 15);

	glfwSetCharCallback(window,[](GLFWwindow *, unsigned int codepoint) {
		ui.screen->charCallbackEvent(codepoint);
	});
	glfwSetDropCallback(window,[](GLFWwindow *, int count, const char **filenames) {
		ui.screen->dropCallbackEvent(count, filenames);
	});
	glfwSetKeyCallback(window, key_callback);                 // Register the keyboard callback
	glfwSetMouseButtonCallback(window, mouse_click_callback); // Register the mouse callback
	glfwSetCursorPosCallback(window, mouse_move_callback);    // Register the cursor move callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Update viewport
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glPointSize(10.0f);


	// Game loop
	while (!glfwWindowShouldClose(window) && !e.quit) {
		// printf("(%f, %f)\n", e.width, e.height);
		program.bind();
		// glEnable(GL_DEPTH_TEST);
		e.update_view(window);
		glClearColor(1.0f, 1.0f, 1.0f, 0.4f);
		glClear(GL_COLOR_BUFFER_BIT);
		e.update_camera();

		// Draw triangles
		VAO.bind(); 
		// ui2.screen->drawWidgets();
		renderer_draw(e, program, diagram);
		// ui.screen->drawWidgets();

		for (int i=0; i<diagram.points.rows(); i++) {
			labels[i].screen->drawWidgets();
		}

		next_event = locate_event(diagram.sweepline_y);
		if (next_event != cur_event) {
			char buff[100];
			Event tmp = pq_event[next_event];
			if (tmp.type == 1) {
				snprintf(buff, sizeof(buff), "NEXT EVENT: SITE EVENT INSERT %s", 
					std::to_string(tmp.associate[0]).c_str());
				diagram.data.block<1000,3>(diagram.points.rows() + diagram.vor_vtx.rows() + vor_edges.rows() + 12 + 2,0) = Eigen::MatrixXd::Zero(1000, 3);
			}
			if (tmp.type == 0) {
				// printf("(%f, %f) event type: %d associate: (%d, %d, %d)\n", tmp.position(0), tmp.position(1), tmp.type, tmp.associate[0], tmp.associate[1], tmp.associate[2]);
				snprintf(buff, sizeof(buff), "NEXT EVENT: CIRCLE EVENT IN REGARD TO (%s, %s, %s)", 
					std::to_string(tmp.associate[0]).c_str(), 
					std::to_string(tmp.associate[1]).c_str(), 
					std::to_string(tmp.associate[2]).c_str());

				std::pair<Eigen::Vector2d, double> pair = get_circle(tmp);
				printf("circle: (%f, %f), r: %f\n", pair.first(0), pair.first(1), pair.second);
				diagram.data.block<1000,3>(diagram.points.rows() + diagram.vor_vtx.rows() + vor_edges.rows() + 12 + 2,0) = gen_circle(pair.first, pair.second);
			}

			// snprintf(buff, sizeof(buff), "CURRENT EVENT: CIRCLE EVENT IN REGARD TO (%s, %s, %s)", std::to_string(next_event).c_str());
			std::string event_str = buff;
			pq_vis.label_init(window, 1050, 700, event_str);

			cur_event = next_event;

		}
		if (next_event > 1) {
			for (int i=0; i<beachline_tree_timeline[next_event-2].size(); i++) {
				beachline_tree_timeline[next_event-2][i].screen->drawWidgets();
			}
		}
		if (next_event > 0) {
			for (int i=0; i<msg_label_timeline[next_event-1].size(); i++) {
				msg_label_timeline[next_event-1][i].screen->drawWidgets();
			}
		}

		pq_vis.screen->drawWidgets();
		// swpl_pos.screen->drawWidgets();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// delete simpleText;
	// Deallocate opengl memory
	program.free();
	VAO.free();
	VBO.free();
	glfwTerminate();
	return 0;
}
