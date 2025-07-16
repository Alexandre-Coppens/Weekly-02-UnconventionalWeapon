#include <iostream>
#include "G_Engine.h"
#include "DrawScreen.h"

//Put here all the other includes
#include "ActorIncludes.h"

using std::cout;
using std::to_string;

G_Engine::G_Engine() {
}

G_Engine::~G_Engine(){
}

void G_Engine::Start() {
	terrain = Terrain::GetInstance();
	assets = AssetList::GetInstance();
	terrain->SetTileSize(Vector2{ 50,50 });
	scroll = { GetScreenWidth() * 0.5f ,GetScreenHeight() * 0.5f };

	//Create Animations here
	AssetList::CreateAnimation("TestAnimation", Vector2{ 8, 8 });

	//Create Objects Here
	//Actor::CreateActor("Cursor", 0, new Cursor());
}

void G_Engine::Update() {

	vector<Actor*> actorList = Actor::GetAllActors();
	for (int i = 0; i < actorList.size(); i++) {
		if (actorList[i]->GetOnDestroyList()) {
			Actor::RemoveActorFromLists(actorList[i]);
		}
		else if (actorList[i]->GetEnabled()) actorList[i]->Update(&scroll);
	}
}

void G_Engine::Draw() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawScreen(&scroll);
	EndDrawing();
}