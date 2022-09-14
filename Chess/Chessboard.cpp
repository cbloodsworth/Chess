#include "Chessboard.h"

Piece::Piece(Type _type, char _team) {
	type = _type;
	team = _team;
	canMoveTo = false;
	threatening = false;
}

void Board::InitializePieces() {

	teamTurn = 0;
	board.clear();
	blackPieces.clear();
	whitePieces.clear();

	board[{0, 0}] = new Piece(Type::rook,   1);
	board[{1, 0}] = new Piece(Type::knight, 1);
	board[{2, 0}] = new Piece(Type::bishop, 1);
	board[{3, 0}] = new Piece(Type::queen,  1);
	board[{4, 0}] = new Piece(Type::king,   1);
	xbKing = 4;
	ybKing = 0;
	board[{5, 0}] = new Piece(Type::bishop, 1);
	board[{6, 0}] = new Piece(Type::knight, 1);
	board[{7, 0}] = new Piece(Type::rook,   1);

	board[{0, 1}] = new Piece(Type::pawn, 1);
	board[{1, 1}] = new Piece(Type::pawn, 1);
	board[{2, 1}] = new Piece(Type::pawn, 1);
	board[{3, 1}] = new Piece(Type::pawn, 1);
	board[{4, 1}] = new Piece(Type::pawn, 1);
	board[{5, 1}] = new Piece(Type::pawn, 1);
	board[{6, 1}] = new Piece(Type::pawn, 1);
	board[{7, 1}] = new Piece(Type::pawn, 1);

	board[{0, 6}] = new Piece(Type::pawn, 0);
	board[{1, 6}] = new Piece(Type::pawn, 0);
	board[{2, 6}] = new Piece(Type::pawn, 0);
	board[{3, 6}] = new Piece(Type::pawn, 0);
	board[{4, 6}] = new Piece(Type::pawn, 0);
	board[{5, 6}] = new Piece(Type::pawn, 0);
	board[{6, 6}] = new Piece(Type::pawn, 0);
	board[{7, 6}] = new Piece(Type::pawn, 0);

	board[{0, 7}] = new Piece(Type::rook,   0);
	board[{1, 7}] = new Piece(Type::knight, 0);
	board[{2, 7}] = new Piece(Type::bishop, 0);
	board[{3, 7}] = new Piece(Type::queen,  0);
	board[{4, 7}] = new Piece(Type::king,   0);
	xwKing = 4;
	ywKing = 7;
	board[{5, 7}] = new Piece(Type::bishop, 0);
	board[{6, 7}] = new Piece(Type::knight, 0);
	board[{7, 7}] = new Piece(Type::rook,   0);

	for (int i = 0; i < 8; i++) {
		for (int j = 2; j < 6; j++) {
			board[{i, j}] = new Piece(Type::empty, -1);
		}
	}

	UpdatePiecesPosition();
}

void Board::UpdatePiecesPosition() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
					board[{i, j}]->x = i;
					board[{i, j}]->y = j;
					if (board[{i, j}]->team == 1) {
						blackPieces.push_back(board[{i, j}]);
					}
					else if (board[{i, j}]->team == 0) {
						whitePieces.push_back(board[{i, j}]);
					}
		}
	}
}

void Board::WhereToMove() {
	if (selection != nullptr) {
		switch (selection->type) {
			case (Type::rook): {
				WTM(Moveset::left);
				WTM(Moveset::up);
				WTM(Moveset::down);
				WTM(Moveset::right);
			} break;
			case (Type::knight): {
				WTM(Moveset::knight);
			} break;
			case (Type::bishop): {
				WTM(Moveset::upleft);
				WTM(Moveset::upright);
				WTM(Moveset::downleft);
				WTM(Moveset::downright);
			} break;
			case (Type::king): {
				WTM(Moveset::king);
			} break;
			case (Type::queen): {
				WTM(Moveset::left);
				WTM(Moveset::up);
				WTM(Moveset::down);
				WTM(Moveset::right);
				WTM(Moveset::upleft);
				WTM(Moveset::upright);
				WTM(Moveset::downleft);
				WTM(Moveset::downright);
			} break;
			case (Type::pawn): {
				if (selection->team) { WTM(Moveset::bpawn); }
				else { WTM(Moveset::wpawn); }

			} break;
		}
	}
}

