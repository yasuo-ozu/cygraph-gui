#include "cyg_common.hpp"

namespace Cygraph {
	int CygMain(int argc, char **argv) {
		UNUSED(argc, argv);
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
		auto *driver = new GtkDriver(800, 600);
		driver->render(graph);
		return 0;
	}
}

int main(int argc, char **argv) {
	return Cygraph::CygMain(argc, argv);
}
