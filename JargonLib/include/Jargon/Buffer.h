#pragma once

#include "Jargon/BufferObject.h"

#include <cstdint>

namespace Jargon{

	template <class T> class ConstBuffer;

	enum BufferOwnership {
		BufferOwnership_OwnBuffer,
		BufferOwnership_DontOwnBuffer,
	};

	template <class T>
	class Buffer : public BufferObject<T>{
		public:
			Buffer() {
				buffer = nullptr;
				bufferSizeItemCount = 0;
				bufferOwnership = BufferOwnership_DontOwnBuffer;
			}

			Buffer(size_t itemCount) {
				buffer = new T[itemCount];
				bufferSizeItemCount = itemCount;
				bufferOwnership = BufferOwnership_OwnBuffer;
			}

			Buffer(T* buffer, size_t bufferSizeItemCount, BufferOwnership bufferOwnership) {
				this->buffer = buffer;
				this->bufferSizeItemCount = bufferSizeItemCount;
				this->bufferOwnership = bufferOwnership;
			}

			virtual ~Buffer() {
				if (bufferOwnership == BufferOwnership_OwnBuffer) {
					delete[] buffer;
				}
			}

			ConstBuffer<T> asConstBuffer() const {
				return ConstBuffer<T>(buffer, bufferSizeItemCount, BufferOwnership_DontOwnBuffer);
			}

			void reset() {
				if (bufferOwnership == BufferOwnership_OwnBuffer) {
					delete[] buffer;
				}
				buffer = nullptr;
				bufferSizeItemCount = 0;
				bufferOwnership = BufferOwnership_DontOwnBuffer;
			}

			void createNewBuffer(size_t itemCount) {
				reset();

				buffer = new T[itemCount];
				bufferSizeItemCount = itemCount;
				bufferOwnership = BufferOwnership_OwnBuffer;
			}

			void createNewBufferAndCopyContents(size_t itemCount) {
				uint8_t * newBuffer = new T[itemCount];
				size_t copyCount = (itemCount < bufferSizeItemCount) ? itemCount : bufferSizeItemCount;

				memcpy_s(newBuffer, itemCount * sizeof(T), buffer, copyCount * sizeof(T));

				setBuffer(newBuffer, itemCount, BufferOwnership_OwnBuffer);
			}

			void setBuffer(T* buffer, size_t bufferSizeItemCount, BufferOwnership bufferOwnership) {
				reset();

				this->buffer = buffer;
				this->bufferSizeItemCount = bufferSizeItemCount;
				this->bufferOwnership = bufferOwnership;
			}

			T* getBuffer() override {
				return buffer;
			}

			const T* getBuffer() const override {
				return buffer;
			}

			size_t getSizeItemCount() const override {
				return bufferSizeItemCount;
			}

			size_t getSizeBytes() const override {
				return bufferSizeItemCount * sizeof(T);
			}

			T& operator[](size_t index) {
				return buffer[index];
			}

			const T& operator[](size_t index) const {
				return buffer[index];
			}

			void zeroBuffer() {
				memset(buffer, 0, bufferSizeItemCount * sizeof(T));
			}
		private:
			T* buffer;
			size_t bufferSizeItemCount;
			BufferOwnership bufferOwnership;
	};

	template <class T>
	class ConstBuffer {
		public:
			ConstBuffer() {
				buffer = nullptr;
				bufferSizeItemCount = 0;
				bufferOwnership = BufferOwnership_DontOwnBuffer;
			}

			ConstBuffer(size_t itemCount) {
				buffer = new T[itemCount];
				bufferSizeItemCount = itemCount;
				bufferOwnership = BufferOwnership_OwnBuffer;
			}

			ConstBuffer(const T* buffer, size_t bufferSizeItemCount, BufferOwnership bufferOwnership) {
				this->buffer = buffer;
				this->bufferSizeItemCount = bufferSizeItemCount;
				this->bufferOwnership = bufferOwnership;
			}

			virtual ~ConstBuffer() {
				if (bufferOwnership == BufferOwnership_OwnBuffer) {
					delete[] buffer;
				}
			}

			void reset() {
				if (bufferOwnership == BufferOwnership_OwnBuffer) {
					delete[] buffer;
				}
				buffer = nullptr;
				bufferSizeItemCount = 0;
				bufferOwnership = BufferOwnership_DontOwnBuffer;
			}

			void setBuffer(const T* buffer, size_t bufferSizeItemCount, BufferOwnership bufferOwnership) {
				reset();

				this->buffer = buffer;
				this->bufferSizeItemCount = bufferSizeItemCount;
				this->bufferOwnership = bufferOwnership;
			}

			const T * getBuffer() const {
				return buffer;
			}

			size_t getSizeItemCount() const {
				return bufferSizeItemCount;
			}

			size_t getSizeBytes() const {
				return bufferSizeItemCount * sizeof(T);
			}

			const T& operator[](size_t index) const {
				return buffer[index];
			}

		private:
			const T* buffer;
			size_t bufferSizeItemCount;
			BufferOwnership bufferOwnership;
	};

	typedef Buffer<uint8_t> ByteBuffer;
	typedef ConstBuffer<uint8_t> ConstByteBuffer;
}
