#include <stdlib.h>
#include <stdio.h>
#include "c4.h"

#define MAX_SCORE 1000000
#define MIN_SCORE -1000000

typedef struct pair {
  int x,y;
}Pair;

double agent_chao(Game_state *current_state, int player,int x, int y);
/* heuristics functions */
int check_attack(Game_state* current_state, int player, Pair** map, int n);
int check_type(Game_state *current_state, Pair** map, int player, int line);
int check_three(Game_state *current_state, int player, Pair** map);
/* help functions */
int check_diagonal(Game_state* current_state, Pair** map, int line);
int n_row(Game_state *current_state, Pair** map, int player,int line,int n);
Pair** r_map(Game_state *current_state);

double agent_chao(Game_state *current_state, int player,int x, int y){
  Pair **p_map = NULL;
  int score = 0;
  int is_won = game_won(current_state);
  /* a for threat odd, b for threat even
   * c for 2 in row, d for 1 in row */
  int a = 0, b = 0, c = 0, d = 0; 
  int win_places,i,threat_t,one_player = -1;
  int other_p = other(player);

  /* check who is playing */
  if(one_player == -1)
    one_player = player;
  /* won the game */
  if(is_won == player)
    score = MAX_SCORE;
  /* lost the game */
  else if (is_won == other_p)
    score = MIN_SCORE;
  else{
    /* Using heuristic to got the socre */
    win_places = num_of_win_places(current_state->width, current_state->height,
        current_state->num_to_connect);
    p_map = r_map(current_state);

    for (i = 0; i < win_places; i++) {
      threat_t = check_type(current_state,p_map,player,i);
      /* odd threat */
      if(threat_t == 1) a++;
      /* even threat */
      else if(threat_t == 0) b++;
      /* 2 in row */
      if (n_row(current_state,p_map,player,i,2)) c++;
      /* 1 in row */
      if (n_row(current_state,p_map,player,i,1)) d++;
    }

    /* Choose the center first - the key for win */
    if(current_state->board[current_state->width/2][y] == player)
      score += 50;

    if(one_player == player)
      score += a * 16 + b * 8 + c * 2 + d;

    else{
      score += b * 16 + a * 8 + c * 2 + d;

      if(current_state->num_of_pieces >= 5 && check_attack(current_state,
            other_p,p_map,2))
        score -= 10;
    }
    /* Check the current state if it good for self */
    if(check_attack(current_state,player,p_map,1)) score += 30;
    /* Check the current state if it good for other */
    if(check_attack(current_state,other_p,p_map,1)) score -= 100;
    /* Check for the three in line is exit or not */
    if(check_three(current_state,other_p,p_map)) score -= 100;
    /* Clean up the map */
    for (i = 0; i < win_places; free(p_map[i]), i++);
    free(p_map);
  }
  return score;
}

/*
 * check_attack()
 * Use for looking for a 3 points with one blank
 */
int check_attack(Game_state* current_state, int player, Pair** map, int n) {
  int connect = current_state->num_to_connect;
  int win_places = num_of_win_places(current_state->width, current_state->height
      , connect);
  int i, j, k, l;
  int x_t, y_t, x_b = -1, y_b = -1;

  for(i = 0; i < win_places; i++) {
    /* Not a diagonal, keep going */
    if(!check_diagonal(current_state, map, i)) continue;

    /* Three in row and one blank */
    if(!current_state->score_array[player][i] == connect-1 && 
        current_state->score_array[other(player)][i] == 0)
      continue;

    /* Get the blank */
    for(j = 0; j < connect; j++) {
      x_t = map[i][j].x;
      y_t = map[i][j].y;

      if(current_state->board[x_t][y_t] == C4_NONE) {
        x_b = x_t;
        y_b = y_t;
        break;
      }
    }

    /* Blank with the diagonla case 1*/
    if(x_b >= 0 && x_b < current_state->width && y_b+1 >= 0
        && y_b+1 < current_state->height){
      k = 0;

      while((l = current_state->map[x_b][y_b+1][k]) != -1) {

        if(check_diagonal(current_state, map, l) && 
            current_state->score_array[player][l] == connect-n &&
            current_state->score_array[other(player)][l] == 0)
          return 1;

        k++;
      }
    }

    /* Blank with the diagonla case 2*/
    if(x_b >= 0 && x_b < current_state->width && y_b-1 >= 0
        && y_b-1 < current_state->height){
      k = 0;

      while((l = current_state->map[x_b][y_b-1][k]) != -1) {

        if(check_diagonal(current_state, map, l) &&
            current_state->score_array[player][l] == connect-n &&
            current_state->score_array[other(player)][l] == 0)
          return 1;

        k++;
      }
    }

    /* Blank with the diagonla case 3*/
    if(x_b-1 >= 0 && x_b-1 < current_state->width && y_b-2 >= 0
        && y_b-2 < current_state->height){
      k = 0;

      while((l = current_state->map[x_b-1][y_b-2][k]) != -1) {

        if(check_diagonal(current_state, map, l) &&
            current_state->score_array[player][l] == connect-n &&
            current_state->score_array[other(player)][l] == 0)
          return 1;

        k++;
      }
    }
  }
  return 0;
}

/*
 * check_diagonal()
 * Use for check the diagonal
 * diagnoal have different x,y
 */
