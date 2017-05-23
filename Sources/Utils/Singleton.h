#pragma once

template<class T>
class Singleton
{
public:
	static T& get()
	{
		static T s_instance;
		return s_instance;
	}
};