# Visualization of the Fortune's Algorithm
A line sweeping algorithm for constructing Voronoi Diagram implemented in C++ combined with OpenGL. Visualizations are created for better understanding of how the algorithm works. My visualization includes:
- Visualization of the Tree structure used for presenting Beachline.
- Visualization of the parabolas & beachline.
- Visualization of the circle formed by circle events.
Some other events are presented with the help of plain text.

## How To Use
`Trackpad`: Use your trackpad to scroll the sweepline up/down to see the events at different time steps..
`Q`: Quit the program.

## Platform/Programming Language
- GLFW/C++
- nanogui

Except as described above, all the work on this project is my/our own. Signed Zili Xie.
My Work: All files in `voronoi/include`, `voronoi/src`, `voronoi/shader`.

## Data Structures
This section introduces the main data structures that I defined for the general use in this project. The following classes are defined in `Voronoi.h` and `AVL.h` file.

### Event
A Event can be either a site event or a circle event. A site event happens when the sweepline reaches a new site that has not yet been reached. A circle happens when the sweepline touch the bottom of the circle where the three associated points lie on. The Event class has a attribute named position that shows where the event actually happens and a integer vector called associate that stores the related points that are associated with this event. For circle event, there are 3 points, and for site event, there is only one.
```
class Event {
	public:
		Eigen::Vector2d position;
		std::vector associate;
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
```

### Voronoi
The Voronoi class is the class that stores all the principle elements of the Voronoi diagram:
- Eigen Matrices of double to store the generated input points points.
- Eigen Matrices of double to store the computed Voronoi vertices vor_vtx.
- a Vector of 2x2 Matrix that collects each voronoi edges as a matrix edges.
- a vector of 3 dimensional vector that collects the parameters for all parabolas lines_param.
- a vector of 1000x3 dimensional matrix to store the point on all parabolas lines.
- a priority queue for all Events event_queue.
```
class Voronoi {
	public:
		double sweepline_y;
		int sub_div;

		// for visualize
		Eigen::MatrixXd points; //sites
		Eigen::MatrixXd vor_vtx; //sites

		std::vector <std::vector <Eigen::Vector2d >> vor_vtx_map; //table
		std::vector <Eigen::Matrix2d> edges;
		std::vector <std::vector <Eigen::Vector3d >> open_edges;

		std::vector < Eigen::Vector3d> lines_param; //Parameters
		std::vector < Eigen::MatrixXd> lines; //sampled points on line
		Eigen::MatrixXd beach_line;
		Eigen::MatrixXd data;

		// for compute
		std::priority_queue < Event, std::vector < Event >, PQ_comparator> event_queue;

		Voronoi(int n);
		Eigen::Vector2d generate_point();
		Eigen::MatrixXd interpolate_line(double start, double end, double a, double b, double c);
		void update_lines_param();
		void update_lines_points();
		void update_data(double yoffset, Eigen::MatrixXd tree_edges);

		void extend_open_edges();
};
```
