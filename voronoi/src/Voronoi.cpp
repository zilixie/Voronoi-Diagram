#include "Voronoi.h"
#include <fstream>
#include <string>
#include <iostream>
#include <random>
// #include "AVL.h"


bool compare_vectors(Event &e1, Event &e2) {
	if (e1.position(1) > e2.position(1)) {
		return true;
	} else if (e1.position(1) < e2.position(1)) {
		return false;
	} else {
		return (e1.position(0) < e2.position(0));
	}
}

Voronoi::Voronoi(int n) {
	srand(time(0));
	sweepline_y = 0.95;
	sub_div = 1000;

	// vor_vtx_map = Eigen::MatrixXd::Zero(n, n);
	for (int i=0; i<n; i++) {
		std::vector<Eigen::Vector2d> row_vec;
		for (int j=0; j<n; j++) {
			row_vec.push_back(Eigen::Vector2d(0,0));
		}
		vor_vtx_map.push_back(row_vec);
	}

	for (int i=0; i<n; i++) {
		std::vector<Eigen::Vector3d> row_vec;
		for (int j=0; j<n; j++) {
			row_vec.push_back(Eigen::Vector3d(0,0,0));
		}
		open_edges.push_back(row_vec);
	}
	std::vector<Event> tmp;
	
	for (int i=0; i<n; i++) {
		Eigen::Vector2d pair = generate_point();
		tmp.push_back(Event(pair, i));
	}

// tmp.push_back(Event(Eigen::Vector2d(-1.117582, 0.793325), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.472035, 0.571038), 1));
// tmp.push_back(Event(Eigen::Vector2d(-1.480546, 0.536816), 2));
// tmp.push_back(Event(Eigen::Vector2d(-0.768346, 0.493440), 3));
// tmp.push_back(Event(Eigen::Vector2d(0.077124, 0.339769), 4));
// tmp.push_back(Event(Eigen::Vector2d(-1.146297, 0.310753), 5));
// tmp.push_back(Event(Eigen::Vector2d(0.045344, 0.240231), 6));
// tmp.push_back(Event(Eigen::Vector2d(0.354250, 0.063342), 7));
// tmp.push_back(Event(Eigen::Vector2d(-0.100445, 0.011209), 8));
// tmp.push_back(Event(Eigen::Vector2d(0.200143, 0.002491), 9));
// tmp.push_back(Event(Eigen::Vector2d(-0.068688, -0.185841), 10));
// tmp.push_back(Event(Eigen::Vector2d(-1.349711, -0.312885), 11));
// tmp.push_back(Event(Eigen::Vector2d(-1.112430, -0.330660), 12));
// tmp.push_back(Event(Eigen::Vector2d(-0.274922, -0.333410), 13));
// tmp.push_back(Event(Eigen::Vector2d(-1.542611, -0.367951), 14));
// tmp.push_back(Event(Eigen::Vector2d(0.162623, -0.472512), 15));
// tmp.push_back(Event(Eigen::Vector2d(-0.036941, -0.532155), 16));
// tmp.push_back(Event(Eigen::Vector2d(-0.883739, -0.642253), 17));
// tmp.push_back(Event(Eigen::Vector2d(-0.515917, -0.658552), 18));
// tmp.push_back(Event(Eigen::Vector2d(-0.790812, -0.780984), 19));

// tmp.push_back(Event(Eigen::Vector2d(-0.643018, 0.799249), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.930758, 0.763748), 0));
// tmp.push_back(Event(Eigen::Vector2d(-1.384738, 0.725903), 0));
// tmp.push_back(Event(Eigen::Vector2d(-1.599770, 0.696249), 0));
// tmp.push_back(Event(Eigen::Vector2d(0.026334, 0.631975), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.554026, 0.551895), 0));
// tmp.push_back(Event(Eigen::Vector2d(-1.424854, 0.542188), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.677558, 0.379554), 0));
// tmp.push_back(Event(Eigen::Vector2d(-1.269932, 0.364114), 0));
// tmp.push_back(Event(Eigen::Vector2d(-1.019461, 0.095824), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.062485, 0.013766), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.286926, -0.132841), 0));
// tmp.push_back(Event(Eigen::Vector2d(0.098981, -0.176534), 0));
// tmp.push_back(Event(Eigen::Vector2d(-1.353752, -0.251417), 0));
// tmp.push_back(Event(Eigen::Vector2d(0.272949, -0.270634), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.776852, -0.284088), 0));
// tmp.push_back(Event(Eigen::Vector2d(-1.465259, -0.321840), 0));
// tmp.push_back(Event(Eigen::Vector2d(-1.399714, -0.628508), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.283754, -0.685578), 0));
// tmp.push_back(Event(Eigen::Vector2d(-1.200992, -0.691430), 0));

// tmp.push_back(Event(Eigen::Vector2d(-0.323163, 0.607531), 0));
// tmp.push_back(Event(Eigen::Vector2d(0.270779, 0.586209), 1));
// tmp.push_back(Event(Eigen::Vector2d(0.154447, 0.586049), 2));
// tmp.push_back(Event(Eigen::Vector2d(-0.338591, 0.501873), 3));
// tmp.push_back(Event(Eigen::Vector2d(-0.225785, 0.435168), 4));
// tmp.push_back(Event(Eigen::Vector2d(0.103980, 0.393550), 5));
// tmp.push_back(Event(Eigen::Vector2d(0.544554, 0.320872), 6));
// tmp.push_back(Event(Eigen::Vector2d(-0.445609, 0.253303), 7));
// tmp.push_back(Event(Eigen::Vector2d(-0.386968, 0.225380), 8));
// tmp.push_back(Event(Eigen::Vector2d(0.630416, 0.209072), 9));
// tmp.push_back(Event(Eigen::Vector2d(-0.422479, 0.202113), 10));
// tmp.push_back(Event(Eigen::Vector2d(-0.234660, 0.068802), 11));
// tmp.push_back(Event(Eigen::Vector2d(0.276363, 0.038276), 12));
// tmp.push_back(Event(Eigen::Vector2d(0.018182, -0.017690), 13));
// tmp.push_back(Event(Eigen::Vector2d(0.167643, -0.029348), 14));
// tmp.push_back(Event(Eigen::Vector2d(0.118995, -0.048667), 15));
// tmp.push_back(Event(Eigen::Vector2d(0.135458, -0.150349), 16));
// tmp.push_back(Event(Eigen::Vector2d(0.269205, -0.280381), 17));
// tmp.push_back(Event(Eigen::Vector2d(-0.437168, -0.290060), 18));
// tmp.push_back(Event(Eigen::Vector2d(-0.496863, -0.373748), 19));
// tmp.push_back(Event(Eigen::Vector2d(0.756805, -0.381275), 20));
// tmp.push_back(Event(Eigen::Vector2d(-0.095227, -0.488550), 21));
// tmp.push_back(Event(Eigen::Vector2d(-0.695549, -0.493633), 22));
// tmp.push_back(Event(Eigen::Vector2d(-0.355888, -0.604794), 23));
// tmp.push_back(Event(Eigen::Vector2d(0.001201, -0.610090), 24));
// tmp.push_back(Event(Eigen::Vector2d(0.108583, -0.641224), 25));
// tmp.push_back(Event(Eigen::Vector2d(-0.217186, -0.645840), 26));
// tmp.push_back(Event(Eigen::Vector2d(-0.517919, -0.657460), 27));
// tmp.push_back(Event(Eigen::Vector2d(-0.328288, -0.739845), 28));
// tmp.push_back(Event(Eigen::Vector2d(0.620361, -0.788900), 29));


// tmp.push_back(Event(Eigen::Vector2d(-0.456621, 0.775636), 0));
// tmp.push_back(Event(Eigen::Vector2d(0.457949, 0.754310), 1));
// tmp.push_back(Event(Eigen::Vector2d(-0.020712, 0.696663), 2));
// tmp.push_back(Event(Eigen::Vector2d(0.114180, 0.629042), 3));
// tmp.push_back(Event(Eigen::Vector2d(-0.799159, 0.529802), 4));
// tmp.push_back(Event(Eigen::Vector2d(-0.711293, 0.500817), 5));
// tmp.push_back(Event(Eigen::Vector2d(0.372156, 0.432453), 6));
// tmp.push_back(Event(Eigen::Vector2d(-0.447123, 0.409802), 7));
// tmp.push_back(Event(Eigen::Vector2d(-0.569360, 0.366446), 8));
// tmp.push_back(Event(Eigen::Vector2d(0.385476, 0.302502), 9));
// tmp.push_back(Event(Eigen::Vector2d(0.579772, 0.227326), 10));
// tmp.push_back(Event(Eigen::Vector2d(-0.209140, 0.183753), 11));
// tmp.push_back(Event(Eigen::Vector2d(-0.548145, 0.134427), 12));
// tmp.push_back(Event(Eigen::Vector2d(0.114525, 0.028131), 13));
// tmp.push_back(Event(Eigen::Vector2d(0.104529, 0.013139), 14));
// tmp.push_back(Event(Eigen::Vector2d(0.327383, -0.067680), 15));
// tmp.push_back(Event(Eigen::Vector2d(-0.126430, -0.103363), 16));
// tmp.push_back(Event(Eigen::Vector2d(-0.648784, -0.115011), 17));
// tmp.push_back(Event(Eigen::Vector2d(-0.377096, -0.256206), 18));
// tmp.push_back(Event(Eigen::Vector2d(0.596308, -0.259353), 19));
// tmp.push_back(Event(Eigen::Vector2d(-0.273429, -0.316785), 20));
// tmp.push_back(Event(Eigen::Vector2d(-0.680786, -0.374900), 21));
// tmp.push_back(Event(Eigen::Vector2d(-0.182995, -0.398258), 22));
// tmp.push_back(Event(Eigen::Vector2d(0.017301, -0.413849), 23));
// tmp.push_back(Event(Eigen::Vector2d(-0.490964, -0.434574), 24));
// tmp.push_back(Event(Eigen::Vector2d(0.214169, -0.458953), 25));
// tmp.push_back(Event(Eigen::Vector2d(-0.143304, -0.510562), 26));
// tmp.push_back(Event(Eigen::Vector2d(-0.723065, -0.547657), 27));
// tmp.push_back(Event(Eigen::Vector2d(0.020238, -0.650859), 28));
// tmp.push_back(Event(Eigen::Vector2d(-0.352086, -0.714810), 29));


// tmp.push_back(Event(Eigen::Vector2d(0.289258, 0.761360), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.764876, 0.726642), 1));
// tmp.push_back(Event(Eigen::Vector2d(-0.355905, 0.711679), 2));
// tmp.push_back(Event(Eigen::Vector2d(-0.679295, 0.680889), 3));
// tmp.push_back(Event(Eigen::Vector2d(0.094278, 0.536131), 4));
// tmp.push_back(Event(Eigen::Vector2d(0.279436, 0.483435), 5));
// tmp.push_back(Event(Eigen::Vector2d(-0.451498, 0.471446), 6));
// tmp.push_back(Event(Eigen::Vector2d(0.383577, 0.383037), 7));
// tmp.push_back(Event(Eigen::Vector2d(0.579114, 0.373863), 8));
// tmp.push_back(Event(Eigen::Vector2d(-0.311659, 0.347428), 9));
// tmp.push_back(Event(Eigen::Vector2d(-0.337174, 0.310703), 10));
// tmp.push_back(Event(Eigen::Vector2d(0.390140, 0.285794), 11));
// tmp.push_back(Event(Eigen::Vector2d(-0.318047, 0.177568), 12));
// tmp.push_back(Event(Eigen::Vector2d(0.036082, 0.022998), 13));
// tmp.push_back(Event(Eigen::Vector2d(0.140511, -0.034769), 14));
// tmp.push_back(Event(Eigen::Vector2d(-0.058553, -0.095002), 15));
// tmp.push_back(Event(Eigen::Vector2d(-0.413082, -0.268505), 16));
// tmp.push_back(Event(Eigen::Vector2d(-0.133104, -0.286615), 17));
// tmp.push_back(Event(Eigen::Vector2d(0.710639, -0.289327), 18));
// tmp.push_back(Event(Eigen::Vector2d(-0.698682, -0.348043), 19));
// tmp.push_back(Event(Eigen::Vector2d(-0.620814, -0.414324), 20));
// tmp.push_back(Event(Eigen::Vector2d(0.508234, -0.509220), 21));
// tmp.push_back(Event(Eigen::Vector2d(0.190454, -0.637406), 22));
// tmp.push_back(Event(Eigen::Vector2d(-0.784570, -0.673518), 23));
// tmp.push_back(Event(Eigen::Vector2d(0.320015, -0.701691), 24));
// tmp.push_back(Event(Eigen::Vector2d(0.454149, -0.709702), 25));
// tmp.push_back(Event(Eigen::Vector2d(0.036608, -0.729186), 26));



	// (-0.388174, 0.762822)
	// (0.066769, 0.588095)
	// (0.776341, -0.033203)
	// (-0.689910, -0.112971)
	// (-0.290843, -0.196260)
	// (0.317271, -0.426893)
	// (0.652837, -0.561667)
	// (-0.049921, -0.623218)
	// (0.500034, -0.736341)
	// (0.349712, -0.788628)

	// (-0.119065, 0.471217)
	// (-0.595545, 0.271683)
	// (-0.025412, 0.097843)
	// (-0.355955, -0.141471)
	// (-0.248099, -0.202418)
	// (-0.444021, -0.267067)
	// (0.095177, -0.367141)
	// (-0.094752, -0.502435)
	// (-0.221944, -0.618411)
	// (0.659398, -0.704190)

	// (0.035071, 0.639801)
	// (0.555328, 0.590499)
	// (0.180052, 0.533864)
	// (-0.152004, 0.464761)
	// (-0.458645, 0.349663)
	// (-0.010359, 0.290451)
	// (-0.275115, 0.143819)
	// (0.231041, -0.090786)
	// (0.007785, -0.361645)
	// (-0.428703, -0.411738)

	// important !!!
	// (-0.206917, 0.742457)
	// (0.776574, 0.675611)
	// (-0.572959, 0.672080)
	// (-0.774116, 0.630363)
	// (-0.686352, 0.484760)
	// (0.242677, 0.278060)
	// (-0.330227, 0.277553)
	// (0.082553, 0.261190)
	// (-0.084442, -0.022438)
	// (-0.035322, -0.055259)
	// (-0.686202, -0.199882)
	// (0.686933, -0.312681)
	// (-0.635672, -0.543148)
	// (0.725757, -0.609705)
	// (0.168465, -0.616507)
	// (0.775542, -0.658507)
	// (-0.244701, -0.689370)
	// (0.582192, -0.692859)
	// (-0.740495, -0.701757)
	// (-0.346850, -0.701979)


	// ALSO IMPORTANT !!
// tmp.push_back(Event(Eigen::Vector2d(0.150076, 0.721711), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.001386, 0.705563), 1));
// tmp.push_back(Event(Eigen::Vector2d(-0.152752, 0.698504), 2));
// tmp.push_back(Event(Eigen::Vector2d(0.295346, 0.682423), 3));
// tmp.push_back(Event(Eigen::Vector2d(0.709933, 0.638847), 4));
// tmp.push_back(Event(Eigen::Vector2d(0.424813, 0.627770), 5));
// tmp.push_back(Event(Eigen::Vector2d(0.031542, 0.534393), 6));
// tmp.push_back(Event(Eigen::Vector2d(-0.798781, 0.496597), 7));
// tmp.push_back(Event(Eigen::Vector2d(-0.280902, 0.472743), 8));
// tmp.push_back(Event(Eigen::Vector2d(-0.796028, 0.361547), 9));
// tmp.push_back(Event(Eigen::Vector2d(-0.202089, 0.292490), 10));
// tmp.push_back(Event(Eigen::Vector2d(0.757225, 0.277433), 11));
// tmp.push_back(Event(Eigen::Vector2d(-0.458746, 0.260482), 12));
// tmp.push_back(Event(Eigen::Vector2d(0.632511, 0.219970), 13));
// tmp.push_back(Event(Eigen::Vector2d(0.678677, 0.131290), 14));
// tmp.push_back(Event(Eigen::Vector2d(0.197541, 0.074386), 15));
// tmp.push_back(Event(Eigen::Vector2d(0.679331, -0.091971), 16));
// tmp.push_back(Event(Eigen::Vector2d(0.712743, -0.136503), 17));
// tmp.push_back(Event(Eigen::Vector2d(-0.568062, -0.224393), 18));
// tmp.push_back(Event(Eigen::Vector2d(0.325750, -0.317449), 19));
// tmp.push_back(Event(Eigen::Vector2d(0.551839, -0.435706), 20));
// tmp.push_back(Event(Eigen::Vector2d(0.612194, -0.458571), 21));
// tmp.push_back(Event(Eigen::Vector2d(-0.174244, -0.511365), 22));
// tmp.push_back(Event(Eigen::Vector2d(0.096881, -0.524320), 23));
// tmp.push_back(Event(Eigen::Vector2d(0.527082, -0.533293), 24));
// tmp.push_back(Event(Eigen::Vector2d(-0.494112, -0.540154), 25));
// tmp.push_back(Event(Eigen::Vector2d(0.184938, -0.549729), 26));
// tmp.push_back(Event(Eigen::Vector2d(0.201976, -0.583879), 27));
// tmp.push_back(Event(Eigen::Vector2d(0.687768, -0.676569), 28));
// tmp.push_back(Event(Eigen::Vector2d(0.736410, -0.764015), 29));


// tmp.push_back(Event(Eigen::Vector2d(-0.011664, 0.762003), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.248427, 0.694655), 1));
// tmp.push_back(Event(Eigen::Vector2d(0.484502, 0.632130), 2));
// tmp.push_back(Event(Eigen::Vector2d(0.293016, 0.623392), 3));
// tmp.push_back(Event(Eigen::Vector2d(-0.679303, 0.552086), 4));
// tmp.push_back(Event(Eigen::Vector2d(0.223559, 0.548300), 5));
// tmp.push_back(Event(Eigen::Vector2d(-0.519943, 0.514163), 6));
// tmp.push_back(Event(Eigen::Vector2d(-0.405040, 0.496761), 7));
// tmp.push_back(Event(Eigen::Vector2d(-0.595820, 0.455540), 8));
// tmp.push_back(Event(Eigen::Vector2d(0.596064, 0.452477), 9));
// tmp.push_back(Event(Eigen::Vector2d(0.730190, 0.309840), 10));
// tmp.push_back(Event(Eigen::Vector2d(-0.425711, 0.273549), 11));
// tmp.push_back(Event(Eigen::Vector2d(-0.135462, 0.082917), 12));
// tmp.push_back(Event(Eigen::Vector2d(-0.272360, 0.043756), 13));
// tmp.push_back(Event(Eigen::Vector2d(-0.368710, -0.115066), 14));
// tmp.push_back(Event(Eigen::Vector2d(-0.544543, -0.128820), 15));
// tmp.push_back(Event(Eigen::Vector2d(0.209522, -0.160496), 16));
// tmp.push_back(Event(Eigen::Vector2d(0.461036, -0.165089), 17));
// tmp.push_back(Event(Eigen::Vector2d(0.268639, -0.187779), 18));
// tmp.push_back(Event(Eigen::Vector2d(-0.715048, -0.205403), 19));
// tmp.push_back(Event(Eigen::Vector2d(-0.798161, -0.296385), 20));
// tmp.push_back(Event(Eigen::Vector2d(-0.061144, -0.449073), 21));
// tmp.push_back(Event(Eigen::Vector2d(0.593626, -0.530049), 22));
// tmp.push_back(Event(Eigen::Vector2d(0.541454, -0.574772), 23));
// tmp.push_back(Event(Eigen::Vector2d(-0.016699, -0.652706), 24));
// tmp.push_back(Event(Eigen::Vector2d(0.604087, -0.703789), 25));
// tmp.push_back(Event(Eigen::Vector2d(0.270129, -0.738526), 26));
// tmp.push_back(Event(Eigen::Vector2d(0.265083, -0.745063), 27));
// tmp.push_back(Event(Eigen::Vector2d(0.511932, -0.759370), 28));
// tmp.push_back(Event(Eigen::Vector2d(0.150366, -0.791980), 29));


// tmp.push_back(Event(Eigen::Vector2d(0.324099, 0.734916), 0));
// tmp.push_back(Event(Eigen::Vector2d(-0.033565, 0.673419), 1));
// tmp.push_back(Event(Eigen::Vector2d(0.448703, 0.544405), 2));
// tmp.push_back(Event(Eigen::Vector2d(0.388820, 0.504123), 3));
// tmp.push_back(Event(Eigen::Vector2d(-0.451784, 0.472429), 4));
// tmp.push_back(Event(Eigen::Vector2d(0.048006, 0.436980), 5));
// tmp.push_back(Event(Eigen::Vector2d(-0.687405, 0.382941), 6));
// tmp.push_back(Event(Eigen::Vector2d(-0.037394, 0.323170), 7));
// tmp.push_back(Event(Eigen::Vector2d(0.586536, 0.307231), 8));
// tmp.push_back(Event(Eigen::Vector2d(0.794909, 0.035507), 9));
// tmp.push_back(Event(Eigen::Vector2d(-0.575949, 0.030459), 10));
// tmp.push_back(Event(Eigen::Vector2d(-0.582614, 0.007240), 11));
// tmp.push_back(Event(Eigen::Vector2d(-0.266188, -0.214464), 12));
// tmp.push_back(Event(Eigen::Vector2d(0.427139, -0.270951), 13));
// tmp.push_back(Event(Eigen::Vector2d(-0.574254, -0.283085), 14));
// tmp.push_back(Event(Eigen::Vector2d(-0.305224, -0.296006), 15));
// tmp.push_back(Event(Eigen::Vector2d(-0.189089, -0.424845), 16));
// tmp.push_back(Event(Eigen::Vector2d(-0.715823, -0.432902), 17));
// tmp.push_back(Event(Eigen::Vector2d(0.758702, -0.488232), 18));
// tmp.push_back(Event(Eigen::Vector2d(-0.262492, -0.507278), 19));
// tmp.push_back(Event(Eigen::Vector2d(-0.081814, -0.642696), 20));
// tmp.push_back(Event(Eigen::Vector2d(0.425879, -0.644235), 21));
// tmp.push_back(Event(Eigen::Vector2d(-0.475460, -0.651039), 22));
// tmp.push_back(Event(Eigen::Vector2d(0.305738, -0.666951), 23));
// tmp.push_back(Event(Eigen::Vector2d(-0.567901, -0.708630), 24));
// tmp.push_back(Event(Eigen::Vector2d(0.590851, -0.764065), 25));
// tmp.push_back(Event(Eigen::Vector2d(0.084871, -0.767222), 26));
// tmp.push_back(Event(Eigen::Vector2d(-0.243659, -0.773580), 27));
// tmp.push_back(Event(Eigen::Vector2d(0.683098, -0.779085), 28));
// tmp.push_back(Event(Eigen::Vector2d(0.313536, -0.793798), 29));

	// tmp.push_back(Event(Eigen::Vector2d(-0.024534, 0.451438), 0));
	// tmp.push_back(Event(Eigen::Vector2d(0.660118, 0.205382), 1));
	// tmp.push_back(Event(Eigen::Vector2d(0.117387, 0.127632), 2));
	// tmp.push_back(Event(Eigen::Vector2d(-0.027698, 0.082767), 3));
	// tmp.push_back(Event(Eigen::Vector2d(0.330712, -0.117980), 4));
	// tmp.push_back(Event(Eigen::Vector2d(-0.493422, -0.137555), 5));
	// tmp.push_back(Event(Eigen::Vector2d(-0.005817, -0.163743), 6));
	// tmp.push_back(Event(Eigen::Vector2d(-0.495333, -0.264939), 7));
	// tmp.push_back(Event(Eigen::Vector2d(0.659313, -0.534825), 8));
	// tmp.push_back(Event(Eigen::Vector2d(-0.007468, -0.708923), 9));

	// tmp.push_back(Event(Eigen::Vector2d(0.035071, 0.639801), 0));
	// tmp.push_back(Event(Eigen::Vector2d(0.555328, 0.590499), 1));
	// tmp.push_back(Event(Eigen::Vector2d(0.180052, 0.533864), 2));
	// tmp.push_back(Event(Eigen::Vector2d(-0.152004, 0.464761), 3));
	// tmp.push_back(Event(Eigen::Vector2d(-0.458645, 0.349663), 4));
	// tmp.push_back(Event(Eigen::Vector2d(-0.010359, 0.290451), 5));
	// tmp.push_back(Event(Eigen::Vector2d(-0.275115, 0.143819), 6));
	// tmp.push_back(Event(Eigen::Vector2d(0.231041, -0.090786), 7));
	// tmp.push_back(Event(Eigen::Vector2d(0.007785, -0.361645), 8));
	// tmp.push_back(Event(Eigen::Vector2d(-0.428703, -0.411738), 9));

	// tmp.push_back(Event(Eigen::Vector2d(0.0, 0.7), 0));
	// tmp.push_back(Event(Eigen::Vector2d(-0.4, 0.6), 1));
	// tmp.push_back(Event(Eigen::Vector2d(0.4, 0.6), 2));
	// tmp.push_back(Event(Eigen::Vector2d(0.0, 0.35), 3));
	// tmp.push_back(Event(Eigen::Vector2d(-0.4, -0.2), 4));

	// tmp.push_back(Event(Eigen::Vector2d(-0.206917, 0.742457), 0));
	// tmp.push_back(Event(Eigen::Vector2d(0.776574, 0.675611), 1));
	// tmp.push_back(Event(Eigen::Vector2d(-0.572959, 0.672080), 2));
	// tmp.push_back(Event(Eigen::Vector2d(-0.774116, 0.630363), 3));
	// tmp.push_back(Event(Eigen::Vector2d(-0.686352, 0.484760), 4));
	// tmp.push_back(Event(Eigen::Vector2d(0.242677, 0.278060), 5));
	// tmp.push_back(Event(Eigen::Vector2d(-0.330227, 0.277553), 6));
	// tmp.push_back(Event(Eigen::Vector2d(0.082553, 0.261190), 7));
	// tmp.push_back(Event(Eigen::Vector2d(-0.084442, -0.022438), 8));
	// tmp.push_back(Event(Eigen::Vector2d(-0.035322, -0.055259), 9));
	// tmp.push_back(Event(Eigen::Vector2d(-0.686202, -0.199882), 10));
	// tmp.push_back(Event(Eigen::Vector2d(0.686933, -0.312681), 11));
	// tmp.push_back(Event(Eigen::Vector2d(-0.635672, -0.543148), 12));
	// tmp.push_back(Event(Eigen::Vector2d(0.725757, -0.609705), 13));
	// tmp.push_back(Event(Eigen::Vector2d(0.168465, -0.616507), 14));
	// tmp.push_back(Event(Eigen::Vector2d(0.775542, -0.658507), 15));
	// tmp.push_back(Event(Eigen::Vector2d(-0.244701, -0.689370), 16));
	// tmp.push_back(Event(Eigen::Vector2d(0.582192, -0.692859), 17));
	// tmp.push_back(Event(Eigen::Vector2d(-0.740495, -0.701757), 18));
	// tmp.push_back(Event(Eigen::Vector2d(-0.346850, -0.701979), 19));

	sort(tmp.begin(), tmp.end(), compare_vectors);
	for (int i=0; i<n; i++) {
		tmp[i].associate[0] = i;
	}

	beach_line = Eigen::MatrixXd::Zero(sub_div + 1, 3);
	for (int i=0; i<sub_div + 1; i++) {
		beach_line.row(i) = Eigen::RowVector3d(0,100000,0);
	}
	printf("points:\n");
	for (int i=0; i<n; i++) {
		int cur_rows = points.rows();
		points.conservativeResize(cur_rows + 1, 3);
		points.row(cur_rows) = Eigen::RowVector3d(tmp[i].position(0), tmp[i].position(1), 0);
		event_queue.push(tmp[i]);
		printf("(%f, %f)\n", tmp[i].position(0), tmp[i].position(1));
	}
}

