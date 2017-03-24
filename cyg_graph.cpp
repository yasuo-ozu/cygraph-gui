#include "cyg_common.hpp"

namespace Cygraph {

	// Graph
	Graph::Graph() {
		this->padding.top = 30.0;
		this->padding.left = 30.0;
		this->padding.right = 30.0;
		this->padding.bottom = 30.0;
	}

	boundary Graph::get_boundary(Graphic g, double resolution) {
		boundary bd{0.0, 0.0, 0.0, 0.0};
		for (auto *axis : this->axises) {
			double h = axis->get_outer_height(g, resolution);
			if (axis->orientation == 0) {
				if (bd.top < h)
					bd.top = h;
			} else if (axis->orientation == 1) {
				if (bd.right < h)
					bd.right = h;
			} else if (axis->orientation == 2) {
				if (bd.bottom < h)
					bd.bottom = h;
			} else if (axis->orientation == 3) {
				if (bd.left < h)
					bd.left = h;
			}
		}
		return bd;
	}

	void Graph::draw(Graphic g, double resolution, rectangle rect) {
		g->set_source_rgb(1.0, 1.0, 1.0);
		g->rectangle(rect.x, rect.y, rect.width, rect.height);
		g->fill();
		g->set_source_rgb(0.0, 0.0, 0.0);
		auto bd = this->get_boundary(g, resolution);
		rectangle area{rect.x + bd.left + this->padding.left,
				 rect.y + bd.top + this->padding.top,
				 rect.width - bd.left - bd.right - this->padding.left -
						 this->padding.right,
				 rect.height - bd.top - bd.bottom - this->padding.top -
						 this->padding.bottom};
		g->rectangle(area.x, area.y, area.width, area.height);
		g->stroke();

		map<Axis *, rectangle> axis_rects;
		for (auto *axis : this->axises) {
			rectangle axis_rect;
			if (axis->orientation == 0) {
				axis_rect = (rectangle){area.x, this->padding.top, area.width, bd.top};
			} else if (axis->orientation == 1) {
				axis_rect = (rectangle){area.x + area.width, area.y, bd.right, area.height};
			} else if (axis->orientation == 2) {
				axis_rect = (rectangle){area.x, area.y + area.height, area.width, bd.bottom};
			} else if (axis->orientation == 3) {
				axis_rect = (rectangle){area.x - bd.left, area.y, bd.left, area.height};
			}
			axis->draw(g, resolution, axis_rect);
			axis_rects[axis] = axis_rect;
		}

		for (auto *line : this->lines) {
			line->draw(g, resolution, axis_rects);
		}
	}

	// Axis
	Axis::Axis(double begin, double end, double step) {
		this->text = nullptr;
		this->padding.top = 5.0;
		this->padding.left = 5.0;
		this->padding.right = 5.0;
		this->padding.bottom = 5.0;
		this->label_format = "%.1lf";
		this->orientation = 2;
		this->begin = begin;
		this->end = end;

		Scale scale;
		scale.width = 1.0;
		scale.step = step;
		scale.inner_height = 5.0;
		scale.outer_height = 0.0;
		scale.label = true;
		this->scales.push_back(scale);
	}

	void Axis::set_text(string s) { this->text = new Text(s); }

	double Axis::get_outer_height(Graphic g, double resolution) {
		double max_outer_height = 0;
		double max_label_height = 0;
		char s[100];

		auto f = [&](double loc) {
			sprintf(s, this->label_format.c_str(), loc);
			Text *text = new Text(s);
			text->padding = this->padding;
			size siz = text->get_size(g, resolution);
			if (this->orientation % 2 == 0) {
				if (max_label_height < siz.height)
					max_label_height = siz.height;
			} else {
				if (max_label_height < siz.width)
					max_label_height = siz.width;
			}
			delete text;
		};
		for (auto scale : this->scales) {
			if (scale.outer_height > max_outer_height)
				max_outer_height = scale.outer_height;
			if (!scale.label)
				continue;
			double loc;
			for (loc = this->begin; (this->begin <= loc && loc < this->end) ||
					(this->begin >= loc && loc > this->end);
					 loc += scale.step)
				f(loc);
			loc = this->end;
			f(loc);
		}
		if (this->text != nullptr) {
			max_label_height += this->text->get_size(g, resolution).height;
		}
		return max_outer_height + max_label_height;
	}

