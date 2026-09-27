#pragma once

#include "Jargon/FileFormats/Ini/IniElement.h"

#include <string>


namespace Jargon{
namespace FileFormats{
namespace Ini{

	class Whitespace : public IniElement {
		public:
			Whitespace();
			~Whitespace();

			bool parse(const std::string& line, Jargon::ErrorContext& errorContext) override;

			const std::string& getContents() const;
			void setContents(const std::string& contents);

			bool canAttachChild(ElementType elementType) const override;
			bool writeTo(TextWriter& destination) const override;

		private:
			std::string contents;
	};

}
}
}
