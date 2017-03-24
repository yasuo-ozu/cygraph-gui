--[[
local axis = builtin.create_axis(0, 10, 1)
builtin.axis_set_text(axis, "ドレイン-ソース間電圧(Vds) [V]")
local axis2 = builtin.create_axis(250, 0, -50)
builtin.axis_set(axis2, "orientation", 0)
builtin.axis_set_text(axis2, "ドレイン-ソース間電流(Id) [mA]")
local axis3 = builtin.create_axis(250, 0, -50)
builtin.axis_set(axis3, "orientation", 3)
builtin.axis_set_text(axis3, "ドレイン-ソース間電流(Id) [mA]")
local line1 = builtin.create_line(axis, axis3)
local line2 = builtin.create_line(axis, axis3)
builtin.line_set(line1, "draw_line", 0)
local point1 = builtin.create_point(1)
local point2 = builtin.create_point(1)
local point3 = builtin.create_point(1)
builtin.point_set(point2, "size", 9.0)
builtin.point_set(point2, "invert", 1)
builtin.point_set(point3, "size", 2)
local line3 = builtin.create_line(axis, axis3)
builtin.line_set(line3, "efficient", 1.5)
local line4 = builtin.create_line(axis, axis3)
builtin.line_set(line4, "efficient", 1.5)
builtin.line_set(line4, "draw_line", 0)
local point4 = builtin.create_point(2)
local point5 = builtin.create_point(2)
local point6 = builtin.create_point(1)
builtin.point_set(point5, "size", 9.0)
builtin.point_set(point5, "invert", 1)
builtin.point_set(point6, "size", 2)
builtin.line_add_point(line4, point4)
builtin.line_add_point(line4, point5)
builtin.line_add_point(line4, point6)
local line5 = builtin.create_line(axis, axis3)
builtin.line_set(line5, "efficient", 2.0)
local line6 = builtin.create_line(axis, axis3)
builtin.line_set(line6, "efficient", 2.0)
builtin.line_set(line6, "draw_line", 0)
local point7 = builtin.create_point(4)
local point8 = builtin.create_point(4)
local point9 = builtin.create_point(1)
builtin.point_set(point8, "size", 9.0)
builtin.point_set(point8, "invert", 1)
builtin.point_set(point9, "size", 2)
builtin.line_add_point(line6, point7)
builtin.line_add_point(line6, point8)
builtin.line_add_point(line6, point9)

local r = builtin.line_set_func(line6, "testfff")
testfff = function(x)
	print ("x is "..x)
	return x * x * 10
end
print ("r = " .. r)
]]

(function (_G)
	local builtin_counter = 1
	local axis = {}
	function justify(value)
		local v = math.abs(value)
		local p = 10 ^ math.floor(math.log(v, 10))
		local a = v / p
		if value > 0 then
			return math.floor(a) * p, math.ceil(a) * p
		else
			return -math.ceil(a) * p, -math.floor(a) * p
		end
	end
	function register_graph(item)
		if #axis == 0 then
			print("error")
			return
		elseif #axis == 1 then
			local min = axis[1].min
			local max = axis[1].max
			local steps = axis[1].steps
			local v_min = 99999
			local v_max = -99999
			local i
			for i = min, max, ((max - min) / steps) do
				local val = item["function"](i);
				if val > v_max then v_max = val end
				if val < v_min then v_min = val end
			end
			v_min, _ = justify(v_min)
			_, v_max = justify(v_max)
			axis[2] = {
				["min"] = v_min,
				["max"] = v_max,
				["steps"] = 20.0,
				["obj"] = builtin.create_axis(v_max, v_min, -(v_max - v_min) / steps)
			}
			builtin.axis_set(axis[2].obj, "orientation", 3)
			builtin.axis_set_text(axis[2].obj, "out")
		end
		local line = builtin.create_line(axis[1].obj, axis[2].obj)
		_G["__builtin_func_" .. builtin_counter] = item["function"]
		local r = builtin.line_set_func(line, "__builtin_func_" .. builtin_counter)
		builtin_counter = builtin_counter + 1
	end
	function register_axis(item)
		local steps = item.steps or 20
		item.min = item.min or -10.0
		item.max = item.max or item.min + 20.0
		item.steps = item.steps or 20.0
		local obj
		if #axis == 0 then
			obj = builtin.create_axis(item.min, item.max, (item.max - item.min) / item.steps)
		else
			obj = builtin.create_axis(item.max, item.min, -(item.max - item.min) / item.steps)
		end
		builtin.axis_set(obj, "orientation", #axis == 0 and 2 or 3)
		builtin.axis_set_text(obj, item["label"] or "in")
		item["obj"] = obj
		axis[#axis + 1] = item
	end
	_G["render"] = function (axis, graph)
		if not graph then
			graph = axis
			axis = { "in" }
		end
		if type(graph) == "function" then
			graph = { graph }
		end
		if #graph > 0 and #axis > 0 then
			local item
			for _, item in ipairs(axis) do
				if type(item) == "string" then
					item = { item }
				end
				if #item > 0 then
					item = {
						["label"] = item[1],
						["min"] = item[2],
						["max"] = item[3],
						["steps"] = item[4]
					}
				end
				register_axis(item)
			end
			for _, item in ipairs(graph) do
				if type(item) == "function" then
					item = { item }
				end
				if #item > 0 then
					item = {
						["function"] = item[1]
					}
				end
				register_graph(item)
			end
		else
			print("format error");
		end
	end
end)(_G)

render({
	function(x) return 5 * x ^ 3 + 7 * x ^ 2 + 3 * x end
});

