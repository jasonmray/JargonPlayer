#include "Jargon/SmartPointer.h"

namespace Jargon{

	ReferenceCountableObject::~ReferenceCountableObject() {
	}



	ReferenceCountableBase::ReferenceCountableBase():
		m_count(0)
	{
	}

	ReferenceCountableBase::~ReferenceCountableBase(){
	}

	void ReferenceCountableBase::dispose() const{
		delete this;
	}

	size_t ReferenceCountableBase::addReference() const{
		return ++m_count;
	}

	size_t ReferenceCountableBase::removeReference() const{
		assert(m_count != 0);
		return --m_count;
	}





	AtomicReferenceCountableBase::AtomicReferenceCountableBase():
		m_count(0)
	{
	}

	AtomicReferenceCountableBase::~AtomicReferenceCountableBase(){
	}

	void AtomicReferenceCountableBase::dispose() const{
		delete this;
	}

	size_t AtomicReferenceCountableBase::addReference() const{
		return ++m_count;
	}

	size_t AtomicReferenceCountableBase::removeReference() const{
		return --m_count;
	}

}
