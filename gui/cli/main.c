#include <time.h>

#include "cli_view.h"

void initNcurses() {
  initscr();
  noecho();
  curs_set(0);
  start_color();
  keypad(stdscr, TRUE);

  nodelay(stdscr, TRUE);

  cbreak();
  use_default_colors();
  srand((unsigned int)time(NULL));

  timeout(50);
}

void initializeColors() {
  init_pair(1, COLOR_BLACK, COLOR_RED);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  init_pair(3, COLOR_BLACK, COLOR_BLUE);
  init_pair(4, COLOR_BLACK, COLOR_YELLOW);
  init_pair(5, COLOR_BLACK, COLOR_CYAN);
  init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(7, COLOR_BLACK, COLOR_WHITE);

  init_color(8, 500, 500, 500);
  init_pair(8, COLOR_BLACK, 8);
}

int main() {
  initNcurses();
  initializeColors();

  startMenuLoop();

  endwin();
  return 0;
}