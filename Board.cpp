#include <iostream>
#include <cstdlib>

#include "Board.hpp"
#include "BoardSquare.hpp"
#include "Piece.hpp"
#include "MovementCoords.hpp"

Board::Board(int line_count, int col_count) : 
    line_count{line_count},
    col_count{col_count},
    white_piece_count{0},
    black_piece_count{0},
    white_invalid_move_count{0},
    black_invalid_move_count{0},
    no_piece_captured_count{0}
{
    char* board_values = (char*) calloc(line_count * col_count, sizeof(char));
    char** lines = (char**) malloc(line_count * sizeof(char*));

    for (int i = 0; i < line_count; i++) {
        lines[i] = board_values + i * col_count;
    }

    // Preenche board com '-'
    for (int i = 0; i < line_count; i++) {
        for (int j = 0; j < col_count; j++) {
            lines[i][j] = '-';
        }
    }

    _squares = lines;

    place_pieces();
}

Board::~Board() {
    free(*_squares); 
    free(_squares); 
}

void Board::print() {
    std::cout << "\n";

    for (int i = 0; i < line_count; i++) {

        for (int j = 0; j < col_count; j++) {
            std::cout << _squares[i][j];
        }

        std::cout << "\n";
    }

    std::cout << "\n";
}

bool Board::square_exists(BoardSquare square1) {
    // Linha ou coluna não existem
    if (square1.line < 0 || square1.col < 0)
        return false;

    else if (square1.line > line_count-1)
        return false;

    else if (square1.col > col_count-1)
        return false;

    return true;
}

bool Board::get_next_square(
    Piece piece1, 
    BoardSquare* next_square, 
    BoardSquare start_square, 
    MovementCoords coords
) 
{
    BoardSquare square1;

    // --- DIREÇÃO VERTICAL ---

    // Peça branca.
    // Para frente, diminui linha.
    // Para trás, aumenta linha.
    if (piece1.type == WHITE_STONE || piece1.type == WHITE_DAME) {
        // square1.line = start_square.line + (-1 * vertical_direction_factor);

        if (coords.verticalDirection == FRONT_DIRECTION)
            square1.line = start_square.line - 1;
        else 
            square1.line = start_square.line + 1;

    }
    
    // Peça preta.
    // Para frente, aumenta linha.
    // Para trás, diminui linha.
    else if (piece1.type == BLACK_STONE || piece1.type == BLACK_DAME)  {

        if (coords.verticalDirection == FRONT_DIRECTION)
            square1.line = start_square.line + 1;
        else 
            square1.line = start_square.line - 1;

    }

    // --- DIREÇÃO HORIZONTAL ---

    //  Movimento vai para direita, aumenta coluna.
    if (coords.horizontalDirection == RIGHT_DIRECTION)
        square1.col = start_square.col + 1;

    // Movimento vai para esquerda, diminui coluna.
    else if (coords.horizontalDirection == LEFT_DIRECTION)
        square1.col = start_square.col - 1;

    // Se square1 existe
    if ( square_exists(square1) ) {
        *next_square = square1;

        return true;
    }
    else 
        return false;
}

bool Board::move_piece(Piece piece1, MovementCoords coords) {
    BoardSquare startSquare(piece1.line, piece1.col);

    BoardSquare nextSquare1;

    // Se square não existe.
    if ( !get_next_square(piece1, 
        &next_square1, start_square, coords) ) 
    {
        return false;
    }

    char square_type = piece1.game_board->squares
        [next_square1.line][next_square1.col];

    // Square está vazio, movimento é feito.
    // Nenhuma peça inimiga capturada.
    if (square_type == '-') 
    {
        make_move(piece1, next_square1);

        // Nenhuma peça foi capturada. Incrementa valor.
        piece1.game_board->no_piece_captured_count += 1;

        return true;
    }
    else {
        piece1.game_board->no_piece_captured_count = 0;
    }

    // Square tem peça aliada, movimento inválido. 
    if ( is_piece_ally(piece1.type, square_type) )
        return false;
    
    // Pega próximo square, após next_square1
    square next_square2;

    // Square não existe.
    if ( !get_next_square(piece1, 
        &next_square2, next_square1, coords) ) 
    {
        return false;
    }

    // Se next_square2 está vazio
    if ( piece1.game_board->squares
        [next_square2.line][next_square2.col] == '-')
    {   
        // --- Come peça inimiga ---
        piece1.game_board->squares
            [next_square1.line][next_square1.col] = '-';

        if (piece1.type == WHITE_STONE || piece1.type == WHITE_DAME)
            piece1.game_board->black_piece_count -= 1;
        else if (piece1.type == BLACK_STONE || piece1.type == BLACK_DAME)
            piece1.game_board->white_piece_count -= 1;
        
        // Move peça aliada
        make_move(piece1, next_square2);

        return true;
    }

    // Square não está vazio, movimento inválido.
    else
        return false;
}

bool Board::is_piece_ally(char type1, char type2) {
    
    if ( type1 == WHITE_STONE 
        || type1 == WHITE_DAME) 
    {

        if ( type2 == WHITE_STONE ||
            type2 == WHITE_DAME) {
            return true;
        }   

    }

    else if ( type1 == BLACK_STONE 
        || type1 == BLACK_DAME) 
    {

        if ( type2 == BLACK_STONE ||
            type2 == BLACK_DAME) {
            return true;
        }   

    }

    return false;
}

void Board::place_pieces() {
    // Coloca peças pretas
    for (int line = 0; line < 3; line++) {
        int col = (line % 2 == 0) ? 1 : 0;
        
        while (col < col_count) {
            _squares[line][col] = BLACK_STONE;
            col += 2;
        }
    }

    int player_piece_count = 0;

    // Coloca peças brancas
    for (int line = line_count-1; line > line_count-4; line--) {
        int col = (line % 2 == 0) ? 1 : 0;
        
        while (col < col_count) {
            _squares[line][col] = WHITE_STONE;
            col += 2;

            player_piece_count++;
        }
    }

    white_piece_count = player_piece_count;
    black_piece_count = player_piece_count;
}

void Board::make_move(Piece piece1, BoardSquare targetSquare) {
    piece1.gameBoard->_squares
        [piece1.line][piece1.col] = '-';

    // Come peça inimiga e vira Dama branca
    if (targetSquare.line == 0) { 
        piece1.gameBoard->_squares
            [targetSquare.line][targetSquare.col] = WHITE_DAME;   
    }

    // Come peça inimiga e vira Dama preta
    else if (targetSquare.line == 5) { 
        piece1.gameBoard->_squares
            [targetSquare.line][targetSquare.col] = BLACK_DAME;   
    }

    // Come peça inimiga
    else {
        piece1.gameBoard->_squares
            [targetSquare.line][targetSquare.col] = piece1.type;
    }
}
