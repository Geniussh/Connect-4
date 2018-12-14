#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define BOARD_SIZE_HORIZ 7
#define BOARD_SIZE_VERT 6

int print_welcome(void);
void display_board(int board[][BOARD_SIZE_VERT]);
int random_move(int board[][BOARD_SIZE_VERT], int computer_num);
void update_board(int board[][BOARD_SIZE_VERT], int m, int player_num);
int player_move(int board[][BOARD_SIZE_VERT], int player_num);
bool is_column_full(int board[][BOARD_SIZE_VERT], int m);
bool check_win_or_tie(int board[][BOARD_SIZE_VERT], int last_move);
int check_winner(int board[][BOARD_SIZE_VERT], int last_move);
int best_move (int board[][BOARD_SIZE_VERT], int computer_num);
int defense(int board[][BOARD_SIZE_VERT], int copy[][BOARD_SIZE_VERT], int player_num, int computer_num);
int align3(int board[][BOARD_SIZE_VERT], int computer_num);
int align3_horiz(int board[][BOARD_SIZE_VERT], int computer_num);
int align3_vert(int board[][BOARD_SIZE_VERT], int computer_num);
int align3_diag(int board[][BOARD_SIZE_VERT], int computer_num);
bool is_trap(int board[][BOARD_SIZE_VERT], int m, int player_num);
void copy_board(int board[][BOARD_SIZE_VERT], int copy[][BOARD_SIZE_VERT], int m, int player_num);
bool is_trap4player(int board[][BOARD_SIZE_VERT], int m, int computer_num);
int create3(int board[][BOARD_SIZE_VERT], int computer_num);
int create3_vert(int board[][BOARD_SIZE_VERT], int computer_num);
int create3_diag(int board[][BOARD_SIZE_VERT], int computer_num);
int create3_horiz(int board[][BOARD_SIZE_VERT], int computer_num);

int main()
{
   int board[BOARD_SIZE_HORIZ][BOARD_SIZE_VERT] = { {0} };
   int player_num, computer_num;
   int last_move; 

   /* Ask Alice if she wants to go first */
   player_num = print_welcome();
   if (player_num == 1) computer_num = 2;
   else computer_num = 1;
   
   /* If Alice wants to go first, let her make a move */
   if (player_num == 1)
   {
	  display_board(board);
	  last_move = player_move(board,player_num);
	  display_board(board);
   }


   /* The main loop */
   
   while (1)
   {
	  /* Make a computer move, then display the board */
	  last_move = best_move(board,computer_num);
	  printf("Computer moved in column: %d\n", last_move);
	  display_board(board);

	  /* Check whether the computer has won */
	  if (check_win_or_tie(board,last_move)) return 0;


	  /* Let Alice make a move, then display the board */
	  last_move = player_move(board,player_num);
	  display_board(board);

	  /* Check whether Alice has won */
	  if (check_win_or_tie(board,last_move)) return 0;


   } /* end of while (1) */

} /* end of main() */

int print_welcome(void) {
  char c;
  srand(time(0)); // initialize random seed for rand() in random_move()

  printf("*** Welcome to the Connect Four game!!! ***\n");
  printf("Would you like to make the first move [y/n]: ");
  c=getchar();
  if ((c=='N' || c=='n') && getchar()=='\n') {
	return 2;
  }
  else {
	while (getchar()!='\n');
	return 1;
  }
}

void display_board(int board[][BOARD_SIZE_VERT]){
  for (int i=0; i<BOARD_SIZE_VERT; i++) {
	for (int j=0; j<BOARD_SIZE_HORIZ; j++) {
	  printf("+---");
	}
	printf("+\n");
	for (int k=0; k<BOARD_SIZE_HORIZ; k++) {
	  printf("| ");
	  if (board[k][i]==1) printf("X ");
	  else if (board[k][i]==2) printf("O ");
	  else printf("  ");
	}
	printf("|\n");
  }
  for (int i=0; i<BOARD_SIZE_HORIZ; i++) {
	printf("+---");
  }
  printf("+\n");
  for (int i=0; i<BOARD_SIZE_HORIZ; i++) {
	printf("  %d ",i+1);
  }
  printf(" \n");
}

int random_move(int board[][BOARD_SIZE_VERT], int computer_num){
  int m = rand() % (BOARD_SIZE_HORIZ - 1 + 1) + 1;
  while (is_column_full(board,m)) m = rand() % (BOARD_SIZE_HORIZ - 1 + 1) + 1;
  update_board(board, m, computer_num);
  return m;
}

int player_move(int board[][BOARD_SIZE_VERT], int player_num){
  int m;

  while (1) {
	printf("Please enter your move: ");
	if ((scanf("%d",&m)!=1) || (m<1) || (m>BOARD_SIZE_HORIZ)) {
	  printf("Not a valid move. Enter a column number!\n");
	  while (getchar()!='\n');
	  continue;
	}
	if (is_column_full(board,m)){
	  printf("This column is full. Try again!\n");
	  while (getchar()!='\n');
	  continue;
	}
	update_board(board,m,player_num);
	while (getchar()!='\n');
	break;
  }
  return m;
}

