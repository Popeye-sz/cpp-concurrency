#include <iostream>
#include <map>
#include <thread>
#include <memory>
#include <shared_mutex>

// demo : mock dns find / add & udpate
class dns_entry {
public:
    std::string ip;
};

class dns_cache {
public:
    dns_entry find_entry(std::string const &domain) const {
        std::shared_lock<std::shared_mutex> r_lock(dns_mutex);
        auto it = cache.find(domain);
        if (it != cache.cend()) {
            return it->second;
        }
        return dns_entry();
    }

    void add_or_update_dns(std::string const &domain,
                           dns_entry const &entry) {
        std::lock_guard<std::shared_mutex> w_lock(dns_mutex);
        cache[domain] = entry;
    }
private:
    std::map<std::string, dns_entry> cache;
    mutable std::shared_mutex dns_mutex;
};

std::shared_ptr<dns_cache> cache_ptr;

void test1() {
    dns_entry entry = {};
    entry.ip = "111.111.222.333";
    cache_ptr->add_or_update_dns("www.baidu.com", entry);
}

void test2() {
    std::string domain = "www.baidu.com";
    dns_entry entry = cache_ptr->find_entry(domain);
    std::cout << "The ip of " << domain << " is " << entry.ip << std::endl;
}

int main() {
    cache_ptr.reset(new dns_cache);
    std::thread t1(test1);
    std::thread t2(test2);
    t1.join();
    t2.join();
    return 0;
}
