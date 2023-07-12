#include "ThreadPool.h"

ThreadPool::ThreadPool(int threadNum) : taskQueue(std::make_unique<TaskQueue>(), shutdown(false), busyThreadNum(0) {
    this->threadNum.store(threadNum);
    for (int i = 0; i < this->threadNum; ++i) {
        threadVec.push_back(std::make_shared<std::thread>(&ThreadPool::worker, this)); // �����߳�
        threadVec.back()->detach();    // �����̺߳�detach�������߳�����
    }
}

ThreadPool::~ThreadPool() {
    shutdown = true;  // �ȴ��߳�ִ���꣬�Ͳ���ȥ����ȡ����
}

void ThreadPool::worker() {
    while (!shutdown) {
        std::unique_lock<std::mutex> uniqueLock(threadPoolMutex);
        notEmptyCondVar.wait(uniqueLock, [this] { return !taskQueue->empty() || shutdown; });  // �������Ϊ�գ������ڴˣ���������в��ǿջ����̳߳عر�ʱ������ִ��
        auto currTask = std::move(taskQueue->takeTask());  // ȡ������
        uniqueLock.unlock();
        ++busyThreadNum;
        currTask();  // ִ������
        --busyThreadNum;
    }
}