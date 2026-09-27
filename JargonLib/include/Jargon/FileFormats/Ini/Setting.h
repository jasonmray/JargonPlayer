#pragma once

#include "Jargon/FileFormats/Ini/IniElement.h"
#include "Jargon/TextWriter.h"

#include <string>


namespace Jargon{
namespace FileFormats{
namespace Ini{

	class Setting : public IniElement {
		public:
			Setting();
			~Setting();

			bool parse(const std::string& line, Jargon::ErrorContext& errorContext) override;

			std::string getName() const;
			void setName(const std::string& name);

			std::string getValue() const;
			void setValue(const char* value);
			void setValue(const std::string& value);

			bool getValueAsInteger(int* valueOut) const;
			bool getValueAsBoolean(bool* valueOut) const;
			bool getValueAsFloat(float* valueOut) const;
			bool getValueAsDouble(double* valueOut) const;

			const std::string& getLeadingWhiteSpace() const;
			void setLeadingWhiteSpace(const std::string& leadingWhiteSpace);

			const std::string& getTrailingComment() const;
			void setTrailingComment(const std::string& trailingComment);

			bool canAttachChild(ElementType elementType) const override;
			bool writeTo(TextWriter& destination) const override;

		private:
			std::string leadingWhiteSpace;
			std::string name;
			std::string value;
			std::string trailingComment;
	};

}
}
}
