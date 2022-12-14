/*
    w - peça branca (white)
    W - dama branca
    b - peça preta (black)
    B - dama preta
    
    D - direita
    E - esquerda

    Para casa ser disponível: ambos números são impares, ou ambos são pares.

    - Peças brancas começam
    - Checar se peça virou dama *
    - Adicionar movimento frente e trás
    - Adicionar pontuação

    - Fix get_next_square for dames
    - Adicionar cheque de peça dentro de get_piece.
*/

#include <stdio.h>
#include <stdlib.h>

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

enum Players {
    WHITE_PLAYER,
    BLACK_PLAYER
};

typedef struct board {
    int line_count;
    int col_count;
    int white_piece_count;
    int black_piece_count;
    int white_invalid_move_count;
    int black_invalid_move_count;
    int no_piece_captured_count;
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

typedef struct move_coords {
    char horizontal_direction; // Direita ou esquerda
    char vertical_direction; // Frente ou trás
} move_coords;

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
    game_board->white_invalid_move_count = 0;
    game_board->black_invalid_move_count = 0;
    game_board->no_piece_captured_count = 0;

    return game_board;
}

void destroy_board(board* game_board) {
    free(*game_board->squares); 
    free(game_board->squares); 
    free(game_board);
}

void print_board(board* game_board) {
    printf("\n");

    for (int i = 0; i < game_board->line_count; i++) {

        for (int j = 0; j < game_board->col_count; j++) {
            printf("%c", game_board->squares[i][j]);
        }

        printf("\n");
    }

    printf("\n");
}

// Preenche board com as peças brancas e pretas.
void place_pieces(board* game_board) {

    // Coloca peças pretas
    for (int line = 0; line < 3; line++) {
        int col = (line % 2 == 0) ? 1 
                                  : 0;
        
        while (col < COL_COUNT) {
            game_board->squares[line][col] = BLACK_STONE;
            col += 2;
        }
    }

    int player_piece_count = 0;

    // Coloca peças brancas
    for (int line = LINE_COUNT-1; line > LINE_COUNT-4; line--) {
        int col = (line % 2 == 0) ? 1 
                                  : 0;
        
        while (col < COL_COUNT) {
            game_board->squares[line][col] = WHITE_STONE;
            col += 2;

            player_piece_count++;
        }
    }

    game_board->white_piece_count = player_piece_count;
    game_board->black_piece_count = player_piece_count;
}

// Move peça para target_square
void make_move(piece piece1, square target_square) 
{
    piece1.game_board->squares
        [piece1.line][piece1.col] = '-';

    // Dama branca
    if (target_square.line == 0) { 
        piece1.game_board->squares
            [target_square.line][target_square.col] = WHITE_DAME;   
    }
    // Dama preta
    else if (target_square.line == 5) { 
        piece1.game_board->squares
            [target_square.line][target_square.col] = BLACK_DAME;   
    }
    else {
        piece1.game_board->squares
            [target_square.line][target_square.col] = piece1.type;
    }
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
    square start_square, move_coords coords) 
{
    square square1;

    // --- DIREÇÃO VERTICAL ---

    // Peça branca.
    // Para frente, diminui linha.
    // Para trás, aumenta linha.
    if (piece1.type == WHITE_STONE || piece1.type == WHITE_DAME) {
        // square1.line = start_square.line + (-1 * vertical_direction_factor);

        if (coords.vertical_direction == FRONT_DIRECTION)
            square1.line = start_square.line - 1;
        else 
            square1.line = start_square.line + 1;

    }
    
    // Peça preta.
    // Para frente, aumenta linha.
    // Para trás, diminui linha.
    else if (piece1.type == BLACK_STONE || piece1.type == BLACK_DAME)  {

        if (coords.vertical_direction == FRONT_DIRECTION)
            square1.line = start_square.line + 1;
        else 
            square1.line = start_square.line - 1;

    }

    // --- DIREÇÃO HORIZONTAL ---

    //  Movimento vai para direita, aumenta coluna.
    if (coords.horizontal_direction == RIGHT_DIRECTION)
        square1.col = start_square.col + 1;

    // Movimento vai para esquerda, diminui coluna.
    else if (coords.horizontal_direction == LEFT_DIRECTION)
        square1.col = start_square.col - 1;

    // Se square1 existe
    if ( square_exists(piece1.game_board, square1) ) {
        *next_square = square1;
        return 1;
    }
    else 
        return 0;
}

int is_piece_ally(char type1, char type2) {

    if ( type1 == WHITE_STONE 
        || type1 == WHITE_DAME) 
    {

        if ( type2 == WHITE_STONE ||
            type2 == WHITE_DAME) {
            return 1;
        }   

    }

    else if ( type1 == BLACK_STONE 
        || type1 == BLACK_DAME) 
    {

        if ( type2 == BLACK_STONE ||
            type2 == BLACK_DAME) {
            return 1;
        }   

    }

    return 0;
}

