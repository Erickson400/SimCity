#include <iostream>
#ifndef  GRAPHICS_HPP
#include <SFML\Graphics.hpp>
#endif
#include <time.h>
using namespace std;
typedef uint16_t Short;
#define TILE(x1, y1) ((w+x1>=0&&h+y1>=0&&w+x1<=20&&h+y1<=11)? GetTile(w+x1, h+y1) >= 3 && GetTile(w+x1, h+y1) <= 13 : false)

constexpr char TOP_LEFT     = (1 << 0);
constexpr char TOP_MID  	= (1 << 1);
constexpr char TOP_RIGHT    = (1 << 2);
constexpr char MID_LEFT  	= (1 << 3);
constexpr char MID_RIGHT  	= (1 << 4);
constexpr char BOTTOM_LEFT  = (1 << 5);
constexpr char BOTTOM_MID   = (1 << 6);
constexpr char BOTTOM_RIGHT = (1 << 7);

struct Terrain {
	Terrain() {
		if (!Tileset.loadFromFile("Media/Tilemap.png")) { std::cout << "Cant loaded Image"; };
		tileDrawer.setTexture(Tileset);
		tileDrawer.setColor(sf::Color(200,200,200));
		Short mapWidth = sizeof(Map) / sizeof(Map[0]), mapHeight = sizeof(Map[0]) / 2;
		srand(time(NULL));
		for (Short h = 0; h < mapHeight; h++) {
			for (Short w = 0; w < mapWidth; w++) {
				Map[w][h] = rand() % 3;
			}
		}
	};

