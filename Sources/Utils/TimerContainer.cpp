#include "pch.h"
#include "TimerContainer.h"

TimerContainer::TimerContainer(int timer_num):
	timer_pool_(timer_num, 10)
{
}

void TimerContainer::update(float dt)
{
	for (auto& timer : active_timers_)
		timer->update(dt);

	remove_finished_timers();
}

TickTimer& TimerContainer::get()
{
	auto* timer = timer_pool_.get();

	active_timers_.push_back(timer);
	return *timer;
}

void TimerContainer::remove_finished_timers()
{
	active_timers_.erase(std::remove_if(active_timers_.begin(), active_timers_.end(), [&](TickTimer * timer)
	{
		bool need_erase = timer->is_finished();
		if (need_erase)
			timer_pool_.destroy(timer);

		return need_erase;
	}), active_timers_.end());
}
