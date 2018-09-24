#include <stdio.h>
#include <stdlib.h>


/* Below, an efficient algorithm for finding knights tours is given. */
/* Preceeding this algorithm, two naive algorithms are provided for comparison */

const int BOARD_DIMENSION = 8;
const int DIM_SQUARED = 64;
const int NUM_MOVES = 8;
const int KNIGHT_MOVES[8][2] =
    { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2,
								     -1} };

/* convert coordinates to array offset*/
int xy_to_offset(int x, int y)
{
    if (x >= 0 && x < BOARD_DIMENSION && y >= 0 && y < BOARD_DIMENSION) {
	return x + y * BOARD_DIMENSION;
    }
    return -1;			/* inform caller of out of bounds */
}

/* convert array offset to coordinates*/
void offset_to_xy(int offset, int *x, int *y)
{
    /* assumes valid offset */
    *x = offset % BOARD_DIMENSION;
    *y = offset / BOARD_DIMENSION;
}

void test_offset_to_xy(int offset)
{
    int x, y;
    offset_to_xy(offset, &x, &y);
    printf("%d %d\n", x, y);
}

typedef struct square {
    /* Null out moves as used, hold as local value, return to available (NULL) slot when backtracking */
    struct square *available_moves[8];
    int tag;			/* a generic tag for debugging or other use */
    int inbound_moves;		/* track number of moves available into this square */
} square;

void remove_move(square * from, square * to)
{
    /* find and remove the move */
    for (int x = 0; x < NUM_MOVES; x++) {
	if (from->available_moves[x] == to) {
	    from->available_moves[x] = NULL;
	    return;
	}
    }
    printf("Tried to remove invalid move!");
    exit(1);
}

/* print available moves from "from" as offsets*/
void print_moves(square * from)
{
    for (int x = 0; x < NUM_MOVES; x++) {
	if (from->available_moves[x]) {
	    int offset = from->available_moves[x] - from;
	    printf("%d ", offset);
	}
    }
    printf("\n");
}


square *new_board()
{
    square *board;
    /* allocate board */
    board = malloc(sizeof(square) * BOARD_DIMENSION * BOARD_DIMENSION);
    if (!board) {
	printf("Failed to allocate board\n");
	exit(1);
    }
    /* setup moves for squares */
    for (int sq = 0; sq < DIM_SQUARED; sq++) {
	/* initialize tag value */
	board[sq].tag = 0;
	/* initialize inbound moves */
	board[sq].inbound_moves = 0;
	/* for each move in KNIGHT_MOVES */
	for (int mov = 0; mov < NUM_MOVES; mov++) {
	    /* we can use the current sq iterator to calculate where we are */
	    int cur_x, cur_y;
	    offset_to_xy(sq, &cur_x, &cur_y);
	    /* and knight moves to calculate where we land in x y coordinates */
	    /* and then convert back to offset coordinates */
	    int offset =
		xy_to_offset(KNIGHT_MOVES[mov][0] + cur_x,
			     KNIGHT_MOVES[mov][1] + cur_y);
	    /* if this offset is within the array, we add it to our moves */
	    if (offset >= 0 && offset <= DIM_SQUARED) {
		board[sq].available_moves[mov] = board + offset;
	    } else {
		/* else we mark the move as impossible */
		board[sq].available_moves[mov] = NULL;
	    }
	}
    }
    /* setup "inbound moves" count for some algorithms */
    for (int sq = 0; sq < DIM_SQUARED; sq++) {
	for (int mov = 0; mov < NUM_MOVES; mov++) {
	    square *target_square = board[sq].available_moves[mov];
	    if (target_square) {
		target_square->inbound_moves++;
	    }
	}
    }
    return board;
}

void print_board_tags(square * board)
{
    for (int y = 0; y < BOARD_DIMENSION; y++) {
	for (int x = 0; x < BOARD_DIMENSION; x++) {
	    printf("%2d ", board[xy_to_offset(x, y)].tag);
	}
	printf("\n");
    }
}

