#include "cli_view.h"

#include <dlfcn.h>
#include <unistd.h>

UserAction_t getAction() {
  UserAction_t rc = -1;
  int user_input = getch();
  switch (user_input) {
    case KEY_UP:
      rc = Up;
      break;
    case KEY_DOWN:
      rc = Down;
      break;
    case KEY_LEFT:
      rc = Left;
      break;
    case KEY_RIGHT:
      rc = Right;
      break;
    case ESCAPE:
      rc = Terminate;
      break;
    case ENTER_KEY:
      rc = Start;
      break;
    case 'p':
    case 'P':
      rc = Pause;
      break;
    case ' ':
      rc = Action;
      break;
    default:
      break;
  }

  return rc;
}

void printField(GameInfo_t* data) {
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      if (data->field[i][j]) {
        attron(COLOR_PAIR((int)data->field[i][j]));
        mvaddch(i + 1, j + 1, '.');
        attroff(COLOR_PAIR((int)data->field[i][j]));
      } else {
        mvaddch(i + 1, j + 1, ' ');
      }
    }
  }

  if (data->next) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (data->next[i][j]) {
          attron(COLOR_PAIR((int)data->next[i][j]));
          mvaddch(i + 12, j + 14, '.');
          attroff(COLOR_PAIR((int)data->next[i][j]));
        } else {
          mvaddch(i + 12, j + 14, ' ');
        }
      }
    }
  }
}

void printInteface(int level, int score, int best_score, int speed) {
  for (int i = 0; i < 22; i++) {
    for (int j = 0; j < 22; j++) {
      mvaddch(j, i, ' ');
    }
  }

  printRectangle(0, 21, 0, 22);

  mvaddch(0, 11, ACS_TTEE);
  mvaddch(21, 11, ACS_BTEE);

  for (int i = 1; i < 21; i++) {
    mvaddch(i, 11, ACS_VLINE);
  }

  mvprintw(2, 13, "LEVEL: %d", level);
  mvprintw(4, 13, "SCORE:");
  mvprintw(5, 16, "%d", score);
  mvprintw(7, 13, "BEST:");
  mvprintw(8, 16, "%d", best_score);
  mvprintw(9, 13, "SPEED: %d", speed);
}

void printRectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);

  for (int i = left_x + 1; i < right_x; i++) mvaddch(top_y, i, ACS_HLINE);
  mvaddch(top_y, right_x, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }

  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  for (int i = left_x + 1; i < right_x; i++) mvaddch(bottom_y, i, ACS_HLINE);
  mvaddch(bottom_y, right_x, ACS_LRCORNER);
}

void printGameover() {
  for (int i = 0; i < 22; i++) {
    for (int j = 0; j < 22; j++) {
      mvaddch(j, i, ' ');
    }
  }

  mvprintw(7, 6, "GAMEOVER");
  mvprintw(8, 2, "Press ENTER: restart");
  mvprintw(9, 4, "Press ESC: exit");

  printRectangle(0, 21, 0, 22);
  refresh();
}

void printStart() {
  for (int i = 0; i < 22; i++) {
    for (int j = 0; j < 22; j++) {
      mvaddch(j, i, ' ');
    }
  }

  mvprintw(7, 7, "START");
  mvprintw(8, 2, "Press ENTER: start");
  mvprintw(9, 2, "Press ESC: exit");

  printRectangle(0, 21, 0, 22);
  refresh();
}

void printPause() {
  for (int i = 0; i < 22; i++) {
    for (int j = 0; j < 22; j++) {
      mvaddch(j, i, ' ');
    }
  }

  mvprintw(7, 8, "PAUSE");
  mvprintw(8, 1, "Press ENTER: continue");
  mvprintw(9, 3, "Press ESC: exit");

  printRectangle(0, 21, 0, 22);
  refresh();
}

void menuRender(int selected) {
  for (int i = 0; i < 22; i++) {
    for (int j = 0; j < 22; j++) {
      mvaddch(j, i, ' ');
    }
  }

  printRectangle(0, 21, 0, 22);
  mvprintw(6, 7, "BRICKGAME");

  const char* choices[] = {"Tetris", "Snake", "Race", "Exit"};
  for (int i = 0; i < 4; i++) {
    if (i == selected) {
      attron(A_REVERSE);
      mvprintw(10 + i * 2, 8, "%s", choices[i]);
      attroff(A_REVERSE);
    } else {
      mvprintw(10 + i * 2, 8, "%s", choices[i]);
    }
  }
  refresh();
}

void startMenuLoop() {
  int selected = 0;
  while (1) {
    menuRender(selected);
    UserAction_t action = getAction();
    if (action == Up) {
      selected--;
      if (selected < 0) selected += 4;
    } else if (action == Down) {
      selected++;
      if (selected > 3) selected = 0;
    } else if (action == Start) {
      if (selected == 0) {
        startGameLoop("./libtetris.so");
      } else if (selected == 1) {
        startGameLoop("./libsnake.so");
      } else if (selected == 2) {
        startGameLoop("./librace_client.so");
      } else if (selected == 3) {
        break;
      }
    }
  }
}

void startGameLoop(const char* lib_name) {
  void* handle = dlopen(lib_name, RTLD_LAZY);
  UserInputFunc userInput = dlsym(handle, "userInput");
  UpdateCurrentStateFunc updateCurrentState =
      dlsym(handle, "updateCurrentState");

  while (true) {
    UserAction_t action = getAction();
    if ((int)action != -1) {
      userInput(action, false);
    }

    if (action == Terminate) {
      break;
    }

    GameInfo_t data = updateCurrentState();

    if (data.level == -1) {
      printStart();
    } else if (data.level == -2) {
      printGameover();
    } else if (data.pause) {
      printPause();
    } else if (data.level > 0) {
      printInteface(data.level, data.score, data.high_score, data.speed);
      printField(&data);
    }
  }

  dlclose(handle);
}