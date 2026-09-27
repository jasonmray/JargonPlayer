#pragma once

#include "Jargon/SmartPointer.h"


namespace Jargon{

	template<class T>
	class BufferObject : public AtomicReferenceCountableBase {
		public:
			virtual ~BufferObject() {
			}

			virtual T* getBuffer() = 0;
			virtual const T* getBuffer() const = 0;

			virtual size_t getSizeItemCount() const = 0;
			virtual size_t getSizeBytes() const = 0;

			T& operator[](size_t index);
			const T& operator[](size_t index) const;
	};

}

