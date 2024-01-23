#pragma once

#include <list>
#include <functional>

namespace raid
{

template<typename ...Args>
class Delegate
{

public:

	using Function = std::function<void(Args...)>;
	
	void Register(Function f)
	{
		m_Functions.push_back(f);
	}

	void Unregister(const Function& f)
	{
		m_Functions.remove_if([&f](const Function& existing)
		{
			return existing.target_type() == f.target_type();
		});
	}

	void Invoke(Args... args)
	{
		for (const auto& f : m_Functions)
		{
			f(args...);
		}
	}

private:

	std::list<Function> m_Functions;
};

}