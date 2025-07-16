#pragma once

#include <vector>
#include "Raylib.h"
#include "AssetList.h"
#include "Actor.h"
#include "Terrain.h"

using std::vector;

static void DrawScreen(Vector2* scroll);

//Draw the terrain and the gameObjects on screen
static void DrawScreen(Vector2* scroll) {
	vector<vector<Actor*>>* actorList = Actor::GetAllActorsLayered();
	vector<vector<Terrain::Tile>>* terrain = Terrain::GetTerrain();
	Vector2 tileSize = Terrain::GetTileSize();

	short max = fmax(terrain->size(), actorList->size());
	for (int i = 0; i < max; i++) {
		if (i < terrain->size()) {
			if ((*terrain)[i].size() > 0) {
				for (Terrain::Tile tile : (*terrain)[i]) {
					if (tile.position.x * tileSize.x > scroll->x - tileSize.y &&
						tile.position.x * tileSize.x < scroll->x + GetScreenWidth() &&
						tile.position.y * tileSize.y > scroll->y - tileSize.y &&
						tile.position.y * tileSize.y < scroll->y + GetScreenHeight()) {
						Texture2D* sprite = AssetList::GetTexture(*Terrain::GetTextureName(tile.dictionaryTexture));
						DrawTexturePro(*sprite,
							Rectangle{ 0, 0, (float)sprite->width, (float)sprite->height },
							Rectangle{ (tile.position.x * tileSize.x) - scroll->x + tileSize.x * 0.5f, (tile.position.y * tileSize.y) - scroll->y + tileSize.y * 0.5f, tileSize.x, tileSize.y },
							Vector2{ tileSize.x * 0.5f, tileSize.y * 0.5f },
							tile.rotation,
							WHITE);
					}
				}
			}
		}

		if (actorList->size() > i) {
			for (Actor* go : (*actorList)[i]) {
				if (go->GetEnabled())  go->Draw(scroll);
			}
		}
	}
}