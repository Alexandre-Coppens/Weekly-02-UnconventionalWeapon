#include <iostream>
#include <fstream>

#include "Terrain.h"

using std::ofstream;
using std::ifstream;
using std::getline;
using std::to_string;
using std::cout;

map<int, string> Terrain::dictionary;
vector<vector<Terrain::Tile>> Terrain::terrain;
Vector2 Terrain::tileSize{ 50,50 };
Vector2 Terrain::position{ 0,0 };
short Terrain::maxLayer{ 5 };
Terrain* Terrain::instance = nullptr;
//void Terrain::SaveMap(string filename);
//void Terrain::LoadMap(string filename);

Terrain::Terrain() {
}

Terrain::~Terrain(){
}

void Terrain::Update()
{
}

Terrain* Terrain::GetInstance() {
	if (instance == nullptr) {
		instance = new Terrain();
	}
	return instance;
}

void Terrain::AddNewTile(int layer,  int rotation, Vector2 pos, string name) {
	int index = CheckInDictionary(name);

	if (terrain.size() < maxLayer) {
		terrain.resize(maxLayer);
	}

	if (terrain[layer].size() == 0) {
		terrain[layer].push_back(Tile{ pos, (short)layer, rotation, index });
		return;
	}

	for (auto it = terrain[layer].begin(); it != terrain[layer].end();) {
		if (Vector2Equals(it->position, pos)) {
			terrain[layer].erase(it);
			break;
		}
		else it++;
	}

	terrain[layer].push_back(Tile{ pos, (short)layer, rotation, index });
}

void Terrain::RemoveTile(int layer,  Vector2 pos){
	if (terrain.size() < maxLayer) {
		return;
	}

	if (terrain[layer].size() == 0) {
		return;
	}

	for (auto it = terrain[layer].begin(); it != terrain[layer].end();) {
		if (Vector2Equals(it->position, pos)) {
			terrain[layer].erase(it);
			break;
		}
		else it++;
	}
}

void Terrain::AddToDictionary(int index, string name) {
	if (AssetList::GetTextureList()->find(name) == AssetList::GetTextureList()->end()) {
		dictionary[index] = "Unknown";
	}
	else {
		dictionary[index] = name;
	}
}

int Terrain::CheckInDictionary(string name) {
	for (auto d : dictionary) {
		if (name == d.second) {
			return d.first;
		}
	}
	int newInt = (int)dictionary.size();
	dictionary[newInt] = name;
	return newInt;
}

void Terrain::SaveMap(string fileName){
	bool checkIfSure = true;
	while (checkIfSure) {
		BeginDrawing();
		DrawText("Do you want to SAVE ?.", 150, 300, 20, RED);
		DrawText("A. Yes   -   B. No", 200, 330, 20, RED);
		EndDrawing();
		if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) checkIfSure = false;
		else if (IsKeyPressed(KEY_BACKSPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) return;
	}

	ofstream saveFile;
	saveFile.open(fileName + ".txt");
	if (saveFile.is_open()) {
		for (auto d : dictionary) {
			saveFile << "D$" + to_string(d.first) + ":" + d.second + "\n";
		}
		for (auto layer : terrain) {
			for (auto tile : layer) {
				saveFile << "T$"  + to_string(tile.position.x) + ":" + to_string(tile.position.y) + "$"
												+ to_string(tile.layer) + "$" + to_string(tile.rotation) + "$" + to_string(tile.dictionaryTexture) + "\n";
			}
		}
		saveFile.close();
	}
	else {
		cout << "Could not load " + fileName + ".txt ." + "\n";
	}
}

void Terrain::LoadMap(string fileName){
	bool checkIfSure = true;
	while (checkIfSure) {
		BeginDrawing();
		DrawText("Do you want to LOAD ?.", 150, 300, 20, RED);
		DrawText("A. Yes   -   B. No", 200, 330, 20, RED);
		EndDrawing();
		if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) checkIfSure = false;
		else if (IsKeyPressed(KEY_BACKSPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) return;
	}

	dictionary.clear();
	terrain.clear();

	string line;
	ifstream loadFile(fileName + ".txt");

	if (loadFile.is_open()) {
		while (getline(loadFile, line) ){
			if (line[0] == 'D') {
				vector<string> dict = BreakString(line, '$');
				dict = BreakString(dict[1], ':');
				AddToDictionary(stoi(dict[0]), dict[1]);
			}
			if (line[0] == 'T') {
				vector<string> tile = BreakString(line, '$');
				vector<string> posS = BreakString(tile[1], ':');
				Vector2 posV  { stof(posS[0]), stof(posS[1]) };
				AddNewTile(stoi(tile[2]), stoi(tile[3]), posV, dictionary[stoi(tile[4])]);
			}
		}
		loadFile.close();
	}
	else {
		cout << "Could not load " + fileName + ".txt ." + "\n";
	}
}

vector<string> Terrain::BreakString(string str, char breacker) {
	vector<string> r;
	string word;
	for(int i = 0; i < str.size(); i++)
	{
		if (str[i] == breacker) {
			r.push_back(word);
			word.clear();
		}
		else{
			word += str[i];
		}
	}
	r.push_back(word);
	return r;
}