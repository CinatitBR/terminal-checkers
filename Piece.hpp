#ifndef PIECE_HPP
#define PIECE_HPP

#include "Board.hpp"

class Piece {

public:
  char type;
  int line;
  int col;
  Board* gameBoard;

  Piece(char type, int line, int col, Board* gameBoard);
}

#endif