void update_board(int board[][BOARD_SIZE_VERT], int m, int player_num){
  for (int i=BOARD_SIZE_VERT-1; i>=0; i--){
	if (board[m-1][i]==0) {
	  board[m-1][i]=player_num;
	  break;
	}
  }
}

bool is_column_full(int board[][BOARD_SIZE_VERT], int m){
  for (int i=0; i<BOARD_SIZE_VERT; i++){
	if (board[m-1][i] == 0) return false;
  }
  return true;
}

bool check_win_or_tie(int board[][BOARD_SIZE_VERT], int last_move){
  int check;

  check = check_winner(board,last_move);
  if (check != 0) {
	printf("\n*****************************\n");
	printf("* Player %c won!!! Game over *\n", (check==1)?'X':'O');
	printf("*****************************\n");
	return true;
  }
  else if (check == 0) {
	for (int i=0; i<BOARD_SIZE_VERT; i++){
	  for (int j=0; j<BOARD_SIZE_HORIZ; j++){
		if (board[j][i]==0) return false;
	  }
	}
	printf("\n*****************************\n");
	printf("*   Tie game!!! Game over   *\n");
	printf("*****************************\n");
	return true;
  }
}

int check_winner (int board[][BOARD_SIZE_VERT], int last_move) {
  int r; // indicate #row of the stone of the last move
  for (int i=0; i<BOARD_SIZE_VERT; i++){
	if (board[last_move-1][i]!=0){
	  r = i;
	  break;
	}
  }
  /* -------------check horizontally------------- */
  int temp, count;
  for (int i=0; i<4+BOARD_SIZE_HORIZ-7; i++){
	count = 1;
	temp = i;
	for (int j=0; j<3; j++){
	  if (board[temp][r]!=0 && board[temp][r]==board[temp+1][r]) count++; 
	  temp++;
	}
	if (count == 4) return board[last_move-1][r];
  }
  /* -------------check vertically------------- */
  for (int i=0; i<3+BOARD_SIZE_VERT-6; i++){
	count = 1;
	temp = i;
	for (int j=0; j<3; j++){
	  if (board[last_move-1][temp]!=0 && board[last_move-1][temp]==board[last_move-1][temp+1]) count++;
	  temp++;
	}
	if (count == 4) return board[last_move-1][r];
  }
  /* -------------check diagonally------------- */
  int diX, diY; // two directions: (diX, diY) = (+1,-1) or (+1,+1)
  int xLim, yLim; // compare with board bound to prevent overflow
  int stp; // create a start point for each diagonal
  for (int x=0; x<BOARD_SIZE_HORIZ; x++){
	for (int y=0; y<BOARD_SIZE_VERT; y++){
	  diX = 1; diY = -1;
	  xLim = x+3*diX; yLim = y+3*diY;
	  if (xLim<BOARD_SIZE_HORIZ && yLim<BOARD_SIZE_VERT && xLim>=0 && yLim>=0){
		stp = board[x][y];
		if (stp!=0 && stp==board[x+diX][y+diY] && stp==board[x+2*diX][y+2*diY] && stp==board[xLim][yLim])
		  return stp;
	  }
	}
  }
  for (int x=0; x<BOARD_SIZE_HORIZ; x++){
	for (int y=0; y<BOARD_SIZE_VERT; y++){
	  diX = 1; diY = 1;
	  xLim = x+3*diX; yLim = y+3*diY;
	  if (xLim<BOARD_SIZE_HORIZ && yLim<BOARD_SIZE_VERT && xLim>=0 && yLim>=0){
		stp = board[x][y];
		if (stp!=0 && stp==board[x+diX][y+diY] && stp==board[x+2*diX][y+2*diY] && stp==board[xLim][yLim])
		  return stp;
	  }
	}
  }
  return 0;
}     // end of function

void copy_board(int board[][BOARD_SIZE_VERT], int copy[][BOARD_SIZE_VERT], int m, int player_num){
	int trapRow;
	for (int i=BOARD_SIZE_VERT-1; i>=0; i--){
		if (board[m-1][i]==0) {
			trapRow = i;
			break;
		}
	}
	copy[m-1][trapRow]=player_num;
}

