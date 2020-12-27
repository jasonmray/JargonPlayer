
#include "Jargon/Advanced/TaskQueue.h"
#include "Jargon/Advanced/impl/PocoTaskQueue.h"

namespace Jargon{
namespace Advanced{

	class TaskQueue::TaskQueueImpl : public Jargon::Advanced::impl::PocoTaskQueue{
		public:
		TaskQueueImpl(size_t threadPoolSize):
			Jargon::Advanced::impl::PocoTaskQueue(threadPoolSize)
		{
		}

	};

	TaskQueue::TaskQueue(size_t threadPoolSize):
		m_taskQueue(new TaskQueueImpl(threadPoolSize))
	{
	}

	TaskQueue::~TaskQueue(){
	}

	size_t TaskQueue::getThreadPoolSize() const{
		return m_taskQueue->getThreadPoolSize();
	}

	void TaskQueue::start(){
		m_taskQueue->start();
	}
	void TaskQueue::stop(StopMode stopMode){
		m_taskQueue->stop(stopMode);
	}

	bool TaskQueue::enqueue(const Task_ptr & task){
		return m_taskQueue->enqueue(task);
	}
}
}
