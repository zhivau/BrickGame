from enum import Enum
import time
from .cars import PlayerCar, EnemyCar
import os
import random


class RaceState(Enum):
    START_S = 0
    MOVING_S = 1
    PAUSE_S = 2
    GAME_OVER_S = 3


class GameRace:
    def __init__(self):
        self.table_race_actions = [
            [self.start_game, None, self.exit_game, None, None, None, None, None],
            [
                None,
                self.pause_game,
                self.exit_game,
                self.move_left,
                self.move_right,
                self.speed_up,
                self.speed_down,
                None,
            ],
            [None, self.unpause_game, self.exit_game, None, None, None, None, None],
            [self.start_game, None, self.exit_game, None, None, None, None, None],
        ]

        self.state = RaceState.START_S
        self.player_car = PlayerCar()
        self.enemy_cars = [EnemyCar(self.player_car.center[0])]

        self.last_moving_time = time.time()

        self.score = 0
        self.high_score = 0
        self.level = 0
        self.speed = 1

    def start_game(self):
        self.set_default_value()

        self.load_best_score()
        self.level = 1
        self.state = RaceState.MOVING_S

    def exit_game(self):
        if self.score == self.high_score and self.score > 0:
            self.save_best_score()
        self.set_default_value()

    def pause_game(self):
        self.state = RaceState.PAUSE_S

    def move_left(self):
        self.player_car.move_left()
        self.check_collision()

    def move_right(self):
        self.player_car.move_right()
        self.check_collision()

    def speed_up(self):
        if self.speed < 10:
            self.speed += 1

    def speed_down(self):
        if self.speed > 1:
            self.speed -= 1

    def unpause_game(self):
        self.state = RaceState.MOVING_S

    def check_out_field_cars(self):
        for index, enemy_car in enumerate(self.enemy_cars):
            if all(enemy_coors[1] > 19 for enemy_coors in enemy_car.coordinates):
                del self.enemy_cars[index]
                self.score += 100

                if self.score > self.high_score:
                    self.high_score = self.score

    def shift(self):
        self.last_moving_time = time.time()
        for enemy_car in self.enemy_cars:
            enemy_car.move_down()
        self.check_collision()
        self.check_out_field_cars()
        self.check_add_enemy_car()
        self.update_level()

    def check_collision(self):
        for enemy_car in self.enemy_cars:
            for enemy_coors in enemy_car.coordinates:
                for player_coors in self.player_car.coordinates:
                    if (
                        enemy_coors[0] == player_coors[0]
                        and enemy_coors[1] == player_coors[1]
                    ):
                        self.state = RaceState.GAME_OVER_S
                        return True
        return False

    def check_add_enemy_car(self):
        if random.random() < 0.15:
            if self.enemy_cars[-1].center[1] > 1:
                self.enemy_cars.append(EnemyCar(self.enemy_cars[-1].center[0]))
        else:
            if self.enemy_cars[-1].center[1] > 5:
                if self.enemy_cars[-1].center[0] == 2:
                    self.enemy_cars.append(EnemyCar(7))
                else:
                    self.enemy_cars.append(EnemyCar(2))

    def update_level(self):
        if self.level < 10:
            self.level = self.score // 500 + 1

    def load_best_score(self):
        if os.path.exists("best_score_race.txt"):
            with open("best_score_race.txt", "r") as f:
                self.high_score = int(f.read())

    def save_best_score(self):
        with open("best_score_race.txt", "w") as f:
            print(self.high_score)
            f.write(str(self.high_score))

    def set_default_value(self):
        self.state = RaceState.START_S
        self.player_car = PlayerCar()
        self.enemy_cars = [EnemyCar(self.player_car.center[0])]

        self.last_moving_time = time.time()

        self.score = 0
        self.high_score = 0
        self.level = 0
        self.speed = 1