	void draw(sf::RenderWindow &window) {
		Short mapWidth = sizeof(Map) / sizeof(Map[0]), mapHeight = sizeof(Map[0]) / 2;
		for (Short h = 0; h < mapHeight; h++) {
			for (Short w = 0; w < mapWidth; w++) {
				sf::Sprite temp = tileDrawer;
				switch (Map[w][h]) {
				// Ground
				case 0: tileDrawer.setTextureRect(sf::IntRect(0, 0, 16, 16)); break;
				case 1: tileDrawer.setTextureRect(sf::IntRect(16, 0, 16, 16)); break;
				case 2:	tileDrawer.setTextureRect(sf::IntRect(32, 0, 16, 16)); break;
				// Streight
				case 3: tileDrawer.setTextureRect(sf::IntRect(0, 16, 16, 16)); break; //Vertical
				case 4: tileDrawer.setTextureRect(sf::IntRect(0, 16, 16, 16)); tileDrawer.setOrigin(0,16); tileDrawer.setRotation(90); break; //Horizontal
				//Curve
				case 5: tileDrawer.setTextureRect(sf::IntRect(16, 16, 16, 16)); break; //Right - Down
				case 6: tileDrawer.setTextureRect(sf::IntRect(16, 16, 16, 16)); tileDrawer.setOrigin(16,0); tileDrawer.setRotation(-90); break; //Up - Right
				case 7: tileDrawer.setTextureRect(sf::IntRect(16, 16, 16, 16)); tileDrawer.setOrigin(16,16); tileDrawer.setRotation(-180); break; //Left - Up
				case 8: tileDrawer.setTextureRect(sf::IntRect(16, 16, 16, 16)); tileDrawer.setOrigin(0,16); tileDrawer.setRotation(90); break; //Down - Left
				//junction
				case 9:	tileDrawer.setTextureRect(sf::IntRect(32, 16, 16, 16)); break;
				//edge
				case 10:tileDrawer.setTextureRect(sf::IntRect(0, 32, 16, 16)); break; // Up
				case 11:tileDrawer.setTextureRect(sf::IntRect(0, 32, 16, 16)); tileDrawer.setOrigin(16,0); tileDrawer.setRotation(-90); break; // Left
				case 12:tileDrawer.setTextureRect(sf::IntRect(0, 32, 16, 16)); tileDrawer.setOrigin(16,16); tileDrawer.setRotation(-180); break; // Down
				case 13:tileDrawer.setTextureRect(sf::IntRect(0, 32, 16, 16)); tileDrawer.setOrigin(0,16); tileDrawer.setRotation(90); break; // Right
				}
				tileDrawer.setPosition(w * 16, h * 16);
				window.draw(tileDrawer);
				tileDrawer = temp;
			}
		}


	}
	void UpdateTiles(Short mousex, Short mousey) {
		Short x = mousex/64, y = mousey/64;
		Short mapWidth = sizeof(Map) / sizeof(Map[0]), mapHeight = sizeof(Map[0]) / 2;
		char Around = 0;
		if (Map[x][y] <= 2) {
			SetTile(x, y, 9);
		}
		else {
			SetTile(x, y, rand() % 3);
		}

		for (Short h = 0; h < mapHeight; h++) {
			for (Short w = 0; w < mapWidth; w++) {
				Around = 0;
				#pragma region CheckBitWise
				if (TILE(-1, -1)) {
					//top-left
					Around |= TOP_LEFT;
				}
				if (TILE(0, -1)) {
					//top-mid
					Around |= TOP_MID;
				}
				if (TILE(1, -1)) {
					//top-right
					Around |= TOP_RIGHT;
				}
				if (TILE(-1, 0)) {
					//mid-left
					Around |= MID_LEFT;
				}
				if (TILE(1, 0)) {
					//mid-right
					Around |= MID_RIGHT;
				}
				if (TILE(-1, 1)) {
					//bottom-left
					Around |= BOTTOM_LEFT;
				}
				if (TILE(0, 1)) {
					//bottom-mid
					Around |= BOTTOM_MID;
				}
				if (TILE(1, 1)) {
					//bottom-right
					Around |= BOTTOM_RIGHT;
				}
				#pragma endregion
				if (GetTile(w, h) >= 3 && GetTile(w, h) <= 13) {
					Check(w, h, Around);
				}	
			}
		}

	}
	void Check(Short x, Short y, Short Mask) {
		if ((Mask&TOP_MID) || (Mask&BOTTOM_MID)) SetTile(x, y, 3); //Vertical
		if ((Mask&MID_LEFT) || (Mask&MID_RIGHT)) SetTile(x, y, 4); //Horizontal

		if (((Mask&MID_RIGHT) && (Mask&BOTTOM_MID))&&!((Mask&MID_LEFT) || (Mask&TOP_MID))) SetTile(x, y, 5); //Curve Right-Down 
		if (((Mask&TOP_MID) && (Mask&MID_RIGHT))&&!((Mask&MID_LEFT) || (Mask&BOTTOM_MID))) SetTile(x, y, 6); //Curve Up-Right
		if (((Mask&MID_LEFT) && (Mask&TOP_MID))&&!((Mask&MID_RIGHT) || (Mask&BOTTOM_MID))) SetTile(x, y, 7); //Curve Left-Up 
		if (((Mask&BOTTOM_MID) && (Mask&MID_LEFT))&&!((Mask&MID_RIGHT) || (Mask&TOP_MID))) SetTile(x, y, 8); //Curve Down-Left

		/*
		10
		11
		12
		13
		*/

		if ((Mask&MID_LEFT) && (Mask&BOTTOM_MID) && (Mask&MID_RIGHT) && !(Mask&TOP_MID)) SetTile(x, y, 10); //Edge Up
		if (!(Mask&MID_LEFT) && (Mask&BOTTOM_MID) && (Mask&MID_RIGHT) && (Mask&TOP_MID)) SetTile(x, y, 11); //Edge Left
		if ((Mask&MID_LEFT) && !(Mask&BOTTOM_MID) && (Mask&MID_RIGHT) && (Mask&TOP_MID)) SetTile(x, y, 12); //Edge Down
		if ((Mask&MID_LEFT) && (Mask&BOTTOM_MID) && !(Mask&MID_RIGHT) && (Mask&TOP_MID)) SetTile(x, y, 13); //Edge Right

		if ((Mask&TOP_MID) && (Mask&BOTTOM_MID) && (Mask&MID_LEFT) && (Mask&MID_RIGHT)) SetTile(x, y, 9); //Junction
	}
	void SetTile(Short x, Short y, Short num) {
		Map[x][y] = num;
	}
	Short GetTile(Short x, Short y) {
		return Map[x][y];
	}

private:
	sf::Texture Tileset;
	sf::Sprite tileDrawer;
	Short Map[20][11] = {};

	/*
	0,1,2 Ground

	3 Vertical Road
	4 Horizontal Road

	5 Right-Down
	6 Up-Right
	7 Left-Up
	8 Down-Left

	9 Junction

	10 Three-Up
	11 Three-Left
	12 Three-Down
	13 Three-Right

	*/
	

};


