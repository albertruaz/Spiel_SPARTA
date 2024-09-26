#ifndef H_CUSTOM_SERVER
#define H_CUSTOM_SERVER

#include <cassert>
#include <string>
#include <ostream>
#include <random>
#include <vector>
#include <tuple>
#include "ThreadPool.h"
#include <future>

namespace Params {
  std::string getParameterString(
    const std::string &name,
    std::string default_val,
    const std::string help=""
  );
  int getParameterInt(
    const std::string &name,
    int default_val,
    const std::string help=""
  );
  float getParameterFloat(
    const std::string &name,
    float default_val,
    const std::string help=""
  );
}

namespace CustomParams {
  // Example parameters
  // const int CUSTOM_PARAM1 = Params::getParameterInt("CUSTOM_PARAM1", 0, "Custom parameter 1");;
}

// Declare Class
namespace CustomGame { 
    class Server;
    class Bot;
    class BotFactory;
}

namespace CustomGame {

inline ThreadPool &getThreadPool() {
  static std::shared_ptr<ThreadPool> pool;
  if (!pool || pool->stop) {
    pool.reset(new ThreadPool(CustomParams::CUSTOM_THREADS));
  }
  return *pool;
}

class ServerError : public std::runtime_error {
public:
  explicit ServerError(const std::string& what_arg) : runtime_error(what_arg) {}
};

// CustomGame에 사용될 기본 타입 및 상수
// typedef enum: int8_t { TYPE_A=0, TYPE_B, TYPE_C, INVALID_TYPE } CustomType;
// constexpr int NUM_TYPES = 3;
// constexpr int CUSTOM_INFOS = 5;

class Server {
public:
    Server();
    virtual ~Server() {}

    /* Set up logging for debugging */
    void setLog(std::ostream *logStream);

    /* Seed the random number generator */
    void srand(unsigned int seed);

    /* Start a new game with a given number of players */
    int runGame(int numPlayers);

    /* Returns the number of players in the game */
    int numPlayers() const;

    /* Returns TRUE if the game is over */
    bool gameOver() const;

    /* Returns the current score */
    int currentScore() const;

    /*================= MUTATORS =============================*/

    /* Example of a custom action */
    virtual void customAction(int index);

    /*================= DEBUGGING TOOLS ======================*/

    std::string customStateAsString() const;

protected:
    std::ostream *log_;
    std::mt19937 rand_;
    int numPlayers_;
    int activePlayer_;
    int score_;
    bool gameEnded_;
    int livesRemaining_;

    /* Private methods */
    void drawCard();
    void loseLife();
    void logState_() const;

    // TODO: 여기에 추가적인 게임 메커니즘 작성
};

}  /* namespace CustomGame */


namespace CustomGame {
  
class Bot {
public:
    virtual ~Bot();
    virtual void pleaseObserveBeforeMove(const Server &) = 0;
    virtual void pleaseMakeMove(Server &) = 0;
    virtual void pleaseObserveBeforeDiscard(const Server &, int from, int card_index) = 0;
    virtual void pleaseObserveBeforePlay(const Server &, int from, int card_index) = 0;
    virtual void pleaseObserveColorHint(const Server &, int from, int to, Color color, CardIndices card_indices) = 0;
    virtual void pleaseObserveValueHint(const Server &, int from, int to, Value value, CardIndices card_indices) = 0;
    virtual void pleaseObserveAfterMove(const Server &) = 0;
    
    virtual Bot *clone() const { throw std::runtime_error("Not implemented."); }
    virtual void setPermissive(bool permissive) { permissive_ = permissive; }
    virtual const std::map<int, float> &getActionProbs() const { throw std::runtime_error("Not implemented."); }
    virtual void setActionUncertainty(float action_unc) { throw std::runtime_error("Not implemented."); }

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
void registerBotFactory(std::string name, std::shared_ptr<Hanabi::BotFactory> factory);
std::shared_ptr<Hanabi::BotFactory> getBotFactory(const std::string &botName);

}


#endif /* H_CUSTOM_SERVER */
