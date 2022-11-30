#include "Board.hpp"
#include <iostream>
#include <cstdlib>

#define LINE_COUNT 8
#define COL_COUNT 8

#define WHITE_STONE 'o'
#define WHITE_DAME '0'

#define BLACK_STONE '#'
#define BLACK_DAME 'X'

#define RIGHT_DIRECTION 'D'
#define LEFT_DIRECTION 'E'
#define FRONT_DIRECTION 'F'
#define BACK_DIRECTION 'T'

Board::Board(int line_count, int col_count) : 
    _line_count{line_count},
    _col_count{col_count},
    _white_piece_count{0},
    _black_piece_count{0},
    _white_invalid_move_count{0},
    _black_invalid_move_count{0},
    _no_piece_captured_count{0}
{
    char* board_values = (char*) calloc(_line_count * _col_count, sizeof(char));
    char** lines = (char**) malloc(_line_count * sizeof(char*));

    for (int i = 0; i < _line_count; i++) {
        lines[i] = board_values + i * _col_count;
    }

    // Preenche board com '-'
    for (int i = 0; i < _line_count; i++) {
        for (int j = 0; j < _col_count; j++) {
            lines[i][j] = '-';
        }
    }

    _squares = lines;
}

Board::~Board() {
    free(*_squares); 
    free(_squares); 
}

void Board::print() {
    std::cout << "\n";

    for (int i = 0; i < _line_count; i++) {

        for (int j = 0; j < _col_count; j++) {
            std::cout << _squares[i][j];
        }

        std::cout << "\n";
    }

    std::cout << "\n";
}

void Board::set_white_piece_count(int white_piece_count) {
    _white_piece_count = white_piece_count;
}

int Board::get_white_piece_count() {
    return _white_piece_count;
}

void Board::set_black_piece_count(int black_piece_count) {
    _black_piece_count = black_piece_count;
}

int Board::get_black_piece_count() {
    return _black_piece_count;
}

void Board::set_white_invalid_move_count(int white_invalid_move_count) {
    _white_invalid_move_count = white_invalid_move_count;
}

int Board::get_white_invalid_move_count() {
    return _white_invalid_move_count;
}

void Board::set_black_invalid_move_count(int black_invalid_move_count) {
    _black_invalid_move_count = black_invalid_move_count;
}

int Board::get_black_invalid_move_count() {
    return _black_invalid_move_count;
}

void Board::set_no_piece_captured_count(int no_piece_captured_count) {
    _no_piece_captured_count = no_piece_captured_count;
}

int Board::get_no_piece_captured_count() {
    return _no_piece_captured_count;
}

void Board::place_pieces() {
    // Coloca peças pretas
    for (int line = 0; line < 3; line++) {
        int col = (line % 2 == 0) ? 1 : 0;
        
        while (col < _col_count) {
            _squares[line][col] = BLACK_STONE;
            col += 2;
        }
    }

    int player_piece_count = 0;

    // Coloca peças brancas
    for (int line = _line_count-1; line > _line_count-4; line--) {
        int col = (line % 2 == 0) ? 1 : 0;
        
        while (col < _col_count) {
            _squares[line][col] = WHITE_STONE;
            col += 2;

            player_piece_count++;
        }
    }

    _white_piece_count = player_piece_count;
    _black_piece_count = player_piece_count;
}