int best_move (int board[][BOARD_SIZE_VERT], int computer_num){
	int m, d, a, c;
	int player_num = (computer_num==1)?2:1;
	int count=0;
	int copy[BOARD_SIZE_HORIZ][BOARD_SIZE_VERT]={{0}};

	if (computer_num == 1){
		for (int i=0; i<BOARD_SIZE_HORIZ; i++)
			for (int j=0; j<BOARD_SIZE_VERT; j++)
				if (board[i][j]==0) count++;
		if (count == BOARD_SIZE_VERT*BOARD_SIZE_HORIZ) {
			update_board(board,(BOARD_SIZE_HORIZ+1)/2,computer_num);
			return (BOARD_SIZE_HORIZ+1)/2;
		}
	}
	if (computer_num == 2 && board[(BOARD_SIZE_HORIZ-1)/2][BOARD_SIZE_VERT-1]==0){
		update_board(board,(BOARD_SIZE_HORIZ+1)/2,computer_num);
		return (BOARD_SIZE_HORIZ+1)/2;
	}

	for (int x=0; x<BOARD_SIZE_HORIZ; x++){
		if (is_trap(board,x+1,player_num)) {
			// printf("column %d is a trap!\n",x+1);
			copy_board(board,copy,x+1,player_num);
			// printf("See the last empty space in column %d is a %c.\n",x+1,player_num==1?'X':'O');
		}
		// else printf("column %d is not a trap\n",x+1);
	}
	

	if ((a=align3(board, computer_num))!=BOARD_SIZE_HORIZ){
		m = a+1;
		update_board(board,m,computer_num);
		return m;
	}
	else if ((d=defense(board,copy,player_num,computer_num))!=BOARD_SIZE_HORIZ){
		m = d+1;
		update_board(board,m,computer_num);
		return m;
	}
	else if ( (c=create3(board,computer_num))!=BOARD_SIZE_HORIZ && (!is_trap(board,c+1,player_num)) ){
		m = c+1;
		update_board(board,m,computer_num);
		// printf("It's a create3 move\n");
		return m;
	}
	else {
		int tempCount=0;
		m = rand() % (BOARD_SIZE_HORIZ - 1 + 1) + 1;
		while (is_column_full(board,m) || is_trap(board,m,player_num) || is_trap4player(board,m,computer_num)) {
			if (is_trap(board,m,player_num)) copy_board(board,copy,m,player_num);
			// printf("Computer intends to move in column %d but it's a trap!\n", m);
			m = rand() % (BOARD_SIZE_HORIZ - 1 + 1) + 1;
			tempCount++;
			if (tempCount==20) return random_move(board,computer_num); // dead move :(
		}
		update_board(board,m,computer_num);
		return m; // better random move :)
	}
}

//defense() returns the #col-1 to move if there's a 3 in a 4 vertically
//or there's a 3 in a horizontal row with one end
//or there's a 2 in a horizontal row without ends
//or BOARD_SIZE_HORIZ if there's no such condition
int defense (int board[][BOARD_SIZE_VERT], int copy[][BOARD_SIZE_VERT], int player_num, int computer_num){
	int diY, xLim, yLim;
	int stp;

	for (int x=0; x<BOARD_SIZE_HORIZ; x++){
		for (int y=BOARD_SIZE_VERT-1; y>=0; y--){
			diY = -1;
			yLim = y+3*diY;
			if (yLim<BOARD_SIZE_VERT && yLim>=0){
				stp = board[x][y];
				if ((stp==player_num||copy[x][y]==player_num) && (board[x][y+diY]==player_num||copy[x][y+diY]==player_num) && (board[x][y+2*diY]==player_num||copy[x][y+2*diY]==player_num) && board[x][y+3*diY]==0)
					return x;
			}
		}
	}


	for (int y=BOARD_SIZE_VERT-1;y>=0;y--){
		for (int x=0;x<4+BOARD_SIZE_HORIZ-7;x++){
			stp = board[x][y];
			if (stp==0 && (board[x+1][y]==player_num||copy[x+1][y]==player_num) && (board[x+2][y]==player_num||copy[x+2][y]==player_num) && (board[x+3][y]==player_num||copy[x+3][y]==player_num))
				if (y==BOARD_SIZE_VERT-1 || board[x][y+1]!=0) return x;
			if ((stp==player_num||copy[x][y]==player_num) && board[x+1][y]==0 && (board[x+2][y]==player_num||copy[x+2][y]==player_num) && (board[x+3][y]==player_num||copy[x+3][y]==player_num))
				if (y==BOARD_SIZE_VERT-1 || board[x+1][y+1]!=0) return x+1;
			if ((stp==player_num||copy[x][y]==player_num) && (board[x+1][y]==player_num||copy[x+1][y]==player_num) && board[x+2][y]==0 && (board[x+3][y]==player_num||copy[x+3][y]==player_num))
				if (y==BOARD_SIZE_VERT-1 || board[x+2][y+1]!=0) return x+2;
			if ((stp==player_num||copy[x][y]==player_num) && (board[x+1][y]==player_num||copy[x+1][y]==player_num) && (board[x+2][y]==player_num||copy[x+2][y]==player_num) && board[x+3][y]==0)
				if (y==BOARD_SIZE_VERT-1 || board[x+3][y+1]!=0) return x+3;
		}
	}

	for (int y=0; y<BOARD_SIZE_VERT; y++){
		for (int x=0; x<BOARD_SIZE_HORIZ; x++){
			if (x-1>=0 && x+2<BOARD_SIZE_HORIZ){
				if (board[x-1][y]==0 && (board[x][y]==player_num||copy[x][y]==player_num) && (board[x][y]==board[x+1][y]||copy[x+1][y]==player_num) && board[x+2][y]==0){
					if (y+1 == BOARD_SIZE_VERT || (board[x+2][y+1]!=0 && board[x-1][y+1]!=0)) return x+2; //either x-1 or x+2
				}
			}
		}
	}

	for (int x=0; x<BOARD_SIZE_HORIZ; x++){
		for (int y=0; y<BOARD_SIZE_VERT; y++){
			xLim=x+3; yLim=y+3;
			stp = board[x][y];
			if (xLim<BOARD_SIZE_HORIZ && yLim<BOARD_SIZE_VERT && xLim>=0 && yLim>=0){
				if (stp == 0 && (board[x+1][y+1]==player_num||copy[x+1][y+1]==player_num) && (board[x+2][y+2]==player_num||copy[x+2][y+2]==player_num) && (board[x+3][y+3]==player_num||copy[x+3][y+3]==player_num))
					if (board[x][y+1]!=0) return x;
				if ((stp == player_num||copy[x][y]==player_num) && board[x+1][y+1]==0 && (board[x+2][y+2]==player_num||copy[x+2][y+2]==player_num) && (board[x+3][y+3]==player_num||copy[x+3][y+3]==player_num))
					if (board[x+1][y+2]!=0) return x+1;
				if ((stp == player_num||copy[x][y]==player_num) && (board[x+1][y+1]==player_num||copy[x+1][y+1]==player_num) && board[x+2][y+2]==0 && (board[x+3][y+3]==player_num||copy[x+3][y+3]==player_num))
					if (board[x+2][y+3]!=0) return x+2;
				if ((stp == player_num||copy[x][y]==player_num) && (board[x+1][y+1]==player_num||copy[x+1][y+1]==player_num) && (board[x+2][y+2]==player_num||copy[x+2][y+2]==player_num) && board[x+3][y+3]==0)
					if (y+3==BOARD_SIZE_VERT-1 || board[x+3][y+4]!=0) return x+3;
			}
		}
	}

	for (int x=0; x<BOARD_SIZE_HORIZ; x++){
		for (int y=0; y<BOARD_SIZE_VERT; y++){
			xLim=x+3; yLim=y-3;
			stp = board[x][y];
			if (xLim<BOARD_SIZE_HORIZ && yLim<BOARD_SIZE_VERT && xLim>=0 && yLim>=0){
				if (stp == 0 && (board[x+1][y-1]==player_num||copy[x+1][y-1]==player_num) && (board[x+2][y-2]==player_num||copy[x+2][y-2]==player_num) && (board[x+3][y-3]==player_num||copy[x+3][y-3]==player_num))
					if (y==BOARD_SIZE_VERT-1 || board[x][y+1]!=0) return x;
				if ((stp == player_num||copy[x][y]==player_num) && board[x+1][y-1]==0 && (board[x+2][y-2]==player_num||copy[x+2][y-2]==player_num) && (board[x+3][y-3]==player_num||copy[x+3][y-3]==player_num))
					if (board[x+1][y]!=0) return x+1;
				if ((stp == player_num||copy[x][y]==player_num) && (board[x+1][y-1]==player_num||copy[x+1][y-1]==player_num) && board[x+2][y-2]==0 && (board[x+3][y-3]==player_num||copy[x+3][y-3]==player_num))
					if (board[x+2][y-1]!=0) return x+2;
				if ((stp == player_num||copy[x][y]==player_num) && (board[x+1][y-1]==player_num||copy[x+1][y-1]==player_num) && (board[x+2][y-2]==player_num||copy[x+2][y-2]==player_num) && board[x+3][y-3]==0)
					if (board[x+3][y-2]!=0) return x+3;
			}
		}
	}
	return BOARD_SIZE_HORIZ;
}

