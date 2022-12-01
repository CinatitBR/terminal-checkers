#ifndef BOARD_HPP
#define BOARD_HPP  

#include <cstdlib>
#include "BoardSquare.hpp"
#include "Piece.hpp"
#include "MovementCoords.hpp"

class Board {

public:
    char** squares;

    const int line_count;
    const int col_count;
    int white_piece_count;
    int black_piece_count;
    int white_invalid_move_count;
    int black_invalid_move_count;
    int no_piece_captured_count;

    Board(int line_count, int col_count);
    ~Board();

    void print();

    bool square_exists(BoardSquare square1);
    bool get_next_square(Piece piece1, BoardSquare* next_square, BoardSquare start_square, MovementCoords coords);

    bool move_piece(Piece piece1, MovementCoords coords);
    bool is_piece_ally(char type1, char type2);

private:
    void place_pieces();
    void make_move(Piece piece1, BoardSquare targetSquare);
};

#endif