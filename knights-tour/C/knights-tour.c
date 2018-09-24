#include <stdlib.h>
#include <stdio.h>

/* find knight's tours around chessboard */

/* first, a chess board is an 8x8 2d array */
const int DIM = 8;

/* print board ... */
void print_board(int ** ar){
  int i,j;
  for(i = 0; i < DIM; i++){
    for(j = 0; j < DIM; j++){
      /* space but not at start of line */
      if(j != 0) putchar(' ');
      /* extra padding space for single digits */
      if(ar[i][j] < 10) putchar(' ');
      /* print ith row jth column*/
      printf("%d", ar[i][j]);
      /* newline after last column */
      if(j == DIM - 1) putchar('\n');
    }
  }
}

int ** new_board(){
  int ** board = malloc(sizeof(int*) * DIM);
  for(int i = 0; i < DIM; i++){
    board[i] = malloc(sizeof(int) * DIM);
  }
  return board;
}

/* track number of function calls */
unsigned long long CALL_COUNT = 0;

/* use backtracking to find solutions */
void solve(int x, int y, int** board){
  ++CALL_COUNT;

  /* all possible components of a legal knight move */
  const int num_comps = 4;
  const int moves[4] = {-2,-1,1,2};
  
  /* base case: if on last move (64th move one 8x8 chessboard), then finish */
  if(board[x][y] == DIM * DIM){
    /* print the solution */
    print_board(board);
    /* display call count */
    printf("\n%llu\n\n", CALL_COUNT);
    return; /* ... and then keep looking for more combinations */
  }
  
  /* try all move combinations */
  int i,j;
  for(i = 0; i < num_comps; i++){
    for(j = 0; j < num_comps; j++){

      /* knight move is assymetrical L shape, so (-2, 1) is valid but (-1, 1) is not*/
      /* consideration: an array of structs of moves probably more efficient */
      if(abs(moves[i]) == abs(moves[j])){
	continue;
      }
      
      /* set position to try */
      int newx = x + moves[i]; /* x is current x, moves[i] is amount to offset from x */
      int newy = y + moves[j]; /* likewise, y is current y, moves[j] */ 

      /* check if move is on the board or onto an already used square */
      if(newx < 0 || newx >= DIM || newy < 0 ||  newy >= DIM || board[newx][newy] != 0){
	continue;
      }
      /* set the co-ordinates of the new position to current move + 1 */
      board[newx][newy] = board[x][y] + 1;

      /* recurse */
      solve(newx, newy, board);

      /* undo change to board */
      board[newx][newy] = 0;
    }/*end for j */
  }/* end for i, all paths exhausted */
  return;
}

int main(int argc, char** argv){
  int ** board = new_board();
  board[0][0] = 1;
  solve(0, 0, board);
  
  return 0;
}
