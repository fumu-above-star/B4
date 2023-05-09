#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int dir[8] = {-10, -9, -8, -1, 1, 8, 9,10};

/*
int value[91] = {-1, -1, -1, -1, -1, -1, -1, -1, -1,
                 -1, 60,-24,  0, -2, -2,  0,-24, 60,
                 -1,-24,-30, -6, -6, -6, -6,-30,-24,
                 -1,  0, -6,  0, -2, -2,  0, -6,  0,
                 -1, -2, -6, -2, -2, -2, -2, -6, -2,
                 -1, -2, -6, -2, -2, -2, -2, -6, -2,
                 -1,  0, -6,  0, -2, -2,  0, -6,  0,
                 -1,-24,-30, -6, -6, -6, -6,-30,-24,
                 -1, 60,-24,  0, -2, -2,  0,-24, 30,
                 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
*/

int value[91] = {-1, -1, -1, -1, -1, -1, -1, -1, -1,
		 -1,120,-20, 20,  5,  5, 20,-20,120,
		 -1,-20,-40, -5, -5, -5, -5,-40,-20,
		 -1, 20, -5, 15,  3,  3, 15, -5, 20,
		 -1,  5, -5,  3,  3,  3,  3, -5,  5,
		 -1,  5, -5,  3,  3,  3,  3, -5,  5,
		 -1, 20, -5, 15,  3,  3, 15, -5, 20,
		 -1,-20,-40, -5, -5, -5, -5,-40,-20,
		 -1,120,-20, 20,  5,  5, 20,-20,120,
		 -1, -1, -1, -1, -1, -1, -1, -1, -1,
  
};

double time;

void printboard(int b[])
{
  int i;
  for(i = 0; i < 91; i++){
    if(i%9 == 0){
      printf("\n");
    }
    printf("%d ",b[i]);
  }
}

int eval(int b[])
{
  int i;
  int point = 0;

  for(i = 10; i <= 80; i++){
    if(b[i] == 1){
      point += value[i];
    }
    if(b[i] == 2){
      point -= value[i];
    }
  }
  return point;
}

int check(int b[], int pos, int me)//合法手があるか
{
  int i,k;
  int enemy = 3 - me;

  if(b[pos]) return 0;
  for(i = 0; i < 8; i++){
    if(b[pos+dir[i]] == enemy){
      k = pos +2*dir[i]; while(b[k] == enemy) k = k + dir[i];
      if(b[k] == me) return 1;
    }
  }
  return 0;
}

void put(int b[],int pos, int me, int rememberpos[]){
  //if(check(b,pos,me) == 0) return 0;
  int enemy = 3 - me;
  int i,k;

  for(i = 0; i < 8; i++){
    rememberpos[i] = 0;//rememberposは置き石と挟む関係にある石
    k = pos + 2*dir[i];
    if(b[pos+dir[i]] == enemy){//posの一つ先にenemyがいるか
      if(b[k] == -1) continue;//enemyの一つ先が盤外ならcontinue
      k = pos+dir[i];
      while(b[k] == enemy){//ひっくり返すために挟む石を探す
        //if(b[k] == -1) break;
        k = k + dir[i];
        if(b[k] == me){
          rememberpos[i] = k;
          break;
        }
        //if(b[k] == enemy){//敵の石が続けばkの座標をひとつずらす
        //k= k + dir[i];
        //}
        if(b[k] == 0){
          break;
        }
      }
      /*if(b[k] == me){
        rememberpos[i] = k;
        }*/
    }else{
      continue;
    }
  }

  for(i = 0; i < 8; i++){
    if(rememberpos[i] == 0) continue;
    k = pos;
    while(k != rememberpos[i]){
      b[k] = me;
      k = k + dir[i];
    }

  }

  //b[pos] = me;
  //return 1;
}

