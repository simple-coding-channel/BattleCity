#pragma once

#include <vector>
#include <string>
#include <memory>

class IGameObject;

class Level {
public:
    Level(const std::vector<std::string>& levelDescription);
    void render() const;
    void update(const uint64_t delta);

private:
    size_t m_width = 0;
    size_t m_height = 0;

    std::vector<std::shared_ptr<IGameObject>> m_levelObjects;
};