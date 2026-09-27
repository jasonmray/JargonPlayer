#pragma once

#include "Jargon/FileFormats/Ini/IniElement.h"

#include <string>


namespace Jargon{
namespace FileFormats{
namespace Ini{

	class Comment : public IniElement {
		public:
			static const char* CommentStartChars;
			static bool IsCommentStartChar(char c);

			Comment();
			~Comment();

			bool parse(const std::string& line, Jargon::ErrorContext& errorContext) override;

			const std::string& getComment() const;
			void setComment(const char* comment);

			const std::string& getLeadingWhiteSpace() const;
			void setLeadingWhiteSpace(const std::string& leadingWhiteSpace);

			char getCommentStartChar() const;
			void setCommentStartChar(char commentStartChar);

			bool canAttachChild(ElementType elementType) const override;
			bool writeTo(TextWriter& destination) const override;

		private:
			std::string leadingWhiteSpace;
			char commentStartChar;
			std::string comment;
	};

}
}
}
