#pragma once

class IGameState {
public:
    virtual void render() const = 0;
    virtual void update(const double delta) = 0;
    virtual ~IGameState() = default;

    virtual unsigned int getStateWidth() const = 0;
    virtual unsigned int getStateHeight() const = 0;
};
