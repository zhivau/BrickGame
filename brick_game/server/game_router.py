from fastapi import APIRouter, Request
from fastapi.responses import JSONResponse
from components import (
    ErrorMessage,
    GameList,
    GameInfo,
    UserAction,
    GameState,
    GameInfoC,
    convert_p_matrix_to_b_matrix,
)
import sys
import os
import ctypes

sys.path.append(os.path.abspath(os.path.join(sys.path[0], "../../")))
from brick_game.race.lib_race import LibRace


game_router = APIRouter()
game_list = GameList(
    games=[
        GameInfo(id=0, name="Tetris"),
        GameInfo(id=1, name="Snake"),
        GameInfo(id=2, name="Race"),
    ]
)
current_game_id = None
current_lib = None


@game_router.get(
    "/games",
    tags=["BrickGame 3.0 REST API"],
    summary="Получение списка доступных игр",
    response_model=GameList | ErrorMessage,
)
def get_available_games():
    return JSONResponse(
        content=[game.dict() for game in game_list.games], status_code=200
    )


@game_router.post(
    "/games/{game_id}",
    tags=["BrickGame 3.0 REST API"],
    summary="Выбор игры",
    response_model=ErrorMessage,
)
def choose_game(game_id: int):
    global current_game_id
    if game_id not in [game.id for game in game_list.games]:
        return JSONResponse(
            content=ErrorMessage(
                message="Игра с заданным идентификатором не найдена"
            ).dict(),
            status_code=404,
        )
    if current_game_id is not None:
        return JSONResponse(
            content=ErrorMessage(
                message="Пользователь уже запустил другую игру"
            ).dict(),
            status_code=409,
        )
    current_game_id = game_id

    global current_lib
    if game_id == 0:
        current_lib = ctypes.CDLL("./libtetris.so")

        current_lib.userInput.restype = None
        current_lib.userInput.argtypes = [ctypes.c_int, ctypes.c_bool]

        current_lib.updateCurrentState.restype = GameInfoC
        current_lib.updateCurrentState.argtypes = []
    elif game_id == 1:
        current_lib = ctypes.CDLL("./libsnake.so")

        current_lib.userInput.restype = None
        current_lib.userInput.argtypes = [ctypes.c_int, ctypes.c_bool]

        current_lib.updateCurrentState.restype = GameInfoC
        current_lib.updateCurrentState.argtypes = []
    elif game_id == 2:
        current_lib = LibRace()

    return JSONResponse(content={}, status_code=200)


@game_router.post(
    "/actions",
    tags=["BrickGame 3.0 REST API"],
    summary="Выполнить команду игрока",
    response_model=ErrorMessage,
)
def execute_user_action(action: UserAction):
    global current_game_id
    if current_game_id is None:
        return JSONResponse(
            content=ErrorMessage(
                message="Ошибка в теле запроса или пользователь не запустил игру"
            ).dict(),
            status_code=400,
        )

    global current_lib
    current_lib.userInput(action.action_id, action.hold)
    if action.action_id == 2:
        current_lib = None
        current_game_id = None
    return JSONResponse(content={}, status_code=200)


@game_router.get(
    "/state",
    tags=["BrickGame 3.0 REST API"],
    summary="Получение текущего состояния игры",
    response_model=GameState | ErrorMessage,
)
def get_game_state():
    global current_game_id
    if current_game_id is None:
        return JSONResponse(
            content=ErrorMessage(message="Пользователь не запустил игру").dict(),
            status_code=400,
        )

    global current_lib
    domain_state = current_lib.updateCurrentState()
    game_state = GameState(
        field=convert_p_matrix_to_b_matrix(domain_state.field, 20, 10),
        next=convert_p_matrix_to_b_matrix(domain_state.next, 4, 4),
        score=domain_state.score,
        high_score=domain_state.high_score,
        level=domain_state.level,
        speed=domain_state.speed,
        pause=bool(domain_state.pause),
    )
    return JSONResponse(content=game_state.dict(), status_code=200)