void Board::WTM(Moveset move) {
#define DIRECTION {selection->x + i * xmult, selection->y + i * ymult}
#define THIS {selection->x, selection->y}

	int xmult, ymult;
	xmult = ymult = 0;

	bool rqb = false; //Rook, queen or bishop (moving in a straight line)
	bool isPawn = false; //Pawn moveset
	bool isKnight = false;
	bool isKing = false;

	switch (move) {
	case (Moveset::up): { ymult = -1; rqb = true; break; }
	case (Moveset::down): { ymult = 1; rqb = true; break; }
	case (Moveset::left): { xmult = -1; rqb = true; break; }
	case (Moveset::right): { xmult = 1; rqb = true; break; }
	case (Moveset::upleft): { ymult = -1; xmult = -1; rqb = true; break; }
	case (Moveset::upright): { ymult = -1; xmult = 1; rqb = true; break; }
	case (Moveset::downleft): { ymult = 1; xmult = -1; rqb = true; break; }
	case (Moveset::downright): { ymult = 1; xmult = 1; rqb = true; break; }

	case (Moveset::bpawn): {ymult = 1; isPawn = true; break; }
	case (Moveset::wpawn): {ymult = -1; isPawn = true; break; }

	case (Moveset::knight): { isKnight = true; break; }

	case (Moveset::king): { isKing = true; break; }
	}

	int i = 1;
	//BELOW IS FOR ROOKS, BISHOPS, or QUEENS.
	while (i != -1 && rqb) {
		if (board[DIRECTION] != nullptr) {
			if (board[DIRECTION]->type == Type::empty) { //if empty
				board[DIRECTION]->canMoveTo = true;
				moveables.push_back(board[DIRECTION]);
				i++; // continue, this square is empty
			}
			else if (board[DIRECTION]->team == board[THIS]->team) {
				break; // stop here, we've found a teammate
			}
			else {
				board[DIRECTION]->canMoveTo = true;
				moveables.push_back(board[DIRECTION]);
				i = -1; //stop here, we can take a piece
			}
		}
		else { break; }
	}

	//BELOW IS FOR PAWNS.
	if (isPawn) {
		int start = 1;
		if (selection->team == 0) { start = 6; }

		if (board[{selection->x, selection->y + ymult}] != nullptr && board[{selection->x, selection->y + ymult}]->type == Type::empty) {
			board[{selection->x, selection->y + ymult}]->canMoveTo = true;
			moveables.push_back(board[{selection->x, selection->y + ymult}]);
			if (selection->y == start && board[{selection->x, selection->y + ymult * 2}]->type == Type::empty) {
				board[{selection->x, selection->y + ymult * 2}]->canMoveTo = true;
				moveables.push_back(board[{selection->x, selection->y + ymult * 2}]);
				// include en passant
			}
		}

		if (board[{selection->x - 1, selection->y + ymult}] != nullptr && board[{selection->x - 1, selection->y + ymult}]->type != Type::empty && board[{selection->x - 1, selection->y + ymult}]->team != selection->team) {
			board[{selection->x - 1, selection->y + ymult}]->canMoveTo = true;
			moveables.push_back(board[{selection->x - 1, selection->y + ymult}]);
		}
		if (board[{selection->x + 1, selection->y + ymult}] != nullptr && board[{selection->x + 1, selection->y + ymult}]->type != Type::empty && board[{selection->x + 1, selection->y + ymult}]->team != selection->team) {
			board[{selection->x + 1, selection->y + ymult}]->canMoveTo = true;
			moveables.push_back(board[{selection->x + 1, selection->y + ymult}]);
		}
	}

	//BELOW IS FOR KNIGHTS.
	if (isKnight) {
		for (int xmove = -2; xmove <= 2; xmove++) {
			for (int ymove = -2; ymove <= 2; ymove++) {
				if (xmove == 0 || ymove == 0 || xmove == ymove || xmove == -ymove) { continue; }
				if (board[{selection->x + xmove, selection->y + ymove}] == nullptr ||
					board[{selection->x + xmove, selection->y + ymove}]->team == selection->team) { continue; }
				board[{selection->x + xmove, selection->y + ymove}]->canMoveTo = true;
				moveables.push_back(board[{selection->x + xmove, selection->y + ymove}]);
			}
		}
	}

	// BELOW IS FOR KINGS
	if (isKing) {
		for (int xmove = -1; xmove <= 1; xmove++) {
			for (int ymove = -1; ymove <= 1; ymove++) {
				if (board[{selection->x + xmove, selection->y + ymove}] == nullptr ||
					board[{selection->x + xmove, selection->y + ymove}]->team == selection->team) {
					continue;
				}
				board[{selection->x + xmove, selection->y + ymove}]->canMoveTo = true;
				moveables.push_back(board[{selection->x + xmove, selection->y + ymove}]);
			}
		}
	}

}