// Retorna 1 se movimento foi feito, e 0 caso contrário.
int move_piece(piece piece1, move_coords coords) 
{
    square start_square;
    start_square.line = piece1.line;
    start_square.col = piece1.col;

    square next_square1;

    // Se square não existe.
    if ( !get_next_square(piece1, 
        &next_square1, start_square, coords) ) 
    {
        return 0;
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

        return 1;
    }
    else {
        piece1.game_board->no_piece_captured_count = 0;
    }

    // Square tem peça aliada, movimento inválido. 
    if ( is_piece_ally(piece1.type, square_type) )
        return 0;
    
    // Pega próximo square, após next_square1
    square next_square2;

    // Square não existe.
    if ( !get_next_square(piece1, 
        &next_square2, next_square1, coords) ) 
    {
        return 0;
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

        return 1;
    }

    // Square não está vazio, movimento inválido.
    else
        return 0;
}

// Limpa carácter extra que restar no buffer do scanf.
void clear_scanf_buffer() {
    char c;

    while ( (c = getchar()) != '\n' && c != EOF ) 
    {}
}

// Verifica se peça é do jogador.
int is_piece_from_player(piece piece1, enum Players player) {

    // Peça é do player branco
    if (piece1.type == WHITE_STONE
        || piece1.type == WHITE_DAME) {
        
        if (player != WHITE_PLAYER)
            return 0;

    }

    // Peça é do player preto
    else if (piece1.type == BLACK_STONE
        || piece1.type == BLACK_DAME) {
        
        if (player != BLACK_PLAYER)
            return 0;

    }

    return 1;
}

int main() {
    board* game_board = make_board(LINE_COUNT, COL_COUNT);
    enum Players current_player = WHITE_PLAYER;

    place_pieces(game_board);
    print_board(game_board);

    while (1) {
        char line_char, col_char;

        // print_board(game_board);

        // 6 jogadas seguidas sem nenhuma peça capturada. 
        // Empate.
        if ( game_board->no_piece_captured_count >= 6 ) {
            printf("EMPATE\n");
            break;
        }

        // 3 movimentos inválidos seguidos, outro jogador vence.
        if ( game_board->white_invalid_move_count >= 3 ) {
            printf("JOGADOR 2 VENCEU\n");
            break;
        }
        else if ( game_board->black_invalid_move_count >= 3 ) {
            printf("JOGADOR 1 VENCEU\n");
            break;
        }

        if (game_board->white_piece_count == 0) {
            printf("JOGADOR 2 VENCEU\n");
            break;
        }
        else if (game_board->black_piece_count == 0) {
            printf("JOGADOR 1 VENCEU\n");
            break;
        }

        // Coordenadas do movimento
        move_coords coords;
        coords.vertical_direction = FRONT_DIRECTION; // Frente ou trás

        // Pega input: linha, coluna, direção horizontal
        scanf(" %c%c %c", &col_char, &line_char, 
            &coords.horizontal_direction);

        // Peça escolhida pelo usuário
        piece input_piece = get_piece(line_char, col_char, game_board);

        // Se peça é vazia, movimento inválido.
        if (input_piece.type == '-') {
            printf("MOVIMENTO INVALIDO\n");
            print_board(game_board);

            // Incrementa contagem do movimento inválido.
            if (current_player == WHITE_PLAYER)
                game_board->white_invalid_move_count += 1;
            else 
                game_board->black_invalid_move_count += 1;

            continue;
        }

        // Verifica se peça é do player jogando esse turno.
        if ( !is_piece_from_player(input_piece, current_player) ) {
            printf("MOVIMENTO INVALIDO");
            print_board(game_board);

            if (current_player == WHITE_PLAYER)
                game_board->white_invalid_move_count += 1;
            else 
                game_board->black_invalid_move_count += 1;

            continue;
        }

        // Se peça for dama
        if ( input_piece.type == WHITE_DAME || 
            input_piece.type == BLACK_DAME ) 
        {
            // Pega direção vertical
            scanf(" %c", &coords.vertical_direction);
        }

        // Se sobrar carácter extra, remove do buffer.
        clear_scanf_buffer();

        // Tenta mover peça
        if ( move_piece(input_piece, coords) ) {

            // Atualiza jogador do próximo turno.
            current_player = current_player == WHITE_PLAYER ?
                BLACK_PLAYER :
                WHITE_PLAYER;

            game_board->white_invalid_move_count = 0;
            game_board->black_invalid_move_count = 0;
        }
        else {
            printf("MOVIMENTO INVALIDO");

            if (current_player == WHITE_PLAYER)
                game_board->white_invalid_move_count += 1;
            else 
                game_board->black_invalid_move_count += 1;
        }

        print_board(game_board);

    }

    // Libera memória ocupada pela board
    destroy_board(game_board);

    return 0;
}
