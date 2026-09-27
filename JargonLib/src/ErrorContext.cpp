#include "Jargon/ErrorContext.h"
#include "Jargon/StringUtilities.h"


namespace Jargon{

	ErrorContext::ErrorContext(){
	}

	ErrorContext::~ErrorContext(){
	}

	bool ErrorContext::hasError() const {
		return !lastError.empty();
	}

	const char* ErrorContext::getLastError() const {
		if (lastError.empty()) {
			return nullptr;
		}
		return lastError.c_str();
	}

	void ErrorContext::setLastError(const char* formatString, ...) {
		va_list args;
		va_start(args, formatString);
		lastError = Jargon::StringUtilities::formatVarArgs(formatString, args);
		va_end(args);
	}

	void ErrorContext::setLastErrorVarArgs(const char* formatString, va_list args) {
		lastError = Jargon::StringUtilities::formatVarArgs(formatString, args);
	}

	void ErrorContext::setLastErrorFrom(const ErrorContext& other) {
		lastError = other.lastError;
	}

	void ErrorContext::clearLastError() {
		lastError.clear();
	}
}
