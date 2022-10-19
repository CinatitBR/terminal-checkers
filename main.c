/*
    w - peça branca (white)
    b - peça preta (black)

    Para casa ser disponível: ambos números são impares, ou ambos são pares.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Board {
    int line_count;
    int col_count;
    char** squares;
} board;

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

}

// // Preenche board com peças
// void fill_board(char** board) {
//     // Coloca peças pretas
//     for (int line = 5; line > 3; line--) {
//         int col = (line % 2 == 0) ? 0 
//                                   : 1;
        
//         while (col <= 5) {
//             board[line][col] = 'b';
//             col += 2;
//         }
//     }
// }

int main() {
    board* game_board = make_board(6, 6);

    print_board(game_board);

    // Libera memória ocupada pela board
    destroy_board(game_board);
}