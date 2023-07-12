#ifndef THREADPOOL_TASKQUEUE_H
#define THREADPOOL_TASKQUEUE_H

#include <queue>
#include <functional>
#include <mutex>
#include <future>
#include <iostream>
#include <functional>
class TaskQueue {

public:
    using Task = std::function<void()>; // ������
    template<typename F, typename ...Args>
    auto addTask(F& f, Args &&...args) -> std::future<decltype(f(args...))>;  // �������
    Task takeTask(); // ȡ����
    bool empty() { return taskQueue.empty(); }

private:
    std::mutex taskQueueMutex;  // ������л�����
    std::queue<Task> taskQueue; // �������
};

template <typename F, typename ...Args> // �ɱ����ģ�壬ģ�������ͷ�ļ�����
auto TaskQueue::addTask(F& f, Args &&...args)-> std::future<decltype(f(args...))> {
    using RetType = decltype(f(args...));  // ��ȡ��������ֵ����
    // ��������װΪ���βε����� std::bind(f, std::forward<Args>(args)...)���������뺯������
    // packaged_task<RetType()>(std::bind(f, std::forward<Args>(args)...)); ���󶨲�����ĺ�����װΪֻ�з���ֵû���βε����������������ʹ��get_future�õ�future����Ȼ��future�������ͨ��get������ȡ����ֵ��
    // std::make_shared<std::packaged_task<RetType()>>(std::bind(f, std::forward<Args>(args)...)); ��������ָ�룬�뿪�������Զ�����
    auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(f, std::forward<Args>(args)...));
    std::lock_guard<std::mutex> lockGuard(taskQueueMutex);  // ����ʱ��������ֹ����߳�ͬʱ����
    // ��������װΪ�޷������β����ͣ�ͨ��lamdba���ʽ�����÷�װ��ĺ�����ע�⣬��ʱ����һ�����β��޷���ֵ�ĺ�������
    taskQueue.emplace([task] {(*task)(); });
    return task->get_future();
}


#endif //THREADPOOL_TASKQUEUE_H