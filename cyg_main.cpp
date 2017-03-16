#include "cyg_common.hpp"

namespace Cygraph {
	int CygMain(int argc, char **argv) {
		UNUSED(argc, argv);
		/*
		auto *graph = new Graph();
		auto *axis = new Axis(0, 10, 1);
		axis->set_text("ドレイン-ソース間電圧(Vds) [V]");
		graph->axises.push_back(axis);
		auto *axis2 = new Axis(250, 0, -50);
		axis2->orientation = 0;
		axis2->set_text("ドレイン-ソース間電流(Id) [mA]");
		graph->axises.push_back(axis2);
		auto *axis3 = new Axis(250, 0, -50);
		axis3->orientation = 3;
		axis3->set_text("ドレイン-ソース間電流(Id) [mA]");
		graph->axises.push_back(axis3);
		auto *line1 = new GraphLine(axis, axis3);
		graph->lines.push_back(line1);
		auto *line2 = new GraphLine(axis, axis3);
		line2->draw_line = false;
		auto *point1 = new GraphPoint(GraphPoint::PS_CIRCLE);
		auto *point2 = new GraphPoint(GraphPoint::PS_CIRCLE);
		auto *point3 = new GraphPoint(GraphPoint::PS_CIRCLE);
		point2->size = 9.0;
		point2->invert = true;
		point3->size = 2;
		line2->points.push_back(point1);
		line2->points.push_back(point2);
		line2->points.push_back(point3);
		graph->lines.push_back(line2);
		auto *line3 = new GraphLine(axis, axis3);
		line3->efficient = 1.5;
		graph->lines.push_back(line3);
		auto *line4 = new GraphLine(axis, axis3);
		line4->efficient = 1.5;
		line4->draw_line = false;
		auto *point4 = new GraphPoint(GraphPoint::PS_TRIANGLE);
		auto *point5 = new GraphPoint(GraphPoint::PS_TRIANGLE);
		auto *point6 = new GraphPoint(GraphPoint::PS_CIRCLE);
		point5->size = 9.0;
		point5->invert = true;
		point6->size = 2;
		line4->points.push_back(point4);
		line4->points.push_back(point5);
		line4->points.push_back(point6);
		graph->lines.push_back(line4);
		auto *line5 = new GraphLine(axis, axis3);
		line5->efficient = 2.0;
		graph->lines.push_back(line5);
		auto *line6 = new GraphLine(axis, axis3);
		line6->efficient = 2.0;
		line6->draw_line = false;
		auto *point7 = new GraphPoint(GraphPoint::PS_SQUARE);
		auto *point8 = new GraphPoint(GraphPoint::PS_SQUARE);
		auto *point9 = new GraphPoint(GraphPoint::PS_CIRCLE);
		point8->size = 9.0;
		point8->invert = true;
		point9->size = 2;
		line6->points.push_back(point7);
		line6->points.push_back(point8);
		line6->points.push_back(point9);
		graph->lines.push_back(line6);
		auto *driver = new GtkDriver(800, 600);
		driver->render(graph);
		*/
const string sample_script = R"([
print('hello,world\n')
])";
		Graph *graph;
		if (argc == 2) {
			fprintf(stderr, "Loading from %s...\n", argv[1]);
			graph = ExecuteScript(argv[1]);
		} else {
			fprintf(stderr, "Loading sample...\n");
			graph = ExecuteScriptFromText(sample_script);
		}
		auto *driver = new GtkDriver(800, 600);
		driver->render(graph);
		return 0;
	}
}

int main(int argc, char **argv) {
	return Cygraph::CygMain(argc, argv);
}
