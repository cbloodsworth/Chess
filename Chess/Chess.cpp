#include "Chessboard.h"


#define HEIGHT 8
#define WIDTH 8

void DisplayBoard(sf::RenderWindow &window, Board &gameboard);
void initTextures();

int main()
{
    sf::RenderWindow window(sf::VideoMode(256, 256), "Chess");
    window.setSize(sf::Vector2u(1024, 1024));
    window.setPosition(sf::Vector2i(448, 0));
    window.clear();
    sf::Sprite boardsprite;
    sf::Sprite turnsprite;

    Board gameboard;
    gameboard.InitializePieces();
    initTextures();
    boardsprite.setTexture(TM::textures["chessboard"]);
    boardsprite.setPosition(0, 0);
   
    

    //=========== RENDER LOOP ===========//
    while (window.isOpen())
    {
        window.draw(boardsprite);
        boardsprite.setPosition(0, 0);

        if (gameboard.teamTurn == 1) { turnsprite.setTexture(TM::textures["b_turn"]); }
        else { turnsprite.setTexture(TM::textures["w_turn"]); }

        window.draw(turnsprite);
        DisplayBoard(window, gameboard);

        //=========== EVENT LOOP ===========//
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
            case (sf::Event::Closed): { window.close(); } break;
            case (sf::Event::KeyPressed): {
                if (event.key.code == sf::Keyboard::R) {
                    gameboard.InitializePieces();
                }
            } break;
            case (sf::Event::MouseButtonPressed): {
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                int mouse_x = localPosition.x / 128;
                int mouse_y = localPosition.y / 128;

                if (event.mouseButton.button == sf::Mouse::Right) {
                    for (int i = 0; i < 8; i++) {
                        for (int j = 0; j < 8; j++) {
                            if (gameboard.board[{i, j}]->threatening) {
                                gameboard.board[{i, j}]->threatening = false;
                            }
                        }
                    }

                    if (gameboard.board[{mouse_x, mouse_y}] == gameboard.selection) {
                        gameboard.selection = nullptr;
                        gameboard.ClearWTM();
                    }
                    else if ((gameboard.board[{mouse_x, mouse_y}]->type != Type::empty) && (gameboard.board[{mouse_x, mouse_y}]->team == gameboard.teamTurn)) {
                        gameboard.selection = gameboard.board[{mouse_x, mouse_y}];
                        gameboard.ClearWTM();
                        gameboard.WhereToMove();
                    }
                    else { 
                        gameboard.selection = nullptr;
                        gameboard.ClearWTM();
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (gameboard.selection != nullptr) {
                        gameboard.MovePiece(mouse_x, mouse_y);

                        gameboard.ClearWTM();
                        gameboard.selection = nullptr;

                    }

                    }
                    
            } break;
            }

            window.display();
        }
        
    }
    TM::Clear();
    return 0;
}

void DisplayBoard(sf::RenderWindow &window, Board &gameboard) {
    sf::Sprite sprite;

    if (gameboard.selection != nullptr) { 
        sprite.setTexture(TM::textures["selection"]);
        sprite.setPosition(gameboard.selection->x * 32, gameboard.selection->y * 32);
        window.draw(sprite);
    }

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (gameboard.board[{i, j}] == nullptr) { continue; }

            sprite.setPosition(i * 32, j * 32);

            switch (gameboard.board[{i, j}]->type) {
            case (Type::rook): { if (gameboard.board[{i, j}]->team) { sprite.setTexture(TM::textures["b_rook"]); } else { sprite.setTexture(TM::textures["w_rook"]); } break; } // Remember: 1 is black, 0 is white
            case (Type::bishop): { if (gameboard.board[{i, j}]->team) { sprite.setTexture(TM::textures["b_bishop"]); } else { sprite.setTexture(TM::textures["w_bishop"]); } break; }
            case (Type::knight): { if (gameboard.board[{i, j}]->team) { sprite.setTexture(TM::textures["b_knight"]); } else { sprite.setTexture(TM::textures["w_knight"]); } break; }
            case (Type::king): { if (gameboard.board[{i, j}]->team) { sprite.setTexture(TM::textures["b_king"]); } else { sprite.setTexture(TM::textures["w_king"]); } break; }
            case (Type::queen): { if (gameboard.board[{i, j}]->team) { sprite.setTexture(TM::textures["b_queen"]); } else { sprite.setTexture(TM::textures["w_queen"]); } break; }
            case (Type::pawn): { if (gameboard.board[{i, j}]->team) { sprite.setTexture(TM::textures["b_pawn"]); } else { sprite.setTexture(TM::textures["w_pawn"]); } break; }
            case (Type::empty): {sprite.setTexture(TM::textures["null"]); }
            }
            
            window.draw(sprite);

            if (gameboard.board[{i, j}]->canMoveTo) {
                sprite.setTexture(TM::textures["move"]);
                window.draw(sprite);
            }

            if (gameboard.board[{i, j}]->threatening) {
                sprite.setTexture(TM::textures["threatening"]);
                window.draw(sprite);
            }
        }
    }
}

void initTextures() {
    TM::textures["chessboard"] = TM::GetTexture("chessboard");
    TM::textures["null"] = TM::GetTexture("null");

    TM::textures["b_turn"] = TM::GetTexture("b_turn");
    TM::textures["w_turn"] = TM::GetTexture("w_turn");

    TM::textures["threatening"] = TM::GetTexture("danger");
    TM::textures["selection"] = TM::GetTexture("selection");
    TM::textures["move"] = TM::GetTexture("move");

    TM::textures["b_rook"] = TM::GetTexture("b_rook");
    TM::textures["b_knight"] = TM::GetTexture("b_knight");
    TM::textures["b_bishop"] = TM::GetTexture("b_bishop");
    TM::textures["b_queen"] = TM::GetTexture("b_queen");
    TM::textures["b_king"] = TM::GetTexture("b_king");
    TM::textures["b_pawn"] = TM::GetTexture("b_pawn");

    TM::textures["w_rook"] = TM::GetTexture("w_rook");
    TM::textures["w_knight"] = TM::GetTexture("w_knight");
    TM::textures["w_bishop"] = TM::GetTexture("w_bishop");
    TM::textures["w_queen"] = TM::GetTexture("w_queen");
    TM::textures["w_king"] = TM::GetTexture("w_king");
    TM::textures["w_pawn"] = TM::GetTexture("w_pawn");
}