#include "AVL.h"

int AVL::height(node* head){
	if (head == NULL) {
		return 0;
	}
	return head->height;
}

Eigen::Vector2d AVL::get_intersection(Eigen::Vector2i bp){
	int site1 = bp(0);
	int site2 = bp(1);
	Eigen::Vector3d params1 = get_line_params(site1);
	Eigen::Vector3d params2 = get_line_params(site2);
	Eigen::Vector3d p1 = points.row(site1);
	Eigen::Vector3d p2 = points.row(site2);
	if (abs(p1(1) - sweepline_y) < 0.000001) {
		return Eigen::Vector2d(p1(0), params2(0) * p1(0) * p1(0) + params2(1) * p1(0) + params2(2));
	}
	if (abs(p2(1) - sweepline_y) < 0.000001) {
		return Eigen::Vector2d(p2(0), params1(0) * p2(0) * p2(0) + params1(1) * p2(0) + params1(2));
	}
	double a = params1(0);
	double b = params1(1);
	double c = params1(2);

	double d = params2(0);
	double e = params2(1);
	double f = params2(2);

	double A = a-d;
	double B = b-e;
	double C = c-f;
	double delta = B*B - 4*A*C;

	double x1 = ((-1)*B - sqrt(delta))/(2*A);
	double x2 = ((-1)*B + sqrt(delta))/(2*A);

	if (site1 < site2) {
		double res = std::min(x1, x2);
		return Eigen::Vector2d(res, a*res*res + b*res + c);
	} else {
		double res = std::max(x1, x2);
		return Eigen::Vector2d(res, a*res*res + b*res + c);
	}
}

Eigen::Vector2d AVL::get_intersection(Eigen::Vector2i bp, double swpl_y){
	int site1 = bp(0);
	int site2 = bp(1);

	Eigen::Vector3d params1 = get_line_params(site1, swpl_y);
	Eigen::Vector3d params2 = get_line_params(site2, swpl_y);
	Eigen::Vector3d p1 = points.row(site1);
	Eigen::Vector3d p2 = points.row(site2);
	if (p1(1) == swpl_y) {
		return Eigen::Vector2d(p1(0), p1(1));
	}
	if (p2(1) == swpl_y) {
		return Eigen::Vector2d(p2(0), p2(1));
	}

	double a = params1(0);
	double b = params1(1);
	double c = params1(2);

	double d = params2(0);
	double e = params2(1);
	double f = params2(2);

	double A = a-d;
	double B = b-e;
	double C = c-f;
	double delta = B*B - 4*A*C;

	double x1 = ((-1)*B - sqrt(delta))/(2*A);
	double x2 = ((-1)*B + sqrt(delta))/(2*A);
	if (site1 < site2) {
		double res = std::min(x1, x2);
		return Eigen::Vector2d(res, a*res*res + b*res + c);
	} else {
		double res = std::max(x1, x2);
		return Eigen::Vector2d(res, a*res*res + b*res + c);
	}
}

Eigen::Vector3d AVL::get_line_params(int i) {
	double a = points(i,0);
	double b = points(i,1);
	double c = sweepline_y;

	double A = 1/(2*b -2*c);
	double B = -2*a/(2*b -2*c);
	double C = (a*a + b*b - c*c)/(2*b - 2*c);
	return Eigen::Vector3d(A,B,C);
}

Eigen::Vector3d AVL::get_line_params(int i, double swpl_y) {
	double a = points(i,0);
	double b = points(i,1);
	double c = swpl_y;

	double A = 1/(2*b -2*c);
	double B = -2*a/(2*b -2*c);
	double C = (a*a + b*b - c*c)/(2*b - 2*c);
	return Eigen::Vector3d(A,B,C);
}

node* AVL::rightRotation(node* head){
	node* newhead = head->left;
	head->left = newhead->right;
	newhead->right = head;
	head->height = 1 + max(height(head->left), height(head->right));
	newhead->height = 1 + max(height(newhead->left), height(newhead->right));
	return newhead;
}
node* AVL::leftRotation(node* head){
	node* newhead = head->right;
	head->right = newhead->left;
	newhead->left = head;
	head->height = 1 + max(height(head->left), height(head->right));
	newhead->height = 1 + max(height(newhead->left), height(newhead->right));
	return newhead;
}

