#pragma once

#include <string>
#include <vector>
#include <map>
#include "Raylib.h"
#include "Animation.h"

using std::string;
using std::vector;
using std::map;

class AssetList {
private:
    static Texture2D textureTemp;

    static map<Texture2D*, Animation> animationList;
    static map<string, Music> musicList;
    static map<string, Sound> soundList;
    static map<string, Font> textFont;
    static map<string, Texture2D> textureList;

    static AssetList* instance;

public:
    static Animation* GetAnimation(Texture2D* texture) { return &animationList[texture]; }
    static Music* GetMusic(string name) { return &musicList[name]; }
    static Sound* GetSound(string name) { return &soundList[name]; }
    static Font* GetFont(string name) { return &textFont[name]; }
    static Texture2D* GetTexture(string name) { return &textureList[name]; }

    static map<string, Texture2D>* GetTextureList() { return &textureList; }

private:
    AssetList();
    ~AssetList();
    void LoadRessources();

    void LoadFontFolder(string path);
    void LoadSoundFolder(string path);
    void LoadSpriteFolder(string path);

    void LoadTexture2D(string name, string link);

public:
    static string GetNameAtPosition(int position);
    static AssetList* GetInstance();
    static void CreateAnimation(string textureName, Vector2 frameSize);
};