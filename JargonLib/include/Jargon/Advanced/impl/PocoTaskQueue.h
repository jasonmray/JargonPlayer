
#ifndef JARGON_ADVANCED_IMPL_POCOTASKQUEUE_H
#define JARGON_ADVANCED_IMPL_POCOTASKQUEUE_H

#include "Jargon/Advanced/Task.h"
#include "Jargon/Advanced/TaskQueue.h"

#include "Poco/Thread.h"
#include "tbb/concurrent_queue.h"

#include <vector>


namespace Jargon{
namespace Advanced{
namespace impl{

	class PocoTaskQueue{
		public:
			PocoTaskQueue(size_t threadPoolSize);
			virtual ~PocoTaskQueue();

			size_t getThreadPoolSize() const;

			void start();
			void stop(Jargon::Advanced::TaskQueue::StopMode stopMode);

			bool enqueue(const Jargon::Advanced::Task_ptr & task);

		private:
			typedef Jargon::ScopedPointer<Poco::Runnable> Runnable_ptr;

			std::vector<Poco::Thread> m_threads;
			std::vector<Runnable_ptr> m_runnables;
			tbb::concurrent_bounded_queue<Jargon::Advanced::Task_ptr> m_taskQueue;

			bool m_running;
			bool m_shouldStop;
			bool m_shouldFlush;

			void threadRun();
			void processTask(const Jargon::Advanced::Task_ptr & task);
	};

}
}
}

#endif
