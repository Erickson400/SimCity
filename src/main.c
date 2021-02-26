#include <stdio.h>
#include "SFML/Graphics.h"
#include "game.h"

int main() {
	// Blocks are 32x32
	// Let it be a 20x20 Grid
	// 32*20 = 640px
	sfRenderWindow* window = sfRenderWindow_create((sfVideoMode) { 640, 640 }, "SimCity, Road Mechanic", sfClose, NULL);
	sfRenderWindow_setFramerateLimit(window, 60);

	Game_Setup();
	while (sfRenderWindow_isOpen(window)) {
		sfEvent event;
		while (sfRenderWindow_pollEvent(window, &event)) {
			Game_Input(&event);
			if (event.type == sfEvtClosed) {
				sfRenderWindow_close(window);
			}
		}

		Game_Draw(window);
		sfRenderWindow_display(window);
	}

	sfRenderWindow_destroy(window);
	return 0;
}