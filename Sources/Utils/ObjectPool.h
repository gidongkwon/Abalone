#pragma once
#include <vector>
#include <list>

// Simple object pool using std::vector.
// Constructor of T has to recieve no argument.
template <class T>
class ObjectPool {
public:
	ObjectPool(int initial_size, int grow_size):
		grow_size_(grow_size)
	{
		grow(initial_size);
	}

	T* get()
	{
		if (available_.empty())
			grow(grow_size_);
		T* first_available = available_.front();
		available_.pop_front();
		return first_available;
	}

	void destroy(T* object)
	{
		available_.push_back(object);
	}

private:
	void grow(int grow_size)
	{
		size_t actual_size_need = std::min(objects_.size(), objects_.capacity()) + grow_size;
		objects_.reserve(actual_size_need);

		for (int i = 0; i < grow_size; i++)
		{
			objects_.emplace_back();
			available_.push_back(&objects_.back());
		}
	}

	std::vector<T> objects_;
	std::list<T*> available_;

	int grow_size_;
};