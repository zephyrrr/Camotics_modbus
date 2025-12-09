#ifndef MULTITHREADQUEUE_H
#define MULTITHREADQUEUE_H

#include <queue>
#include <mutex>

template<typename T>
class MultiThreadQueue
{
public:
    static_assert(std::is_pointer<T>::value, "T must be a pointer type");

    void push(const T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        m_queue.push(item);
    }

    void pop() {
        std::lock_guard<std::mutex> lock(mutex_);
		if (m_queue.empty()) {
			return;
		}
        m_queue.pop();
    }

    T front() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (m_queue.empty()) {
            return nullptr;
        }
        return m_queue.front();
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mutex_);
        return m_queue.empty();
    }

	int size() {
		std::lock_guard<std::mutex> lock(mutex_);
		return m_queue.size();
	}
public:
    std::mutex mutex_;

private:
    std::queue<T> m_queue;
};

#endif
