#include "time.hpp"

Time::Time() : m_now(0), m_last(0),
               m_deltaTime(0) {}

const float Time::getDelta() const {
    return m_deltaTime;
}
const float Time::getNow() const {
    return m_now;
}

const void Time::update(const float time) {
    m_last = m_now;
    m_now = time;
    m_deltaTime = m_now - m_last;

    if (m_deltaTime > 0.1f)
        m_deltaTime = 0.1f;
}