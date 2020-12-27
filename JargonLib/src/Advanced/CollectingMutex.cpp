
#include "Jargon/Advanced/CollectingMutex.h"


namespace Jargon{
namespace Advanced{

	CollectingMutex::CollectingMutex(unsigned int count):
		m_count((int)count),
		m_event(false)
	{
	}

	void CollectingMutex::signalOne(){
		assert(m_count > 0);
		if( --m_count == 0 ){
			m_event.set();
		}
	}

	void CollectingMutex::wait(){
		m_event.wait();
	}

	void CollectingMutex::tryWait(long milliseconds){
		m_event.tryWait(milliseconds);
	}

}
}
