#pragma once

#include "Jargon/DataStructures/NaryTree.h"
#include "Jargon/ErrorContext.h"
#include "Jargon/TextWriter.h"


namespace Jargon{
namespace FileFormats{
namespace Ini{

	enum ElementType {
		ElementType_File,
		ElementType_Section,
		ElementType_Setting,
		ElementType_Comment,
		ElementType_Whitespace
	};

	class IniElement : public Jargon::DataStructures::SimpleNaryTreeNode<IniElement> {
		public:
			~IniElement();

			ElementType getElementType() const;
			virtual bool parse(const std::string& line, Jargon::ErrorContext& errorContext) = 0;
			virtual bool canAttachChild(ElementType elementType) const = 0;
			virtual bool writeTo(TextWriter& destination) const = 0;

			bool appendChild(IniElement* child) override;
			bool prependChild(IniElement* child) override;
			bool attachPreviousSibling(IniElement* sibling) override;
			bool attachNextSibling(IniElement* sibling) override;

		protected:
			IniElement(ElementType elementType);

		private:
			typedef Jargon::DataStructures::SimpleNaryTreeNode<IniElement> Superclass;

			ElementType elementType;
	};

}
}
}
