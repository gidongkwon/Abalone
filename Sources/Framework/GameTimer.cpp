#include "pch.h"
#include "GameTimer.h"

GameTimer::GameTimer(bool start):
	paused_(false)
{
	pause_duration_ = duration::zero();

	if (start)
		reset();
}

float GameTimer::total_time() const
{
	duration d = clock::now() - start_point_ - pause_duration_;
	return d.count();
}

float GameTimer::delta_time() const
{
	return delta_time_.count();
}

float GameTimer::fps() const
{
	return 1.f / delta_time();
}

void GameTimer::update()
{
	if (paused_)
	{
		if (delta_time_.count() != 0)
			delta_time_ = duration::zero();
		return;
	}

	current_point_ = clock::now();
	delta_time_ = current_point_ - prev_point_;
	prev_point_ = current_point_;
}

void GameTimer::pause()
{
	paused_ = true;
	latest_pause_point_ = clock::now();
}

void GameTimer::unpause()
{
	paused_ = false;
	pause_duration_ += latest_pause_point_ - clock::now();
}

void GameTimer::reset()
{
	start_point_ = clock::now();
}
