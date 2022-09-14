#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>

using namespace std;

enum class Type {
	pawn, rook, knight, bishop, queen, king, empty
};

enum class Moveset {
	up, down, left, right, upleft, upright, downleft, downright, bpawn, wpawn, knight, king
};

struct Piece {
	Type type;
	char team; // 1 == black, 0 == white, -1 == null
	bool canMoveTo;
	bool threatening;
	int x, y;

	Piece();
	Piece(Type _type, char _team);
};

class Board {
	
public:

	map<vector<int>, Piece*> board;
	vector<Piece*> whitePieces;
	vector<Piece*> blackPieces;
	vector<Piece*> moveables;

	bool teamTurn;

	Piece* selection = nullptr;

	int xbKing, ybKing;
	int xwKing, ywKing;

	vector<int> previousPlacement;
	vector<int> currentPlacement;

	void InitializePieces();
	void UpdatePiecesPosition();
	void WhereToMove();
	void MovePiece(int _x, int _y);

	bool isKingInDanger(char kingteam);
	bool isCheckMate(char kingteam);

	void WTM(Moveset move); // experimental

	void Undo();
	
	void ClearWTM();
	
	// Constructor
	Board();
};

struct TM {

	static unordered_map<string, sf::Texture> textures;
	static void Load(string textureName);
	static sf::Texture& GetTexture(string textureName);
	static void Clear();

};
