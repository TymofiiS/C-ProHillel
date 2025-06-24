#include <vector>
#include <thread>
class SimpleThreadPool
{
private:
	std::vector<std::thread> threads;

public:
	explicit SimpleThreadPool(size_t numThreads) {
		threads.reserve(numThreads);
	}

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

	~SimpleThreadPool() {
		waitAll();
	}
};