#include <gbdk/platform.h>
#include <stdint.h>
#include <stdio.h>

#include "snes_mouse.h"

#define SPRITE_MOUSE_CURSOR 10u
#define SPRITE_MOUSE_TILE_START 10u

const unsigned char mouse_cursors[] = {
  // Arrow 1
  0xFE, 0xFE, 0xFC, 0x84, 0xF8, 0x98, 0xF8, 0xA8,
  0xFC, 0xB4, 0xCE, 0xCA, 0x87, 0x85, 0x03, 0x03,
  // Arrow 2
  0x80, 0x80, 0xC0, 0xC0, 0xE0, 0xA0, 0xF0, 0x90,
  0xF8, 0x88, 0xF0, 0xB0, 0xD8, 0xD8, 0x08, 0x08,
  // Arrow 3
  0x80, 0x80, 0xC0, 0xC0, 0xE0, 0xA0, 0xF0, 0x90,
  0xF8, 0x88, 0xFC, 0x84, 0xF8, 0x98, 0xE0, 0xE0,
  // Hand
  0x10, 0x10, 0x38, 0x28, 0x38, 0x28, 0x7E, 0x6E,
  0xFE, 0xA2, 0xFE, 0x82, 0x7E, 0x42, 0x3E, 0x3E
};

int16_t mouse_x = DEVICE_SCREEN_PX_WIDTH / 2;
int16_t mouse_y = DEVICE_SCREEN_PX_HEIGHT / 2;
uint8_t mouse_buttons = 0u, mouse_buttons_last = 0u;
uint8_t mouse_buttons_clicked = 0u;


static void use_mouse_data(void) {

    // Update buttons
    mouse_buttons_last = mouse_buttons;
    mouse_buttons = (snes_mouse.buttons & SNES_MOUSE_BUTTON_MASK);
    mouse_buttons_clicked = mouse_buttons_last & ~mouse_buttons;

    // Update X,Y movement and cursor
    int8_t mouse_y_move = (snes_mouse.move_y & SNES_MOUSE_Y_MASK);
    if (snes_mouse.move_y & SNES_MOUSE_Y_DIR) mouse_y_move *= -1;

    int8_t mouse_x_move = (snes_mouse.move_x & SNES_MOUSE_X_MASK);
    if (snes_mouse.move_x & SNES_MOUSE_X_DIR) mouse_x_move *= -1;

    mouse_x += mouse_x_move;
    mouse_y += mouse_y_move;

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_x >= DEVICE_SCREEN_PX_WIDTH) mouse_x = DEVICE_SCREEN_PX_WIDTH - 1;
    if (mouse_y >= DEVICE_SCREEN_PX_HEIGHT) mouse_y = DEVICE_SCREEN_PX_HEIGHT - 1;

    // Move the cursor
    move_sprite(SPRITE_MOUSE_CURSOR, mouse_x + DEVICE_SPRITE_PX_OFFSET_X, mouse_y + DEVICE_SPRITE_PX_OFFSET_Y);

    // if (mouse_buttons == SNES_MOUSE_BUTTON_LEFT) {
    // if (mouse_buttons == SNES_MOUSE_BUTTON_RIGHT) {
}


void mouse_init(void) {
    set_sprite_data(SPRITE_MOUSE_TILE_START, 2u, mouse_cursors);
    set_sprite_tile(SPRITE_MOUSE_CURSOR, SPRITE_MOUSE_TILE_START + 1u);
    move_sprite(SPRITE_MOUSE_CURSOR, mouse_x, mouse_y);
    hide_sprite(SPRITE_MOUSE_CURSOR);

    // Init and do an first read to get the interrupt cycle running
    snes_mouse_interrupt_init();
    snes_mouse_interrupt_read_start();

}


uint8_t mouse_button_clicked(uint8_t buttons) {
    return mouse_buttons_clicked & buttons;
}


bool mouse_update(void) {

    // Check if data ready for use
    if (snes_mouse_interrupt_data_ready()) {
        use_mouse_data();

        // Queue next read
        snes_mouse_interrupt_read_start();

        return true;
    }

    return false;
}
