#include <stdio.h>
#include <stdlib.h>

/* Finding Open Knight Tours */
/* a reasonably efficient approach*/
/* preceded by two naive approaches */
/* Mark Bowden 2018 */

/* An 8x8 chessboard has */
const int BOARD_DIMENSION = 8;
/* 64 squares. */
const int DIM_SQUARED = 64;
/* Coincidentally, there are also eight knight moves. */
const int NUM_MOVES = 8;
/* These eight moves have two components each, namely */
const int KNIGHT_MOVES[8][2] =
    { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2,
								     -1}
};

/* 
 *  The chessboard will be represented as an array of squares (nodes)
 *  connected by moves (edges).
 *  While forming the connections, it will be necessary to 
 *  convert between x, y coordinates and array indices (offsets)
 *  to form the appropriate edges. 
 */

int xy_to_offset(int x, int y);
void offset_to_xy(int offset, int *x, int *y);

/* A chessboard is made of squares */
typedef struct square {
    /* each of which is adjacent via knight moves to, at most, eight squares. */
    struct square *available_moves[8];
    /* Moreover, the squares are associated with */
    int tag;			/* solution data and */
    int degree;			/* a number of adjacent squares */
} square;

/* The chessboard is dynamically allocated and */
/* squares are initialized with
/* available-moves: pointers to squares adjacent via knight moves */
/* degree:          a tally of the pointers */
/* tag:             zero */
square *new_board()
{
    /* A board */
    square *board;
    /* is an array of squares. */
    board = malloc(sizeof(square) * DIM_SQUARED);
    if (!board) {
	printf("Failed to allocate board\n");
	exit(1);
    }
    /* Each square (determined by offset sq) */
    for (int sq = 0; sq < DIM_SQUARED; sq++) {
	/* is associated with an (x,y) coordinate. */
	int cur_x, cur_y;
	offset_to_xy(sq, &cur_x, &cur_y);
	/* The square begins with a tag value of zero and */
	board[sq].tag = 0;
	/* zero moves connected with it. */
	board[sq].degree = 0;
	/* Each prototype knight move is used to generate a */
	/* destination square when valid */
	for (int mov = 0; mov < NUM_MOVES; mov++) {
	    /* To do this, 
	     * add the knight move components to current coordinate
	     * and convert back to an offset */
	    int target_offset = xy_to_offset(cur_x + KNIGHT_MOVES[mov][0],
					     cur_y + KNIGHT_MOVES[mov][1]);
	    /* If the offset is valid, */
	    if (target_offset >= 0 && target_offset <= DIM_SQUARED) {
		/* set the current squares available move */
		/* to point to the appropriate destination square */
		board[sq].available_moves[mov] = board + target_offset;
		/* and increment the degree of the current square */
		board[sq].degree++;
	    } else {
		/* Invalid moves are marked as unavailable. */
		board[sq].available_moves[mov] = NULL;
	    }
	}
    }
    return board;
}

/* To display a board. */
void print_board_tags(square * board);

/* Backtracking is one approach to finding knight tours. */
/* This may take a very long while to find any tours for some orderings of KNIGHT_MOVES. */
void knights_tour(square * board, square * current_position, int depth)
{
    /* The board pointer is retained for simple printing of solutions, */
    /* The current position is the square from which to move, and */
    /* depth counts from one and is for storing the current move number. */

    /* To begin, set the current squares tag to depth, it must be reset before returning. */
    current_position->tag = depth;

    /*  Testing a base case reveals whether the algorithm is finished or not. */
    if (depth == DIM_SQUARED) {
	print_board_tags(board);
    }

    /* We begin by trying all of the moves */
    for (int x = 0; x < NUM_MOVES; x++) {
	/* which are not NULL and not already occupied by a move */
	square *target_square = current_position->available_moves[x];
	if (target_square && target_square->tag == 0) {
	    /* and recursing. */
	    knights_tour(board, target_square, depth + 1);
	}
    }
    /* Finally, we reset the tag for the current square as promised. */
    current_position->tag = 0;
}

