#ifndef eegy2ruwry
#define eegy2ruwry

typedef struct {
	int x, y;
} p/*oint*/;

enum buttons {
	A = 1,
	B = 2,
	Y = 3,
	X = 0,
	Z = 7,
	START = 9,
	L = 4,
	R = 5,
	DU = 12,
	DD = 14,
	DL = 15,
	DR = 13
};

int button_valid(int b) {
	switch (b) {
		case A:	case B:	case Y:	case X:	case Z:	case START:
		case L:	case R:	case DU: case DD: case DL: case DR:
			return 1;
	}
	return 0;
}

p buttons_places[] = {
	[A] = {446,153},
	[B] = {346,193},
	[Y] = {431,60},
	[X] = {545,136},
	[Z] = {308,26},
	[START] = {240,160},
	[L] = {20,5},
	[R] = {468,5},
	[DU] = {122,235},
	[DD] = {122,355},
	[DL] = {56,286},
	[DR] = {188,290}
};


#endif
