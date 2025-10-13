#pragma once
#include<vector>
#include<functional>

template<typename... Args>
class Event
{
private:
	std::vector<std::function<void(Args...)>> listeners;

public:
	//Subscribe to event - operator
	void operator +=(std::function<void(Args...)> listener)
	{
		listeners.push_back(listener);
	}

	//Subscribe to event - function
	void subscribe(std::function<void(Args...)> listener)
	{
		listeners.push_back(listener);
	}

	//Trigger event - operator
	void operator()(Args... args)
	{
		for (auto& i : listeners)
		{
			i(args...);
		}
	}

	//Trigger event - function
	void trigger(Args... args)
	{
		(*this)(args...);
	}


	bool hasLiteners()
	{
		return !listeners.empty();
	}

	void clean()
	{
		listeners.clear();
	}

};
