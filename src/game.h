#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include "SFML/Graphics.h"

typedef unsigned char	uint8;
typedef char			sint8;
typedef unsigned short	uint16;
typedef short			sint16;
typedef unsigned int	uint32;
typedef int				sint32;

#define GRID_SIZE 20
#define START_SEED 230
#define TYPE_ROAD_START 4 // Equals or greater than TYPE_ROAD_START is a road
#define TYPE_ROAD_CORNER_START 6

enum Type {
	grass,
	tree,
	house,
	pond, // 3

	vertical_road,
	horizontal_road, // 5
	
	topLeft_corner_road, // top left refers to the corner where the sand is
	topRight_corner_road,
	bottomLeft_corner_road,
	bottomRight_corner_road,  // 9

	junction_road,
	upT_junction_road,
	leftT_junction_road,
	downT_junction_road,
	rightT_junction_road // 14
};
enum Side {
	TOP,
	LEFT,
	BOTTOM,
	RIGHT,
};
typedef struct {
	uint8 TOP : 1;
	uint8 LEFT : 1;
	uint8 RIGHT : 1;
	uint8 BOTTOM : 1;
} SideFlag;

// Grid
uint8 tileMap[GRID_SIZE][GRID_SIZE];

// Misc Rendering variables
sfTexture* tileset;
sfSprite* tileDrawer;

// Misc Functions
static void UpdateTiles(const uint16 p_mouseX, const uint16 p_mouseY);
static void Check_Tile_Neighbour(const uint8 x, const uint8 y, uint8 p_side, SideFlag* flag); // takes a Side enum
static void Update_Road_Tile_To_Neighbours(const uint8 x, const uint8 y, const SideFlag flag);

void Game_Setup();
void Game_Draw(sfRenderWindow* p_window);
void Game_Input(sfEvent* p_event);
void Game_Destroy();

#endif // !GAME_H