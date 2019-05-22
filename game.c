#include "mbed.h"
#include "game.h"
#include "Adafruit_SSD1306.h"
#include <stdlib.h>
#include <fsl_device_registers.h>

I2C i2c(PTE25,PTE24); // I2C device sda, scl
Adafruit_SSD1306_I2c display(i2c, PTB23, 0x78, 32, 128);

void initialize (void) {
  // initialize SW3 button on FRDM board
  SIM->SCGC5 |= (1 << 9); //enable clock to Port A
  PORTA->PCR[4] |= PORT_PCR_MUX(001);
  PTA->PDDR &= ~GPIO_PDDR_PDD(1 << 4);
      
  display.begin(5);
  draw();
}

void reset_game (void) {
  int pressed = (GPIOA->PDIR >> 4);
  if (last_pressed == 0 && pressed) {
  	pipe * tmp = pipe_list;
    while (tmp != NULL) {
      pipe * next = tmp->next;
      free(tmp);
      tmp = next;
    }
    pipe_list = NULL;
    create_pipe();
    first_pipe = 141;
    last_pipe = 141;
    game_over = 0;
    bird_y = 16;
    time_since_flap = 10;
    score = 0;
  }
  last_pressed = pressed;
}

int vert_movement (void) {
	if (time_since_flap < 6) {
		return 1;
	} else if (time_since_flap < 15) {
		return 0;
	} else if (time_since_flap < 23) {
		return -1;
	} else if (time_since_flap < 32) {
		return -2;
	} else {
		return -3;
	}
}

void create_pipe (void) {
  pipe * new_pipe = (pipe*) malloc(sizeof(pipe));
  new_pipe->x = 141;
  new_pipe->bottom = (rand() % (8)) + 2;
  new_pipe->top = 33-(new_pipe->bottom + 20);
  new_pipe->next = NULL;
  if (pipe_list == NULL) {
    pipe_list = new_pipe;
  } else {
    pipe * tmp = pipe_list; //temp pointer to traverse 
    while (tmp->next != NULL) {
      // while there are more elements in the list
      tmp = tmp->next;
    }
    // now tmp is the last element in the list
    tmp->next = new_pipe;
  }
}

int collides (pipe * p) {
  // check these with pipe top
  // (bird_x + 1, bird_y + 3)
  if (p->x-10 <= bird_x+1 && bird_x+1 <= p->x && bird_y+3 >= 31-p->top) {
    return 1;
  }
  // (bird_x - 1, bird_y + 3)
  if (p->x-10 <= bird_x-1 && bird_x-1 <= p->x && bird_y+3 >= 31-p->top) {
    return 1;
  }
  // (bird_x + 3, bird_y + 1)
  if (p->x-10 <= bird_x+3 && bird_x+3 <= p->x && bird_y+1 >= 31-p->top) {
    return 1;
  }
  // (bird_x - 3, bird_y + 1)
  if (p->x-10 <= bird_x-3 && bird_x-3 <= p->x && bird_y+1 >= 31-p->top) {
    return 1;
  }

  //check these with pipe bottom
  // (bird_x + 1, bird_y - 3)
  if (p->x-10 <= bird_x+1 && bird_x+1 <= p->x && bird_y-3 <= p->bottom) {
    return 1;
  }
  // (bird_x - 1, bird_y - 3)
  if (p->x-10 <= bird_x-1 && bird_x-1 <= p->x && bird_y-3 <= p->bottom) {
    return 1;
  }
  // (bird_x + 3, bird_y - 1)
  if (p->x-10 <= bird_x+3 && bird_x+3 <= p->x && bird_y-1 <= p->bottom) {
    return 1;
  }
  // (bird_x - 3, bird_y - 1)
  if (p->x-10 <= bird_x-3 && bird_x-3 <= p->x && bird_y-1 <= p->bottom) {
    return 1;
  }

  // no previous collision check passed
  return 0;
}

void update (void) {
  // Check input from joystick change time_since_flap accordingly
  if (0) {
    time_since_flap = 0;
  } else {
    time_since_flap++;
  }
  bird_y += vert_movement();

  // Move the pipes and check for collisions
  if (pipe_list != NULL) {
    int movement = (score / 10) + 1; // distance to move all pipes
    pipe * tmp = pipe_list; 
		while (tmp->next != NULL) {
      tmp->x -= movement;
      tmp = tmp->next;
		}
    tmp->x -= movement; //decrease the x of the last pipe
    if (tmp->x <= 96 && last_pipe > 96) {
      create_pipe();
    }
    last_pipe = tmp->x;

    if (pipe_list->x == 17) { // if the bird passed the leftmost pipe, score +1
      score++;
    }
    if (pipe_list->x < 0) { // if leftmost pipe is off screen, free its struct
      tmp = pipe_list;
      pipe_list = pipe_list->next;
      free(tmp);
    }

    if (collides(pipe_list)) {
      game_over = 1;
    }
  }

  if (bird_y - 4 <= 0 ) {
    game_over = 1;
  }
}

void draw (void) {
  display.clearDisplay();
  display.fillCircle(bird_x, 31-bird_y, 3, WHITE);
  pipe * tmp = pipe_list; 
  while (tmp != NULL) {
    display.fillRect(tmp->x-10, 0, 10, tmp->top, WHITE);
    display.fillRect(tmp->x-10, 31-(tmp->bottom), 10, tmp->bottom, WHITE);
    tmp = tmp->next;
  }
  display.display();
}

void display_loss_msg (void) {
  display.clearDisplay();
  for (int i = 0; i<strlen(loss_msg); i++) {
    display.drawChar(10*(i+1), 16, loss_msg[i], WHITE, BLACK, 1);
  }
  display.display();
}

void display_score_msg (void) {
  display.clearDisplay();
  for (int i = 0; i<strlen(score_msg); i++) {
    display.drawChar(10*(i+1), 10, score_msg[i], WHITE, BLACK, 1);
  }
  if (score == 0) {
    char scr = 48;
    display.drawChar(64, 20, scr, WHITE, BLACK, 1);
  } else {
    int temp = score;
    int x = 70;
    while (temp > 0) {
      char scr = (temp%10)+48;
      display.drawChar(x, 24, scr, WHITE, BLACK, 1);
      temp = temp/10;
      x -= 10;
    }
  }
  display.display();
}