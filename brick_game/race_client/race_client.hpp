#ifndef RACE_CLIENT_H
#define RACE_CLIENT_H

#include <string>
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "../library_specification.h"

void selectGame(int gameId);

extern "C" {
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
}

#endif  // GAME_CLIENT_H
