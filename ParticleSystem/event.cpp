#include "event.h"

template<typename ...Args>
void Event<Args...>::operator+=(std::function<void(Args...)> listener)
{
	listeners.push_back(listener);
}

template<typename ...Args>
void Event<Args...>::subscribe(std::function<void(Args...)> listener)
{
	listeners.push_back(listener);
}

template<typename ...Args>
void Event<Args...>::operator()(Args ...args)
{
	for (auto& i : listeners)
	{
		i(args...);
	}
}

template<typename ...Args>
void Event<Args...>::trigger(Args ...args)
{
	(*this)(args...);
}

template<typename ...Args>
bool Event<Args...>::hasLiteners()
{
	return !listeners.empty();
}

template<typename ...Args>
void Event<Args...>::clean()
{
	listeners.clear();
}
