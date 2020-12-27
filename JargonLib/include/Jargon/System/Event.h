
#ifndef JARGON_SYSTEM_EVENT_H
#define JARGON_SYSTEM_EVENT_H

#include <cstdint>

namespace Jargon{
namespace System{

	class Event{
		public:
			Event(bool autoReset);
			~Event();

			void reset();
			void set();
			bool tryWait(uint32_t milliseconds);
			void wait();

		private:
			void* eventHandle;
	};

}
}

#endif