void Board::ClearWTM() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[{i, j}] != nullptr) { 
				board[{i, j}]->canMoveTo = false; 
				
			}
		}
	}
	moveables.clear();
}

void Board::MovePiece(int _x, int _y) {
	Piece* pieceToBringBack = new Piece(board[{_x, _y}]->type, board[{_x, _y}]->team);
	int tempx, tempy;

	if (board[{_x, _y}]->canMoveTo) {
		previousPlacement = { selection->x, selection->y };
		currentPlacement = { _x, _y };

		delete board[{_x, _y}];
		board[{_x, _y}] = selection;

		board[{selection->x, selection->y}] = new Piece(Type::empty, -1);
		board[{selection->x, selection->y}]->x = selection->x;
		board[{selection->x, selection->y}]->y = selection->y;

		tempx = selection->x;
		tempy = selection->y;

		selection->x = _x;
		selection->y = _y;

		if (board[{_x, _y}]->type == Type::king) { // update king location
			if (board[{_x, _y}]->team == 1) {
				xbKing = _x;
				ybKing = _y;
			}
			else if (board[{_x, _y}]->team == 0) {
				xwKing = _x;
				ywKing = _y;
			}
		}

		if (isKingInDanger(teamTurn)) {
			delete board[{tempx, tempy}];
			board[{tempx, tempy}] = selection;
			selection->x = tempx;
			selection->y = tempy;

			board[{_x, _y}] = pieceToBringBack;

			if (board[{tempx, tempy}]->type == Type::king) { // update king location
				if (board[{tempx, tempy}]->team == 1) {
					xbKing = tempx;
					ybKing = tempy;
				}
				else if (board[{tempx, tempy}]->team == 0) {
					xwKing = tempx;
					ywKing = tempy;
				}
			}
		}
		else { delete pieceToBringBack; teamTurn = !teamTurn; }
	}

	if (board[{_x, _y}]->type == Type::pawn) {
		if (_y == 7 || _y == 0) { board[{_x, _y}]->type = Type::queen; }
	}
}

void Board::Undo() {
	Piece* tempsel = selection;
	selection = board[currentPlacement];
	MovePiece(previousPlacement[0], previousPlacement[1]);
}

bool Board::isKingInDanger(char kingteam) {
	Piece* tempsel = selection;
	bool inDanger = false;
	if (kingteam == 1) {
		for (int i = 0; i < whitePieces.size(); i++) {
			selection = whitePieces[i];
			WhereToMove();
			for (int j = 0; j < moveables.size(); j++) {
				if (board[{moveables[j]->x, moveables[j]->y}] == board[{xbKing, ybKing}]) {
					selection = tempsel;
					inDanger = true;
					board[{xbKing, ybKing}]->threatening = true;
				}
			}
			
		}
	}
	else {
		for (int i = 0; i < blackPieces.size(); i++) {
			selection = blackPieces[i];
			WhereToMove();
			for (int j = 0; j < moveables.size(); j++) {
				if (moveables[j] == board[{xwKing, ywKing}]) {
					selection = tempsel;
					inDanger = true;
					board[{moveables[j]->x, moveables[j]->y}]->threatening = true;
				}
			}

		}
	}
	if (!inDanger) {
		selection = tempsel;
	}
	
	return inDanger;
}

//bool Board::isCheckMate(char kingteam) {
//	if ((bool)kingteam) { //checking if black king lost
//		vector<Piece*>& pieces = blackPieces;
//	} else { vector<Piece*>& pieces = whitePieces; } //checking if white king lost
//
//
//}

Board::Board() {

}