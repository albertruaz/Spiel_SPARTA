#include <iostream>
#include <memory>
#include <tuple>
#include <ctime>
// #include "open_spiel/spiel.h"
#include "open_spiel/games/tiny_hanabi/tiny_hanabi.h"  // tiny_hanabi 사용


#include "BotFactory.h"
// #include "PyBot.h"
#include "SearchBot.h"


using namespace Hanabi;


struct Statistics {
    int games;
    int totalScore;
    int scoreDistribution[26];
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
}

void eval_bot() {

    std::string botname = "SimpleBot";
    int player_num = 2;
    int games = 1000;
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
        int score = server.runGame(*botFactory, player_num);
        std::cout << "Score " << i << " : " << score << std::endl;
        
        stats.games++;
        stats.totalScore += score;
        stats.scoreDistribution[score] += 1;
        if (i % log_every == 0) dump_stats(botname, stats);
    }
    dump_stats(botname, stats);
}

int main(){
    eval_bot();
    return 0;
}

