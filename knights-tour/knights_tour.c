#include <stdlib.h>
#include <stdio.h>


/* To demonstrate rudimentary ability in C and C-like languages */ 


/***
Find knight's tours around a chessboard, that is, visit every square exactly once with a knight

a complete tour is a cycle returning to the starting square, this finds general tours

the particular move order used below finds more solutions in less time than others I have tried

It should be noted that this program may run for a very long time.

One possible modification could be saving solutions in a linked list of board

This algorithm can be improved by selecting the square (graph node) having the least available moves (vertices). 
***/


/* simple printer funtion for board aka an array */
void prar(int ** ar, int n);

/* track number of function calls */
unsigned long long CALL_COUNT = 0;

/* use backtracking to find solutions */
void _solve(int x, int y, int** board, int n){
  ++CALL_COUNT;

  /* all possible components of a legal knight move */
  const int moves[4] = {-2,-1,1,2};
  
  /* base case: check if solution */
  /* if the current move is the largest possible (8 * 8 = 64), the board represents a solution */
  if(board[x][y] == n * n){
    /* print the solution */
    prar(board, n);
    /* display call count */
    printf("\n%llu\n\n", CALL_COUNT);
  } /* ... and then keep looking for more combinations */
  
  /* try all move combinations */
  int i,j;
  for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++){

      /* knight move is assymetrical L shape, so (-2, 1) is valid but (-1, 1) is not*/
      if(abs(moves[i]) == abs(moves[j])){
	continue;
      }
      
      /* set position to try */
      int newx = x + moves[i]; /* x is current x, moves[i] is amount to offset from x */
      int newy = y + moves[j]; /* likewise, y is current y, moves[j] */ 

      /* check if move is on the board or onto an already used square */
      if(newx < 0 || newx >= n || newy < 0 ||  newy >= n || board[newx][newy] != 0){
	continue;
      }
      /* set the co-ordinates of the new position to current move + 1 */
      board[newx][newy] = board[x][y] + 1;

      /* recurse */
      _solve(newx, newy, board, n);

      /* undo change to board */
      board[newx][newy] = 0;
    }/*end for j */
  }/* end for i, all paths exhausted */
  return;
}

/* wrapper for solving function */
void solve(int n){
  /* allocate a board */
  int i;
  int ** board = malloc(sizeof(int*) * n);
  for(i = 0; i < n; i++){
    board[i] = malloc(sizeof(int) * n);
  }
  /* start at position 0,0 */
  int x = 0;
  int y = 0;
  /* start move index as 1 (either this or change _solve's base case) */
  board[x][y] = 1;
  _solve(x,y, board, n);
  free(board);
}

/* simple printer funtion for board aka a 2d array */
void prar(int ** ar, int n){
  int i,j;
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      if(j != 0) putchar(' '); /* put ' ' between squares that are not last */
      if(ar[i][j] < 10) putchar(' '); /* pad single digit numbers */
      printf("%d", ar[i][j]);
      if(j == n - 1) putchar('\n');
    }
  }
}

/* driver */
int main(int argc, char** argv){
  /* argument is board dimenstion, default is 8x8 board */
  int n = 8;
  if(argc >= 2){
    n = atoi(argv[1]);
  }
  solve(n);
}
