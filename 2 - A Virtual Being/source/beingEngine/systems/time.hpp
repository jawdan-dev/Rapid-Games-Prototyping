#pragma once

class Time {
public:
	static Time* s_time;

	Time();

	const float getDelta() const;
	const float getNow() const;

	const void update(const float time);

private:
	float m_now, m_last;
	float m_deltaTime;
};