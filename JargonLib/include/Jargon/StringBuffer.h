#pragma once

#include "Jargon/Buffer.h"
#include "Jargon/StringUtilities.h"

#include <cassert>
#include <string>


namespace Jargon{

	template<class CharType>
	class StringBuffer{
		public:
			StringBuffer(size_t length) :
				buffer(length)
			{
				assert(length > 0);
				buffer.zeroBuffer();
			}

			~StringBuffer() {
			}

			const CharType* c_str() const {
				return buffer.getBuffer();
			}

			CharType* data() {
				return buffer.getBuffer();
			}

			size_t getBufferSizeCharacters() const {
				return buffer.getSizeItemCount();
			}

			size_t getBufferSizeBytes() const {
				return buffer.getSizeBytes();
			}

			size_t getMaxStringLength() const {
				return buffer.getSizeItemCount() - 1;
			}

			void set(const CharType* s) {
				assert(s != nullptr);
				Jargon::StringUtilities::stringCopy(data(), getBufferSizeCharacters(), s);
			}

			void set(const std::basic_string<CharType>& s) {
				Jargon::StringUtilities::stringCopy(data(), getBufferSizeCharacters(), s.c_str(), s.length());
			}

			void clear() {
				buffer.zeroBuffer();
				length = 0;
			}

			void updateLength() {
				length = Jargon::StringUtilities::stringLength(data());
			}

			size_t getRemainingLengthIncludingNull() const {
				return buffer.getSizeItemCount() - length;
			}

			size_t getRemainingLengthWithoutNull() const {
				return buffer.getSizeItemCount() - length - 1;
			}

			void append(const CharType* s, size_t sLengthWithoutNull) {
				assert(getRemainingLengthWithoutNull() >= sLengthWithoutNull);
				Jargon::StringUtilities::stringCopy(data() + length, getRemainingLengthIncludingNull(), s, sLengthWithoutNull);
				length += sLengthWithoutNull;
			}

			void append(const std::basic_string<CharType>& s) {
				append(s.c_str(), s.length());
			}

			void append(CharType c) {
				assert(getRemainingLengthWithoutNull() > 0);
				*(data() + length) = c;
				*(data() + length + 1) = 0;
				length++;
			}

		private:
			size_t length = 0;
			Jargon::Buffer<CharType> buffer;
	};

}