void AVL::insert(Event e, Voronoi* diagram) {
	char buff[100];
	if (e.type == 1) {
		snprintf(buff, sizeof(buff), "CURRENT EVENT: SITE EVENT INSERT %s", 
			std::to_string(e.associate[0]).c_str());
	}
	std::string event_str = buff;
	msg.push_back(event_str);

	int site = e.associate[0];
	Eigen::Vector3d new_point = points.row(site);
	sweepline_y = new_point(1);

	std::pair<node*, node*> result = searchUtil(root, new_point(0));
	// if (result.first == NULL) {
	// 	if (abs(get_intersection(result.second->break_point)(0) - new_point(0)) < 0.0001) {
	// 		points.row(site)(0) += 0.1;
	// 	}
	// } else if (result.second == NULL) {
	// 	if (abs(get_intersection(result.first->break_point)(0) - new_point(0)) < 0.0001) {
	// 		points.row(site)(0) += 0.1;
	// 	}
	// } else {
	// 	if (abs(get_intersection(result.first->break_point)(0) - new_point(0)) < 0.0001 || 
	// 		abs(get_intersection(result.second->break_point)(0) - new_point(0)) < 0.0001) {
	// 		points.row(site)(0) += 0.1;
	// 	}
	// }

	// new_point = points.row(site);
	// sweepline_y = new_point(1);

	result = searchUtil(root, new_point(0));
	int arc = -1;
	if (result.first == NULL) {
		arc = result.second->break_point(0);
		snprintf(buff, sizeof(buff), "HIT THE PARABOLA: %d", arc);
		event_str = buff;
		msg.push_back(event_str);

		Eigen::Vector2d tmp = check_circle_event(site, arc, result.second->break_point(1));
		if (tmp(1) < sweepline_y || abs(tmp(1) - sweepline_y) < 0.00001) {
			diagram->event_queue.push(Event(tmp, site, arc, result.second->break_point(1)));

			snprintf(buff, sizeof(buff), "NEW CIRCLE event: (%d, %d, %d)", site, arc, result.second->break_point(1));
			event_str = buff;
			msg.push_back(event_str);
		}
	} else if (result.second == NULL) {
		arc = result.first->break_point(1);
		snprintf(buff, sizeof(buff), "HIT THE PARABOLA: %d", arc);
		event_str = buff;
		msg.push_back(event_str);

		Eigen::Vector2d tmp = check_circle_event(result.first->break_point(0), arc, site);
		if (tmp(1) < sweepline_y || abs(tmp(1) - sweepline_y) < 0.00001) {
			diagram->event_queue.push(Event(tmp, result.first->break_point(0), arc, site));
			
			snprintf(buff, sizeof(buff), "NEW CIRCLE event: (%d, %d, %d)", result.first->break_point(0), arc, site);
			event_str = buff;
			msg.push_back(event_str);
		}
	} else {
		arc = result.first->break_point(1);
		snprintf(buff, sizeof(buff), "HIT THE PARABOLA: %d", arc);
		event_str = buff;
		msg.push_back(event_str);

		Eigen::Vector2d tmp1 = check_circle_event(result.first->break_point(0), arc, site);
		Eigen::Vector2d tmp2 = check_circle_event(site, arc, result.second->break_point(1));
		// printf("sweepline_y: %f, (%d, %d, %d): %f and (%d, %d, %d): %f \n", sweepline_y, result.first->break_point(0), arc, site, tmp1(1), site, arc, result.second->break_point(1), tmp2(1));
		if (tmp1(1) < sweepline_y || abs(tmp1(1) - sweepline_y) < 0.00001) {
			diagram->event_queue.push(Event(tmp1, result.first->break_point(0), arc, site));
			printf("push to queue: (%f, %f) (%d, %d, %d)\n", tmp1(0), tmp1(1), result.first->break_point(0), arc, site);

			snprintf(buff, sizeof(buff), "NEW CIRCLE event: (%d, %d, %d)", result.first->break_point(0), arc, site);
			event_str = buff;
			msg.push_back(event_str);
		}
		if (tmp2(1) < sweepline_y || abs(tmp2(1) - sweepline_y) < 0.00001) {
			diagram->event_queue.push(Event(tmp2, site, arc, result.second->break_point(1)));
			printf("push to queue: (%f, %f) (%d, %d, %d)\n", tmp2(0), tmp2(1), site, arc, result.second->break_point(1));

			snprintf(buff, sizeof(buff), "NEW CIRCLE event: (%d, %d, %d)", site, arc, result.second->break_point(1));
			event_str = buff;
			msg.push_back(event_str);
		}
	}
	root = insertUtil(root, Eigen::Vector2i(arc, site), new_point(0));
	root = insertUtil(root, Eigen::Vector2i(site, arc), new_point(0));

	snprintf(buff, sizeof(buff), "INSERT NODE TO TREE: (%d, %d)", arc, site);
	event_str = buff;
	msg.push_back(event_str);

	snprintf(buff, sizeof(buff), "INSERT NODE TO TREE: (%d, %d)", site, arc);
	event_str = buff;
	msg.push_back(event_str);


	Eigen::Vector3d arc_params = get_line_params(arc);
	double pa = arc_params(0);
	double pb = arc_params(1);
	double pc = arc_params(2);
	double intersect_y = pa*new_point(0)*new_point(0) + pb*new_point(0) + pc;
	diagram->open_edges[arc][site] = Eigen::Vector3d(new_point(0), intersect_y, 1);
	diagram->open_edges[site][arc] = Eigen::Vector3d(new_point(0), intersect_y, 1);
}

