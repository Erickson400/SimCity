#include <iostream>
#ifndef  GRAPHICS_HPP
#include <SFML\Graphics.hpp>
#endif
#include "Game.h"

int main(int ergc, char** argv) {
	//Default: 1300, 700 
	//16 Grid 1280, 706
	//20*16= 320, 11*16= 176
	sf::RenderWindow *App = new sf::RenderWindow(sf::VideoMode(1280, 706), "SimCity_Builder", sf::Style::Titlebar | sf::Style::Close);
	App->setFramerateLimit(60); 

	Game *game = new Game(App);
	while (App->isOpen()) {
		sf::Clock timer;
		sf::Time elapsed1 = timer.getElapsedTime();
		while (App->pollEvent(game->event)) { game->EventHandling(); }
		game->Update();
		game->KeyCheck();
		game->Rendering();
		sf::Time elapsed2 = elapsed1 - timer.getElapsedTime();
		game->delta = -elapsed2.asMilliseconds();
	}
	delete game, App;
	return 0;
}
