#include "pch.h"
#include "TickTimer.h"

TickTimer::TickTimer():
	started_(false), paused_(false), finished_(true),
	count_(0),
	tick_time_(0), elapsed_time_(0)
{
}

bool TickTimer::is_paused() const
{
	return paused_;
}

bool TickTimer::is_finished() const
{
	return finished_;
}

void TickTimer::update(float dt)
{
	if (!started_)
		return;

	elapsed_time_ += dt;

	if (elapsed_time_ >= tick_time_)
	{
		elapsed_time_ = 0;

		on_tick_();
		
		if (count_ > 0)
		{
			count_--;
		}
		else if (count_ == 0)
		{
			reset();
		}
	}
}

void TickTimer::pause()
{
	paused_ = true;
}

void TickTimer::unpause()
{
	paused_ = false;
}

void TickTimer::once(float tick_time, callback on_tick)
{
	repeat(tick_time_, 1, on_tick);
}

// To repeat endlessly, put negative value into count.
void TickTimer::repeat(float tick_time, int count, callback on_tick)
{
	tick_time_ = tick_time;
	count_ = count;
	on_tick_ = on_tick;

	started_ = true;
	paused_ = false;
	finished_ = false;

	elapsed_time_ = 0;
}

void TickTimer::reset()
{
	elapsed_time_ = 0;
	started_ = false;
	paused_ = false;
	finished_ = true;
}

