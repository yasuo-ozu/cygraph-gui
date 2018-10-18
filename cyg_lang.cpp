#include "cyg_common.hpp"

using namespace std;

namespace Cygraph {

	map<lua_State *, Graph *> graphs;
	vector<Axis *> axises;
	vector<GraphLine *> lines;
	vector<GraphPoint *> points;

char const * const default_script_pre = R"(

--- disable apis
_G['io'] = nil
_G['file'] = nil
_G['rt'] = nil
_G['hwd'] = nil
_G['tcp'] = nil
_G['udp'] = nil
_G['os'] = nil
_G['package'] = nil
collectgarbage('collect')

)";
	// builtin_create_point(shape)
	int builtin_create_point(lua_State *L) {
		int shape = luaL_checkinteger(L, 1);
		points.push_back(new GraphPoint((GraphPoint::PointShape) shape));
		lua_pushinteger(L, points.size() - 1);
		return 1;
	}

	// builtin_create_axis(min, max, step)
	int builtin_create_axis(lua_State *L) {
		double min, max, step;
		min = luaL_checknumber(L, 1);
		max = luaL_checknumber(L, 2);
		step = luaL_checknumber(L, 3);
		auto *axis = new Axis(min, max, step);
		axises.push_back(axis);
		graphs[L]->axises.push_back(axis);
		lua_pushinteger(L, axises.size() - 1);
		return 1;
	}

	// builtin_create_line(axis1, axis2)
	int builtin_create_line(lua_State *L) {
		int axis1, axis2;
		axis1 = luaL_checkinteger(L, 1);
		axis2 = luaL_checkinteger(L, 2);
		if (axis1 < 0 || axis2 < 0 || (unsigned) axis1 >= axises.size() || (unsigned) axis2 >= axises.size()) {
			lua_pushinteger(L, -1);
		} else {
			auto *line = new GraphLine(axises[axis1], axises[axis2]);
			lines.push_back(line);
			graphs[L]->lines.push_back(line);
			lua_pushinteger(L, lines.size() - 1);
		}
		return 1;
	}

	// builtin_axis_set_text(axis, text)
	int builtin_axis_set_text(lua_State *L) {
		int axis = luaL_checkinteger(L, 1);
		const char *s = luaL_checkstring(L, 2);
		if (axis < 0 || (unsigned) axis >= axises.size()) {
			lua_pushinteger(L, -1);
		} else {
			axises[axis]->set_text(s);
			lua_pushinteger(L, 0);
		}
		return 1;
	}

	// builtin_line_add_point(line, point)
	int builtin_line_add_point(lua_State *L) {
		int line = luaL_checkinteger(L, 1);
		int point = luaL_checkinteger(L, 2);
		if (line < 0 || (unsigned) line >= lines.size() ||
			point < 0 || (unsigned) point > points.size()) {
			lua_pushinteger(L, -1);
			return 1;
		}
		lines[line]->points.push_back(points[point]);
		lua_pushinteger(L, 0);
		return 1;
	}

	// builtin_line_set(line, prop, value)
	int builtin_line_set(lua_State *L) {
		int line = luaL_checkinteger(L, 1);
		const char *prop = luaL_checkstring(L, 2);
		if (line < 0 || (unsigned) line >= lines.size()) {
			lua_pushinteger(L, -1);
			return 1;
		}
		auto *line_o = lines[line];
		if (strcmp(prop, "weight") == 0) {
			int weight = luaL_checknumber(L, 3);
			line_o->weight = weight;
		} else if (strcmp(prop, "efficient") == 0) {
			double efficient = luaL_checknumber(L, 3);
			line_o->efficient = efficient;
		} else {
			lua_pushinteger(L, -1);
			return 1;
		}
		lua_pushinteger(L, 0);
		return 1;
	}

	// builtin_line_set_func(line, func)
	int builtin_line_set_func(lua_State *L) {
		int line = luaL_checkinteger(L, 1);
		const char *s = luaL_checkstring(L, 2);
		/*
		if (!lua_isfunction(L, 2)) {
			lua_pushinteger(L, -1);
			return 1;
		}
		//lua_getuservalue(L, 2);
		lua_pushnumber(L, 10.3);
		if (lua_pcall(L, 1, 1, 0) != 0) {
			cout << "pcall err" << endl;
			cout << (lua_tostring(L, -1)) << endl;
			lua_pushinteger(L, -1);
			return 1;
		}
		double res = luaL_checknumber(L, -1);
		cout << res << endl;
		*/
		auto *line_o = lines[line];
		line_o->tag = (void *) L;
		line_o->tag2 = (void *) s;
		line_o->func = [](void *tag, void *tag2, double x){
			auto *L = (lua_State *) tag;
			char *s = (char *) tag2;
			lua_getglobal(L, s);
			lua_pushnumber(L, x);
			if (lua_pcall(L, 1, 1, 0) != 0) {
				cout << "pcall err" << endl;
				cout << (lua_tostring(L, -1)) << endl;
				return 0.0;
			}
			double r = luaL_checknumber(L, -1);
			return r;
		};

		return 1;
	}

	// builtin_axis_set(axis, prop, value)
	int builtin_axis_set(lua_State *L) {
		int axis = luaL_checkinteger(L, 1);
		const char *prop = luaL_checkstring(L, 2);
		if (axis < 0 || (unsigned) axis >= axises.size()) {
			lua_pushinteger(L, -1);
			return 1;
		}
		auto *axis_o = axises[axis];
		if (strcmp(prop, "orientation") == 0) {
			int orientation = luaL_checkinteger(L, 3);
			axis_o->orientation = orientation;
		} else {
			lua_pushinteger(L, -1);
			return 1;
		}
		lua_pushinteger(L, 0);
		return 1;
	}

	// builtin_point_set(point, prop, value)
	int builtin_point_set(lua_State *L) {
		int point = luaL_checkinteger(L, 1);
		const char *prop = luaL_checkstring(L, 2);
		if (point < 0 || (unsigned) point >= points.size()) {
			lua_pushinteger(L, -1);
			return 1;
		}
		auto *point_o = points[point];
		if (strcmp(prop, "size") == 0) {
			double size = luaL_checknumber(L, 3);
			point_o->size = size;
		} else if (strcmp(prop, "invert") == 0) {
			int invert = luaL_checkinteger(L, 3);
			point_o->invert = invert ? true : false;
		} else {
			lua_pushinteger(L, -1);
			return 1;
		}
		lua_pushinteger(L, 0);
		return 1;
	}

	void register_builtins(lua_State *L) {
		static const luaL_Reg builtin[] = {
			{"create_point", builtin_create_point},
			{"create_axis", builtin_create_axis},
			{"create_line", builtin_create_line},
			{"axis_set_text", builtin_axis_set_text},
			{"line_add_point", builtin_line_add_point},
			{"line_set", builtin_line_set},
			{"axis_set", builtin_axis_set},
			{"point_set", builtin_point_set},
			{"line_set_func", builtin_line_set_func},
			{NULL, NULL}
		};
		// luaL_setfuncs(L, builtin, 1);
		luaL_newlib(L, builtin);
		lua_setglobal(L, "builtin");
	}

	Graph *ExecuteScriptInternal(char const *str, bool is_fname) {
		UNUSED(str, is_fname);
		auto *graph = new Graph();
		// int status;
		lua_State* L = luaL_newstate();
		graphs[L] = graph;
		luaL_openlibs(L);
		register_builtins(L);

		int ret = luaL_dostring(L, default_script_pre);
		if (ret != 0) {
			cerr << default_script_pre << endl;
			cerr << "Default script failed." << endl;
			return nullptr;
		}

		if (is_fname) {
			ret = luaL_loadfile(L, str);
			if (ret == 0)
				ret = ::lua_pcall(L, 0, 0, 0);
		} else {
			ret = luaL_dostring(L, str);
		}

		if (ret != 0) {
			const char *msg = lua_tostring(L, 1);
			cerr << "Script error" << endl;
			if (msg != nullptr) {
				cerr << "Error detail:" << endl;
				cerr << "\t" << msg << endl;
			}
		}

		return graph;
	}

	Graph *ExecuteScript(char const *fname) {
		return ExecuteScriptInternal(fname, true);

	}

	Graph *ExecuteScriptFromText(char const *src) {
		return ExecuteScriptInternal(src, false);
	}

}
