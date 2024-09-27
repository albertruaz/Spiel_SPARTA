// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#include <tuple>
#include <torch/extension.h>
#include <ctime>

#include "BotFactory.h"
#include "PyBot.h"
#include "SearchBot.h"

using namespace Hanabi;

// Removed GUI code

////////////////////////////////////////////////////////////////////////////////
// Test harness code
////////////////////////////////////////////////////////////////////////////////

struct Statistics {
    int games;
    int totalScore;
    int scoreDistribution[26];
    int mulligansUsed[4];
};

static void dump_stats(std::string botname, Statistics stats)
{
    const double dgames = stats.games;
    const int perfectGames = stats.scoreDistribution[25];

    std::cout << "Over " << stats.games << " games, " << botname << " scored an average of "
              << (stats.totalScore / dgames) << " points per game.\n";
    if (perfectGames != 0) {
        const double winRate = 100*(perfectGames / dgames);
        std::cout << "  " << winRate << " percent were perfect games.\n";
    }
    if (stats.mulligansUsed[0] != stats.games) {
        std::cout << "  Mulligans used: 0 (" << 100*(stats.mulligansUsed[0] / dgames)
                  << "%); 1 (" << 100*(stats.mulligansUsed[1] / dgames)
                  << "%); 2 (" << 100*(stats.mulligansUsed[2] / dgames)
                  << "%); 3 (" << 100*(stats.mulligansUsed[3] / dgames) << "%).\n";
    }
}

void eval_bot(
  std::string botname,
  int players,
  int games,
  int log_every,
  int seed
) {
    // special case slurm runs
    if (seed < 0 && std::getenv("SLURM_PROCID")) {
      // CAREFUL! make sure this doesn't wrap around and become negative
      seed = (std::stol(std::getenv("SLURM_JOBID")) + std::stol(std::getenv("SLURM_PROCID")) * 102797) % 1000000000;
      printf("Set seed from slurm to %d\n", seed);
    }
    if (seed <= 0) {
        std::srand(std::time(NULL));
        seed = std::rand();
    }
    printf("--seed %d\n", seed);

    Statistics stats = {};

    Hanabi::Server server;
    server.setLog(&std::cerr);
    auto botFactory = getBotFactory(botname);

    server.srand(seed);

    for (int i=0; i < games; ++i) {
        int score = server.runGame(*botFactory, players);
        std::cout << "Final score " << i << " : " << score << " bomb: "
                  << (server.mulligansRemaining() == 0 ? 1 : 0) << std::endl;
        assert(score == server.currentScore());
        assert(0 <= server.mulligansUsed() && server.mulligansUsed() <= 3);
        stats.games++;
        stats.totalScore += score;
        stats.scoreDistribution[score] += 1;
        stats.mulligansUsed[server.mulligansUsed()] += 1;

        if (i % log_every == 0) {
          dump_stats(botname, stats);
        }
    }
    dump_stats(botname, stats);

    Hanabi::getThreadPool().close();
}



PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {

  // test harness code
  // FIXME: make params configurable not just through env vars
  m.def("eval_bot", &eval_bot,
      py::arg("botname"),
      py::arg("players")=2,
      py::arg("games")=1000,
      py::arg("log_every")=100,
      py::arg("seed")=-1
  );

  // GUI interface code
  m.def("start_game", &start_game, py::return_value_policy::reference,
      py::arg("botname"),
      py::arg("seed")=-1);
  m.def("end_game", &end_game);
  m.def("get_botname", &get_botname);
  m.def("get_search_thresh", &get_search_thresh);
  m.def("set_search_thresh", &set_search_thresh);


  py::class_<Server, std::shared_ptr<Server>>(m, "HanabiServer")
    .def("whoAmI", &Server::whoAmI)
    .def("activeCard", &Server::activeCard)
    .def("piles", &server_piles)
    .def("discards", &Server::discards)
    .def("hintStonesRemaining", &Server::hintStonesRemaining)
    .def("mulligansRemaining", &Server::mulligansRemaining)
    .def("cardsRemainingInDeck", &Server::cardsRemainingInDeck)
    .def("finalCountdown", &Server::finalCountdown)
    .def("gameOver", &Server::gameOver)
    .def("currentScore", &Server::currentScore)
    .def("handSize", &Server::handSize)
    .def("sizeOfHandOfPlayer", &Server::sizeOfHandOfPlayer)
    .def("handOfPlayer", &Server::cheatGetHand)
    .def("cardIdsOfHandOfPlayer", &Server::cardIdsOfHandOfPlayer)
    .def("getCurrentDeckComposition", &getCurrentDeckComposition)
    .def("activePlayer", &Server::activePlayer)
    .def_readonly("seed", &Server::seed_)
  ;

  py::class_<Card>(m, "Card")
    .def_readwrite("color", &Card::color)
    .def_readwrite("value", &Card::value)
    .def("__repr__", &Card::toString)
  ;

  py::class_<Move>(m, "Move")
    .def(py::init<MoveType, int, int>(),
      py::arg("move_type"),
      py::arg("value"),
      py::arg("to")=-1)
    .def_readwrite("type", &Move::type)
    .def_readwrite("value", &Move::value)
    .def_readwrite("to", &Move::to)
    .def("__repr__", &Move::toString)
  ;

  py::class_<PyBot>(m, "PyBot")
    .def("wait", &PyBot::wait)
    .def("obs", [](PyBot& bot) { return bot.obs_; })
    .def("obs_indices", &pybot_indices)
    .def("getCardKnowledge", &pybot_get_card_knowledge)
    .def("makeMove", &PyBot::setMove)
    .def_readonly("ready", &PyBot::py_wakeup_)
    .def_readonly("move_history_", &PyBot::move_history_)
  ;

  py::enum_<Color>(m, "Color", py::arithmetic())
    .value("RED", Color::RED)
    .value("ORANGE", Color::ORANGE)
    .value("YELLOW", Color::YELLOW)
    .value("GREEN", Color::GREEN)
    .value("BLUE", Color::BLUE)
    .export_values();
  ;

  py::enum_<Value>(m, "Value", py::arithmetic())
    .export_values()
  ;

  py::enum_<MoveType>(m, "MoveType", py::arithmetic())
    .value("PLAY_CARD", MoveType::PLAY_CARD)
    .value("DISCARD_CARD", MoveType::DISCARD_CARD)
    .value("HINT_COLOR", MoveType::HINT_COLOR)
    .value("HINT_VALUE", MoveType::HINT_VALUE)
    .export_values()
  ;

  py::class_<std::thread>(m, "thread");

}
