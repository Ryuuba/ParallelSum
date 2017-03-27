#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdlib>
#include <chrono>

using std::thread;
using std::cout;
using std::endl;
using std::mutex;
using std::cin;
using std::vector;
using std::chrono::system_clock;
using std::chrono::microseconds;
using std::chrono::seconds;
using std::chrono::duration_cast;

mutex mx;

void sum(vector<unsigned>& base, size_t start, size_t n, unsigned& result) {
    unsigned acum = 0;
    for (size_t i = start; i < start+n; i++)
        acum += base[i];
    mx.lock();
    result+=acum;
    mx.unlock();
}

int main(int argc, char* argv[]){
    unsigned n = thread::hardware_concurrency(), size = atoi(argv[1]), result = 0;
    vector<unsigned> data;
    vector<thread> thread_pool;
    for(unsigned i = 1; i <= size; i++) 
        data.push_back(i);
    unsigned portion = data.size()/n;
    auto start = system_clock::now();
    for (unsigned i = 0; i < n; i++)
        thread_pool.push_back(thread(sum, std::ref(data), i*portion, portion, std::ref(result)));
    for (auto& th : thread_pool)
        th.join();
    cout << "Concurrent result: " << result << endl;
    auto elapsed = system_clock::now() - start;
    cout << "Concurrent time: " 
        << duration_cast<microseconds>(elapsed).count()
        << " u " << endl;
    
    result = 0;
    start = system_clock::now();
    for (unsigned i = 1; i <= size; i++)
        result += i;
    cout << "Sequential result: " << result << endl;
    elapsed = system_clock::now() - start;
    cout << "Sequential time: " 
        << duration_cast<microseconds>(elapsed).count()
        << " u " << endl;
    return 0;
}