#include <iostream>
#include <memory>
#include <tuple>
#include <ctime>
// #include "open_spiel/spiel.h"
#include "open_spiel/games/tiny_hanabi/tiny_hanabi.h"  // tiny_hanabi 사용


#include "BotFactory.h"
#include "PyBot.h"
#include "SearchBot.h"


using namespace Hanabi;


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

void eval_bot() {

    std::string botname = "SimpleBot";
    int players = 2;
    int games = 100;
    int log_every = 100;
    int seed = 100;
    // if (seed <= 0) {
    //     std::srand(std::time(NULL));
    //     seed = std::rand();
    // }

    Statistics stats = {};

    Hanabi::Server server;
    server.setLog(&std::cerr); // Print Error
    
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

    // Hanabi::getThreadPool().close();
}

int main(){
    eval_bot();
    return 0;
}

