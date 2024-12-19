from random import choice


class PlayerCar:
    def __init__(self):
        x_cor, y_cor = choice([2, 7]), 19

        self.coordinates = [
            [x_cor, y_cor - 3],
            [x_cor - 1, y_cor - 2],
            [x_cor, y_cor - 2],
            [x_cor + 1, y_cor - 2],
            [x_cor, y_cor - 1],
            [x_cor - 1, y_cor],
            [x_cor + 1, y_cor],
        ]
        self.center = [x_cor, y_cor - 1]

    def move_left(self):
        if self.center[0] == 7:
            for coordinate in self.coordinates:
                coordinate[0] -= 5
            self.center[0] -= 5

    def move_right(self):
        if self.center[0] == 2:
            for coordinate in self.coordinates:
                coordinate[0] += 5
            self.center[0] += 5


class EnemyCar:
    def __init__(self, x_coordinate):
        x_cor, y_cor = x_coordinate, -1

        self.coordinates = [
            [x_cor - 1, y_cor - 3],
            [x_cor + 1, y_cor - 3],
            [x_cor, y_cor - 2],
            [x_cor - 1, y_cor - 1],
            [x_cor, y_cor - 1],
            [x_cor + 1, y_cor - 1],
            [x_cor, y_cor],
        ]
        self.center = [x_cor, y_cor - 2]

    def move_down(self):
        if self.center[1] < 19 + 2:
            for coordinate in self.coordinates:
                coordinate[1] += 1
            self.center[1] += 1
