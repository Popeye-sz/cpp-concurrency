#include <iostream>
#include <thread>

struct callee {
    int &var_i;
    callee(int &i) : var_i(i) {}

    void operator()() {
        for (int i = 0; i < 100; i++) {
            var_i += i;
            std::cout << var_i << std::endl;
        }
    }
};

class thread_guard {
    std::thread &t;
public:
    explicit thread_guard(std::thread &thread) : t(thread) {}
    ~thread_guard() {
        if (t.joinable()) {
            t.join();
        }
    }
    thread_guard(thread_guard const &guard) = delete;
    thread_guard& operator=(thread_guard const &guard) = delete;
};

int main() {
    int local_var = 0;
    callee func(local_var);
    std::thread t(func);
    {
        thread_guard guard(t);
    }
    std::cout << "before main end" << std::endl;

    // hardware concurrency
    std::cout << "hardware concurrency : " << std::thread::hardware_concurrency() << std::endl;

    // thread id
    std::cout << "thread id is : " << std::this_thread::get_id() << std::endl;
    return 0;
}