void AVL::remove(Event e, Voronoi* diagram){
	char buff[100];
	snprintf(buff, sizeof(buff), "CURRENT EVENT: CIRCLE EVENT IN REGARD TO (%s, %s, %s)", 
			std::to_string(e.associate[0]).c_str(), 
			std::to_string(e.associate[1]).c_str(), 
			std::to_string(e.associate[2]).c_str());

	std::string event_str = buff;
	msg.push_back(event_str);


	Eigen::Vector2d p = e.position;
	sweepline_y = p(1);
	std::vector<int> v = e.associate;
	Eigen::Vector2d pos = e.position;
	int a = v[0];
	int b = v[1];
	int c = v[2];

	Eigen::Vector2d tmp = get_intersection(Eigen::Vector2i(a, c));
	int greatest = std::max(a, std::max(b, c));
	if (a == greatest) {
		tmp = get_intersection(Eigen::Vector2i(b, c));
	} else if (c == greatest) {
		tmp = get_intersection(Eigen::Vector2i(a, b));
	}

	std::pair<node*, node*> check = searchUtil(root, tmp(0));
	int arc = -1;
	if (check.first == NULL) {
		arc = check.second->break_point(0);
	} else if (check.second == NULL) {
		arc = check.first->break_point(1);
	} else {
		arc = check.first->break_point(1);
	}

	bool valid = false;
	Eigen::Vector3d arc_params = get_line_params(arc);
	double pa = arc_params(0);
	double pb = arc_params(1);
	double pc = arc_params(2);

	if (pa > 10000 || pb > 10000 || pc > 10000) {
		if (abs(pa*pos(0)*pos(0) + pb*pos(0) + pc - tmp(1)) <= 0.1) {
			valid = true;
		} else {
			valid = false;
		}
	} else {
		if (abs(pa*pos(0)*pos(0) + pb*pos(0) + pc - tmp(1)) <= 0.001) {
			valid = true;
		} else {
			valid = false;
		}
	}


	if (valid) {
		delete_event(diagram, a, b, c);
		int cur_rows = diagram->vor_vtx.rows();
		diagram->vor_vtx.conservativeResize(cur_rows + 1, 3);
		diagram->vor_vtx.row(cur_rows) = Eigen::RowVector3d(tmp(0), tmp(1), 0);

		if (diagram->vor_vtx_map[b][c] != Eigen::Vector2d(0,0)) {
			Eigen::Matrix2d m;
			m << tmp, diagram->vor_vtx_map[b][c];
			diagram->edges.push_back(m);
		}
		if (diagram->vor_vtx_map[a][b] != Eigen::Vector2d(0,0)) {
			Eigen::Matrix2d m;
			m << tmp, diagram->vor_vtx_map[a][b];
			diagram->edges.push_back(m);
		}
		if (diagram->vor_vtx_map[b][a] != Eigen::Vector2d(0,0)) {
			Eigen::Matrix2d m;
			m << tmp, diagram->vor_vtx_map[b][a];
			diagram->edges.push_back(m);
		}
		if (diagram->vor_vtx_map[c][b] != Eigen::Vector2d(0,0)) {
			Eigen::Matrix2d m;
			m << tmp, diagram->vor_vtx_map[c][b];
			diagram->edges.push_back(m);
		}
		diagram->vor_vtx_map[a][c] = Eigen::Vector2d(tmp(0), tmp(1));
		diagram->vor_vtx_map[b][c] = Eigen::Vector2d(tmp(0), tmp(1));
		diagram->vor_vtx_map[a][b] = Eigen::Vector2d(tmp(0), tmp(1));

		// printf("ADD VOR VTX: (%f, %f)\n", tmp(0), tmp(1));
		snprintf(buff, sizeof(buff), "ADD VOR VTX: (%f, %f)", tmp(0), tmp(1));
		event_str = buff;
		msg.push_back(event_str);

		Eigen::Vector2d ab = get_intersection(Eigen::Vector2i(a,b));
		Eigen::Vector2d bc = get_intersection(Eigen::Vector2i(b,c));
		Eigen::Vector2d ba = get_intersection(Eigen::Vector2i(b,a));
		Eigen::Vector2d cb = get_intersection(Eigen::Vector2i(c,b));

		Eigen::Vector2d ac = get_intersection(Eigen::Vector2i(a,c));

		diagram->open_edges[a][b] = Eigen::Vector3d(ab(0), ab(1), 0);
		diagram->open_edges[b][c] = Eigen::Vector3d(bc(0), bc(1), 0);
		diagram->open_edges[b][a] = Eigen::Vector3d(ba(0), ba(1), diagram->open_edges[b][a](2));
		diagram->open_edges[c][b] = Eigen::Vector3d(cb(0), cb(1), diagram->open_edges[c][b](2));

		diagram->open_edges[a][c] = Eigen::Vector3d(ac(0), ac(1), -1);
		if (diagram->open_edges[c][a] == Eigen::Vector3d(0,0,0)) {
			Eigen::Vector2d ac2 = get_intersection(Eigen::Vector2i(a,c), sweepline_y - 1);
			diagram->open_edges[c][a] = Eigen::Vector3d(ac2(0), ac2(1), 0);
		}

		root = removeUtil(root, p(0));
		root = removeUtil(root, p(0));
		root = insertUtil(root, Eigen::Vector2i(a, c), p(0));

		snprintf(buff, sizeof(buff), "REMOVE NODE (%d, %d) and NODE (%d, %d) FROM TREE", a, b, b, c);
		event_str = buff;
		msg.push_back(event_str);

		snprintf(buff, sizeof(buff), "INSERT NODE TO TREE: (%d, %d)", a, c);
		event_str = buff;
		msg.push_back(event_str);

		std::pair<node*, node*> next;
		next = searchUtil(root, p(0) + 0.0000001);
		// printf("p(0) to remove: %f\n", p(0));
		// if (next.second != NULL) {printf("second: (%d, %d)\n", next.second->break_point(0), next.second->break_point(1));}
		if (next.second != NULL && next.second->break_point(1) != b) {
			Eigen::Vector2d tmp1 = check_circle_event(a, c, next.second->break_point(1));
			if (tmp1(1) < sweepline_y) {
				printf("NEW CIRCLE event: (%d, %d, %d)\n", a, c, next.second->break_point(1));
				snprintf(buff, sizeof(buff), "NEW CIRCLE event: (%d, %d, %d)", 
					a, c, next.second->break_point(1));
				event_str = buff;
				msg.push_back(event_str);

				diagram->event_queue.push(Event(tmp1, a, c, next.second->break_point(1)));
			}
		}
		next = searchUtil(root, p(0) - 0.0000001);
		// if (next.first != NULL) {printf("first: (%d, %d)\n", next.first->break_point(0), next.first->break_point(1));}
		if (next.first != NULL && next.first->break_point(0) != b) {
			Eigen::Vector2d tmp2 = check_circle_event(next.first->break_point(0), a, c);
			// printf("tmp2(1): %f\n",tmp2(1));
			if (tmp2(1) < sweepline_y) {
				printf("NEW CIRCLE event: (%d, %d, %d)\n", next.first->break_point(0), a, c);
				snprintf(buff, sizeof(buff), "NEW CIRCLE event: (%d, %d, %d)", 
					next.first->break_point(0), a, c);
				event_str = buff;
				msg.push_back(event_str);

				diagram->event_queue.push(Event(tmp2, next.first->break_point(0), a, c));
			}
		}
	} else {
		printf("INVALID CIRCLE EVENT!!!!\n");
		snprintf(buff, sizeof(buff), "INVALID CIRCLE EVENT!!!! (%d, %d, %d)", a, b, c);
		event_str = buff;
		msg.push_back(event_str);
	}
}