	void Axis::draw(Graphic g, double resolution, rectangle rect) {
		UNUSED(g, resolution);
		char s[100];

		size t_siz = this->text->get_size(g, resolution);
		if (this->orientation == 0) {
			this->text->draw(g, resolution, rect.x + (rect.width - t_siz.width) / 2,
											 rect.y);
		} else if (this->orientation == 1) {
			this->text->orientation = 1;
			this->text->draw(g, resolution, rect.x + rect.width - t_siz.width,
											 rect.y + (rect.height - t_siz.width) / 2);
		} else if (this->orientation == 2) {
			this->text->draw(g, resolution, rect.x + (rect.width - t_siz.width) / 2,
											 rect.y + rect.height - t_siz.height);
		} else if (this->orientation == 3) {
			this->text->orientation = -1;
			this->text->draw(g, resolution, rect.x,
											 rect.y + (rect.height - t_siz.width) / 2);
		}
		for (auto scale : this->scales) {
			auto f = [&](double loc) {
				double p_loc = (this->orientation % 2 == 0 ? rect.width : rect.height) /
											 abs(this->end - this->begin) * abs(loc - this->begin);
				if (this->orientation == 0) {
					g->move_to(rect.x + p_loc, rect.y + rect.height - scale.outer_height);
					g->line_to(rect.x + p_loc, rect.y + rect.height + scale.inner_height);
				} else if (this->orientation == 1) {
					g->move_to(rect.x - scale.inner_height, rect.y + p_loc);
					g->line_to(rect.x + scale.outer_height, rect.y + p_loc);
				} else if (this->orientation == 2) {
					g->move_to(rect.x + p_loc, rect.y + scale.outer_height);
					g->line_to(rect.x + p_loc, rect.y - scale.inner_height);
				} else if (this->orientation == 3) {
					g->move_to(rect.x + rect.width + scale.inner_height, rect.y + p_loc);
					g->line_to(rect.x + rect.width - scale.outer_height, rect.y + p_loc);
				}
				g->stroke();
				sprintf(s, this->label_format.c_str(), loc);
				Text *text = new Text(s);
				text->padding = this->padding;
				size siz = text->get_size(g, resolution);
				if (this->orientation == 0) {
					text->draw(g, resolution, rect.x + p_loc - siz.width / 2,
							 rect.y + rect.height - scale.outer_height - siz.height);
				} else if (this->orientation == 1) {
					text->draw(g, resolution, rect.x + scale.outer_height,
							 rect.y + p_loc - siz.height / 2);
				} else if (this->orientation == 2) {
					text->draw(g, resolution, rect.x + p_loc - siz.width / 2,
							 rect.y + scale.outer_height);
				} else if (this->orientation == 3) {
					text->draw(g, resolution,
							 rect.x + rect.width - siz.width - scale.outer_height,
							 rect.y + p_loc - siz.height / 2);
				}
				delete text;
			};
			for (double loc = this->begin; (this->begin <= loc && loc < this->end) ||
						 (this->begin >= loc && loc > this->end); loc += scale.step)
				f(loc);
			f(this->end);
		}
	}

	double Axis::get_location(Graphic g, double resolution, rectangle rect, double value) {
		UNUSED(g, resolution);
		if (this->orientation % 2 == 0)
			return rect.x + rect.width * ((value - this->begin) / (this->end - this->begin));
		else {
			return rect.y + rect.height * ((value - this->begin) / (this->end - this->begin));
		}
	}

	// Text
	Text::Text() {
		this->font_size = 10.0;
		this->orientation = 0;
		this->padding.top = 5.0;
		this->padding.left = 5.0;
		this->padding.right = 5.0;
		this->padding.bottom = 5.0;
	}
	Text::Text(string s) : Text() { this->text = s; }

