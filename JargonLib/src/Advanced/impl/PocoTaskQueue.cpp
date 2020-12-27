
#include "Jargon/Advanced/impl/PocoTaskQueue.h"
#include "Poco/RunnableAdapter.h"

namespace Jargon{
namespace Advanced{
namespace impl{

	PocoTaskQueue::PocoTaskQueue(size_t threadPoolSize):
		m_threads(threadPoolSize),
		m_runnables(threadPoolSize),
		m_running(false)
	{
		for( Runnable_ptr & runnable : m_runnables ){
			runnable = new Poco::RunnableAdapter<PocoTaskQueue>(*this, &PocoTaskQueue::threadRun);
		}
	}

	PocoTaskQueue::~PocoTaskQueue(){
		assert(m_running == false);
	}

	size_t PocoTaskQueue::getThreadPoolSize() const{
		return m_threads.size();
	}

	void PocoTaskQueue::start(){
		m_shouldStop = false;
		m_running = true;

		for(size_t i = 0; i < m_threads.size(); i++ ){
			m_threads[i].start(*m_runnables[i]);
		}
	}

	void PocoTaskQueue::stop(Jargon::Advanced::TaskQueue::StopMode stopMode){
		if( stopMode == Jargon::Advanced::TaskQueue::StopMode_WaitForAllTasks ){
			m_shouldFlush = true;
		}else{
			m_shouldFlush = false;
		}
		m_shouldStop = true;


		for(size_t i = 0; i < m_threads.size(); i++ ){
			Jargon::Advanced::Task_ptr nullTask;
			m_taskQueue.push(nullTask);
		}

		for( Poco::Thread & thread : m_threads ){
			thread.join();
		}
		m_running = false;
	}

	bool PocoTaskQueue::enqueue(const Jargon::Advanced::Task_ptr & task){
		assert(m_shouldStop == false);

		if( m_shouldStop ){
			return false;
		}

		m_taskQueue.push(task);

		return false;
	}

	void PocoTaskQueue::threadRun(){
		Jargon::Advanced::Task_ptr task;

		while( ! m_shouldStop ){
			m_taskQueue.pop(task);

			if( task == NULL ){
				return;
			}
			processTask(task);
		}

		if( m_shouldFlush ){
			while( m_taskQueue.try_pop(task) ){
				if( task == NULL ){
					return;
				}
				processTask(task);
			}
		}
	}

	void PocoTaskQueue::processTask(const Jargon::Advanced::Task_ptr & task){
		task->run();
	}
}
}
}
