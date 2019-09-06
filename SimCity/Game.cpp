#include "Game.h"

Game::Game(sf::RenderWindow *app) : App(app) {
	char ScreenSize = 4;
	view1.setSize(sf::Vector2f(App->getSize().x/ ScreenSize, App->getSize().y/ ScreenSize));
	view1.setCenter(sf::Vector2f((App->getSize().x/ ScreenSize)/2, (App->getSize().y/ ScreenSize)/2));
	
}

void Game::Update() {

}

void Game::Rendering() {
	unsigned char BGsat = 100;
	App->clear(sf::Color(BGsat, BGsat, BGsat));
	App->setView(view1);

	Map.draw(*App);

	App->display();
}

void Game::EventHandling() {
	switch (event.type) {
	case sf::Event::Closed: App->close(); break;
	case sf::Event::KeyPressed:
		switch (event.key.code) {
		case sf::Keyboard::Escape: App->close(); break;
		case sf::Keyboard::W: Up = true; break;
		case sf::Keyboard::A: Left = true; break;
		case sf::Keyboard::S: Down = true; break;
		case sf::Keyboard::D: Right = true; break;
		}break;
	case sf::Event::KeyReleased:
		switch (event.key.code) {
		case sf::Keyboard::W: Up = false; break;
		case sf::Keyboard::A: Left = false; break;
		case sf::Keyboard::S: Down = false; break;
		case sf::Keyboard::D: Right = false; break;
		}break;
	case sf::Event::MouseButtonPressed: 
		if (event.mouseButton.button == sf::Mouse::Left) Map.UpdateTiles(event.mouseButton.x, event.mouseButton.y);
		 break;
	}
}

void Game::KeyCheck() {
	if (Up&&Down) {
		Vkey = 0;
	}
	else if (!Up && !Down) {
		Vkey = 0;
	}
	else if (!Up&&Down) {
		Vkey = -1;
	}
	else if (Up && !Down) {
		Vkey = 1;
	}

	if (Right&&Left) {
		Hkey = 0;
	}
	else if (!Right && !Left) {
		Hkey = 0;
	}
	else if (!Right&&Left) {
		Hkey = -1;
	}
	else if (Right && !Left) {
		Hkey = 1;
	}
}















Game::~Game() {
	App->close();
	delete  App;
}
