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
	void operator +=(std::function<void(Args...)> listener);

	//Subscribe to event - function
	void subscribe(std::function<void(Args...)> listener);

	//Trigger event - operator
	void operator()(Args... args);

	//Trigger event - function
	void trigger(Args... args);

	bool hasLiteners();

	void clean();
};
