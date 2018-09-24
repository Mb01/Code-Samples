#include <stdlib.h>
#include <stdio.h>

/* Find knight's tours using a 2d array and backtracking. */

/* An 8 x 8 array representing a chesssboard */
const int DIM = 8;

/* is dynamically allocated and */
int **new_board();

/* can be represented in row/column format. */
void print_board(int **ar);

/* For the curious, function calls are tallied. */
unsigned long long CALL_COUNT = 0;

/* In particular, the following recursive algorithm  */
void solve(int x, int y, int **board)
{
    ++CALL_COUNT;

    /* tests for a base case of reaching the maximum or last move number. */
    /* Move numbers are stored at respective (x,y) positions on the board. */
    if (board[x][y] == DIM * DIM) {
	/* Solutions are printed */
	print_board(board);
	/* along with the call tally */
	printf("\n%llu\n\n", CALL_COUNT);
	/* before backtracking for more solutions. */
	return;
    }

    /* Knight moves are pairs of (four) move components. */
    const int num_comps = 4;
    const int components[4] = { -2, -1, 1, 2 };

    /* indexed here by i and j. */
    for (int i = 0; i < num_comps; i++) {
	for (int j = 0; j < num_comps; j++) {

	    /* However, valid move components must not have the same absolute value. */
	    if (abs(components[i]) == abs(components[j])) {
		continue;
	    }

	    /* These move components represent x and y translations for calculating new x and new y coordinates. */
	    int newx = x + components[i];
	    int newy = y + components[j];

	    /* However, valid moves must remain within the chess board and must not be into previously used squares. */
	    if (newx < 0 || newx >= DIM || newy < 0 || newy >= DIM
		|| board[newx][newy] != 0) {
		continue;
	    }

	    /* On finding a valid move, the new square is marked with the next move number */
	    board[newx][newy] = board[x][y] + 1;

	    /* before recursing. */
	    solve(newx, newy, board);

	    /* Now, the marked square is no longer the next move, so it is unmarked. */
	    board[newx][newy] = 0;
	}
    }
    return;
}

/* This driver */
int main(int argc, char **argv)
{
    /* allocates a board */
    int **board = new_board();
    /* sets coordinates 0,0 to move number 1 */
    board[0][0] = 1;
    /* and calls the above algorithm as follows. */
    solve(0, 0, board);

    return 0;
}

/* The 2d array representing the chessboard is allocated as follows. */
int **new_board()
{
    int **board = malloc(sizeof(int *) * DIM);
    for (int i = 0; i < DIM; i++) {
	board[i] = malloc(sizeof(int) * DIM);
    }
    return board;
}

/* To print the board */
void print_board(int **ar)
{
    /* Iterate over each row and column */
    for (int i = 0; i < DIM; i++) {
	for (int j = 0; j < DIM; j++) {
	    /* and print entry for ith row jth column with formatting */
	    printf("%2d ", ar[i][j]);
	}
	/* and with a newline after the last column. */
	printf("\n");
    }
}
