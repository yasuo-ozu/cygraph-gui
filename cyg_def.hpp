
// caro
using Graphic = Cairo::RefPtr<Cairo::Context>;

typedef struct {
	double top, right, bottom, left;
} boundary;

typedef struct {
	double x, y, width, height;
} rectangle;

typedef struct {
	double width, height;
} size;

typedef struct {
	double x, y;
} location;