node* AVL::insertUtil(node* head, Eigen::Vector2i bp, double x){
	if (head == NULL) {
		// printf("---> insert bp: (%d, %d) head is NULL\n", bp(0), bp(1));
		return new node(bp);
	} else {
		// printf("---> insert bp: (%d, %d) head is (%d, %d)\n", bp(0), bp(1), head->break_point(0), head->break_point(1));
	}
	double i = get_intersection(head->break_point)(0);

	if (x < i) {
		// printf("---> left, x: %f, i: %f\n", x, i);
		head->left = insertUtil(head->left, bp, x);
	}
	else if (x >= i) {
		// printf("---> right, x: %f, i: %f\n", x, i);
		head->right = insertUtil(head->right, bp, x);
	}
	head->height = 1 + max(height(head->left), height(head->right));
	int bal = height(head->left) - height(head->right);

	if (bal>1) {
		// printf("---> ROTATION!!!! bal>1 %d, %d\n", height(head->left), height(head->right));
		double i = get_intersection(head->left->break_point)(0);
		// printf("---> HEAD (%d, %d), x: %f, i: %f\n", head->break_point(0), head->break_point(1), x, i);
		if (x < i) {
			return rightRotation(head);
		} else {
			head->left = leftRotation(head->left);
			return rightRotation(head);
		}
	} else if (bal<-1) {
		// printf("---> ROTATION!!!! bal<-1 %d, %d\n", height(head->left), height(head->right));
		double i = get_intersection(head->right->break_point)(0);
		// printf("---> HEAD (%d, %d), x: %f, i: %f\n", head->break_point(0), head->break_point(1), x, i);
		if (x >= i) {
			return leftRotation(head);
		} else {
			head->right = rightRotation(head->right);
			return leftRotation(head);
		}
	}
	return head;
}

