#include <iostream>
#include <filesystem>
#include <cassert>
#include "AssetList.h"

Texture2D AssetList::textureTemp;
map<string, Music> AssetList::musicList;
AssetList* AssetList::instance = nullptr;
map<string, Font> AssetList::textFont;
map<string, Texture2D> AssetList:: textureList;
map<string, Sound> AssetList:: soundList;
map<Texture2D*, Animation> AssetList:: animationList;

AssetList::AssetList() {
    if (textureList.size() > 0) return;
    LoadRessources();
}

AssetList::~AssetList(){
}

void AssetList::LoadRessources(){
    string path = "resources";
    const std::filesystem::path resourcePath{ path };

    for (const auto& entry : std::filesystem::directory_iterator(resourcePath)) {

        const auto filenameStr = entry.path().filename().string();

        if (entry.is_directory()) {
            std::cout << "dir:  " << filenameStr << '\n';
            if (filenameStr == "fonts") LoadFontFolder(path + "/" + filenameStr);
            if (filenameStr == "sounds") LoadSoundFolder(path + "/" + filenameStr);
            if (filenameStr == "sprites") LoadSpriteFolder(path + "/" + filenameStr);
        }
        else if (entry.is_regular_file()) {
            std::cout << "file: " << filenameStr << '\n';
        }
    }
}

void AssetList::LoadFontFolder(string path) {
    const std::filesystem::path resourcePath{ path };

    for (const auto& entry : std::filesystem::directory_iterator(resourcePath)) {
        if (!entry.is_regular_file()) {
            std::cout << "??    " << entry.path().filename().string() << '\n';
            continue;
        }

        const auto filenameStr = entry.path().filename().string();
        const auto fileStemStr = entry.path().stem().string();
        const auto fileExtensionStr = entry.path().extension().string();

        std::cout << "      -file: " << filenameStr << '\n';
        if (fileExtensionStr == ".png") {
            AssetList::textFont[fileStemStr] = LoadFont((path + "/" + filenameStr).c_str());
        }
        else {
            std::cout << "ResourceLoader: File skipped, other files than png are not supported.          " << '\n';
        }
    }
}

void AssetList::LoadSoundFolder(string path) {
    const std::filesystem::path resourcePath{ path };

    for (const auto& entry : std::filesystem::directory_iterator(resourcePath)) {
        if (!entry.is_regular_file()) {
            std::cout << "??    " << entry.path().filename().string() << '\n';
            continue;
        }

        const auto filenameStr = entry.path().filename().string();
        const auto fileStemStr = entry.path().stem().string();
        const auto fileExtensionStr = entry.path().extension().string();

        std::cout << "      -file: " << filenameStr << '\n';
        if (fileExtensionStr == ".wav" || fileExtensionStr == ".mp3") {
            AssetList::soundList[fileStemStr] = LoadSound((path + "/" + filenameStr).c_str());
        }
        else {
            std::cout << "ResourceLoader: File skipped, other files than wav or mp3 are not supported.          " << '\n';
        }
    }
}

void AssetList::LoadSpriteFolder(string path) {
    const std::filesystem::path resourcePath{ path };

    for (const auto& entry : std::filesystem::directory_iterator(resourcePath)) {
        if (!entry.is_regular_file()) {
            std::cout << "??    " << entry.path().filename().string() << '\n';
            continue;
        }

        const auto filenameStr = entry.path().filename().string();
        const auto fileStemStr = entry.path().stem().string();
        const auto fileExtensionStr = entry.path().extension().string();

        std::cout << "      -file: " << filenameStr << '\n';
        if (fileExtensionStr == ".png") {
            LoadTexture2D(fileStemStr, path + "/" + filenameStr);
        }
        else {
            std::cout << "ResourceLoader: File skipped, other files than png are not supported.          " << '\n';
        }
    }
}

string AssetList::GetNameAtPosition(int position) {
    int i = 0;
    for (auto s : textureList) {
        if (i == position) return s.first;
        i++;
    }
    return "Unknown";
}

void AssetList::LoadTexture2D(string name, string link) {
    Image temp = LoadImage(link.c_str());
    textureTemp = LoadTextureFromImage(temp);
    textureList[name] = textureTemp;
    UnloadImage(temp);
}

AssetList* AssetList::GetInstance() {
    if (instance == nullptr) {
        instance = new AssetList();
    }
    return instance;
}

void AssetList::CreateAnimation(string textureName, Vector2 frameSize){
    std::cout << "    -Creating animation table for " + textureName + "\n";

    auto it = textureList.find(textureName);
    if (it == textureList.end()) {
        std::cout << "    -WARNING: Texture not found / Check texture name.. \n";
        assert(false);
        return;
    }

    Texture2D* texture = &textureList[textureName];
    Image image = LoadImageFromTexture(*texture);

    Sequence temp;
    Animation animation;
    animation.length = texture->height / frameSize.y;
    animation.size = frameSize;

    for (int i = 0; i < animation.length; i++) {
        for (int j = 0; j < (int)texture->width / frameSize.x; j++) {
            Color pixel = GetImageColor(image, j * frameSize.x, i * frameSize.y);
            if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255 || (j + 1) * frameSize.x >= texture->width){
                temp.length = pixel.r == 255 && pixel.g == 0 && pixel.b == 255 ? j -1  : j;
                animation.sequences.push_back(temp);
                std::cout << "    -Created animation n°" << animation.sequences.size() << " - Length: " << std::to_string(temp.length) << "\n";
                break;
            }
        }
    }

    UnloadImage(image);

    animationList[texture] = animation;
}
