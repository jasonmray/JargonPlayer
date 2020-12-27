
#ifndef JARGON_ADVANCED_TASKQUEUE_H
#define JARGON_ADVANCED_TASKQUEUE_H

#include "Jargon/ScopedPointer.h"
#include "Jargon/Macros.h"
#include "Jargon/Advanced/Task.h"

namespace Jargon{
namespace Advanced{


	class TaskQueue{
		public:
			enum StopMode{
				StopMode_IgnoreQueuedTasks,
				StopMode_WaitForAllTasks
			};

			TaskQueue(size_t threadPoolSize);
			~TaskQueue();

			size_t getThreadPoolSize() const;

			void start();
			void stop(StopMode stopMode);

			bool enqueue(const Task_ptr & task);

		private:
			class TaskQueueImpl;

			Jargon::ScopedPointer<TaskQueueImpl> m_taskQueue;

			JARGON_DISABLE_COPY(TaskQueue);
	};

}
}

#endif
