#ifndef _SNES_MOUSE_H
#define _SNES_MOUSE_H

#include <stdint.h>
#include <stdbool.h>

// #define DEBUG_VISUALIZE_INTERRUPT_TIME_BGP

#define SNES_MOUSE_TX_LATCH      0b11111111
#define SNES_MOUSE_REPORT_LEN    4

// In Byte 2
#define SNES_MOUSE_BUTTON_LEFT  0b01000000u
#define SNES_MOUSE_BUTTON_RIGHT 0b10000000u
#define SNES_MOUSE_BUTTON_MASK  0b11000000u
#define SNES_MOUSE_BUTTON_BOTH  0b11000000u

// In Byte 3
#define SNES_MOUSE_Y_DIR   0b10000000u   // .7: 1 = Down, 0 = Up, 6..0: Movement
#define SNES_MOUSE_Y_MASK  0b01111111u

// In Byte 4
#define SNES_MOUSE_X_DIR   0b10000000u   // .7: 1 = Right, 0 = Left, 6..0: Movement
#define SNES_MOUSE_X_MASK  0b01111111u

#define SNES_MOUSE_OEM      0
#define SNES_MOUSE_HYPERKIN 1

typedef struct snes_mouse_t {
    uint8_t first_byte;  // Unused by mouse
    uint8_t buttons;
    uint8_t move_y;
    uint8_t move_x;
} snes_mouse_t;

extern snes_mouse_t snes_mouse;
extern bool         snes_mouse_model;

// State indicates the meaning of the data in SB Reg
// when the SIO interrupt fires for a completed serial transfer
enum {
    SNES_MOUSE_STATE_LATCH,      // Ignore serial data during this state
    SNES_MOUSE_STATE_FIRST_BYTE,
    SNES_MOUSE_STATE_BUTTONS,
    SNES_MOUSE_STATE_MOVE_Y,
    SNES_MOUSE_STATE_MOVE_X,     // Last transfer received, do not trigger another serial transfer
    SNES_MOUSE_STATE_DONE,
};


void snes_mouse_blocking_wait_poll(void);

bool snes_mouse_interrupt_data_ready(void);
void snes_mouse_interrupt_read_start(void);
void snes_mouse_interrupt_init(void);
void snes_mouse_interrupt_deinstall(void);

void snes_mouse_set_model(uint8_t);

#define SNES_MOUSE_GET_MODEL() (snes_mouse_model)

#endif // _SNES_MOUSE_H
