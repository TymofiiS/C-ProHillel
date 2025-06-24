#include <string>
#include <thread>
#include <sstream>
class Utils {
public:
    static long long factorial(int n)
    {
        // Base case - If n is 0 or 1, return 1
        if (n == 0 || n == 1) {
            return 1;
        }
        // Recursive case - Return n multiplied by
        // factorial of (n-1)

        return n * factorial(n - 1);
    }

    static long long sumSquares(int n)
    {
        long long res = 0;
        for (int i = 1; i <= n; i++) {
            res += i * i;
        }

        return res;
    }

    static long long fib(int n) {

        // If n is 1 or 0, then return n, 
        // works for 0th and 1st terms
        if (n <= 1)
            return n;

        // Recurrence relation to find 
        // the rest of the terms
        return fib(n - 1) + fib(n - 2);
    }
    
    static std::string getCurrentThredIdStr(bool addLabel = true) {
        auto myid = std::this_thread::get_id();
        std::stringstream ss;
        ss << myid;
        return (addLabel ? "Thread ": "") + ss.str();
    }

    static bool isPrime(int n) {

        // Since 1 is not prime return false
        if (n == 1)
            return false;

        // Check divisiblity with all number in
        // range [2, n - 1]
        for (int i = 2; i < n; i++) {

            // If the n is divisible by i,
            if (n % i == 0)
                return false;
        }

        // If no divisor is found
        return true;
    }

    static std::string primeNumbers(int n) {

        std::string res;
        for (int i = 1; i <= n; i++) {
            if (!isPrime(i)) { continue; }
            res += std::to_string(i) + "; ";
        }
        return "{ " + res + " }";
    }
};