//return #col-1 if there's a winning strategy
int align3(int board[][BOARD_SIZE_VERT], int computer_num){
	int m;

	if ((m=align3_vert(board, computer_num))!=BOARD_SIZE_HORIZ){
		return m;
	}
	if ((m=align3_horiz(board, computer_num))!=BOARD_SIZE_HORIZ){
		return m;
	}
	if ((m=align3_diag(board, computer_num))!=BOARD_SIZE_HORIZ){
		return m;
	}
	return BOARD_SIZE_HORIZ;
}

int align3_horiz(int board[][BOARD_SIZE_VERT], int computer_num){
	int x,y;
	int stp; //startpoint

	for (y=BOARD_SIZE_VERT-1;y>=0;y--){
		for (x=0;x<4+BOARD_SIZE_HORIZ-7;x++){
			stp = board[x][y];
			if (stp==0 && board[x+1][y]==computer_num && board[x+2][y]==computer_num && board[x+3][y]==computer_num)
				if (y==BOARD_SIZE_VERT-1 || board[x][y+1]!=0) return x;
			if (stp==computer_num && board[x+1][y]==0 && board[x+2][y]==computer_num && board[x+3][y]==computer_num)
				if (y==BOARD_SIZE_VERT-1 || board[x+1][y+1]!=0) return x+1;
			if (stp==computer_num && board[x+1][y]==computer_num && board[x+2][y]==0 && board[x+3][y]==computer_num)
				if (y==BOARD_SIZE_VERT-1 || board[x+2][y+1]!=0) return x+2;
			if (stp==computer_num && board[x+1][y]==computer_num && board[x+2][y]==computer_num && board[x+3][y]==0)
				if (y==BOARD_SIZE_VERT-1 || board[x+3][y+1]!=0) return x+3;
		}
	}
	return BOARD_SIZE_HORIZ;
}

