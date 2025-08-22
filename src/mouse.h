#include <gbdk/platform.h>
#include <stdint.h>

void mouse_init(void);
uint8_t mouse_button_clicked(uint8_t);
bool mouse_update(void);

extern int16_t mouse_x;
extern int16_t mouse_y;
extern uint8_t mouse_buttons;
extern uint8_t mouse_buttons_last;
extern uint8_t mouse_buttons_clicked;


#define MOUSE_X() ((uint8_t)mouse_x)
#define MOUSE_Y() ((uint8_t)mouse_y)