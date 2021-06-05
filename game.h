#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 30
#define WSIZE SIZE*SIZE

#define crd_to_num(x, y) x * SIZE + y

enum { empty, snk, fd, brd };

typedef struct snake_t {
  int x, y;
  int dx, dy;
  int* coords;
  int coord_len;
  int length;
} snake_t;

typedef struct {
  int x, y;
} food_t;

typedef struct {
  int* coords;
  int score;
  int alive;
} world_t;

void game_init();
void draw();
void generate_food();
void set_direction(int, int);
void update_world();
bool in_snake(int, int);
void add_to_snake(int, int);
void delete_from_snake();
void check_direction();
void end_game();
