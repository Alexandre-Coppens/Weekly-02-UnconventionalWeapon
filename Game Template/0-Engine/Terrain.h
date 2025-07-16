#pragma once

#include <string>
#include <vector>
#include <map>
#include "Raylib.h"
#include "Actor.h"

using std::string;
using std::vector;
using std::map;

class Terrain {
public:
    struct Tile {
        Vector2 position{};
        short layer{};
        int rotation{};
        int dictionaryTexture{};
    };

private:
    static map<int, string> dictionary;
    static vector<vector<Tile>> terrain;
    static Vector2 tileSize;
    static Vector2 position;
    static short maxLayer;

    static Terrain* instance;

public:
    //Get the texture name via it's indice in the texture dictionary
    static string* GetTextureName(int indice) { return &dictionary[indice]; }
    static vector<vector<Tile>>* GetTerrain() { return &terrain; }
    static Vector2 GetPosition() { return position; }
    static short GetMaxLayer() { return maxLayer; }

    static Vector2 GetTileSize() { return tileSize; }
    static void SetTileSize(Vector2 newTileSize) { tileSize = newTileSize; }

private:
public:
    Terrain();
    ~Terrain();

    void Update();

    static Terrain* GetInstance();

    static void AddNewTile(int layer, int rotation, Vector2 pos, string name);
    static void RemoveTile(int layer, Vector2 pos);
    static void AddToDictionary(int index, string name);
    static int CheckInDictionary(string n);

    static void SaveMap(string fileName);
    static void LoadMap(string fileName);
    static vector<string> BreakString(string str, char breacker);
};