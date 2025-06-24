#pragma once
#include <vector>
#include <thread>
class SimpleThreadPool
{
private:
	std::vector<std::thread> threads;

public:
	template<typename Function, typename... Args>
	void addTask(Function&& func, Args&&... args) {
		threads.emplace_back(std::forward<Function>(func),
			std::forward<Args>(args)...);
	}

	void waitAll() {
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
		threads.clear();
	}
};