node* AVL::removeUtil(node* head, double x) {
	if (head == NULL) {
		return NULL;
	}
	Eigen::Vector2d tmp = get_intersection(head->break_point);
	double i = tmp(0);
	if (abs(x-i)<=0.00000001) {
		node* r = head->right;
		node* l = head->left;
		if (head->right == NULL) {
			head = l;
		} else if (head->left == NULL) {
			head = r;
		} else {
			while (r->left!=NULL) {
				r = r->left;
			}
			head->break_point = r->break_point;
			double k = get_intersection(r->break_point)(0);
			// printf("want to remove right leftmost\n");
			head->right = removeUtil(head->right, k);
		}
	} else if (x < i){
		// printf("x < i\n");
		head->left = removeUtil(head->left, x);
	} else if(x > i){
		// printf("x > i\n");
		head->right = removeUtil(head->right, x);
	}
	if (head == NULL) {
		return head;
	}

	head->height = 1 + max(height(head->left), height(head->right));
	int bal = height(head->left) - height(head->right);
	if (bal>1) {
		if (height(head->left) >= height(head->right)) {
			return rightRotation(head);
		} else {
			head->left = leftRotation(head->left);
			return rightRotation(head);
		}
	} else if(bal < -1) {
		if (height(head->right) >= height(head->left)) {
			return leftRotation(head);
		} else {
			head->right = rightRotation(head->right);
			return leftRotation(head);
		}
	}
	return head;
}

std::pair<node*, node*> AVL::searchUtil(node* head, double x) {
	node* cur_node = head;
	node* last_left = NULL;
	node* last_right = NULL;
	std::pair<node*, node*> tmp;

	while (true) {
		if (x < get_intersection(cur_node->break_point)(0)) {
			if (cur_node->left == NULL) {
				// return cur_node->break_point(0);
				tmp.first = last_right;
				tmp.second = cur_node;
				return tmp;
			} else {
				last_left = cur_node;
				cur_node = cur_node->left;
			}
		} else {
			if (cur_node->right == NULL) {
				// return cur_node->break_point(1);
				tmp.first = cur_node;
				tmp.second = last_left;
				return tmp;
			} else {
				last_right = cur_node;
				cur_node = cur_node->right;
			}
		}
	}
}


void AVL::report_tree(node* head){
	if (head == NULL) {
		return;
	}
	else {
		printf("(%d, %d) h: %d ", head->break_point(0), head->break_point(1), head->height);
		if (head->left == NULL) {
			printf("left NULL ");
		} else {
			printf("left (%d, %d) ", head->left->break_point(0), head->left->break_point(1));
		}

		if (head->right == NULL) {
			printf("right NULL \n");
		} else {
			printf("right (%d, %d) \n", head->right->break_point(0), head->right->break_point(1));
		}
		report_tree(head->left);
		report_tree(head->right);
	}
}

