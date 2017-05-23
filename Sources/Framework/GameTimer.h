#pragma once

// A simple timer provides delta time using std::chrono
class GameTimer
{
public:
	GameTimer(bool start = true);

	float total_time() const;
	float delta_time() const;
	float fps() const;

	void update();
	void pause();
	void unpause();
	void reset();

private:
	using clock = std::chrono::high_resolution_clock;
	using time_point = std::chrono::time_point<clock>;
	using duration = std::chrono::duration<float>;

	time_point start_point_;
	time_point latest_pause_point_;

	// Members for calculating delta time.
	time_point prev_point_;
	time_point current_point_;

	duration pause_duration_;
	duration delta_time_;

	bool paused_;
};