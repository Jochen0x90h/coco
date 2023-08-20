#pragma once

/**
	Simple debug output on up to three LEDs.
	 Only header file, the implementation has to come from a board implementation
*/
namespace coco {
namespace debug {

void init();

void setRed(bool value = true);
inline void clearRed() {setRed(false);}
void toggleRed();

void setGreen(bool value = true);
inline void clearGreen() {setGreen(false);}
void toggleGreen();

void setBlue(bool value = true);
inline void clearBlue() {setBlue(false);}
void toggleBlue();

inline void set(int state) {
	setRed(state & 1);
	setGreen(state & 2);
	setBlue(state & 4);
}

enum Color {
	BLACK = 0,

	RED = 1,
	GREEN = 2,
	BLUE = 4,

	YELLOW = 3,
	MAGENTA = 5,
	CYAN = 6,

	WHITE = 7,
};

inline void set(Color color) {
	int c = int(color);
	setRed(c & 1);
	setGreen(c & 2);
	setBlue(c & 4);
}

class Counter {
public:

	Counter &operator ++() {
		++this->c;
		set(this->c);
		return *this;
	}

	Counter &operator --() {
		--this->c;
		set(this->c);
		return *this;
	}

	int c = 0;
};

} // namespace debug
} // namespace coco
