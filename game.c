#include "game.h"

world_t world;
snake_t snake;
food_t food;

int main()
{
  srand(time(NULL));
  initscr();
  cbreak();
  noecho();
  intrflush(stdscr, true);
  curs_set(false);
  keypad(stdscr, true);
  set_escdelay(100);
  start_color();
  timeout(100);

  game_init();

  draw();
  int key;
  while(world.alive) {
    key = getch();
    switch(key) {
      case KEY_UP:
        if(snake.dx == 1 && snake.length != 1)
          break;
        set_direction(-1, 0);
        break;
      case KEY_DOWN:
        if(snake.dx == -1 && snake.length != 1)
          break;
        set_direction(1, 0);
        break;
      case KEY_LEFT:
        if(snake.dy == 1 && snake.length != 1)
          break;
        set_direction(0, -1);
        break;
      case KEY_RIGHT:
        if(snake.dy == -1 && snake.length != 1)
          break;
        set_direction(0, 1);
        break;
      case ERR:
        update_world();
        draw();
        break;
      case 'q':
        end_game();
        break;
      default:
        update_world();
        draw();
        break;
    }
  }

  endwin();
  return 0;
}

void game_init()
{
  //world init
  world.score = 0;
  world.alive = true;
  world.coords = (int*)malloc(sizeof(int)*WSIZE);
  for(int i=0;i<WSIZE;i++) {
    world.coords[i] = empty;
  }
  //make border
  for(int i=0;i<SIZE;i++) {
    for(int j=0;j<SIZE;j++) {
      if(i == 0 || j == 0)
        world.coords[crd_to_num(i, j)] = brd;
      else if(i == SIZE-1 || j == SIZE-1)
        world.coords[crd_to_num(i, j)] = brd;
    }
  }


  //snake init
  snake.coords = (int*) malloc(sizeof(int*)*2);
  snake.coord_len = 2;
  int x = rand() % (SIZE-2) + 1;
  int y = rand() % (SIZE-2) + 1;
  snake.coords[0] = x;
  snake.coords[1] = y;
  snake.x = x;
  snake.y = y;
  snake.length = 1;

  world.coords[crd_to_num(x, y)] = snk;
  //food init
  generate_food();

  //snake and food on board
  world.coords[crd_to_num(food.x, food.y)] = fd;
}

void add_to_snake(int x, int y)
{
  snake.coord_len += 2;
  snake.coords = realloc(snake.coords, sizeof(int) * snake.coord_len);
  snake.coords[snake.coord_len-2] = x;
  snake.coords[snake.coord_len-1] = y;
}

void delete_from_snake()
{
  world.coords[crd_to_num(snake.coords[0], snake.coords[1])] = empty;
  for(int i=0;i<snake.coord_len;i++)
    snake.coords[i] = snake.coords[i+2];
  snake.coord_len -= 2;
  snake.coords = realloc(snake.coords, sizeof(int) * snake.coord_len);
}

void end_game()
{
  world.alive = false;
  endwin();
  printf("Your score: %i\n", world.score);
}

void update_world()
{
  //move snake
  int sx = snake.x + snake.dx;
  if(sx <= 0)
    sx = SIZE-2;
  else if(sx >= SIZE-1)
    sx = 1;
  int sy = snake.y + snake.dy;
  if(sy <= 0)
    sy = SIZE-2;
  else if(sy >= SIZE-1)
    sy = 1;

  for(int i=2;i<snake.coord_len;i+=2) {
    if(sx == snake.coords[i] && sy == snake.coords[i+1])
      end_game();
  }

  snake.x = sx;
  snake.y = sy;

  add_to_snake(sx, sy);

  if((snake.coord_len/2) > snake.length)
    delete_from_snake();

  world.coords[crd_to_num(sx, sy)] = snk;

  if(sx == food.x && sy == food.y) {
    world.score++;
    snake.length++;
    generate_food();
    world.coords[crd_to_num(food.x, food.y)] = fd;
  }
   
}

void set_direction(int x, int y)
{
  snake.dx = x;
  snake.dy = y;
}

bool in_snake(int x, int y)
{
  for(int i=0;i<snake.coord_len;i+=2) {
    if(x == snake.coords[i] && y == snake.coords[i+1])
      return true;
  }
  return false;
}

void generate_food()
{
  int x = rand() % (SIZE-2)+1;
  int y = rand() % (SIZE-2)+1;
  while(in_snake(x, y)) {
    x = rand() % (SIZE-2)+1;
    y = rand() % (SIZE-2)+1;
  }
  food.x = x;
  food.y = y;
}

void draw()
{
  move(0, 0);
  init_pair(snk, 0, COLOR_GREEN);
  init_pair(fd, 0, COLOR_RED);
  init_pair(brd, 0, COLOR_WHITE);
  for(int i=0;i<WSIZE;i++) {
    if(i%SIZE == 0)
      printw("\n");
    attron(COLOR_PAIR(world.coords[i]));
    printw("  ");
    attroff(COLOR_PAIR(world.coords[i]));
  }
  refresh();
}
