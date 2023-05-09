#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int board[91] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,
		 -1, 0, 0, 0, 0, 0, 0, 0, 0,
		 -1, 0, 0, 1, 0, 0, 0, 0, 0,
		 -1, 0, 0, 0, 1, 0, 0, 2, 0,
		 -1, 0, 0, 0, 4, 1, 2, 0, 0,
		 -1, 0, 0, 4, 3, 2, 0, 0, 0,
		 -1, 0, 4, 0, 0, 3, 0, 0, 0,
		 -1, 0, 0, 0, 0, 0, 3, 0, 0,
		 -1, 0, 0, 0, 0, 0, 0, 0, 0,
		 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

int c[91] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,
	     -1, 0, 0, 0, 0, 0, 0, 0, 0,
	     -1, 0, 0, 0, 0, 0, 0, 0, 0,
	     -1, 0, 0, 0, 0, 0, 0, 0, 0,
	     -1, 0, 0, 0, 0, 0, 0, 0, 0,
	     -1, 0, 0, 0, 0, 0, 0, 0, 0,
	     -1, 0, 0, 0, 0, 0, 0, 0, 0,
	     -1, 0, 0, 0, 0, 0, 0, 0, 0,
	     -1, 0, 0, 0, 0, 0, 0, 0, 0,
	     -1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

int dir[8] = {-10,-9,-8,-1,1,8,9,10};

void count_stone(int board[], int *c1, int *c2, int *c3, int *c4){
  int i;

  *c1 = 0;
  *c2 = 0;
  *c3 = 0;
  *c4 = 0;
  
  for(i = 10; i <= 80; i++){
    if(board[i] == 1){
      (*c1)++;
      continue;
    }
    if(board[i] == 2){
      (*c2)++;
      continue;
    }
    if(board[i] == 3){
      (*c3)++;
      continue;
    }
    if(board[i] == 4){
      (*c4)++;
      continue;
    }
  }
}

void print_board(int board[]){
  int i,j;
  printf("    a b c d e f g h\n");
  printf("   -----------------\n");
  printf("1 |");
  for(i = 10; i <= 80; i++ ){
    if(i%9 == 0){
      printf(" |");
      printf("\n");
      printf("%d |",i/9);
      continue;
    }
    printf("%2d",board[i]);
  }
  printf(" |\n   ----------------\n");
}

int check(int board[], int pos , int me){
  int i,k;
  //int enemy = 3 - me;
  if(board[pos]) return 0;
  int checkflag = 0;
  int cflag1 = 0;
  int cflag2 = 0;
  
  for(i = 0; i < 8; i++){
    if(board[pos + dir[i]] >= 1 && board[pos + dir[i]] != me){
      if(c[pos + dir[i]] != 0) cflag1 = 1;
      k = pos + 2*dir[i];
      while(board[k] >= 1 && board[k] != me){
	k = k + dir[i];
	if(c[k] != 0) cflag2 = 1;
      }
      if(board[k] == me) checkflag = 1;
      if( (cflag1 || cflag2) && board[k] == me){
	return 0;
      }
    }
    cflag1 = 0;
    cflag2 = 0;
  }
  if(checkflag) return 1;
  return 0;
}

void turn_over(int board[], int pos, int me){
  if(check(board,pos,me) == 0) return;
  int i,k;
  int rememberpos[8];
  for(i = 0; i < 8; i++){
    rememberpos[i] = 0;
    k = pos + 2*dir[i];
    if(board[pos + dir[i]] >= 1 && board[pos + dir[i]] != me){
      if(board[k] == -1) continue;
      k = pos + dir[i];
      while(board[k] >= 1 && board[k] != me){
        if(board[k] == -1) break;
        k = k + dir[i];
	if(board[k] == 0){
	  break;
	}
	if(board[k] == me){
	  rememberpos[i] = k;
	}
      }
    }else{
      continue;
    }
  }

  for(i = 0; i < 8; i++){
    if(rememberpos[i] == 0) continue;
    k = pos;
    while(k != rememberpos[i]){
      board[k] = me;
      k = k + dir[i];
    }
  }
}

int exist_legal_move(int board[], int me){
  int i;
  for(i = 10; i<=80; i++){
    if(check(board,i,me)) return 1;
  }

  return 0;
}

int get_move(int board[], int pos , int me ){
  char str[100];
  //int i,j;
  int d1,d2;

  printf("Player %d", me);
  while(1){
    printf("> ");
    fgets(str,sizeof(str),stdin);
    d1 = str[0] - '1'+1;
    d2 = str[1] - '1'+1;
    pos = d1*10+d2;
    if(check(board, pos, me)) return pos;
    
  }
}

int look8(int board[], int pos){//自身が-1や石が置かれておらず8方位のどこかに1以上の駒があるか
  if(board[pos] == -1) return 0;
  if(board[pos] == 1 || board[pos] == 2 || board[pos] == 3 || board[pos] == 4) return 0;
  //if(pos > 0 && pos < 10) return 0;
  int i;
  for(i = 0; i < 8; i++){
    if(board[pos + dir[i]] >= 1) return 1;
  }
  return 0;
}

int get_move0(int board[], int pos , int me ){
  char str[100];
  //int i,j;
  int d1,d2;

  printf("Player %d", me);
  str[1] = -1;
  while(1){
    printf("> ");
    fgets(str,sizeof(str),stdin);
    //if((str[0] < 0 && str[0] >9) || (str[1] < 0 && str[1] > 0)) continue;
    d1 = str[0] - '1'+1;
    d2 = str[1] - '1'+1;
    pos = d1*10+d2;
    if(look8(board, pos)) return pos;

  }
}

void clear(int c[])
{
  int i;
  for(i = 10; i <= 80; i++){
    if(c[i] == -1) continue;
    c[i] = 0;
  }
}

int main(){
  //int i,j;
  int pos;
  int me = 1;
  int c1,c2,c3,c4;
  int flag = 0;
  int lostpeople[2];//全滅用フラッグ
  lostpeople[0] = 0;
  int cnt4 = 0;
  while(1){
    print_board(board);
    count_stone(board, &c1, &c2, &c3, &c4);
    printf("c1 = %d c2 = %d c3 = %d c4 = %d\n",c1 ,c2 ,c3 ,c4);
    if(lostpeople[0] != 0){
      if(lostpeople[0] == me){
	clear(c);
      }
    }
    if(me == 1 && c1 == 0) flag = 1;
    if(me == 2 && c2 == 0) flag = 1;
    if(me == 3 && c3 == 0) flag = 1;
    if(me == 4 && c4 == 0) flag = 1;
    
    if(!exist_legal_move(board,me) && flag == 0){
      printf("%dplayer pass\n",me);
      me = me % 4 + 1;
      if(!exist_legal_move(board,me)){
	printf("%dplayer pass\n",me);
	me = me % 4 + 1;
	if(!exist_legal_move(board,me)){
	  printf("%dplayer pass\n",me);
	  me = me % 4 + 1;
	  if(!exist_legal_move(board,me)){
	    printf("%dplayer pass\n",me);
	    printf("gameend\n");
	    break;
	  }
	}
      }
    }
    if(flag){
      pos = get_move0(board,pos,me);
      board[pos] = me;
      c[pos] = me;
      flag = 0;
      if(lostpeople[0] == 0){
	lostpeople[0] = me;
      }else{
	lostpeople[1] = me;
      }
    }else{
      pos = get_move(board, pos, me);
      turn_over(board, pos, me);
    }
    me = me % 4 + 1;
    //printf("%d\n",board[3]);
  }
  //turn_over(board,57,1);
  //print_board(board);
  
  
}
