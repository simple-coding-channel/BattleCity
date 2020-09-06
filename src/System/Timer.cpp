#include "Timer.h"

Timer::Timer()
    : m_isRunning(false)
    , m_timeLeft(0)
{

}

void Timer::update(const double delta)
{
    if (m_isRunning)
    {
        m_timeLeft -= delta;
        if (m_timeLeft <= 0)
        {
            m_isRunning = false;
            m_callback();
        }
    }
}

void Timer::start(const double duration)
{
    m_timeLeft = duration;
    m_isRunning = true;
}

void Timer::setCallback(std::function<void()> callback)
{
    m_callback = callback;
}