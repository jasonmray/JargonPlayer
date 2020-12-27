
#ifndef JARGON_ADVANCED_TASK_H
#define JARGON_ADVANCED_TASK_H

#include "Jargon/SmartPointer.h"


namespace Jargon{
namespace Advanced{

	class Task : public Jargon::AtomicReferenceCountableBase{
		public:
			virtual ~Task() = default;
			virtual void run() = 0;
	};

	typedef Jargon::SmartPointer<Task> Task_ptr;
	typedef Jargon::SmartPointer<const Task> ConstTask_ptr;

}
}

#endif
