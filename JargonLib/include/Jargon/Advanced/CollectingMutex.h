
#ifndef JARGON_ADVANCED_COLLECTINGMUTEX_H
#define JARGON_ADVANCED_COLLECTINGMUTEX_H

#include "Poco/AtomicCounter.h"
#include "Poco/Event.h"

#include <cassert>


namespace Jargon{
namespace Advanced{

	class CollectingMutex{
		public:
			CollectingMutex(unsigned int count);

			void signalOne();
			void wait();
			void tryWait(long milliseconds);

		private:
			Poco::AtomicCounter m_count;
			Poco::Event m_event;
	};

}
}

#endif
