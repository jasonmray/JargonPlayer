#pragma once

#include <cstdint>
#include <string>
#include <cstdarg>

namespace Jargon{

	class ErrorContext{
		public:
			ErrorContext();
			virtual ~ErrorContext();

			// Indicates whether this ErrorContext has an error message.
			bool hasError() const;

			// Get the last error message set on this ErrorContext.
			// Returns nullptr if no error has been set.
			const char* getLastError() const;

			void setLastError(const char* formatString, ...);
			void setLastErrorVarArgs(const char* formatString, va_list args);
			void setLastErrorFrom(const ErrorContext& other);

			void clearLastError();
		private:
			std::string lastError;

	};

}
