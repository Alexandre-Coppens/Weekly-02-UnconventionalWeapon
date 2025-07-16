#pragma once

#include "raylib.h"

#include <string>
#include <vector>
#include "AssetList.h"
#include "Terrain.h"

using std::string;
using std::vector;

class G_Engine {
private:
	AssetList* assets{};
	Vector2 scroll{};
	Terrain* terrain{nullptr};

private:
	//Set this function in DrawScreen
	//void CheckIfOutOfBound();

public:
	G_Engine();
	~G_Engine();

	void Start();
	void Update();
	void Draw();
};
