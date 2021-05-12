#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <cmath>
#include "Voronoi.h"
using namespace std;

class node{
	public:
		int height;
		Eigen::Vector2i break_point;
		node* left;
		node* right;

		node (Eigen::Vector2i bp) {
			height = 1;
			break_point = bp;
			left = NULL;
			right = NULL;
		}
};

class AVL{
	public:
		node* root = NULL;
		Eigen::MatrixXd points;
		double sweepline_y;
		std::vector<Eigen::Vector3i> tree_rep;
		std::vector<std::string> msg;

		AVL(Voronoi* diagram) {
			points = diagram->points;
			Eigen::Vector3d new_point = points.row(1);
			sweepline_y = new_point(1);
			Eigen::Vector3d arc_params = get_line_params(0);
			double pa = arc_params(0);
			double pb = arc_params(1);
			double pc = arc_params(2);
			double intersect_y = pa*new_point(0)*new_point(0) + pb*new_point(0) + pc;
			diagram->open_edges[0][1] = Eigen::Vector3d(new_point(0), intersect_y, 1);
			diagram->open_edges[1][0] = Eigen::Vector3d(new_point(0), intersect_y, 1);
		}

		std::pair<node*, node*> searchUtil(node* head, double x);

		void insert(Event e, Voronoi* diagram);
		void remove(Event e, Voronoi* diagram);
		
		node* rightRotation(node* head);
		node* leftRotation(node* head);

		node* insertUtil(node* head, Eigen::Vector2i bp, double x);
		node* removeUtil(node* head, double x);
		
		Eigen::Vector2d get_intersection(Eigen::Vector2i bp);
		Eigen::Vector2d get_intersection(Eigen::Vector2i bp, double swpl_y);
		Eigen::Vector3d get_line_params(int i);
		Eigen::Vector3d get_line_params(int i, double swpl_y);

		int height(node* head);
		void report_tree(node* head);
		void report_tree(node* head, int n, int depth);

		Eigen::Vector2d check_circle_event(int site1, int site2, int site3);

		void delete_event(Voronoi* diagram, Eigen::Vector2i bp1, Eigen::Vector2i bp2);
		void delete_event(Voronoi* diagram, int a, int b, int c);

};