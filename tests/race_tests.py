import unittest
from brick_game.race.lib_race import LibRace, RaceState, Action


lib_race = LibRace()


class TestRaceState(unittest.TestCase):

    def setUp(self):
        lib_race.game_race.set_default_value()

    def test_start_to_moving_state(self):
        self.assertEqual(lib_race.game_race.state, RaceState.START_S)
        lib_race.userInput(Action.Start.value, False)
        self.assertEqual(lib_race.game_race.state, RaceState.MOVING_S)

    def test_moving_to_pause_state(self):
        self.assertEqual(lib_race.game_race.state, RaceState.START_S)
        lib_race.userInput(Action.Start.value, False)
        self.assertEqual(lib_race.game_race.state, RaceState.MOVING_S)
        lib_race.userInput(Action.Pause.value, False)
        self.assertEqual(lib_race.game_race.state, RaceState.PAUSE_S)

    def test_pause_to_moving_state(self):
        self.assertEqual(lib_race.game_race.state, RaceState.START_S)
        lib_race.userInput(Action.Start.value, False)
        self.assertEqual(lib_race.game_race.state, RaceState.MOVING_S)
        lib_race.userInput(Action.Pause.value, False)
        self.assertEqual(lib_race.game_race.state, RaceState.PAUSE_S)
        lib_race.userInput(Action.Pause.value, False)
        self.assertEqual(lib_race.game_race.state, RaceState.MOVING_S)

    def test_moving_gameover_state(self):
        self.assertEqual(lib_race.game_race.state, RaceState.START_S)
        lib_race.userInput(Action.Start.value, False)
        self.assertEqual(lib_race.game_race.state, RaceState.MOVING_S)

        while lib_race.game_race.state != RaceState.GAME_OVER_S:
            lib_race.updateCurrentState()
        self.assertEqual(lib_race.game_race.state, RaceState.GAME_OVER_S)

    def test_gameover_to_moving_state(self):
        self.assertEqual(lib_race.game_race.state, RaceState.START_S)
        lib_race.userInput(Action.Start.value, False)
        self.assertEqual(lib_race.game_race.state, RaceState.MOVING_S)

        while lib_race.game_race.state != RaceState.GAME_OVER_S:
            lib_race.game_race.shift()
        self.assertEqual(lib_race.game_race.state, RaceState.GAME_OVER_S)

        lib_race.userInput(Action.Start.value, False)
        self.assertEqual(lib_race.game_race.state, RaceState.MOVING_S)


class TestPlayerControl(unittest.TestCase):

    def setUp(self):
        lib_race.game_race.set_default_value()
        lib_race.userInput(Action.Start.value, False)

    def test_move_player_car_left(self):
        p_car = lib_race.game_race.player_car
        lib_race.userInput(Action.Left.value, False)
        self.assertEqual(p_car.center[0], 2)

    def test_move_player_car_right(self):
        p_car = lib_race.game_race.player_car
        lib_race.userInput(Action.Right.value, False)
        self.assertEqual(p_car.center[0], 7)

    def test_speed_up_player_car(self):
        self.assertEqual(lib_race.game_race.speed, 1)
        lib_race.userInput(Action.Up.value, False)
        self.assertEqual(lib_race.game_race.speed, 2)

    def test_speed_down_player_car(self):
        self.assertEqual(lib_race.game_race.speed, 1)
        lib_race.userInput(Action.Up.value, False)
        self.assertEqual(lib_race.game_race.speed, 2)
        lib_race.userInput(Action.Down.value, False)
        self.assertEqual(lib_race.game_race.speed, 1)


if __name__ == "__main__":
    unittest.main()
