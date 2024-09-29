// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// This source code is licensed under the license found in the
// LICENSE file in the root directory of this source tree.

#ifndef H_HANABI_TINY_SERVER
#define H_HANABI_TINY_SERVER

#include <cassert>
#include <string>
#include <ostream>
#include <random>
#include <vector>
#include <tuple>
// #include "ThreadPool.h"
#include <future>


namespace TinyHanabi {

class Bot {
public:
    virtual ~Bot();  /* virtual destructor */
    virtual void pleaseObserveBeforeMove(const Server &) = 0;
    virtual void pleaseMakeMove(Server &) = 0;
      virtual void pleaseObserveBeforeDiscard(const Server &, int from, int card_index) = 0;
      virtual void pleaseObserveBeforePlay(const Server &, int from, int card_index) = 0;
      virtual void pleaseObserveColorHint(const Server &, int from, int to, Color color, CardIndices card_indices) = 0;
      virtual void pleaseObserveValueHint(const Server &, int from, int to, Value value, CardIndices card_indices) = 0;
    virtual void pleaseObserveAfterMove(const Server &) = 0;

    /* Return a copy of this bot with identical state.
     * The clone object is unmanaged, and must be deleted by the caller. */
    virtual Bot *clone() const { throw std::runtime_error("Not implemented."); }

    /* By default, Bots assume that they are playing with another copy of the
     * same Bot class, and may throw exceptions if that assumption is violated.
     * if permissive=true, then the Bot should degrade gracefully when 'confused'
     * by a partner that violates its assumptions. */
    virtual void setPermissive(bool permissive) { permissive_ = permissive; }

    /* hacks for playing TorchBot with humans */
    virtual const std::map<int, float> &getActionProbs() const {
      throw std::runtime_error("Not implemented.");
    }
    virtual void setActionUncertainty(float action_unc) {
      throw std::runtime_error("Not implemented.");
    }

protected:
    bool permissive_ = false;
};

class BotFactory {
public:
    virtual Bot *create(int index, int numPlayers, int handSize) const = 0;
    virtual void destroy(Bot *bot) const = 0;
    virtual ~BotFactory() = default;
};

// simple registration of BotFactory's by string key
void registerBotFactory(std::string name, std::shared_ptr<TinyHanabi::BotFactory> factory);
std::shared_ptr<TinyHanabi::BotFactory> getBotFactory(const std::string &botName);

}


#endif
