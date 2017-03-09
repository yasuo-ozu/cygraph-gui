#include "cyg_common.hpp"

namespace Cygraph {
	int CygMain(int argc, char **argv) {
		UNUSED(argc, argv);
		return 0;
	}
}

int main(int argc, char **argv) {
	return Cygraph::CygMain(argc, argv);
}
