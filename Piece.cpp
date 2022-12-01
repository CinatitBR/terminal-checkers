#include "Piece.hpp"
#include "Board.hpp" 

Piece::Piece(char type, int line, int col, Board* gameBoard) :
  type{type},
  line{line},
  col{col},
  gameBoard{gameBoard}
{}