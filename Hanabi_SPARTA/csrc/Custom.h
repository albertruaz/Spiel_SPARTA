// CustomBot.h
#ifndef CUSTOM_BOT_H
#define CUSTOM_BOT_H

#include "Bot.h"
#include "Server.h"
#include <random>
#include <iostream>

namespace Hanabi {

class CustomBot : public Bot {
public:
    CustomBot(int index, int numPlayers, int handSize)
        : index_(index), numPlayers_(numPlayers), handSize_(handSize), rng_(std::random_device{}()) {}

    virtual ~CustomBot() override {}

    virtual void pleaseObserveBeforeMove(const Server &server) override {
        // Implement any observation logic before making a move
        std::cout << "Bot " << index_ << " observing before move.\n";
    }

    virtual void pleaseMakeMove(Server &server) override {
        // Implement the logic to make a move based on the current server state
        std::cout << "Bot " << index_ << " making a move.\n";
        auto actions = server.LegalActions(index_);
        if (actions.empty()) {
            std::cout << "No legal actions available for Bot " << index_ << ".\n";
            return;
        }
        // For demonstration, select a random action
        std::uniform_int_distribution<> dis(0, actions.size() - 1);
        auto action = actions[dis(rng_)];
        server.ApplyAction(action);
        std::cout << "Bot " << index_ << " applied action " << action << ".\n";
    }

    virtual void pleaseObserveBeforeDiscard(const Server &server, int from, int card_index) override {
        // Implement logic to observe before discarding a card
        std::cout << "Bot " << index_ << " observing before discard.\n";
    }

    virtual void pleaseObserveBeforePlay(const Server &server, int from, int card_index) override {
        // Implement logic to observe before playing a card
        std::cout << "Bot " << index_ << " observing before play.\n";
    }

    virtual void pleaseObserveColorHint(const Server &server, int from, int to, Color color, CardIndices card_indices) override {
        // Implement logic to handle color hints
        std::cout << "Bot " << index_ << " received a color hint.\n";
    }

    virtual void pleaseObserveValueHint(const Server &server, int from, int to, Value value, CardIndices card_indices) override {
        // Implement logic to handle value hints
        std::cout << "Bot " << index_ << " received a value hint.\n";
    }

    virtual void pleaseObserveAfterMove(const Server &server) override {
        // Implement any observation logic after making a move
        std::cout << "Bot " << index_ << " observing after move.\n";
    }

    virtual Bot* clone() const override {
        return new CustomBot(*this);
    }

private:
    int index_;
    int numPlayers_;
    int handSize_;
    mutable std::mt19937 rng_;
};

} // namespace Hanabi

#endif // CUSTOM_BOT_H
