#include "Jargon/FileFormats/Ini/IniElement.h"


namespace Jargon{
namespace FileFormats{
namespace Ini{

	IniElement::IniElement(ElementType elementType) :
		elementType(elementType)
	{
	}

	IniElement::~IniElement(){
	}

	ElementType IniElement::getElementType() const {
		return elementType;
	}

	bool IniElement::appendChild(IniElement* child)
	{
		if (!canAttachChild(child->getElementType())) {
			return false;
		}
		return Superclass::appendChild(child);
	}

	bool IniElement::prependChild(IniElement* child)
	{
		if (!canAttachChild(child->getElementType())) {
			return false;
		}
		return Superclass::prependChild(child);
	}

	bool IniElement::attachNextSibling(IniElement* sibling)
	{
		auto parent = getParent();
		if (parent == nullptr){
			return false;
		}
		if (!parent->canAttachChild(sibling->getElementType())) {
			return false;
		}
		return Superclass::attachNextSibling(sibling);
	}

	bool IniElement::attachPreviousSibling(IniElement* sibling)
	{
		auto parent = getParent();
		if (parent == nullptr) {
			return false;
		}
		if (!parent->canAttachChild(sibling->getElementType())) {
			return false;
		}
		return Superclass::attachPreviousSibling(sibling);
	}
}
}
}