int align3_vert(int board[][BOARD_SIZE_VERT], int computer_num){
	int x,y;
	int stp; //startpoint

	for (y=1; y<4+BOARD_SIZE_VERT-6; y++){
		for (x=0; x<BOARD_SIZE_HORIZ; x++){
			stp = board[x][y];
			if (board[x][y-1]==0 && stp==computer_num && stp==board[x][y+1] && stp==board[x][y+2])
				return x;
		}
	}
	return BOARD_SIZE_HORIZ;
}

int align3_diag(int board[][BOARD_SIZE_VERT], int computer_num){
	int x,y,xLim,yLim;
	int stp;

	for (x=0; x<BOARD_SIZE_HORIZ; x++){
		for (y=0; y<BOARD_SIZE_VERT; y++){
			xLim=x+3; yLim=y+3;
			stp = board[x][y];
			if (xLim<BOARD_SIZE_HORIZ && yLim<BOARD_SIZE_VERT && xLim>=0 && yLim>=0){
				if (stp == 0 && board[x+1][y+1]==computer_num && board[x+2][y+2]==computer_num && board[x+3][y+3]==computer_num)
					if (board[x][y+1]!=0) return x;
				if (stp == computer_num && board[x+1][y+1]==0 && board[x+2][y+2]==computer_num && board[x+3][y+3]==computer_num)
					if (board[x+1][y+2]!=0) return x+1;
				if (stp == computer_num && board[x+1][y+1]==computer_num && board[x+2][y+2]==0 && board[x+3][y+3]==computer_num)
					if (board[x+2][y+3]!=0) return x+2;
				if (stp == computer_num && board[x+1][y+1]==computer_num && board[x+2][y+2]==computer_num && board[x+3][y+3]==0)
					if (y+3==BOARD_SIZE_VERT-1 || board[x+3][y+4]!=0) return x+3;
			}
		}
	}

	for (x=0; x<BOARD_SIZE_HORIZ; x++){
		for (y=0; y<BOARD_SIZE_VERT; y++){
			xLim=x+3; yLim=y-3;
			stp = board[x][y];
			if (xLim<BOARD_SIZE_HORIZ && yLim<BOARD_SIZE_VERT && xLim>=0 && yLim>=0){
				if (stp == 0 && board[x+1][y-1]==computer_num && board[x+2][y-2]==computer_num && board[x+3][y-3]==computer_num)
					if (y==BOARD_SIZE_VERT-1 || board[x][y+1]!=0) return x;
				if (stp == computer_num && board[x+1][y-1]==0 && board[x+2][y-2]==computer_num && board[x+3][y-3]==computer_num)
					if (board[x+1][y]!=0) return x+1;
				if (stp == computer_num && board[x+1][y-1]==computer_num && board[x+2][y-2]==0 && board[x+3][y-3]==computer_num)
					if (board[x+2][y-1]!=0) return x+2;
				if (stp == computer_num && board[x+1][y-1]==computer_num && board[x+2][y-2]==computer_num && board[x+3][y-3]==0)
					if (board[x+3][y-2]!=0) return x+3;
			}
		}
	}
	return BOARD_SIZE_HORIZ;
}

