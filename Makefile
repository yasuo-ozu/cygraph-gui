CXX = g++
HEADERS = cyg_*.hpp
SRCS = cyg_*.cpp
CXX_OPTS = -lglut -lGLU -lGL -lm --std=c++11
CXX_DEBUG = -g3 -Wall -Wextra -DDEBUG
CXX_RELEASE = -Ofast -DRELEASE
.PHONY:	all clean release
all:	cygraph
cygraph:	${SRCS} ${HEADERS} Makefile
	rm -f cyg_common.hpp.gch cyg_class.hpp.gch
	${CXX} ${CXX_OPTS} ${CXX_DEBUG} ${SRCS} -o cygraph
release:	${SRCS} ${HEADERS} Makefile
	${CXX} ${CXX_OPTS} ${CXX_RELEASE} ${SRCS} -o cygraph
clean:
	rm -f *.o cygraph *.gch

