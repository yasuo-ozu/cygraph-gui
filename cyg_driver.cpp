#include "cyg_common.hpp"

namespace Cygraph {

#ifdef CTX_CLASS_DEFINE
	class Driver {
	public:
		virtual void createWindow(int width, int height, const char *title, void (*displayFunc)(Driver *, int, int)) = 0;
		virtual void flushWindow() = 0;

		virtual void clear() = 0;
		virtual void drawLine(int x0, int y0, int x1, int y1) = 0;
		virtual void drawLines(int points, const int *a) = 0;
		virtual void drawText(int x, int y, char *c) = 0;

		virtual void setColor(double r, double g, double b) = 0;
	};
#endif
#ifndef CTX_CLASS_DEFINE

#endif
}