Eigen::Vector2d Voronoi::generate_point() {
	double r1 = -1.6 + static_cast <double> (rand())/(static_cast <double>(RAND_MAX/(2.0)));
	double r2 = -0.8 + static_cast <double> (rand())/(static_cast <double>(RAND_MAX/(1.6)));
	return Eigen::Vector2d(r1, r2);
}

Eigen::MatrixXd Voronoi::interpolate_line(double start, 
							   double end, 
							   double a, 
							   double b, 
							   double c) {
	Eigen::MatrixXd line_points;
	line_points.resize(sub_div + 1, 3);
	double stride = (end - start)/sub_div;
	// printf("%f, %f, %f\n", direction(0), direction(1), direction(2));
	for (int i=0; i<sub_div + 1; i++) {
		// line_points.row(i) = start + direction * i;
		double x = start + i*stride;
		double y = a*x*x + b*x + c;
		line_points.row(i) = Eigen::RowVector3d(x, y, 0);
	}
	// std::cout << line_points << std::endl;
	// lines.push_back(line_points);
	return line_points;
}

void Voronoi::update_lines_param() {
	// lines_param.clear();
	std::vector<Eigen::Vector3d> tmp;
	for (int i = 0; i < points.rows(); i++) {
		if (points(i,1) >= sweepline_y) {
			double a = points(i,0);
			double b = points(i,1);
			double c = sweepline_y;

			double A = 1/(2*b -2*c);
			double B = -2*a/(2*b -2*c);
			double C = (a*a + b*b - c*c)/(2*b - 2*c);

			tmp.push_back(Eigen::Vector3d(A,B,C));
		} else {
			break;
		}
	}
	lines_param = tmp;
	// printf("%ld ", lines_param.size());
	// printf("--> %ld\n", lines_param.size());
}

