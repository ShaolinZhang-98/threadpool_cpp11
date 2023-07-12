#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include "ThreadPool.h"
using namespace std;

mutex mut;
int func(int x) {
    auto now = time(nullptr);
    auto dateTime = localtime(&now);
    mut.lock(); // 为了防止打印错乱，我们在这里加锁
    cout << "任务编号：" << x << " 执行线程ID: " << this_thread::get_id() << " 当前时间: " << dateTime->tm_min << ":" << dateTime->tm_sec << endl;
    mut.unlock();
    this_thread::sleep_for(chrono::seconds(2));
    return x;
}


int main() {
    ThreadPool threadPool;
    for (int i = 0; i < 20; ++i) auto ret = threadPool.commit(func, i);
    this_thread::sleep_for(chrono::seconds(20));  // 主线程等待，因为现在子线程是脱离状态，如果主线程关闭，则看不到打印
}