namespace Cygraph {
	// cyg_driver.cpp
	class Graph;
	class Driver {
	public:
		virtual void render(Graph *s) = 0;
	};

	class GtkDriver {
		int width, height;
	public:
		GtkDriver(double width, double height);
		void render(Graph *s);
	};
	// cyg_graph.cpp
	class Axis;
	class Text;
	class GraphLine;
	class Graph {
	public:
		vector<Axis *> axises;
		vector<GraphLine *> lines;
		boundary padding;

		Graph();
		boundary get_boundary(Graphic g, double resolution);
		void draw(Graphic g, double resolution, rectangle rect);
	};

	class Axis {
	public:
		Text *text;
		int orientation;
		double begin, end;
		string label_format;

		struct Scale {
			double width, step, inner_height, outer_height;
			bool label;
		};
		vector<Scale> scales;
		boundary padding;

		Axis(double begin, double end, double step);
		void set_text(string s);
		double get_outer_height(Graphic g, double resolution);
		void draw(Graphic g, double resolution, rectangle rect);
		double get_location(Graphic g, double resolution, rectangle rect, double value);
	};

	class Text {
	public:
		string text;
		double font_size;
		string font_name;
		int orientation;
		boundary padding;

		Text();
		Text(string s);
		size get_size(Graphic g, double resolution);
		void draw(Graphic g, double resolution, double x, double y);
	};

	class GraphPoint;
	class GraphLine {
	public:
		Axis *axis[2];
		double weight;
		vector<GraphPoint *> points;
		double efficient;
		void *tag, *tag2;
		double (*func)(void *tag, void *tag2, double x);

		GraphLine(Axis *axis1, Axis *axis2);
		vector<location> get_points();
		void draw(Graphic g, double resolution, map<Axis *, rectangle> locs);
	};

	class GraphPoint {
	public:
		enum PointShape {
			PS_CIRCLE = 1,
			PS_TRIANGLE = 2,
			PS_SQUARE = 4,
		} shape;
		double size;
		bool invert;

		GraphPoint(PointShape);
		void draw(Graphic g, double resolution, double x, double y);
	};
}