void Voronoi::update_lines_points() {
	// lines.clear();
	std::vector<Eigen::MatrixXd> tmp;
	for (int i = 0; i < lines_param.size(); i++) {
		Eigen::Vector3d params = lines_param[i];
		double A = params(0);
		double B = params(1);
		double C = params(2);

		tmp.push_back(interpolate_line(-1.8,0.8, A,B,C));
	}
	lines = tmp;
	// printf("%ld \n", lines.size());
}

void Voronoi::update_data(double yoffset, Eigen::MatrixXd tree_edges) {
	// printf("%ld\n", tree_edges.rows());
	int unchange_rows = points.rows() + vor_vtx.rows() + edges.size() * 2 + 12 + 2 + 1000 + tree_edges.rows();
	data.conservativeResize(unchange_rows + ((sub_div + 1) * (lines.size() + 1)), 3);
	data.row(points.rows() + vor_vtx.rows() + edges.size() * 2 + 12) = Eigen::RowVector3d(-2, sweepline_y, 0);
	data.row(points.rows() + vor_vtx.rows() + edges.size() * 2 + 12 + 1) = Eigen::RowVector3d(0.8, sweepline_y, 0);

	for (int i=0; i<tree_edges.rows(); i++) {
		data.row(points.rows() + vor_vtx.rows() + edges.size() * 2 + 12 + 2 + 1000 + i) = tree_edges.row(i);
	}

	if (yoffset < 0) {
		beach_line = Eigen::MatrixXd::Zero(sub_div + 1, 3);
		for (int i=0; i<sub_div + 1; i++) {
			beach_line.row(i) = Eigen::RowVector3d(0,100000,0);
		}
	}
	for (int i=0; i<lines.size(); i++) {
		for (int j=0; j<(sub_div + 1) ; j++) {
			data.row(unchange_rows + i*(sub_div + 1) + j) = lines[i].row(j);
			if (lines[i].row(j)(1) < beach_line(j,1)) {
				beach_line.row(j) = lines[i].row(j);
			}
		}
	}
	for (int j=0; j<(sub_div + 1) ; j++) {
		data.row(unchange_rows + lines.size()*(sub_div + 1) + j) = beach_line.row(j);
	}
}


void Voronoi::extend_open_edges() {
	for (int i=0; i<points.rows(); i++) {
		for (int j=0; j<points.rows(); j++) {
			if (open_edges[i][j](2) == 1) {
				Eigen::Matrix2d m;
				Eigen::Vector2d ij = Eigen::Vector2d(open_edges[i][j](0), open_edges[i][j](1));
				Eigen::Vector2d ji = Eigen::Vector2d(open_edges[j][i](0), open_edges[j][i](1));
				Eigen::Vector2d d = ij - ji;
				ij = ji + 1000*d;
				m << ij, ji;
				edges.push_back(m);
			} else if (open_edges[i][j](2) == -1) {
				Eigen::Matrix2d m;
				Eigen::Vector2d ij = Eigen::Vector2d(open_edges[i][j](0), open_edges[i][j](1));
				Eigen::Vector2d ji = Eigen::Vector2d(open_edges[j][i](0), open_edges[j][i](1));
				Eigen::Vector2d d = ji - ij;
				ji = ij + 1000*d;
				m << ij, ji;
				edges.push_back(m);
			}
		}
	}
}
