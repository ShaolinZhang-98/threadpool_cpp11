#ifndef THREADPOOL_THREADPOOL_H
#define THREADPOOL_THREADPOOL_H

#include <atomic>
#include <thread>
#include <condition_variable>
#include "TaskQueue.h"

class ThreadPool {
    std::atomic<int> threadNum{};  // ��С�߳���
    std::atomic<int> busyThreadNum; // æ�߳���
    std::condition_variable notEmptyCondVar; // �ж���������Ƿ�ǿ�
    std::mutex threadPoolMutex; // �̳߳ػ�����
    bool shutdown;          // �̳߳��Ƿ�����
    std::unique_ptr<TaskQueue> taskQueue;  // �������
    std::vector<std::shared_ptr<std::thread>> threadVec;  // �̳߳�
public:
    explicit ThreadPool(int threadNum = 5);   // �����̳߳�
    ~ThreadPool();          // �����̳߳�

    template <typename F, typename ...Args>
    auto commit(F& f, Args &&...args) -> decltype(taskQueue->addTask(f, std::forward<Args>(args)...)); // �ύһ������
    void worker();
};

template <typename F, typename ...Args>  // �ɱ����ģ��
auto ThreadPool::commit(F& f, Args &&...args) -> decltype(taskQueue->addTask(f, std::forward<Args>(args)...)) {
    // ���Ŀ�ľ��ǰѽ��յĲ���ֱ��ת������������д��addTask�������������Ϳ��Զ�ʹ��������TaskQueue��ϸ�ڣ�ֻ���û���¶ThreadPool����
    auto ret = taskQueue->addTask(f, std::forward<Args>(args)...);
    notEmptyCondVar.notify_one();
    return ret;
}


#endif //THREADPOOL_THREADPOOL_H