#include "buttons.h"
#include "buttons_control.h"
#include "display.h"
#include "display_control.h"
#include "includes.h"
#include "security_system_control.h"

uint8_t *buttons_control_pin_code_input() {
  uint8_t *pin_code =
      malloc(4 * sizeof(uint8_t));          // allocate memory for pin_code
  memset(pin_code, 0, 4 * sizeof(uint8_t)); // initialize with 4 zeros

  int current_position = 0;

  while (current_position < 4) {
    display_setValues(pin_code[0], pin_code[1], pin_code[2], pin_code[3]);

    if (buttons_1_pressed()) {
      pin_code[current_position]++;
      pin_code[current_position] = pin_code[current_position] % 10;

      _delay_ms(200);
    } else if (buttons_3_pressed()) { // Next number / submit
      current_position++;
      display_control_show_pin_code_position(pin_code, current_position);
      _delay_ms(200);
    } else if (buttons_2_pressed()) { // Previous number
      if (current_position != 0) {
        current_position--;
        display_control_show_pin_code_position(pin_code, current_position);
        _delay_ms(200);
      }
    }
  }

  char message[25];
  sprintf(message, "Buttons control: %d%d%d%d\n", pin_code[0], pin_code[1],
          pin_code[2], pin_code[3]);

  pc_comm_send_string_blocking(message);

  return pin_code;
}

void buttons_control_listen() {
  if (buttons_1_pressed()) {
    // LOCK / UNLOCK
    _delay_ms(200);
    security_system_control_evaluate();
  } else if (buttons_2_pressed()) {
    // figure something out
  } else if (buttons_3_pressed()) {
    security_system_control_override_pin_code();
  }
  display_setValues(33, 18, 37, 37); // Show "Hi  "
}