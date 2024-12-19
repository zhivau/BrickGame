from enum import Enum
from .game import GameRace, RaceState
import time


class Action(Enum):
    Start = 0
    Pause = 1
    Terminate = 2
    Left = 3
    Right = 4
    Up = 5
    Down = 6
    Action = 7


class State:
    def __init__(
        self,
        field: list[list[bool]],
        next: list[list[bool]],
        score: int,
        high_score: int,
        level: int,
        speed: int,
        pause: bool,
    ):
        self.field = field
        self.next = next
        self.score = score
        self.high_score = high_score
        self.level = level
        self.speed = speed
        self.pause = pause


class LibRace:
    def __init__(self):
        self.game_race = GameRace()

    def userInput(self, action: int, hold: bool) -> None:
        func = self.game_race.table_race_actions[self.game_race.state.value][action]
        if func:
            func()

    def updateCurrentState(self) -> State:
        current_moving_time = time.time()
        duration_ms = int(
            (current_moving_time - self.game_race.last_moving_time) * 1000
        )
        intervals_ms = [
            500,
            485,
            470,
            455,
            440,
            425,
            410,
            395,
            380,
            365,
            350,
            335,
            320,
            305,
            290,
            275,
            260,
            245,
            230,
            215,
        ]
        if (
            self.game_race.state == RaceState.MOVING_S
            and duration_ms
            > intervals_ms[self.game_race.level + self.game_race.speed - 2]
        ):
            self.game_race.shift()

        return self.convert_game_race_to_state()

    def convert_game_race_to_state(self) -> State:
        cur_field = [[False for _ in range(10)] for _ in range(20)]

        for i in range(len(cur_field)):
            for j in range(len(cur_field[i])):
                for player_cor in self.game_race.player_car.coordinates:
                    if j == player_cor[0] and i == player_cor[1]:
                        cur_field[i][j] = True

                for enemy_car in self.game_race.enemy_cars:
                    for enemy_cor in enemy_car.coordinates:
                        if j == enemy_cor[0] and i == enemy_cor[1]:
                            cur_field[i][j] = True

        cur_pause = False
        if self.game_race.state == RaceState.PAUSE_S:
            cur_pause = True

        cur_level = self.game_race.level
        if self.game_race.state == RaceState.START_S:
            cur_level = -1
        elif self.game_race.state == RaceState.GAME_OVER_S:
            cur_level = -2

        return State(
            field=cur_field,
            next=[],
            score=self.game_race.score,
            high_score=self.game_race.high_score,
            level=cur_level,
            speed=self.game_race.speed,
            pause=cur_pause,
        )