void put_back(int b[],int pos,int me,int rememberpos[]){
  int enemy = 3 - me;
  int i, k;
  b[pos] = 0;
  for(i = 0; i < 8; i++){
    if(rememberpos[i] != 0){
      k = rememberpos[i] - dir[i];
    }else{
      continue;
    }
    while( k != pos){
      b[k] = enemy;
      k -= dir[i];
      //if(k == pos){
        //b[k] = 0;
        //break;
        // }
      //b[pos] = 0;
    }
  }
}

int minimax(int b[], int me, int depth,int passflag)//searchと併用して手との連携はとれたこのアルゴリズムが正しく動いているか整合性はチェックできた気がする
{
  // printboard(b);//確認用boardcheck
  if(depth == 0) return eval(b);

  int i;

  int rememberpos[8];
  int best = -999999;
  int worst = 999999;
  int val;
  int pass = 0;
  for(i = 1; i <= 80; i++){
    if(check(b,i,me)){
      put(b,i,me,rememberpos);
      val = minimax(b,3-me,depth-1,0);
      //printf("score = %d\n",val);//score確認
      put_back(b,i,me,rememberpos);

      if(me == 1 && best < val) best = val;
      if(me == 2 && worst > val) worst = val;
    }
  }
   if(me == 1 && best == -999999){
      if(passflag) return eval(b);
      return minimax(b,3-me,depth,1);
   }
   if(me == 2 && worst == 999999){
      if(passflag) return eval(b);
      return minimax(b,3-me,depth,1);
   }
   if(me == 1){
     return best;
   }else{
     return worst;
   }
}

int search(int b[], int me,int depth){//searchの中でminimaxを走らせて手を返す
  int i;
  int max_score = -999999;
  int res = -1;
  int score;
  int rememberpos[8];
  for(i = 10; i <= 80; i++){
    if(check(b,i,me)){
      put(b,i,me,rememberpos);
      score = minimax(b,3-me,depth-1,0);
      //printf("score = %d\n",score);//score確認
      put_back(b,i,me,rememberpos);
      if(max_score < score){
        max_score = score;
        printf("%d\n",max_score);
        res = i;
      }
    }
  }
  return res;
}

void play(int b[],int me)
{
  int i, k;
  int p_cnt = 0;
  int p[32];
  
  k = search(b,1,5);
  //k = p[random() % p_cnt];
  if(k != -1){
    printf("%c%c\n", 'a'+((k-10) % 9), '1'+((k-10)/9));
  }else{
    printf("%%pass\n");
  }
}

int main(int argc, char *argv[])
{
  int i, j;
  if( argc != 4 ){
    printf("Parameter error!\n");
    return 0;
  }
  if( strlen(argv[1]) != 32 ){
    printf("Parameter error!\n");
    return 0;
  }
  int b[91] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,
               -1, 0, 0, 0, 0, 0, 0, 0, 0,
               -1, 0, 0, 0, 0, 0, 0, 0, 0,
               -1, 0, 0, 0, 0, 0, 0, 0, 0,
               -1, 0, 0, 0, 2, 1, 0, 0, 0,
               -1, 0, 0, 0, 1, 2, 0, 0, 0,
               -1, 0, 0, 0, 0, 0, 0, 0, 0,
               -1, 0, 0, 0, 0, 0, 0, 0, 0,
               -1, 0, 0, 0, 0, 0, 0, 0, 0,
               -1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  
  j = 10;
  for(i = 0; i < 32; i++){
    switch(argv[1][i]-'0'){
    case 1:       b[j+1]=1;break;
    case 2:       b[j+1]=2;break;
    case 3:b[j]=1;         break;
    case 4:b[j]=1;b[j+1]=1;break;
    case 5:b[j]=1;b[j+1]=2;break;
    case 6:b[j]=2;         break;
    case 7:b[j]=2;b[j+1]=1;break;
    case 8:b[j]=2;b[j+1]=2;break;
    }
    j+=2;
    if( (i & 3)==3 ) j++;//3の倍数の時-1をとばすためにj++
  }
  time = atof(argv[3]);

  if( argv[2][1]=='b' ) play(b,1);
  else if( argv[2][1]=='w' ) play(b,2);
  else printf("Parameter error!\n");
  return 0;
}