void print_inbound_moves(square * board)
{
    for (int y = 0; y < BOARD_DIMENSION; y++) {
	for (int x = 0; x < BOARD_DIMENSION; x++) {
	    printf("%2d ", board[xy_to_offset(x, y)].inbound_moves);
	}
	printf("\n");
    }
}

/* naive algorithm, backtracking */
/* this may take a very long while */
void knights_tour(square * board, square * current_position, int depth)
{
    /* board pointer is retained for simple printing of solutions */
    /* current position is square from which to move */
    /* depth counts from one for marking move number */
    /* base case */
    if (depth == 64) {
	print_board_tags(board);
    }
    /* set the current squares tag to depth, must reset before returning */
    current_position->tag = depth;

    /* try all available moves */
    for (int x = 0; x < NUM_MOVES; x++) {
	/* which are not NULL and not occupied by a move */
	square *target_square = current_position->available_moves[x];
	if (target_square && target_square->tag == 0) {
	    /* recurse */
	    knights_tour(board, target_square, depth + 1);
	}
    }
    /* reset current squares tag */
    current_position->tag = 0;
}

/* naive but chooses corner when available */
/* still pretty slow */
void knights_tour_corner(square * board, square * current_position,
			 int depth)
{
    /* board pointer is retained for simple printing of solutions */
    /* current position is square from which to move */
    /* depth counts from one for marking move number */
    /* base case */
    if (depth == 64) {
	print_board_tags(board);
    }
    /* set the current squares tag to depth, must reset before returning */
    current_position->tag = depth;

    /* skipping a corner is a (very) bad strategy, so see if corner is available */
    /* and try corner first, this does lead to replication below */
    if (depth == 25)
	print_board_tags(board);

    /* test for corner and move there if found */
    for (int x = 0; x < NUM_MOVES; x++) {
	square *target_square = current_position->available_moves[x];
	if (target_square && target_square->tag == 0) {
	    int offset = target_square - board;
	    if (offset == xy_to_offset(0, 0)
		|| offset == xy_to_offset(0, 7)
		|| offset == xy_to_offset(7, 0)
		|| offset == xy_to_offset(7, 7)) {
		knights_tour_corner(board, target_square, depth + 1);
	    }
	}
    }

    /* try all available moves */
    for (int x = 0; x < NUM_MOVES; x++) {
	/* which are not NULL and not occupied by a move */
	square *target_square = current_position->available_moves[x];
	if (target_square && target_square->tag == 0) {
	    /* recurse */
	    knights_tour_corner(board, target_square, depth + 1);
	}
    }
    /* reset current squares tag */
    current_position->tag = 0;
}

/* a much better method is tracking "incoming moves" and  */
/* choose the square with the least as the next square to which to move */
/* note that tracking used inbound moves would be redundant */
void knights_tour_count(square * board, square * current_position,
			int depth)
{
    /* board pointer is retained for simple printing of solutions */
    /* current position is square from which to move */
    /* depth counts from one for marking move number */
    /* base case */
    if (depth == 64) {
	print_board_tags(board);
	printf("\n");
    }
    /* set the current squares tag to depth, must reset before returning */
    current_position->tag = depth;

    /* try all available moves */
    /* but try lowest number inbound available first */
    /* (range is 1 to 8 inbound moves for a target square) */
    for (int inbound = 1; inbound < 9; inbound++) {
	for (int x = 0; x < NUM_MOVES; x++) {
	    /* which are not NULL and not occupied by a move */
	    square *target_square = current_position->available_moves[x];
	    if (target_square && target_square->tag == 0
		&& target_square->inbound_moves == inbound) {
		knights_tour_count(board, target_square, depth + 1);
	    }
	}
    }
    /* reset current squares tag */
    current_position->tag = 0;
}

int main()
{
    square *board = new_board();
    print_inbound_moves(board);
    knights_tour_count(board, board, 1);
    return 0;
}