/* Automatically using the corner when available is a good strategy, */
/* but apparently not good enough. */
void knights_tour_corner(square * board, square * current_position,
			 int depth)
{
    /* Mark the current square with the current move number. */
    /*Must reset before returning. */
    current_position->tag = depth;

    /* base case */
    if (depth == DIM_SQUARED) {
	print_board_tags(board);
    }

    /* Skipping a corner is a (very) bad strategy. */
    /* Corners have only two knight moves, missing one */
    /* severely limits opportunities to complete the tour. */

    /* Provided the opportunity, move to the corner. */
    /* (intentionally ignoring degree data.) */
    for (int x = 0; x < NUM_MOVES; x++) {
	square *target_square = current_position->available_moves[x];
	if (target_square && target_square->tag == 0) {
	    int offset = target_square - board;
	    int bd = BOARD_DIMENSION - 1;
	    if (offset == xy_to_offset(0, 0) ||
		offset == xy_to_offset(0, bd) ||
		offset == xy_to_offset(bd, 0) ||
		offset == xy_to_offset(bd, bd)) {
		knights_tour_corner(board, target_square, depth + 1);
	    }
	}
    }

    /* And then go back to trying all the moves (possible improvement, don't try corner again) */
    for (int x = 0; x < NUM_MOVES; x++) {
	/* which are not NULL and not occupied by a move */
	square *target_square = current_position->available_moves[x];
	if (target_square && target_square->tag == 0) {
	    /* and then recurse. */
	    knights_tour_corner(board, target_square, depth + 1);
	}
    }
    /* reset the tag */
    current_position->tag = 0;
}


/* A much better method is prioritizing squares having fewer connections. */
/* This extends the idea behind the above corner-prioritizing approach. */
/* Corner squares have only two knight moves, that leaves only one for */
/* moving into the square and one for moving out. */
void knights_tour_count(square * board, square * current_position,
			int depth)
{
    /* Store solution data and mark the square as unavailable */
    current_position->tag = depth;

    /* base case */
    if (depth == 64) {
	print_board_tags(board);
	printf("\n");
    }

    /* We try all available moves starting with those to squares */
    /* having the fewest connections. */
    for (int inbound = 1; inbound < 9; inbound++) {
	/* For each move */
	for (int x = 0; x < NUM_MOVES; x++) {
	    /* which is not NULL and not pointing to a square occupied by a move, */
	    square *target_square = current_position->available_moves[x];
	    if (target_square && target_square->tag == 0
		&& target_square->degree == inbound) {
		/* recurse. */
		knights_tour_count(board, target_square, depth + 1);
	    }
	}
    }
    /* Make this square available again */
    current_position->tag = 0;
}

/* In the future, I may consider */
/* the next logical step of updating connection counts as squares are used */

/* driver for showcasing best algorithm */
int main()
{
    square *board = new_board();
    /* Too slow */
    /* knights_tour(board, board, 1); */
    /* Better */
    /* knights_tour_corner(board, board, 1); */
    /* Best of three */
    knights_tour_count(board, board, 1);
    return 0;
}

/* Given a valid offset, */
void offset_to_xy(int offset, int *x, int *y)
{
    /* when dividing by the dimension of the board, */
    /* the column is the remainder and */
    *x = offset % BOARD_DIMENSION;
    /* the row is the quotient */
    *y = offset / BOARD_DIMENSION;
}

/* Given an (x,y) coordinate, */
int xy_to_offset(int x, int y)
{
    /* if valid, */
    if (x >= 0 && x < BOARD_DIMENSION && y >= 0 && y < BOARD_DIMENSION) {
	/* then the offset is */
	return y * BOARD_DIMENSION + x;
    }
    /* else inform caller of out of bounds */
    return -1;
}

/* To display a board. */
void print_board_tags(square * board)
{
    for (int y = 0; y < BOARD_DIMENSION; y++) {
	for (int x = 0; x < BOARD_DIMENSION; x++) {
	    printf("%2d ", board[xy_to_offset(x, y)].tag);
	}
	printf("\n");
    }
}
