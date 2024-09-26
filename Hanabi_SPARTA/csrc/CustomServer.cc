#include <algorithm>
#include <cassert>
#include <ostream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include "custom.h" // 새로 만든 헤더 파일

#ifdef CUSTOM_SERVER_NDEBUG
#define CUSTOM_SERVER_ASSERT(x, msg) (void)0
#else
#define CUSTOM_SERVER_ASSERT(x, msg) do { if (!(x)) throw ServerError(msg); } while (0)
#endif

// namespace Params {
// std::string getParameterString(const std::string &name, std::string default_val, const std::string help) {
//   static std::map<std::string, std::string> memoized;
//   if (memoized.count(name)) {
//     return memoized.at(name);
//   }
//   char *val = getenv(name.c_str());
//   std::string ret = (val && std::string(val) != "") ? std::string(val) : default_val;
//   std::cerr << name << ": " << ret << std::endl;
//   if (help != "") {
//     std::cerr << "\t" << help << std::endl;
//   }
//   memoized[name] = ret;
//   return ret;
// }
// int getParameterInt(const std::string &name, int default_val, const std::string help) {
//   std::string val = getParameterString(name, std::to_string(default_val), help);
//   return stoi(val);
// }
// float getParameterFloat(const std::string &name, float default_val, const std::string help) {
//   std::string val = getParameterString(name, std::to_string(default_val), help);
//   return std::stof(val);
// }
// }

using namespace CustomParams;

namespace CustomGame {


Server::Server() : activePlayer_(0), finalCountdown_(0), score_(0) {
    // TODO: 게임 초기화 로직 추가
}

int Server::runGame(int numPlayers) {
    numPlayers_ = numPlayers;

    // TODO: 덱 초기화, 플레이어 상태 초기화 등 게임 준비 작업

    while (!gameOver()) {
        // TODO: 각 플레이어가 자신의 턴에 할 수 있는 동작 구현
        activePlayer_ = (activePlayer_ + 1) % numPlayers_;
    }

    return currentScore();
}

bool Server::gameOver() const {
    return finalCountdown_ >= numPlayers_;
}

int Server::currentScore() const {
    return score_;
}

int Server::drawCard() {
    // TODO: 덱에서 카드를 뽑는 로직 추가
    return 0;
}

void Server::endGame() {
    // TODO: 게임을 종료하는 로직 작성
}

}