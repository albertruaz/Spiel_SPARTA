#include <iostream>
#include <memory>
#include <vector>
#include <random>

#include "open_spiel/spiel_utils.h"
#include "open_spiel/spiel.h"
#include "open_spiel/games/tiny_hanabi/tiny_hanabi.h"

// #include <pybind11/pybind11.h>

// Pybind11 네임스페이스 사용
// namespace py = pybind11;

// #include <torch/extension.h>


// Tiny Hanabi 게임을 실행하는 함수 정의
int main() { 
  std::mt19937 rng(time(0));  // 랜덤 시드

  // Tiny Hanabi 로드
  std::shared_ptr<const open_spiel::Game> game =
      open_spiel::LoadGame("tiny_hanabi");  // tiny_hanabi 로드

  if (!game) {
    std::cerr << "Failed to load Tiny Hanabi game.\n";
    return 0;
  }

  // 초기 상태 생성
  std::unique_ptr<open_spiel::State> state = game->NewInitialState();

  std::cout << "Starting Tiny Hanabi game...\n";

  // 게임 루프
  while (!state->IsTerminal()) {
    if (state->IsChanceNode()) {
      auto outcomes = state->ChanceOutcomes();
      std::vector<double> probabilities;
      for (const auto& outcome : outcomes) {
        probabilities.push_back(outcome.second);
      }
      std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
      int index = dist(rng);
      auto action = outcomes[index].first;
      state->ApplyAction(action);
    } else {
      open_spiel::Player player = state->CurrentPlayer();
      auto actions = state->LegalActions(player);
      // Legal actions 출력 생략 또는 필요 시 추가

      std::uniform_int_distribution<> dis(0, actions.size() - 1);
      auto action = actions[dis(rng)];
      // 액션 선택 및 출력 생략 또는 필요 시 추가

      state->ApplyAction(action);
    }

    // 현재 상태 출력 생략 또는 필요 시 추가
  }

  auto returns = state->Returns();
  for (open_spiel::Player p = 0; p < game->NumPlayers(); ++p) {
    std::cout << "Final return for player " << p << ": " << returns[p] << "\n";
  }
  return 0;
}

// // Pybind11 모듈 정의
// PYBIND11_MODULE(hanabi_lib_tiny, m) {
//     m.def("play_tiny_hanabi", &PlayTinyHanabi, "Play a game of Tiny Hanabi");
// }