int check_diagonal(Game_state* current_state, Pair** map, int line) {
  int x_t = 0, y_t = 0, x_l = -1, y_l = -1, i;

  for(i = 0; i < current_state->num_to_connect; i++) {
    x_t = map[line][i].x;
    y_t = map[line][i].y;

    if(x_l == x_t || y_l == y_t) return 0;
    x_l = x_t;
    y_l = y_t;
  }
  return 1;
}

/*
 * check_three()
 * Use for check the three attack
 */
int check_three(Game_state *current_state, int player, Pair** map){
  int i,j,x_t,y_t;
  int left_h,right_h, upper_v, lower_v;
  int connect = current_state->num_to_connect;
  int win_places = num_of_win_places(current_state->width
      ,current_state->height,connect);

  for (i = 0; i < win_places; i++) {
    /* n-1 in row on the line */
    if(!current_state->score_array[player][i] == connect-1) continue;
    /* n-1 contiguous in row */
    if(!n_row(current_state,map,player,i,connect-1)) continue;
    left_h = right_h = upper_v = lower_v = 0;
    /* Check the blank on both sides */
    if (check_diagonal(current_state,map,i)) {

      for (j = 0; j < connect; j++) {
        x_t = map[i][j].x;
        y_t = map[i][j].y;

        /* Upper right */
        if(((x_t+1 >= 0) && (x_t+1 < current_state->width) && (y_t+1 >= 0) &&
              (y_t+1) < (current_state->height)) &&
            (current_state->board[x_t+1][y_t+1] == C4_NONE))
          upper_v = 1;
        /* Upper left */
        if(x_t-1 >= 0 && x_t-1 < current_state->width && y_t+1 >= 0 &&
            y_t+1 < current_state->height &&
            (current_state->board[x_t-1][y_t+1] == C4_NONE))
          upper_v = 1;
        /* Lower right */
        if(x_t+1 >= 0 && x_t+1 < current_state->width && y_t-1 >= 0 &&
            y_t-1 < current_state->height &&
            (current_state->board[x_t+1][y_t-1] == C4_NONE))
          lower_v = 1;
        /* Lower left */
        if(x_t-1 >= 0 && x_t-1 < current_state->width && y_t-1 >= 0 &&
            y_t-1 < current_state->height && 
            (current_state->board[x_t-1][y_t-1] == C4_NONE))
          lower_v = 1;
        /* Three in row !!! */
        if(upper_v && lower_v) return 1;
      }
    }
    else{
      /* Horizontal three attack */
      for (j = 0; j < connect; j++) {
        x_t = map[i][j].x;
        y_t = map[i][j].y;
        /* Left side check */
        if(current_state->board[x_t][y_t] == player){

          if(x_t-1 >= 0 && x_t-1 < current_state->width && y_t >= 0 &&
              y_t < current_state->height &&
              (current_state->board[x_t-1][y_t] == C4_NONE))
            left_h = 1;
          /* right side check */
          if(x_t+1 >= 0 && x_t+1 < current_state->width && y_t >= 0 &&
              y_t < current_state->height &&
              (current_state->board[x_t+1][y_t] == C4_NONE))
            right_h = 1;
        }

        if(left_h && right_h)
          return 1;
      }
    }
  }
  /* No three attack */
  return 0;
}

/*
 * n_row()
 * Use for check 1 or 2 ponit in row
 */
int n_row(Game_state *current_state, Pair** map, int player,int line,int n){
  Pair *p = NULL;
  int good = 0, found = 0, i, controller;

  if(!(current_state->score_array[player][line] == n))
    return 0;

  for (i = 0;  i < current_state->num_to_connect; i++){
    p = &(map[line][i]);
    controller = current_state->board[p->x][p->y];

    if(controller == C4_NONE || controller == other(player)){

      if (found > good) {
        good = found;
        found = 0;
      }
    }

    else
      found++;
  }

  return good >= n;
}

/*
 * check_type()
 * Use for check the type of the threat
 * -1 not threat
 *  1 odd threat
 *  0 even threat
 */
int check_type(Game_state *current_state, Pair** map, int player, int line){
  Pair *p = NULL;
  int i,is_threat = 0;
  int num_to_connect = current_state->num_to_connect;

  /* Good for current play, not threat */
  if(!(current_state->score_array[player][line] == num_to_connect-1 
        && current_state->score_array[other(player)][line] == 0))
    return -1;
  /* find x,y for a line */
  for (i = 0; i < num_to_connect; i++) {
    p = &(map[line][i]);
    if(current_state->board[p->x][p->y] == C4_NONE)
      break;
  }
  /* Check for a blank */
  if(p->y -1 >=0 && (current_state->board[p->x][p->y-1] == C4_NONE))
    is_threat = 1;

  return (is_threat) ? p->x % 2 : -1;

}

/*
 * r_map()
 * Use for build up the map
 */
Pair** r_map(Game_state *current_state){
  int win_places = num_of_win_places(current_state->width,
      current_state->height,current_state->num_to_connect);
  Pair **map = malloc(win_places * sizeof(Pair*));
  int *c = calloc(sizeof(int),win_places);
  int x,y,i,m;

  for (i = 0; i < win_places; i++)
    map[i] = malloc(sizeof(Pair) * current_state->num_to_connect);

  for (x = 0; x < current_state->width; x++) {

    for (y = 0; y < current_state->height; y++) {
      i=0;

      while((m = current_state->map[x][y][i]) != -1){
        Pair p;
        p.x = x;
        p.y = y;
        map[m][c[m]] = p;
        c[m]++;
        i++;
      }
    }
  }

  free(c);
  return map;
}
