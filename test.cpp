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
    mut.lock(); // Ϊ�˷�ֹ��ӡ���ң��������������
    cout << "�����ţ�" << x << " ִ���߳�ID: " << this_thread::get_id() << " ��ǰʱ��: " << dateTime->tm_min << ":" << dateTime->tm_sec << endl;
    mut.unlock();
    this_thread::sleep_for(chrono::seconds(2));
    return x;
}


int main() {
    ThreadPool threadPool;
    for (int i = 0; i < 20; ++i) auto ret = threadPool.commit(func, i);
    this_thread::sleep_for(chrono::seconds(20));  // ���̵߳ȴ�����Ϊ�������߳�������״̬��������̹߳رգ��򿴲�����ӡ
}