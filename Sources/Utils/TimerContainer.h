#pragma once
#include "TickTimer.h"
#include "ObjectPool.h"

class TimerContainer
{
public:
	TimerContainer(int timer_num);

	void update(float dt);

	TickTimer& get();

private:
	void remove_finished_timers();

	ObjectPool<TickTimer> timer_pool_;
	std::vector<TickTimer*> active_timers_;
};