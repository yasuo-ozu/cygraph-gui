#include "cyg_common.hpp"

namespace Cygraph {
	void display(Driver *driver, int width, int height) {
		driver->clear();
		driver->setColor(0, 0, 0);
		driver->drawLine(0, 0, width, height);
		driver->drawLine(-1,-1,1,1);
		driver->drawText(0, 0, "Hello!");
		driver->flushWindow();

	}
	int CygMain(int argc, char **argv) {
		UNUSED(argc, argv);
		Driver *driver = new DriverGL();
		driver->createWindow(600, 400, "hello", display);
		return 0;
	}
}

int main(int argc, char **argv) {
	return Cygraph::CygMain(argc, argv);
}
