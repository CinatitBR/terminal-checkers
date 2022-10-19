/*
    w - peça branca (white)
    b - peça preta (black)

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
    int line;
    int col;
} piece;

piece get_piece(char line_char, char col_char) {
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
    new_piece.col = col;
    new_piece.line = line;

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
void fill_board(board* game_board) {

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

// Move peça para (line1, col1)
void move_piece(board* game_board, 
    int line, int col, int line1, int col1) 
{
    char piece = game_board->squares[line][col];

    game_board->squares[line][col] = '-';
    game_board->squares[line1][col1] = piece;
}

int main() {
    board* game_board = make_board(6, 6);
    fill_board(game_board);

    while (1) {
        char line_char, col_char, direction;
        scanf("%c%c %c", &col_char, &line_char, &direction);

        piece new_piece = get_piece(line_char, col_char);
        printf("line: %d\n", new_piece.line);
        printf("col: %d\n", new_piece.col);
    }

    // Libera memória ocupada pela board
    destroy_board(game_board);
}