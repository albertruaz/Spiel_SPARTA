// main.cpp
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <ctime>

// Include OpenSpiel headers
#include "open_spiel/spiel.h"
#include "open_spiel/games/tiny_hanabi/tiny_hanabi.h"



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

int main() {
    std::mt19937 rng(time(0));  // Random seed

    // Load Tiny Hanabi game
    std::shared_ptr<const open_spiel::Game> game = open_spiel::LoadGame("tiny_hanabi");
    if (!game) {std::cerr << "Failed to load Tiny Hanabi game.\n"; return 1;}

    // Number of players and hand size can be configured as needed
    int numPlayers = game->NumPlayers();
    int handSize = 5; // Example hand size

    

    // Initialize the game state
    std::unique_ptr<open_spiel::State> state = game->NewInitialState();
    std::shared_ptr<Server> server = std::make_shared<Server>(state->Clone());

    std::cout << "Starting Tiny Hanabi game...\n";

    // Game loop
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
            server = std::make_shared<Server>(state->Clone());
        } else {
            int player = state->CurrentPlayer();
            if (player == open_spiel::kChancePlayerId) {
                std::cerr << "Unexpected chance node.\n";
                break;
            }

            if (player >= bots.size()) {
                std::cerr << "Invalid player index: " << player << ".\n";
                break;
            }

            Bot* bot = bots[player];
            bot->pleaseObserveBeforeMove(*server);
            bot->pleaseMakeMove(*server);
            bot->pleaseObserveAfterMove(*server);

            // Update the game state based on the bot's action
            // Assuming the bot's pleaseMakeMove has already applied the action
            // If not, you need to retrieve the action from the bot and apply it here

            // For this example, we'll assume the bot modifies the server's state directly
            state = server->GetState()->Clone();
        }

        // Optionally, print the current state for debugging
        // std::cout << state->ToString() << "\n";
    }

    auto returns = state->Returns();
    for (open_spiel::Player p = 0; p < game->NumPlayers(); ++p) {
        std::cout << "Final return for player " << p << ": " << returns[p] << "\n";
    }

    return 0;
}
