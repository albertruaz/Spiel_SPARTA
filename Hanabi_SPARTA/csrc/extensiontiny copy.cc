// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#include <tuple>
#include <ctime>
#include "open_spiel/spiel.h"
#include "open_spiel/games/tiny_hanabi/tiny_hanabi.h"  // tiny_hanabi를 사용
#include <torch/extension.h>

struct Statistics {
    int games;
    int totalScore;
    int scoreDistribution[26];
    int mulligansUsed[4];
};

static void dump_stats(std::string botname, Statistics stats) {
    const double dgames = stats.games;
    const int perfectGames = stats.scoreDistribution[25];

    std::cout << "Over " << stats.games << " games, " << botname << " scored an average of "
              << (stats.totalScore / dgames) << " points per game.\n";
    if (perfectGames != 0) {
        const double winRate = 100 * (perfectGames / dgames);
        std::cout << "  " << winRate << " percent were perfect games.\n";
    }
    if (stats.mulligansUsed[0] != stats.games) {
        std::cout << "  Mulligans used: 0 (" << 100 * (stats.mulligansUsed[0] / dgames)
                  << "%); 1 (" << 100 * (stats.mulligansUsed[1] / dgames)
                  << "%); 2 (" << 100 * (stats.mulligansUsed[2] / dgames)
                  << "%); 3 (" << 100 * (stats.mulligansUsed[3] / dgames) << "%).\n";
    }
}

void eval_bot(
  std::string botname,
  int players,
  int games,
  int log_every,
  int seed
) {
    if (seed <= 0) {
        std::srand(std::time(nullptr));
        seed = std::rand();
    }
    std::cout << "--seed " << seed << std::endl;

    Statistics stats = {};

    // Create the TinyHanabi game and state
    std::shared_ptr<const open_spiel::Game> game = open_spiel::LoadGame("tiny_hanabi");  // tiny_hanabi 사용
    std::unique_ptr<open_spiel::State> state;

    for (int i = 0; i < games; ++i) {
        // Reset the game state for a new game
        state = game->NewInitialState();

        while (!state->IsTerminal()) {
            // Play random moves (replace with actual bot logic later)
            auto actions = state->LegalActions();
            state->ApplyAction(actions[0]);
        }

        int score = state->Returns()[0]; // Get the game score
        std::cout << "Final score " << i << " : " << score << std::endl;

        stats.games++;
        stats.totalScore += score;
        stats.scoreDistribution[score] += 1;
        // Note: Mulligans logic can be added based on specific TinyHanabi rules

        if (i % log_every == 0) {
            dump_stats(botname, stats);
        }
    }

    dump_stats(botname, stats);
}

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
    m.def("eval_bot", &eval_bot,
        py::arg("botname"),
        py::arg("players")=2,
        py::arg("games")=1000,
        py::arg("log_every")=100,
        py::arg("seed")=-1
    );
}
