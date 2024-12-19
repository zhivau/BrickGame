#include "race_client.hpp"

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb,
                     std::string* response) {
  size_t totalSize = size * nmemb;
  response->append((char*)contents, totalSize);
  return totalSize;
}

void selectGame(int gameId) {
  CURL* curl = curl_easy_init();
  if (!curl) {
    throw std::runtime_error("Failed to initialize CURL");
  }

  std::string url = "http://localhost:8080/api/games/" + std::to_string(gameId);
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    curl_easy_cleanup(curl);
    throw std::runtime_error("Failed to make request: " +
                             std::string(curl_easy_strerror(res)));
  }

  curl_easy_cleanup(curl);
}

static GameInfo_t gameInfo;
static bool selectedGame = false;

void userInput(UserAction_t action, bool hold) {
  CURL* curl = curl_easy_init();
  if (!curl) throw std::runtime_error("Failed to initialize cURL");

  if (action == Start) {
    if (!gameInfo.field) {
      gameInfo.field = new int*[20];
      for (int i = 0; i < 20; i++) {
        gameInfo.field[i] = new int[10];
        for (int j = 0; j < 10; j++) {
          gameInfo.field[i][j] = 0;
        }
      }
    }
  }

  if (action == Terminate) {
    if (gameInfo.field) {
      for (int i = 0; i < 20; i++) {
        delete[] gameInfo.field[i];
      }
      delete[] gameInfo.field;
      gameInfo.field = nullptr;
    }

    selectedGame = false;
  }

  std::string url = "http://localhost:8080/api/actions";
  json actionJson = {{"action_id", action}, {"hold", hold}};

  std::string response;
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1L);

  std::string action_string = actionJson.dump();
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, action_string.c_str());

  struct curl_slist* headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    curl_easy_cleanup(curl);
    throw std::runtime_error("Failed to perform request: " +
                             std::string(curl_easy_strerror(res)));
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
}

GameInfo_t updateCurrentState() {
  if (!selectedGame) {
    selectGame(2);
    selectedGame = true;
  }

  CURL* curl = curl_easy_init();
  if (!curl) throw std::runtime_error("Failed to initialize cURL");

  std::string url = "http://localhost:8080/api/state";
  std::string response;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    curl_easy_cleanup(curl);
    throw std::runtime_error("Failed to perform request: " +
                             std::string(curl_easy_strerror(res)));
  }

  curl_easy_cleanup(curl);

  json responseJson = json::parse(response);

  if (gameInfo.field) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        if (responseJson["field"][i][j].get<bool>()) {
          gameInfo.field[i][j] = 1;
        } else {
          gameInfo.field[i][j] = 0;
        }
      }
    }
  }

  gameInfo.next = nullptr;
  gameInfo.score = responseJson["score"].get<int>();
  gameInfo.high_score = responseJson["high_score"].get<int>();
  gameInfo.level = responseJson["level"].get<int>();
  gameInfo.speed = responseJson["speed"].get<int>();
  gameInfo.pause = responseJson["pause"].get<bool>() ? 1 : 0;

  return gameInfo;
}
