#ifndef VORONOI_H
#define VORONOI_H
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <queue>
#include <unordered_map>


class Event {
	public:
		Eigen::Vector2d position;
		std::vector<int> associate;
		int type;

		Event(Eigen::Vector2d p, int s) {
			position = p;
			associate.push_back(s);
			type = 1;
		}
		Event(Eigen::Vector2d p, int s1, int s2, int s3) {
			position = p;
			associate.push_back(s1);
			associate.push_back(s2);
			associate.push_back(s3);
			type = 0;
		}
};

class PQ_comparator {
	bool reverse;
	public:
		PQ_comparator(const bool& revparam=false) {
			reverse=revparam;
		}
		bool operator() (const Event &e1, const Event &e2) const {
			if (e1.position(1) > e2.position(1)) {
				return false;
			} else if (e1.position(1) < e2.position(1)) {
				return true;
			} else {
				return (e1.position(0) < e2.position(0));
			}
		}
};

class Voronoi {
	public:
		double sweepline_y;
		int sub_div;

		// for visualize
		Eigen::MatrixXd points; //sites
		Eigen::MatrixXd vor_vtx; //sites

		std::vector<std::vector<Eigen::Vector2d>> vor_vtx_map; //table
		std::vector<Eigen::Matrix2d> edges;
		std::vector<std::vector<Eigen::Vector3d>> open_edges;


		std::vector<Eigen::Vector3d> lines_param; //Parameters
		std::vector<Eigen::MatrixXd> lines; //sampled points on line
		Eigen::MatrixXd beach_line;
		Eigen::MatrixXd data;

		// for compute
		std::priority_queue<Event, std::vector<Event>, PQ_comparator> event_queue;

		Voronoi(int n);
		Eigen::Vector2d generate_point();
		Eigen::MatrixXd interpolate_line(double start, 
							  double end, 
							  double a, 
							  double b, 
							  double c);
		void update_lines_param();
		void update_lines_points();
		void update_data(double yoffset, Eigen::MatrixXd tree_edges);

		void extend_open_edges();
};

#endif