#include "Cursor.h"
#include "Terrain.h"
#include "AssetList.h"
#include "UI_TilesMenu.h"

#include "rlgl.h"
#include "raymath.h"
#include <algorithm>

using std::to_string;

Cursor::Cursor(){
}

Cursor::~Cursor(){
}

void Cursor::Update(Vector2* scroll) {
	//Move
	if (IsMouseButtonDown(2)) {
		*scroll = Vector2Subtract(*scroll, GetMouseDelta());
	}

	scroll->x -= GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) * scrollSpeed.x * GetFrameTime();
	scroll->y -= GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) * scrollSpeed.y * GetFrameTime();

	//Rotation + Layer change
	if (IsKeyPressed(KEY_UP) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
		if (placementLayer == Terrain::GetMaxLayer() - 1) placementLayer = 0;
		else placementLayer++;
	}
	if (IsKeyPressed(KEY_DOWN) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
		if (placementLayer == 0) placementLayer = Terrain::GetMaxLayer() - 1;
		else placementLayer--;
	}
	if (IsKeyPressed(KEY_Q) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
		if (placementRotation == 0) placementRotation = 270;
		else placementRotation -= 90;
	}
	if (IsKeyPressed(KEY_E) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
		if (placementRotation == 270) placementRotation = 0;
		else placementRotation += 90;
	}

	//Tiles placement
	Vector2 mPos{ (int)floor((scroll->x + GetMouseX()) / Terrain::GetTileSize().x), (int)floor((scroll->y + GetMouseY()) / Terrain::GetTileSize().y)};
	if (IsMouseButtonPressed(0)) {
		UI_TilesMenu* menu = static_cast<UI_TilesMenu*>(GetActorWithName("UITilesMenu"));
		if (menu->GetCurrentTextureName() != "" && menu->GetCurrentTextureName() != ".") {
			currentTexture = std::distance(AssetList::GetTextureList()->begin(), AssetList::GetTextureList()->find(menu->GetCurrentTextureName()));
			menu->Clicked();
		}
		else if(menu->GetCurrentTextureName() != "."){
			vector<Actor*> actors = GetAllActorsInCollisionVector2(GetMousePosition());
			if (actors.size() > 0) {
				for (int i = 0; i < actors.size(); i++) {
					if (actors[i]->GetActorType() == ActorType::UI) {
						actors[0]->Clicked();
						break;
					}
				}
			}
			else Terrain::AddNewTile(placementLayer, placementRotation, mPos, AssetList::GetNameAtPosition(currentTexture));
		}
	}
	if (IsMouseButtonPressed(1)) Terrain::RemoveTile(placementLayer, mPos);
	Vector2 cPos{ (int)floor((scroll->x + GetScreenWidth() * 0.5f) / Terrain::GetTileSize().x), (int)floor((scroll->y + Terrain::GetTileSize().y * 0.5f + GetScreenWidth() * 0.5f) / Terrain::GetTileSize().y)};
	if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) Terrain::AddNewTile(placementLayer, placementRotation, cPos, AssetList::GetNameAtPosition(currentTexture));
	if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) Terrain::RemoveTile(placementLayer, cPos);

	//Save / Load
	if ((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT))Terrain::SaveMap("TowerPath");
	if ((IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_O)) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_LEFT))Terrain::LoadMap("TowerPath");
}

void Cursor::Draw(Vector2* scroll) {
	rlPushMatrix();
	rlTranslatef(0 - scroll->x, 25 * 50 - scroll->y, 0);
	rlRotatef(90, 1, 0, 0);
	DrawGrid(100, 50);
	rlPopMatrix();

	DrawText(("X. " + to_string((scroll->x - GetMouseX()) / Terrain::GetTileSize().x)).c_str(), 10, 10, 20, GRAY);
	DrawText(("Y. " + to_string((scroll->y - GetMouseY()) / Terrain::GetTileSize().y)).c_str(), 10, 30, 20, GRAY);
	DrawText(("Current Sprite:  " + AssetList::GetNameAtPosition(currentTexture)).c_str(), 10, 50, 20, ORANGE);
	DrawText(("Current Layer:  " + to_string(placementLayer)).c_str(), 10, 70, 20, ORANGE);
	DrawText(("Current Rotation:  " + to_string(placementRotation)).c_str(), 10, 90, 20, ORANGE);

	Texture2D* sprite = AssetList::GetTexture(AssetList::GetNameAtPosition(currentTexture));
	DrawTexturePro(*sprite,
		Rectangle{ 0, 0, (float)sprite->width, (float)sprite->height },
		Rectangle{ 500, 50, 80, 80 },
		Vector2{ 40,40 },
		placementRotation,
		WHITE);

	if (GetFrameTime() != 0)
	{
		DrawText(TextFormat("CURRENT FPS: %i", (int)(1.0f / GetFrameTime())), 10, GetScreenHeight() - 30, 20, BLACK);
	}

	DrawRectangle(GetScreenWidth() * 0.5f - 5, GetScreenHeight() * 0.5f - 5, 10, 10, BLACK);
}