#include "Piece.hpp"
#include "Board.hpp" 

Piece::Piece(char type, int line, int col, Board* gameBoard) :
  type{type},
  line{line},
  col{col},
  gameBoard{gameBoard}
{}

// Verifica se peça é do jogador.
static bool Piece::is_from_player(Piece piece1, enum Players player) {

  // Peça é do player branco
  if (piece1.type == WHITE_STONE
      || piece1.type == WHITE_DAME) {
      
      if (player != WHITE_PLAYER)
        return false;

  }

  // Peça é do player preto
  else if (piece1.type == BLACK_STONE
      || piece1.type == BLACK_DAME) {
      
      if (player != BLACK_PLAYER)
        return false;

  }

  return true;
}