#pragma once

class TickTimer
{
public:
	using callback = std::function<void()>;

	TickTimer();

	bool is_paused() const;
	bool is_finished() const;

	void update(float dt);
	void pause();
	void unpause();

	void once(float tick_time, callback on_tick);
	void repeat(float tick_time, int count, callback on_tick);

	void reset();

private:
	bool started_;
	bool paused_;
	bool finished_;

	int count_;
	float tick_time_;
	float elapsed_time_;
	
	callback on_tick_;
};