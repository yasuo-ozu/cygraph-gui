#include "cyg_common.hpp"

namespace Cygraph {

	class MyDrawArea : public Gtk::DrawingArea
	{
		Graph *child;
	public:
		MyDrawArea(Graph *s){
			this->child = s;
		}

	protected:
		virtual bool on_expose_event( GdkEventExpose* e );
	};

	bool MyDrawArea::on_expose_event( GdkEventExpose* event )
	{
		UNUSED(event);
		int width  = get_width();
		int height = get_height();
		double resolution = get_pango_context()->get_resolution();
		auto ctx = get_window()->create_cairo_context();
		this->child->draw(ctx, resolution, (rectangle){0.0, 0.0, (double) width, (double) height});
		return false;
	}

	class MainWin : public Gtk::Window
	{
		MyDrawArea *m_drawarea;
	public:
		MainWin(Graph *s){
			m_drawarea = new MyDrawArea(s);
			add( *m_drawarea );
			show_all_children();
		}
	};

	GtkDriver::GtkDriver(double width, double height) {
		this->width = width;
		this->height = height;
	}
	void GtkDriver::render(Graph *s) {
		int argc = 1;
		char *argv_[] = {
			strdup("cygraph"),
			strdup("")
		}, **argv = argv_;
		Gtk::Main kit( argc, argv );
		MainWin mainwin(s);
		mainwin.resize(this->width, this->height);
		Gtk::Main::run( mainwin );
	}
}
