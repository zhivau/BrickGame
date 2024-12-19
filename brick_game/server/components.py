from pydantic import BaseModel
import ctypes


class GameInfoC(ctypes.Structure):
    _fields_ = [
        ("field", ctypes.POINTER(ctypes.POINTER(ctypes.c_int))),
        ("next", ctypes.POINTER(ctypes.POINTER(ctypes.c_int))),
        ("score", ctypes.c_int),
        ("high_score", ctypes.c_int),
        ("level", ctypes.c_int),
        ("speed", ctypes.c_int),
        ("pause", ctypes.c_int),
    ]


def convert_p_matrix_to_b_matrix(p_matrix, c_rows, c_cols):
    b_matrix = []

    if not p_matrix:
        b_matrix = [[False for _ in range(c_cols)] for _ in range(c_rows)]
        return b_matrix

    for i in range(c_rows):
        b_matrix.append([])
        for j in range(c_cols):
            if p_matrix[i][j]:
                b_matrix[i].append(True)
            else:
                b_matrix[i].append(False)
    return b_matrix


class GameInfo(BaseModel):
    id: int
    name: str


class GameList(BaseModel):
    games: list[GameInfo]


class UserAction(BaseModel):
    action_id: int
    hold: bool


class GameState(BaseModel):
    field: list[list[bool]]
    next: list[list[bool]]
    score: int
    high_score: int
    level: int
    speed: int
    pause: bool


class ErrorMessage(BaseModel):
    message: str
