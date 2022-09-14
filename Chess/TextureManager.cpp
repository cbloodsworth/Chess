#include "Chessboard.h"

unordered_map<string, sf::Texture> TM::textures;

sf::Texture& TM::GetTexture(string textureName) {
    if (textures.find(textureName) == textures.end()) { Load(textureName); }

    return textures[textureName];
}

void TM::Load(string fileName) {
    string loc = "images/";
    loc += fileName + ".png";

    textures[fileName].loadFromFile(loc);
}

void TM::Clear() {
    textures.clear();
}
