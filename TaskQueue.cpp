#include "TaskQueue.h"

/**
 * �����������ȡ����
 * @return ȡ��������
 */
TaskQueue::Task TaskQueue::takeTask() {
    Task task;
    std::lock_guard<std::mutex> lockGuard(taskQueueMutex);  // ����
    if (!taskQueue.empty()) {
        task = std::move(taskQueue.front());    // ȡ������
        taskQueue.pop();  // ������Ӷ�����ɾ��
        return task;
    }
    return nullptr;
}