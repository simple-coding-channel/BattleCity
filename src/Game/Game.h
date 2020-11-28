#pragma once

#include <glm/vec2.hpp>
#include <array>
#include <memory>

class Tank;
class Level;
class StartScreen;

class Game {
public:
    Game(const glm::ivec2& windowSize);
    ~Game();

    void render();
    void update(const double delta);
    void setKey(const int key, const int action);
    bool init();
    unsigned int getCurrentWidth() const;
    unsigned int getCurrentHeight() const;

private:
    enum class EGameState {
        StartScreen,
        LevelStart,
        Level,
        Pause,
        Scores,
        GameOver
    };

    std::array<bool, 349> m_keys;

    glm::ivec2 m_windowSize;
    EGameState m_eCurrentGameState;
    std::shared_ptr<Tank> m_pTank;
    std::shared_ptr<Level> m_pLevel;

    std::shared_ptr<StartScreen> m_pStartScreen;
};