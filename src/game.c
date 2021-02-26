#include "game.h"

static void Check_Tile_Neighbour(const uint8 x, const uint8 y, uint8 p_side, SideFlag* flag) {
	// Works fine

	switch (p_side)
	{
	case TOP: 
		if (y != 0) {
			if (tileMap[y - 1][x] >= TYPE_ROAD_START) flag->TOP = 1;
		}
		break;
	case LEFT: 
		if (x != 0) {
			if (tileMap[y][x - 1] >= TYPE_ROAD_START) flag->LEFT = 1;
		}
		break;
	case BOTTOM: 
		if (y != GRID_SIZE - 1) {
			if (tileMap[y + 1][x] >= TYPE_ROAD_START) flag->BOTTOM = 1;
		}
		break;
	case RIGHT:
		if (x != GRID_SIZE - 1) {
			if (tileMap[y][x + 1] >= TYPE_ROAD_START) flag->RIGHT = 1;
		}
		break;
	}
}
static void Update_Road_Tile_To_Neighbours(const uint8 x, const uint8 y, const SideFlag flag) {

	if (flag.TOP && flag.LEFT && flag.RIGHT && flag.BOTTOM) { tileMap[y][x] = junction_road; return; }
	if (!flag.TOP && !flag.LEFT && !flag.RIGHT && !flag.BOTTOM) { tileMap[y][x] = junction_road; return; }

	if (flag.LEFT && flag.RIGHT && flag.BOTTOM) { tileMap[y][x] = upT_junction_road; return; }
	if (flag.TOP && flag.RIGHT && flag.BOTTOM) { tileMap[y][x] = leftT_junction_road; return; }
	if (flag.TOP && flag.LEFT && flag.RIGHT) { tileMap[y][x] = downT_junction_road; return; }
	if (flag.TOP && flag.LEFT && flag.BOTTOM) { tileMap[y][x] = rightT_junction_road; return; }

	if (flag.BOTTOM && flag.RIGHT) { tileMap[y][x] = topLeft_corner_road; return; }
	if (flag.LEFT && flag.BOTTOM) { tileMap[y][x] = topRight_corner_road; return; }
	if (flag.TOP && flag.RIGHT) { tileMap[y][x] = bottomLeft_corner_road; return; }
	if (flag.TOP && flag.LEFT) { tileMap[y][x] = bottomRight_corner_road; return; }

	if (flag.TOP && flag.BOTTOM) { tileMap[y][x] = vertical_road; return; }
	if (flag.LEFT && flag.RIGHT) { tileMap[y][x] = horizontal_road; return; }

	if (flag.LEFT||flag.RIGHT) { tileMap[y][x] = horizontal_road; return; }
	if (flag.TOP || flag.BOTTOM) { tileMap[y][x] = vertical_road; return; }
}
static void UpdateTiles(const uint16 p_mouseX, const uint16 p_mouseY) {
	// Function runs when a block is clicked

	uint8 local_x = (p_mouseX / 32);
	uint8 local_y = (p_mouseY / 32);

	if (local_x > GRID_SIZE - 1 || local_x < 0) return;
	if (local_y > GRID_SIZE - 1 || local_y < 0) return;

	// Toggle between Road and Ground
	tileMap[local_y][local_x] = (tileMap[local_y][local_x] < TYPE_ROAD_START) ? junction_road : rand() % TYPE_ROAD_START;

	for (uint8 y = 0; y < GRID_SIZE; y++) {
		for (uint8 x = 0; x < GRID_SIZE; x++) {
			
			//if (tileMap[y][x] <= 3) continue; // skip if its not a road

			// Check if there are neighbouring roads and set the data to the Around variable
			SideFlag Around = { 0,0,0,0 };
			Check_Tile_Neighbour(x, y, TOP, &Around);
			Check_Tile_Neighbour(x, y, LEFT, &Around);
			Check_Tile_Neighbour(x, y, BOTTOM, &Around);
			Check_Tile_Neighbour(x, y, RIGHT, &Around);
			
			// Change the tile depending on its neighbours
			if (tileMap[y][x]  >= TYPE_ROAD_START) {
				Update_Road_Tile_To_Neighbours(x, y, Around);
			}
		}
	}
}











void Game_Setup() {
	// Setup rendering
	tileset = sfTexture_createFromFile("Media/FinalTilemap.png", NULL);
	tileDrawer = sfSprite_create();
	sfSprite_setTexture(tileDrawer, tileset, sfFalse);

	// Set grid to random tiles
	srand(START_SEED);
	for (uint8 y = 0; y < GRID_SIZE; y++) {
		for (uint8 x = 0; x < GRID_SIZE; x++) {

			// grass 60%
			// tree 20%
			// houses 15%
			// pond 5%

			uint8 randomNum = rand() % 100; // Probablility
			if (randomNum < 60) tileMap[y][x] = grass;
			if (randomNum > 60 && randomNum < 70) tileMap[y][x] = tree;
			if (randomNum > 70 && randomNum < 95) tileMap[y][x] = house;
			if (randomNum > 95) tileMap[y][x] = pond;
		}
	}
}
void Game_Draw(sfRenderWindow* p_window) {
	//sfSprite* tempSprite = sfSprite_create();

	// Draw the Grid
	for (uint8 y = 0; y < GRID_SIZE; y++) {
		for (uint8 x = 0; x < GRID_SIZE; x++) {
			sfIntRect local_rect;

			// Ground
			if (tileMap[y][x] < TYPE_ROAD_START)  local_rect = (sfIntRect){ tileMap[y][x] * 32, 0, 32, 32 };
			// Vertical/Horizontal Roads
			if (tileMap[y][x] >= TYPE_ROAD_START) local_rect = (sfIntRect){ (tileMap[y][x] - 4) * 32, 32, 32, 32 };
			// Corners
			if (tileMap[y][x] >= TYPE_ROAD_CORNER_START) local_rect = (sfIntRect){ (tileMap[y][x] - 6) * 32, 64, 32, 32 };
			// Juntions
			if (tileMap[y][x] == 10) local_rect = (sfIntRect){ 64, 32, 32, 32 };
			if (tileMap[y][x] > 10) local_rect = (sfIntRect){ (tileMap[y][x] - 11) * 32, 96, 32, 32 };

			// Set the texture rect accorditing to tile type
			sfSprite_setTextureRect(tileDrawer, local_rect);

			sfSprite_setPosition(tileDrawer, (sfVector2f) { (float)x*32, (float)y*32});
			sfRenderWindow_drawSprite(p_window, tileDrawer, NULL);
		}
	}
}
void Game_Input(sfEvent* p_event) {
	if (p_event->type == sfEvtMouseButtonPressed) {
		if(p_event->mouseButton.button == sfMouseLeft){
			UpdateTiles(p_event->mouseButton.x, p_event->mouseButton.y);
		}
	}
}
void Game_Destroy() {
	sfTexture_destroy(tileset);
	sfSprite_destroy(tileDrawer);
}