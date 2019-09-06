#pragma once
#include <iostream>
#ifndef  GRAPHICS_HPP
#include <SFML\Graphics.hpp>
#endif
#include "Objects.cpp"

class Game {
public:
	Game(sf::RenderWindow *app); ~Game();
	sf::RenderWindow *App;
	sf::Event event;
	sf::View view1;
	int delta;
	char Hkey = 0, Vkey = 0;
	bool Up, Down, Right, Left;

	Terrain Map;


	void Update();
	void EventHandling();
	void KeyCheck();
	void Rendering();
};