bool is_trap(int board[][BOARD_SIZE_VERT], int m, int player_num){
	/*check from left top to right bottom*/
	for (int y=0; y<BOARD_SIZE_VERT; y++){
		if (y+3<BOARD_SIZE_VERT && m+2<BOARD_SIZE_HORIZ){
			if (board[m-1][y]==0 && board[m][y+1]==player_num && board[m+1][y+2]==player_num && board[m+2][y+3]==player_num 
				&& board[m-1][y+1]==0 && board[m-1][y+2]!=0) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT; y++){
		if (y+3<BOARD_SIZE_VERT && m+1<BOARD_SIZE_HORIZ && m-2>=0){
			if (board[m-2][y]==player_num && board[m-1][y+1]==0 && board[m][y+2]==player_num && board[m+1][y+3]==player_num 
				&& board[m-1][y+2]==0 && board[m-1][y+3]!=0) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT; y++){
		if (y+3<BOARD_SIZE_VERT && m<BOARD_SIZE_HORIZ && m-3>=0){
			if (board[m-3][y]==player_num && board[m-2][y+1]==player_num && board[m-1][y+2]==0 && board[m][y+3]==player_num 
				&& board[m-1][y+3]==0 && (y+4==BOARD_SIZE_VERT || board[m-1][y+4]!=0)) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT; y++){
		if (y+4<BOARD_SIZE_VERT && m-1<BOARD_SIZE_HORIZ && m-4>=0){
			if (board[m-4][y]==player_num && board[m-3][y+1]==player_num && board[m-2][y+2]==player_num && board[m-1][y+3]==0 
				&& board[m-1][y+4]==0 && (y+5==BOARD_SIZE_VERT || board[m-1][y+5]!=0)) return true;
		}
	}
	/*check from left bottom to right top*/
	for (int y=BOARD_SIZE_VERT-2; y>=3; y--){
		if (m-1>=0 && m+2<BOARD_SIZE_HORIZ){
			if (board[m-1][y]==0 && board[m][y-1]==player_num && board[m+1][y-2]==player_num && board[m+2][y-3]==player_num 
				&& board[m-1][y+1]==0 && (y+2==BOARD_SIZE_VERT || board[m-1][y+2]!=0)) return true;
		}
	}
	for (int y=BOARD_SIZE_VERT-1; y>=0; y--){
		if (y-3>=0 && m-2>=0 && m+1<BOARD_SIZE_HORIZ){
			if (board[m-2][y]==player_num && board[m-1][y-1]==0 && board[m][y-2]==player_num && board[m+1][y-3]==player_num 
				&& board[m-1][y]==0 && (y+1==BOARD_SIZE_VERT || board[m-1][y+1]!=0)) return true;
		}
	}
	for (int y=BOARD_SIZE_VERT-1; y>=0; y--){
		if (y-3>=0 && m-3>=0 && m<BOARD_SIZE_HORIZ){
			if (board[m-3][y]==player_num && board[m-2][y-1]==player_num && board[m-1][y-2]==0 && board[m][y-3]==player_num 
				&& board[m-1][y-1]==0 && board[m-1][y]!=0) return true;
		}
	}
	for (int y=BOARD_SIZE_VERT-1; y>=0; y--){
		if (y-3>=0 && m-4>=0 && m-1<BOARD_SIZE_HORIZ){
			if (board[m-4][y]==player_num && board[m-3][y-1]==player_num && board[m-2][y-2]==player_num && board[m-1][y-3]==0 
				&& board[m-1][y-2]==0 && board[m-1][y-1]!=0) return true;
		}
	}
	/*check horizontally*/
	for (int y=0; y<BOARD_SIZE_VERT-1; y++){
		if (m+2<BOARD_SIZE_HORIZ){
			if (board[m-1][y]==0 && board[m][y]==player_num && board[m+1][y]==player_num && board[m+2][y]==player_num 
				&& board[m-1][y+1]==0 && (y+2==BOARD_SIZE_VERT || board[m-1][y+2]!=0)) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT-1; y++){
		if (m-2>=0 && m+1<BOARD_SIZE_HORIZ){
			if (board[m-2][y]==player_num && board[m-1][y]==0 && board[m][y]==player_num && board[m+1][y]==player_num 
				&& board[m-1][y+1]==0 && (y+2==BOARD_SIZE_VERT || board[m-1][y+2]!=0)) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT-1; y++){
		if (m-3>=0 && m<BOARD_SIZE_HORIZ){
			if (board[m-3][y]==player_num && board[m-2][y]==player_num && board[m-1][y]==0 && board[m][y]==player_num 
				&& board[m-1][y+1]==0 && (y+2==BOARD_SIZE_VERT || board[m-1][y+2]!=0)) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT-1; y++){
		if (m-4>=0){
			if (board[m-4][y]==player_num && board[m-3][y]==player_num && board[m-2][y]==player_num && board[m-1][y]==0 
				&& board[m-1][y+1]==0 && (y+2==BOARD_SIZE_VERT || board[m-1][y+2]!=0)) return true;
		}
	}
	return false;
}

bool is_trap4player(int board[][BOARD_SIZE_VERT], int m, int computer_num){
	/*check from left top to right bottom*/
	for (int y=0; y<BOARD_SIZE_VERT; y++){
		if (y+3<BOARD_SIZE_VERT && m+2<BOARD_SIZE_HORIZ){
			if (board[m-1][y]==0 && board[m][y+1]==computer_num && board[m+1][y+2]==computer_num && board[m+2][y+3]==computer_num 
				&& board[m-1][y+1]==0 && board[m-1][y+2]!=0) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT; y++){
		if (y+3<BOARD_SIZE_VERT && m+1<BOARD_SIZE_HORIZ && m-2>=0){
			if (board[m-2][y]==computer_num && board[m-1][y+1]==0 && board[m][y+2]==computer_num && board[m+1][y+3]==computer_num 
				&& board[m-1][y+2]==0 && board[m-1][y+3]!=0) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT; y++){
		if (y+3<BOARD_SIZE_VERT && m<BOARD_SIZE_HORIZ && m-3>=0){
			if (board[m-3][y]==computer_num && board[m-2][y+1]==computer_num && board[m-1][y+2]==0 && board[m][y+3]==computer_num 
				&& board[m-1][y+3]==0 && (y+4==BOARD_SIZE_VERT || board[m-1][y+4]!=0)) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT; y++){
		if (y+4<BOARD_SIZE_VERT && m-1<BOARD_SIZE_HORIZ && m-4>=0){
			if (board[m-4][y]==computer_num && board[m-3][y+1]==computer_num && board[m-2][y+2]==computer_num && board[m-1][y+3]==0 
				&& board[m-1][y+4]==0 && (y+5==BOARD_SIZE_VERT || board[m-1][y+5]!=0)) return true;
		}
	}
	/*check from left bottom to right top*/
	for (int y=BOARD_SIZE_VERT-2; y>=3; y--){
		if (m-1>=0 && m+2<BOARD_SIZE_HORIZ){
			if (board[m-1][y]==0 && board[m][y-1]==computer_num && board[m+1][y-2]==computer_num && board[m+2][y-3]==computer_num 
				&& board[m-1][y+1]==0 && (y+2==BOARD_SIZE_VERT || board[m-1][y+2]!=0)) return true;
		}
	}
	for (int y=BOARD_SIZE_VERT-1; y>=0; y--){
		if (y-3>=0 && m-2>=0 && m+1<BOARD_SIZE_HORIZ){
			if (board[m-2][y]==computer_num && board[m-1][y-1]==0 && board[m][y-2]==computer_num && board[m+1][y-3]==computer_num 
				&& board[m-1][y]==0 && (y+1==BOARD_SIZE_VERT || board[m-1][y+1]!=0)) return true;
		}
	}
	for (int y=BOARD_SIZE_VERT-1; y>=0; y--){
		if (y-3>=0 && m-3>=0 && m<BOARD_SIZE_HORIZ){
			if (board[m-3][y]==computer_num && board[m-2][y-1]==computer_num && board[m-1][y-2]==0 && board[m][y-3]==computer_num 
				&& board[m-1][y-1]==0 && board[m-1][y]!=0) return true;
		}
	}
	for (int y=BOARD_SIZE_VERT-1; y>=0; y--){
		if (y-3>=0 && m-4>=0 && m-1<BOARD_SIZE_HORIZ){
			if (board[m-4][y]==computer_num && board[m-3][y-1]==computer_num && board[m-2][y-2]==computer_num && board[m-1][y-3]==0 
				&& board[m-1][y-2]==0 && board[m-1][y-1]!=0) return true;
		}
	}
	/*check horizontally*/
	for (int y=0; y<BOARD_SIZE_VERT-1; y++){
		if (m+2<BOARD_SIZE_HORIZ){
			if (board[m-1][y]==0 && board[m][y]==computer_num && board[m+1][y]==computer_num && board[m+2][y]==computer_num 
				&& board[m-1][y+1]==0 && (y+2==BOARD_SIZE_VERT || board[m-1][y+2]!=0)) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT-1; y++){
		if (m-2>=0 && m+1<BOARD_SIZE_HORIZ){
			if (board[m-2][y]==computer_num && board[m-1][y]==0 && board[m][y]==computer_num && board[m+1][y]==computer_num 
				&& board[m-1][y+1]==0 && (y+2==BOARD_SIZE_VERT || board[m-1][y+2]!=0)) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT-1; y++){
		if (m-3>=0 && m<BOARD_SIZE_HORIZ){
			if (board[m-3][y]==computer_num && board[m-2][y]==computer_num && board[m-1][y]==0 && board[m][y]==computer_num 
				&& board[m-1][y+1]==0 && (y+2==BOARD_SIZE_VERT || board[m-1][y+2]!=0)) return true;
		}
	}
	for (int y=0; y<BOARD_SIZE_VERT-1; y++){
		if (m-4>=0){
			if (board[m-4][y]==computer_num && board[m-3][y]==computer_num && board[m-2][y]==computer_num && board[m-1][y]==0 
				&& board[m-1][y+1]==0 && (y+2==BOARD_SIZE_VERT || board[m-1][y+2]!=0)) return true;
		}
	}
	return false;
}

int create3(int board[][BOARD_SIZE_VERT], int computer_num){
	int m;

	if ((m=create3_diag(board, computer_num))!=BOARD_SIZE_HORIZ)
		return m;
	if ((m=create3_horiz(board, computer_num))!=BOARD_SIZE_HORIZ)
		return m;
	if ((m=create3_vert(board, computer_num))!=BOARD_SIZE_HORIZ)
		return m;

	return BOARD_SIZE_HORIZ;
}

int create3_vert(int board[][BOARD_SIZE_VERT], int computer_num){
	int x,y;
	for (y=BOARD_SIZE_VERT-1; y>=3; y--){
		for (x=0; x<BOARD_SIZE_HORIZ; x++){
			if (board[x][y]==computer_num && board[x][y-1]==computer_num && board[x][y-2]==0)
				return x;
		}
	}
	return BOARD_SIZE_HORIZ;
}

int create3_horiz(int board[][BOARD_SIZE_VERT], int computer_num){

	for (int y=0; y<BOARD_SIZE_VERT; y++){
		for (int x=0; x<4+BOARD_SIZE_HORIZ-7; x++){
			if (board[x][y]==0 && board[x+1][y]==computer_num && board[x+2][y]==computer_num && board[x+3][y]==0){
				if (y+1==BOARD_SIZE_VERT || board[x][y+1]!=0) return x;
				if (y+1==BOARD_SIZE_VERT || board[x+3][y+1]!=0) return x+3;
			}
			if (board[x][y]==computer_num && board[x+1][y]==0 && board[x+2][y]==0 && board[x+3][y]==computer_num){
				if (y+1==BOARD_SIZE_VERT || board[x+1][y+1]!=0) return x+1;
				if (y+1==BOARD_SIZE_VERT || board[x+2][y+1]!=0) return x+2;
			}
			if (board[x][y]==computer_num && board[x+1][y]==0 && board[x+2][y]==computer_num && board[x+3][y]==0){
				if (y+1==BOARD_SIZE_VERT || board[x+1][y+1]!=0) return x+1;
				if (y+1==BOARD_SIZE_VERT || board[x+3][y+1]!=0) return x+3;
			}
			if (board[x][y]==0 && board[x+1][y]==computer_num && board[x+2][y]==0 && board[x+3][y]==computer_num){
				if (y+1==BOARD_SIZE_VERT || board[x][y+1]!=0) return x;
				if (y+1==BOARD_SIZE_VERT || board[x+2][y+1]!=0) return x+2;
			}
			if (board[x][y]==computer_num && board[x+1][y]==computer_num && board[x+2][y]==0 && board[x+3][y]==0){
				if (y+1==BOARD_SIZE_VERT || board[x+2][y+1]!=0) return x+2;
				if (y+1==BOARD_SIZE_VERT || board[x+3][y+1]!=0) return x+3;
			}
			if (board[x][y]==0 && board[x+1][y]==0 && board[x+2][y]==computer_num && board[x+3][y]==computer_num){
				if (y+1==BOARD_SIZE_VERT || board[x][y+1]!=0) return x;
				if (y+1==BOARD_SIZE_VERT || board[x+1][y+1]!=0) return x+1;
			}
		}
	}
	return BOARD_SIZE_HORIZ;
}

int create3_diag(int board[][BOARD_SIZE_VERT], int computer_num){
	int x,y;

	for (x=0; x<4+BOARD_SIZE_HORIZ-7; x++){
		for (y=0; y<3+BOARD_SIZE_VERT-6; y++){
			if (board[x][y]==computer_num && board[x+1][y+1]==computer_num && board[x+2][y+2]==0 && board[x+3][y+3]==0){
				if (y+4==BOARD_SIZE_VERT || board[x+3][y+4]!=0) return x+3;
				if (board[x+2][y+3]!=0) return x+2;
			}
			if (board[x][y]==computer_num && board[x+1][y+1]==0 && board[x+2][y+2]==computer_num && board[x+3][y+3]==0){
				if (y+4==BOARD_SIZE_VERT || board[x+3][y+4]!=0) return x+3;
				if (board[x+1][y+2]!=0) return x+1;
			}
			if (board[x][y]==computer_num && board[x+1][y+1]==0 && board[x+2][y+2]==0 && board[x+3][y+3]==computer_num){
				if (board[x+2][y+3]!=0) return x+2;
				if (board[x+1][y+2]!=0) return x+1;
			}
			if (board[x][y]==0 && board[x+1][y+1]==0 && board[x+2][y+2]==computer_num && board[x+3][y+3]==computer_num){
				if (board[x][y+1]!=0) return x;
				if (board[x+1][y+2]!=0) return x+1;
			}
			if (board[x][y]==0 && board[x+1][y+1]==computer_num && board[x+2][y+2]==0 && board[x+3][y+3]==computer_num){
				if (board[x][y+1]!=0) return x;
				if (board[x+2][y+3]!=0) return x+2;
			}
			if (board[x][y]==0 && board[x+1][y+1]==computer_num && board[x+2][y+2]==computer_num && board[x+3][y+3]==0){
				if (y+4==BOARD_SIZE_VERT || board[x+3][y+4]!=0) return x+3;
				if (board[x][y+1]!=0) return x;
			}
		}
	}

	for (x=0; x<4+BOARD_SIZE_HORIZ-7; x++){
		for (y=BOARD_SIZE_VERT-1; y>=3; y--){
			if (board[x][y]==computer_num && board[x+1][y-1]==computer_num && board[x+2][y-2]==0 && board[x+3][y-3]==0){
				if (board[x+3][y-2]!=0) return x+3;
				if (board[x+2][y-1]!=0) return x+2;
			}
			if (board[x][y]==0 && board[x+1][y-1]==0 && board[x+2][y-2]==computer_num && board[x+3][y-3]==computer_num){
				if (y+1==BOARD_SIZE_VERT || board[x][y+1]!=0) return x;
				if (board[x+1][y]!=0) return x+1;
			}
			if (board[x][y]==computer_num && board[x+1][y-1]==0 && board[x+2][y-2]==computer_num && board[x+3][y-3]==0){
				if (board[x+1][y]!=0) return x+1;
				if (board[x+3][y-2]!=0) return x+3;
			}
			if (board[x][y]==0 && board[x+1][y-1]==computer_num && board[x+2][y-2]==0 && board[x+3][y-3]==computer_num){
				if (y+1==BOARD_SIZE_VERT || board[x][y+1]!=0) return x;
				if (board[x+2][y-1]!=0) return x+2;
			}
			if (board[x][y]==computer_num && board[x+1][y-1]==0 && board[x+2][y-2]==0 && board[x+3][y-3]==computer_num){
				if (board[x+1][y]!=0) return x+1;
				if (board[x+2][y-1]!=0) return x+2;
			}
			if (board[x][y]==0 && board[x+1][y-1]==computer_num && board[x+2][y-2]==computer_num && board[x+3][y-3]==0){
				if (y+1==BOARD_SIZE_VERT || board[x][y+1]!=0) return x;
				if (board[x+3][y-2]!=0) return x+3;
			}
		}
	}
	return BOARD_SIZE_HORIZ;
}