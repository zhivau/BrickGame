TETRIS_LIB := $(shell find brick_game/tetris -name '*.c')
SNAKE_LIB := $(shell find brick_game/snake -name '*.cpp')
RACE_CLIENT_LIB := $(shell find brick_game/race_client -name '*.cpp')

GUI_CLI := $(shell find gui/cli -name '*.c')

DYNAMIC=-fPIC -shared

GPP= g++ -Wall -Wextra -Werror -std=c++17
GCC= gcc -Wall -Wextra -Werror -std=c11

all: tests

clean:
	rm -rf build best_score_snake.txt best_score_tetris.txt best_score_race.txt brick_game_cli report test .coverage htmlcov coverage_report *.out *.so *.dll

desktop_gui:
	rm -rf build
	mkdir build
	cd build && cmake ../gui/desktop && make

cli_gui:
	$(GCC) -o brick_game_cli -g $(GUI_CLI) -lncurses -ldl

run_server:
	python3 brick_game/server/main.py

run_cli:
	./brick_game_cli

run_desktop:
	./build/brick_game_desktop

tests:
	coverage run -m unittest tests/race_tests.py
	coverage report
.PHONY: tests

libtetris:
	$(GCC) $(DYNAMIC) -o libtetris.so $(TETRIS_LIB)

libsnake:
	$(GPP) $(DYNAMIC) -o libsnake.so $(SNAKE_LIB)

librace_client:
	$(GPP) $(DYNAMIC) -o librace_client.so $(RACE_CLIENT_LIB) -lcurl

