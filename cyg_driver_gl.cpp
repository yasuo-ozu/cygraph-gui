#include "cyg_common.hpp"

namespace Cygraph {

#ifdef CTX_CLASS_DEFINE
	class DriverGL : public Driver {
	public:
		DriverGL() { 
			DriverGL::instance = this;
		}
		void createWindow(int width, int height, const char *title, void (*displayFunc)(Driver *, int, int));
		void flushWindow();

		void clear();
		void drawLine(int x0, int y0, int x1, int y1);
		void drawLines(int points, const int *a);
		void drawText(int x, int y, char *c);

		void setColor(double r, double g, double b);
	private:
		static DriverGL *instance;
		static void (*displayFunc)(Driver *, int, int);
		static int width, height;
	};
#else
	DriverGL *DriverGL::instance = nullptr;
	void (*DriverGL::displayFunc)(Driver *driver, int, int) = nullptr;
	int DriverGL::width = 0, DriverGL::height = 0;

	namespace Glut {
// #include <GL/glut.h>
#include <GL/freeglut.h>
	}

	void DriverGL::createWindow(int width, int height, const char *title, void (*dFunc)(Driver *, int, int)) {
		// Available GLUT options are described below:
		// https://www.opengl.org/resources/libraries/glut/spec3/node10.html
		int argc = 1;
		char *argv[] = {strdup("cygraph"), strdup("")};
		DriverGL::displayFunc = dFunc;
		Glut::glutInit(&argc, argv);
		Glut::glutInitWindowSize(width, height);
		Glut::glutInitDisplayMode(GLUT_RGBA);
		Glut::glutCreateWindow(title);
		Glut::glutDisplayFunc([]{ DriverGL::displayFunc(DriverGL::instance, DriverGL::width, DriverGL::height); });
		Glut::glutReshapeFunc([](int w, int h){
			Glut::glViewport(0, 0, w, h);
			Glut::glMatrixMode(GL_PROJECTION_MATRIX);
			Glut::glLoadIdentity();
			// Glut::glOrtho(-w / 200.0, w / 200.0, -h / 200.0, h / 200.0, -1.0, 1.0);
			Glut::glOrtho(0.0, w, h, 0.0, -1.0, 1.0);
			DriverGL::width = w;
			DriverGL::height = h;
		});
		// glutMouseFunc(mouse);
		Glut::glClearColor(1.0, 1.0, 1.0, 1.0);
		Glut::glutMainLoop();
	}

	void DriverGL::flushWindow() {
		Glut::glFlush();
	}

	void DriverGL::clear() {
		Glut::glClear(GL_COLOR_BUFFER_BIT);
	}

	void DriverGL::drawLine(int x0, int y0, int x1, int y1) {
		Glut::glBegin(GL_LINES);
		Glut::glVertex2d(x0, y0);
		Glut::glVertex2d(x1, y1);
		Glut::glEnd();
	}

	void DriverGL::drawLines(int points, const int *a) {
		int x, y;
		Glut::glBegin(GL_LINE_STRIP);
		while (points--) {
			x = *a++;
			y = *a++;
			Glut::glVertex2d(x, y);
		}
		Glut::glEnd();
	}

	void DriverGL::drawText(int x, int y, char *text) {
		// Glut::glRasterPos2d(x, y);
		// while (*text != '\0') {
		// 	Glut::glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *text++);
		// 	Glut::glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *text++);
		// }
	}

	void DriverGL::setColor(double r, double g, double b) {
		Glut::glColor3d(r, g, b);
	}


#endif
}

