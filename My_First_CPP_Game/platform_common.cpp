struct Button_state {
	bool is_down;
	bool changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
	BUTTON_ESCAPE,

	BUTTON_COUNT,
};

struct Input {
	Button_state buttons[BUTTON_COUNT];
};