	size Text::get_size(Graphic g, double resolution) {
		size siz;
		Cairo::TextExtents ext;
		g->save();
		if (!this->font_name.empty())
			g->select_font_face(this->font_name, Cairo::FONT_SLANT_NORMAL,
													Cairo::FONT_WEIGHT_BOLD);
		g->set_font_size(this->font_size * resolution / 72);
		g->get_text_extents(this->text, ext);
		g->restore();
		siz.width = ext.width;
		siz.height = ext.height;
		siz.width += this->padding.left + this->padding.right;
		siz.height += this->padding.top + this->padding.bottom;
		return siz;
	}

	void Text::draw(Graphic g, double resolution, double x, double y) {
		g->save();
		auto siz = this->get_size(g, resolution);
		if (!this->font_name.empty())
			g->select_font_face(this->font_name, Cairo::FONT_SLANT_NORMAL,
													Cairo::FONT_WEIGHT_BOLD);
		g->set_font_size(this->font_size * resolution / 72);
		g->move_to(x + this->padding.left, y + siz.height - this->padding.bottom);
		if (this->orientation % 2 != 0) {
			g->move_to(x + this->padding.left, y + siz.width - this->padding.bottom);
			g->rotate(0.5 * this->orientation * 3.142);
		}
		g->show_text(this->text);
		g->restore();
	}

	// GraphLine
	GraphLine::GraphLine(Axis *axis1, Axis *axis2) {
		this->axis[0] = axis1;
		this->axis[1] = axis2;
		this->draw_line = true;
		this->efficient = 1.0;
		this->func = nullptr;
	}

	vector<location> GraphLine::get_points() {
		double min, max;
		vector<location> list;
		if (this->axis[0]->begin <= this->axis[0]->end) {
			min = this->axis[0]->begin;
			max = this->axis[0]->end;
		} else {
			max = this->axis[0]->begin;
			min = this->axis[0]->end;
		}
		for (double x = min; x <= max; x += this->draw_line ? 0.1 : 1.0) {
			if (this->func == nullptr) {
				list.push_back((location){x, x * x * this->efficient});
			} else {
				list.push_back((location){x, this->func(this->tag, this->tag2, x) });
			}
		}
		return list;
	}

	void GraphLine::draw(Graphic g, double resolution, map<Axis *, rectangle> locs) {
		vector<location> points = this->get_points();
		bool is_beginning = true;
		for (auto point : points) {
			location loc{0.0, 0.0};
			for (auto *axis : this->axis) {
				if (axis->orientation % 2 == 0) {
					loc.x = axis->get_location(g, resolution, locs[axis], point.x);
				} else if (axis->orientation % 2 == 1) {
					loc.y = axis->get_location(g, resolution, locs[axis], point.y);
				}
			}
			if (this->draw_line) {
				if (is_beginning) g->move_to(loc.x, loc.y);
				else			  g->line_to(loc.x, loc.y);
			}
			for (auto *point : this->points) {
				point->draw(g, resolution, loc.x, loc.y);
			}
			is_beginning = false;
		}
		g->stroke();

	}

	GraphPoint::GraphPoint(PointShape ps) {
		this->shape = GraphPoint::PS_CIRCLE;
		this->size = 10.0;
		this->invert = false;
		this->shape = ps;
	}

	void GraphPoint::draw(Graphic g, double resolution, double x, double y) {
		UNUSED(resolution);
		g->save();
		if (this->invert) g->set_source_rgb(1.0, 1.0, 1.0);
		else g->set_source_rgb(0.0, 0.0, 0.0);
		if (this->shape == PS_CIRCLE) {
			g->arc(x, y, this->size, 0.0, 2 * 3.1416);
		} else if (this->shape == PS_TRIANGLE) {
			g->move_to(x, y - this->size * 1.5);
			g->line_to(x - this->size * 1.5 / 2.0 * 1.73, y + this->size * 1.5 / 2.0);
			g->line_to(x + this->size * 1.5 / 2.0 * 1.73, y + this->size * 1.5 / 2.0);
			g->close_path();
		} else if (this->shape == PS_SQUARE) {
			g->rectangle(x - this->size * 1.73 / 2.0, y - this->size * 1.73 / 2.0, this->size * 1.73, this->size * 1.73);
		}
		g->fill();
		g->restore();
	}
}
