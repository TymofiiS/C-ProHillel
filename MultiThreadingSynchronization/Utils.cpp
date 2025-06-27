#include <iosfwd>
#include <random>
#include <thread>
#include <string>
#include <sstream>

class Randomiser {
private:
    std::mt19937 _rng;
    std::uniform_int_distribution<std::mt19937::result_type> _dist6;
public:
    Randomiser(int start, int end) {
        std::random_device dev;
        _rng = std::mt19937(dev());
        _dist6 = std::uniform_int_distribution<std::mt19937::result_type>(start, end);
    }

    int randomInt() { return _dist6(_rng); }
};
