CXX = g++
HEADERS = cyg_*.hpp
SRCS = cyg_*.cpp
CXX_OPTS = -lm --std=c++11 `pkg-config gtkmm-2.4 --cflags --libs` -llua
CXX_DEBUG = -g3 -Wall -Wextra -DDEBUG
CXX_RELEASE = -Ofast -DRELEASE
.PHONY:	all clean release
all:	cygraph
cygraph:	${SRCS} ${HEADERS} Makefile
	${CXX} ${CXX_OPTS} ${CXX_DEBUG} ${SRCS} -o cygraph
release:	${SRCS} ${HEADERS} Makefile
	${CXX} ${CXX_OPTS} ${CXX_RELEASE} ${SRCS} -o cygraph
clean:
	rm -f *.o cygraph *.gch

