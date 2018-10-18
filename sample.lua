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

	local builtin = _G["builtin"]
	_G["builtin"] = nil

	local builtin_counter = 1
	local axis = {}
	function justify(value)
		if value == 0.0 then return 0.0, 0.0 end
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
		_G["__builtin_func_" .. builtin_counter] = item["function"]
		local dottedLine = 0
		local dottedSpace = 0
		local pointList = {}
		local heavy = false
		local i
		if type(item["design"]) == "string" then
			local s = item["design"]
			s:gsub("◎", "Oo")
			s:gsub("⦿", "O・")

			for i = 1, s:len() do
				local c = s:sub(i, i)
				if c == "-" then dottedLine = dottedLine + 1
				elseif c == " " then dottedSpace = dottedSpace + 1
				elseif c == "=" then heavy = 1 dottedLine = dottedLine + 1
				else
					local shape
					local inv = false
					local size = 10.0
					if c == "O" or c == "○" or c == "◯" or c == "c" then shape = 1
					elseif c == "o" or c == "。" then shape = 1 size = 7.0
					elseif c == "●" or c == "C" then shape = 1 inv = true
					elseif c == "△" or c == "t" then shape = 2
					elseif c == "▲" or c == "T" then shape = 2 inv = true
					elseif c == "□" or c == "s" then shape = 4
					elseif c == "■" or c == "S" then shape = 4 inv = true
					elseif c == "・" or c == "." then shape = 1 inv = true size = 2
					else
						print("bad design: " .. c)
						return
					end
					local point = builtin.create_point(shape)
					builtin.point_set(point, "size", size)
					builtin.point_set(point, "invert", 0)
					pointList[#pointList + 1] = point
					if not inv then
						local point2 = builtin.create_point(shape)
						builtin.point_set(point2, "size", size - 2)
						builtin.point_set(point2, "invert", 1)
						pointList[#pointList + 1] = point2
					end
				end
			end
		end
		local line = builtin.create_line(axis[1].obj, axis[2].obj)
		builtin.line_set_func(line, "__builtin_func_" .. builtin_counter)
		if dottedLine == 0 and item["design"]:len() > 0 then
			builtin.line_set(line, "weight", 0.0)
		else
			builtin.line_set(line, "weight", heavy and 4.0 or 1.0)
		end
		if #pointList > 0 then
			local line2 = builtin.create_line(axis[1].obj, axis[2].obj)
			local point
			for _, point in ipairs(pointList) do
				builtin.line_add_point(line2, point)
			end
			builtin.line_set_func(line2, "__builtin_func_" .. builtin_counter)
			builtin.line_set(line2, "weight", 0.0)
		end
		builtin_counter = builtin_counter + 1
	end
	function register_axis(item)
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
					local newItem = {
						["label"] = null,
						["min"] = -10.0,
						["max"] = 10.0,
						["steps"] = 20
					}
					local numberCount = 0
					for _, anItem in ipairs(item) do
						if type(anItem) == "string" then
							newItem["label"] = anItem
						elseif type(anItem) == "number" then
							if numberCount == 0 then newItem["min"] = anItem
							elseif numberCount == 1 then newItem["max"] = anItem
							elseif numberCount == 2 then newItem["steps"] = anItem
							else
								print("too many args")
								return
							end
							numberCount = numberCount + 1
						else
							print("axis type error")
							return
						end
					end
					item = newItem
				end
				register_axis(item)
			end
			for _, item in ipairs(graph) do
				if type(item) == "function" then
					item = { item }
				end
				if #item > 0 then
					local newItem = {
						["function"] = nil,
						["design"] = ""
					}
					for _, anItem in ipairs(item) do
						if type(anItem) == "function" then
							newItem["function"] = anItem
						elseif type(anItem) == "string" then
							newItem["design"] = anItem
						else
							print("graph type error")
							return
						end
					end
					if newItem["function"] == nil then
						print("graph require function")
						return
					end
					item = newItem
				end
				register_graph(item)
			end
		else
			print("format error");
		end
	end


end)(_G)

--render({function(x) return x * x end})

function Id(Vds, Vgs)
	local Vtn = 2.409
	local bN = 202.3
	if Vds <= Vgs - Vtn then
		return bN * ((Vgs - Vtn) * Vds - 0.5 * Vds * Vds)
	else
		return 0.5 * bN * (Vgs - Vtn) * (Vgs - Vtn)
	end
end

render({
	{"ドレイン-ソース間電圧(Vds) [V]", 0, 4, 4},
	{"ドレイン-ソース間電流(Id) [mA]", 0, 250, 5}
}, {
	{function(x) return Id(x, 3.2) end, "==O.=="},
	{function(x) return Id(x, 3.4) end, "--Oo--"},
	{function(x) return Id(x, 3.6) end, "==t.=="},
	{function(x) return Id(x, 3.8) end, "--s.--"}
});


--[[
render({
	{"ドレイン-ソース間電圧(Vds) [V]", 0, 4, 4},
	{"ドレイン-ソース間電流(Id) [mA]", 0, 250, 5}
}, {
	function(x) return Id(x, 3.2) end,
	function(x) return Id(x, 3.4) end,
	function(x) return Id(x, 3.6) end,
	function(x) return Id(x, 3.8) end,
	function(x) return Id(x, 4.0) end,
	function(x) return Id(x, 5.0) end
});
--]]