void AVL::report_tree(node* head, int n, int depth){
	if (head == NULL) {
		return;
	}
	else {
		tree_rep.push_back(Eigen::Vector3i(head->break_point(0), head->break_point(1), n));
		int level_rank = n - (pow(2, depth) - 1);
		int level_full_rank = pow(2, depth + 1) - 2;

		printf("(%d, %d) depth: %d, n: %d level_rank: %d, level_full_rank: %d ", head->break_point(0), head->break_point(1), depth, n, level_rank, level_full_rank);
		if (head->left == NULL) {
			printf("left NULL ");
		} else {
			printf("left (%d, %d) ", head->left->break_point(0), head->left->break_point(1));
		}

		if (head->right == NULL) {
			printf("right NULL \n");
		} else {
			printf("right (%d, %d) \n", head->right->break_point(0), head->right->break_point(1));
		}

		report_tree(head->left, level_full_rank + 2 * level_rank + 1, depth + 1);
		report_tree(head->right, level_full_rank + 2 * level_rank + 2, depth + 1);
	}
}

Eigen::Vector2d AVL::check_circle_event(int site1, int site2, int site3) {
	Eigen::Vector3d p1 = points.row(site1);
	Eigen::Vector3d p2 = points.row(site2);
	Eigen::Vector3d p3 = points.row(site3);

	double x1 = p1(0);
	double y1 = p1(1);
	double x2 = p2(0);
	double y2 = p2(1);
	double x3 = p3(0);
	double y3 = p3(1);

	double x12 = x1 - x2;
	double x13 = x1 - x3;
	double y12 = y1 - y2;
	double y13 = y1 - y3;
	double y31 = y3 - y1;
	double y21 = y2 - y1;
	double x31 = x3 - x1;
	double x21 = x2 - x1;

	double sx13 = pow(x1, 2) - pow(x3, 2);
	double sy13 = pow(y1, 2) - pow(y3, 2);
	double sx21 = pow(x2, 2) - pow(x1, 2);
	double sy21 = pow(y2, 2) - pow(y1, 2);

	double f = ((sx13) * (x12) + (sy13) * (x12) + (sx21) * (x13) + (sy21) * (x13))/ (2 * ((y31) * (x12) - (y21) * (x13)));
	double g = ((sx13) * (y12) + (sy13) * (y12) + (sx21) * (y13) + (sy21) * (y13))/ (2 * ((x31) * (y12) - (x21) * (y13)));
	double c = -pow(x1, 2) - pow(y1, 2) - 2 * g * x1 - 2 * f * y1;

	double h = -g;
	double k = -f;
	double r = sqrt(h * h + k * k - c);
	return Eigen::Vector2d(h, k-r);
}

void AVL::delete_event(Voronoi* diagram, Eigen::Vector2i bp1, Eigen::Vector2i bp2) {
	// printf("delete associte arc: (%d, %d) (%d, %d)\n", bp1(0), bp1(1), bp2(0), bp2(1));
	int a = bp1(0);
	int b = bp1(1);
	int c = bp2(0);

	printf("delete 1: %d, %d, %d\n", a, b, c);

	std::priority_queue<Event, std::vector<Event>, PQ_comparator> pq = diagram->event_queue;
	std::priority_queue<Event, std::vector<Event>, PQ_comparator> new_pq;
	while (!pq.empty()) {
		Event tmp = pq.top();
		// printf("(%f, %f)\n", tmp.position(0), tmp.position(1));
		pq.pop();
		if (tmp.type == 0 && (tmp.associate[0] == a && tmp.associate[1] == b && tmp.associate[2] == c)) {
		} else {
			new_pq.push(tmp);
		}
	}
	diagram->event_queue = new_pq;
}

void AVL::delete_event(Voronoi* diagram, int a, int b, int c) {
	// printf("delete2 associte arc: (%d, %d) (%d, %d)\n", bp1(0), bp1(1), bp2(0), bp2(1));
	printf("delete 2: %d, %d, %d\n", a, b, c);
	std::priority_queue<Event, std::vector<Event>, PQ_comparator> pq = diagram->event_queue;
	std::priority_queue<Event, std::vector<Event>, PQ_comparator> new_pq;
	while (!pq.empty()) {
		Event tmp = pq.top();
		// printf("(%f, %f)\n", tmp.position(0), tmp.position(1));
		pq.pop();
		if (tmp.type == 0 && ((tmp.associate[1] == a && tmp.associate[2] == b) || (tmp.associate[0] == b && tmp.associate[1] == c))) {
		} else {
			new_pq.push(tmp);
		}
	}
	diagram->event_queue = new_pq;
}

