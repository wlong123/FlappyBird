#include "game.c"
#include <stdlib.h>

int main() {
	create_pipe();
	initialize();
	while (1) {
		wait_ms(33);
		if (game_over == 0) {
			update();
			draw();
		} else if (game_over < 60) {
			display_loss_msg();
			game_over++;
		} else if (game_over < 120) {
			display_score_msg();
			game_over++;
		} else {
			reset_game();
		}
	}
}
