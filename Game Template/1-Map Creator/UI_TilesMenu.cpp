#include "UI_TilesMenu.h"
#include "AssetList.h"
#include "Terrain.h"

UI_TilesMenu::UI_TilesMenu():
Actor(Vector2{ 0, GetScreenHeight() * 0.375f },
			Vector2{ 30, GetScreenHeight() * 0.250f }, 
			ActorType::UI)
{
}

UI_TilesMenu::~UI_TilesMenu(){
}

void UI_TilesMenu::Update(Vector2* scroll){
	//Change textures
	if (GetMouseWheelMove() > 0){
		mouseScroll = Clamp(mouseScroll - mouseScrollSpeed, 0, mouseScrollMax);
	}
	if (GetMouseWheelMove() < 0) {
		mouseScroll = Clamp(mouseScroll + mouseScrollSpeed, 0, mouseScrollMax);
	}
}

void UI_TilesMenu::Draw(Vector2* scroll){
	Rectangle interactibleBar{ position.x, position.y, size.x, size.y };
	DrawRectanglePro(interactibleBar, Vector2Zero(), 0, BLACK);
	if (open) {
		currentTextureName = "";

		Rectangle menuBackground{ 0, GetScreenHeight() - GetScreenHeight() * 0.95f, 300, GetScreenHeight() * 0.9f };
		DrawRectanglePro(menuBackground, Vector2Zero(), 0, LIGHTGRAY);

		Rectangle source{ 0, 0, texture->width, texture->height };
		Rectangle dest{ 0, 0, Terrain::GetTileSize().x, Terrain::GetTileSize().y};
		Vector2 pos = Vector2{ menuBackground.x +15 + dest.width * 0.5f, menuBackground.y +15 - mouseScroll + dest.height};

		if (menuBackground.x <= GetMouseX() && menuBackground.x + menuBackground.width >= GetMouseX() &&
			menuBackground.y <= GetMouseY() && menuBackground.y + menuBackground.height >= GetMouseY()) {
			currentTextureName = ".";
		}

		for (auto i : *AssetList::GetTextureList()) {
			source = Rectangle{0, 0, (float)i.second.width, (float)i.second.height };
			if (pos.x + dest.width >= menuBackground.width) {
				pos.x = menuBackground.x + 15 + dest.width * 0.5f;
				pos.y += dest.height + 15;
			}
			if (pos.y - dest.height * 0.5f > menuBackground.y && pos.y + dest.height * 0.5f < menuBackground.x + menuBackground.height) {
				dest.x = pos.x;
				dest.y = pos.y;
				DrawTexturePro(i.second, source, dest, Vector2{ dest.width * 0.5f, dest.height * 0.5f }, 0, color);
				if (dest.x - dest.width * 0.5f <= GetMouseX() && dest.x + dest.width * 0.5f >= GetMouseX() &&
					dest.y - dest.height * 0.5f <= GetMouseY() && dest.y + dest.height * 0.5f >= GetMouseY()) {
					currentTextureName = i.first;
				}
			}
			pos.x += dest.width + 15;
		}
		mouseScrollMax = pos.y;
	}
}

void  UI_TilesMenu::Clicked(){
	open = !open;
	mouseScroll = 0;
	currentTextureName = "";
	if(open) position = Vector2{ 300, GetScreenHeight() * 0.375f };
	else position = Vector2{ 0, GetScreenHeight() * 0.375f };
}

void UI_TilesMenu::OpenTilesTab(){
}
