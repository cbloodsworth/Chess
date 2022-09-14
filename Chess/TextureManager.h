#pragma once
#include "includes.h"

struct TM {

	// Variables
	static unordered_map<string, sf::Texture> textures;


	// Functions
	static void Load(string textureName);
	static sf::Texture& GetTexture(string textureName);
	static void Clear();

	// Constructor

};