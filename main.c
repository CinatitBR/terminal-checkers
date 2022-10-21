/*
    w - peça branca (white)
    W - dama branca
    b - peça preta (black)
    B - dama preta
    
    D - direita
    E - esquerda

    Para casa ser disponível: ambos números são impares, ou ambos são pares.

    - Peças brancas começam
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct board {
    int line_count;
    int col_count;
    char** squares;
} board;

typedef struct piece {
    char type;
    int line;
    int col;
    board* game_board;
} piece;

typedef struct square {
    int col;
    int line;
} square;

piece get_piece(char line_char, char col_char, board* game_board) {
    int line, col;

    switch (col_char) 
    {
        case 'a':
            col = 0;
            break;
        case 'b':
            col = 1;
            break;
        case 'c':
            col = 2;
            break;
        case 'd':
            col = 3;
            break;
        case 'e':
            col = 4;
            break;
        case 'f':
            col = 5;
            break;
    }

    switch (line_char) {
        case '1':
            line = 5;
            break;
        case '2':
            line = 4;
        break;
        case '3':
            line = 3;
        break;
        case '4':
            line = 2;
        break;
        case '5':
            line = 1;
        break;
        case '6':
            line = 0;
        break;
    }

    piece new_piece;
    new_piece.type = game_board->squares[line][col];
    new_piece.col = col;
    new_piece.line = line;
    new_piece.game_board = game_board;

    return new_piece;
}

board* make_board(int l, int c) {
    
    char* board_values = calloc(l*c, sizeof(char));
    char** lines = malloc(l * sizeof(char*));

    for (int i = 0; i < l; i++) {
        lines[i] = board_values + i*c;
    }

    // Preenche board com '-'
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            lines[i][j] = '-';
        }
    }

    board* game_board = malloc(sizeof(board));
    game_board->line_count = l;
    game_board->col_count = c;
    game_board->squares = lines;

    return game_board;
}

void destroy_board(board* game_board) {
    free(*game_board->squares); 
    free(game_board->squares); 
    free(game_board);
}

void print_board(board* game_board) {

    for (int i = 0; i < game_board->line_count; i++) {

        for (int j = 0; j < game_board->col_count; j++) {
            printf("%c", game_board->squares[i][j]);
        }

        printf("\n");
    }

    printf("\n\n");
}

// Preenche board com as peças brancas e pretas
void place_pieces(board* game_board) {

    // Coloca peças pretas
    for (int line = 0; line < 2; line++) {
        int col = (line % 2 == 0) ? 1 
                                  : 0;
        
        while (col <= 5) {
            game_board->squares[line][col] = 'b';
            col += 2;
        }
    }

    int line_count = game_board->line_count;

    // Coloca peças brancas
    for (int line = line_count-1; line > line_count-3; line--) {
        int col = (line % 2 == 0) ? 1 
                                  : 0;
        
        while (col <= 5) {
            game_board->squares[line][col] = 'w';
            col += 2;
        }
    }
}

// Move peça para target_square
void make_move(piece piece1, square target_square) 
{
    piece1.game_board->squares
        [piece1.line][piece1.col] = '-';

    piece1.game_board->squares
        [target_square.line][target_square.col] = piece1.type;
}

// Verifica se square existe no board.
// Retorna 1 para sim, 0 para não.
int square_exists(board* game_board, square square1) {

    // Linha ou coluna não existem
    if (square1.line < 0 || square1.col < 0)
        return 0;
    if (square1.line > game_board->line_count-1)
        return 0;
    if (square1.col > game_board->col_count-1)
        return 0;

    return 1;
}

// If next square exists, assign to next_square and return 1,
// otherwise, return 0.
int get_next_square(piece piece1, square* next_square,
    square start_square, char direction) 
{
    square square1;

    // Peça branca, próximo movimento diminui linha
    if (piece1.type == 'w')
        square1.line = start_square.line - 1;
    // Peça preta, próxima movimento aumenta linha
    else if (piece1.type == 'b') 
        square1.line = start_square.line + 1;

    // Próximo movimento vai para direita
    if (direction == 'D')
        square1.col = start_square.col + 1;
    // Próximo movimento vai para esquerda
    else if (direction == 'E')
        square1.col = start_square.col - 1;

    // Se square1 existe
    if ( square_exists(piece1.game_board, square1) ) {
        *next_square = square1;
        return 1;
    }
    else 
        return 0;
}

// Retorna 1 se movimento foi feito, e 0 caso contrário.
int move_piece(piece piece1, char direction) 
{
    square start_square;
    start_square.line = piece1.line;
    start_square.col = piece1.col;

    square next_square1;

    // Se square não existe
    if ( !get_next_square(piece1, 
        &next_square1, start_square, direction) ) 
    {
        return 0;
    }

    char square_type = piece1.game_board->squares
        [next_square1.line][next_square1.col];

    // Square está vazio, movimento é feito.
    if (square_type == '-') 
    {
        make_move(piece1, next_square1);
        return 1;
    }

    // Square tem peça aliada, movimento inválido
    else if (square_type == piece1.type)
        return 0;
    
    // Square tem peça inimiga
    else if (square_type != piece1.type) {
        // Pega próximo square, após next_square1
        square next_square2;

        if ( !get_next_square(piece1, 
            &next_square2, next_square1, direction) ) 
        {
            return 0;
        }

        // Se next_square2 está vazio
        if ( piece1.game_board->squares
            [next_square2.line][next_square2.col] == '-')
        {   
            // Come peça inimiga
            piece1.game_board->squares
                [next_square1.line][next_square1.col] = '-';
            
            // Move peça aliada
            make_move(piece1, next_square2);

            return 1;
        }

        // Square não está vazio, movimento inválido.
        else
            return 0;
    }
}

int main() {
    board* game_board = make_board(6, 6);

    place_pieces(game_board);
    print_board(game_board);

    while (1) {
        char line_char, col_char, direction;

        // Pega input do usuário
        scanf(" %c%c %c", 
            &col_char, &line_char, &direction);

        // Peça escolhida pelo usuário
        piece input_piece = get_piece(line_char, col_char, game_board);

        // Tenta mover peça
        if ( move_piece(input_piece, direction) )
            print_board(game_board);
        else
            printf("\n\nMOVIMENTO INVALIDO\n\n");
    }

    // Libera memória ocupada pela board
    destroy_board(game_board);
}
