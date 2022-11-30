#ifndef BOARD_HPP
#define BOARD_HPP  

#include <cstdlib>

class Board {

public:
    Board(int line_count, int col_count);
    ~Board();

    void print();

    void set_white_piece_count(int white_piece_count);
    int get_white_piece_count();

    void set_black_piece_count(int black_piece_count);
    int get_black_piece_count();

    void set_white_invalid_move_count(int white_invalid_move_count);
    int get_white_invalid_move_count();

    void set_black_invalid_move_count(int black_invalid_move_count);
    int get_black_invalid_move_count();

    void set_no_piece_captured_count(int no_piece_captured_count);
    int get_no_piece_captured_count();

private:
    char** _squares;

    const int _line_count;
    const int _col_count;
    int _white_piece_count;
    int _black_piece_count;
    int _white_invalid_move_count;
    int _black_invalid_move_count;
    int _no_piece_captured_count;

    void place_pieces();
};

#endif