#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#define TASKQUEUE_MAX_COUNT 3
#define TASK_WAIT_MILLSECONDS 50

#include <thread>
#include <functional>
#include <cbang/log/Logger.h>
#include "utils/MultiThreadQueue.h"

template<typename T>
class TaskThread
{
public:
	TaskThread<T>::TaskThread(std::function<int(T*, TaskThread<T>*)> doTaskFunction, int tasksQueueCount = TASKQUEUE_MAX_COUNT) :
		m_doTaskFunction(doTaskFunction), m_tasksQueueCount(tasksQueueCount)
	{
		for (int i = 0; i < TASKQUEUE_MAX_COUNT; ++i) {
			LastRets[i] = -1;
		}
	}

	void startTaskThread() {
		//std::lock_guard<std::mutex> lock(mutex_);
		m_threadToContinue = true;
		if (m_thread.get_id() == std::thread::id()) {
			m_thread = std::thread([this]() { 
				taskThread(this); 
			});
		}
	}
	void stopTaskThread() {
		{
			//std::lock_guard<std::mutex> lock(mutex_);
			m_threadToContinue = false;
		}
		if (m_thread.joinable()) {
			m_thread.join();
			//m_thread.i
		}
	}
	void pauseTaskThread() { 
		//std::lock_guard<std::mutex> lock(mutex_);
		m_taskPause = true; 
	}
	void resumeTaskThread() {
		//std::lock_guard<std::mutex> lock(mutex_);
		m_taskPause = false; 
	}
	bool isTaskThreadPausing() { return m_taskPause; }

	void addTask(T* task, int idx = 0) { assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT);  m_tasks[idx].push(task); }
	size_t getTasksCnt(int idx = 0) { assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT); return m_tasks[idx].size(); }

	bool isTaskToContinue() { return m_taskToContinue && m_threadToContinue; }
	void clearTasks(int tasksQueueIdx) {
		m_taskToContinue = false;

		std::lock_guard<std::mutex> lock(mutex_);
		//while (m_taskDoing) {
		//	std::this_thread::sleep_for(std::chrono::milliseconds(TASK_WAIT_MILLSECONDS));
		//}
		while (!m_tasks[tasksQueueIdx].empty())
		{
			T* item = m_tasks[tasksQueueIdx].front();
			if (item == nullptr)
				continue;
			delete item;
			m_tasks[tasksQueueIdx].pop();
		}
		m_taskToContinue = true;
	}

	T* getCurrentTask(int idx = 0) { assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT); return m_tasks[idx].front(); }
	int LastRets[TASKQUEUE_MAX_COUNT];
private:
	int m_tasksQueueCount;
	MultiThreadQueue<T*> m_tasks[TASKQUEUE_MAX_COUNT];

	std::atomic<bool> m_threadToContinue = true;	// 是否继续线程，如果为false，则会退出线程
	std::atomic<bool> m_taskToContinue = true;	// 是否继续任务，如果为false，会不再执行task，但是也不会清空task
	std::atomic<bool> m_taskPause = false;		// 线程是否暂停
	//bool m_taskDoing = false;
	std::thread m_thread;
	//static void taskThread(TaskThread* parent);

	std::function<int(T*, TaskThread<T>*)> m_doTaskFunction;
	std::mutex mutex_;

	template<typename T>
	static void taskThread(TaskThread<T>* parent)
	{
		while (parent && parent->m_threadToContinue)
		{
			try
			{
				bool allEmpty = true;
				for (int i = 0; i < parent->m_tasksQueueCount; ++i) {
					if (!parent->m_threadToContinue || !parent->m_taskToContinue)
						break;
					if (parent->m_taskPause) {
						std::this_thread::sleep_for(std::chrono::milliseconds(TASK_WAIT_MILLSECONDS));
						continue;
					}

					MultiThreadQueue<T*>* const tasks = &parent->m_tasks[i];
					while (!tasks->empty()) {
						std::lock_guard<std::mutex> lock(parent->mutex_);

						if (!parent->m_threadToContinue || !parent->m_taskToContinue)
							break;
						if (parent->m_taskPause) {
							std::this_thread::sleep_for(std::chrono::milliseconds(TASK_WAIT_MILLSECONDS));
							continue;
						}
						T* task;
						task = tasks->front();

						// !tasks->empty() 在lock外面，可能出错顺序1. not empty； 2. clear all；3. get front
						if (task == nullptr)
							break;

						allEmpty = false;
						//parent->m_taskDoing = true;
						int ret = parent->m_doTaskFunction(task, parent);
						parent->LastRets[i] = ret;
						//parent->m_taskDoing = false;
						// when ret > 0, pop it; otherwise, keep it
						if (ret >= -1)
						{
							delete task;
							tasks->pop();
						}
						else
						{
							// MODBUS_FC_WAIT
							if (ret == -2) {
								//
							}
							else if (ret == -9) {
								LOG_WARNING("TaskThread: doTask(" << i << ", " << tasks->size() << ") return -9, Clear the task thread. " << typeid(T).name());

								while (!tasks->empty())
								{
									T* item = tasks->front();
									if (item == nullptr)
										continue;
									delete item;
									tasks->pop();
								}
								break;
							}
							// !taskThread->isTaskToContinue()
							else if (ret == -8) {

							}
							else
							{
								LOG_WARNING("Task Thread do function return unknown " << parent->m_doTaskFunction.target_type().name() << ", " << ret);
							}

							// maybe in wait, continue to next taskqueue
							break;
						}
					}
				}
				if (allEmpty)
					std::this_thread::sleep_for(std::chrono::milliseconds(TASK_WAIT_MILLSECONDS));
			}
			catch (const std::exception& e) {
				LOG_WARNING("EDM: exception: " << e.what());
			}
			catch (...) {
				LOG_ERROR("EDM: exception: " << "Unknown error");
			}
		}
	}
};

#endif

