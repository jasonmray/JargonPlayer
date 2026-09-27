
#include "Jargon/DataStructures/NaryTree.h"

namespace Jargon{
namespace DataStructures{

	NaryTreeNode::NaryTreeNode(){
	}

	NaryTreeNode::~NaryTreeNode(){
		NaryTreeNode* child;
		child = getFirstChild();

		while (child != nullptr) {
			child->detachFromTree();
			delete child;
			child = getFirstChild();
		}
	}

	NaryTreeNode* NaryTreeNode::getParent() {
		return parent;
	}

	const NaryTreeNode* NaryTreeNode::getParent() const {
		return parent;
	}

	NaryTreeNode* NaryTreeNode::getFirstChild() {
		return firstChild;
	}

	const NaryTreeNode* NaryTreeNode::getFirstChild() const {
		return firstChild;
	}

	NaryTreeNode* NaryTreeNode::getLastChild() {
		return lastChild;
	}

	const NaryTreeNode* NaryTreeNode::getLastChild() const {
		return lastChild;
	}

	NaryTreeNode* NaryTreeNode::getPreviousSibling() {
		return previousSibling;
	}

	const NaryTreeNode* NaryTreeNode::getPreviousSibling() const {
		return previousSibling;
	}

	NaryTreeNode* NaryTreeNode::getNextSibling() {
		return nextSibling;
	}

	const NaryTreeNode* NaryTreeNode::getNextSibling() const {
		return nextSibling;
	}

	bool NaryTreeNode::attachPreviousSibling(NaryTreeNode* sibling) {
		if (sibling == nullptr) {
			return false;
		}

		if (parent == nullptr) {
			return false;
		}

		sibling->parent = parent;

		if (previousSibling != nullptr) {
			previousSibling->nextSibling = sibling;
			sibling->previousSibling = previousSibling;
		} else {
			parent->firstChild = sibling;
		}

		previousSibling = sibling;
		sibling->nextSibling = this;

		return true;
	}

	bool NaryTreeNode::attachNextSibling(NaryTreeNode* sibling) {
		if (sibling == nullptr) {
			return false;
		}

		if (parent == nullptr) {
			return false;
		}

		sibling->parent = parent;

		if (nextSibling != nullptr) {
			nextSibling->previousSibling = sibling;
			sibling->nextSibling = nextSibling;
		} else {
			parent->lastChild = sibling;
		}

		nextSibling = sibling;
		sibling->previousSibling = this;

		return true;
	}

	bool NaryTreeNode::prependChild(NaryTreeNode* child) {
		if (child == nullptr) {
			return false;
		}

		child->parent = this;

		if (firstChild == nullptr) {
			firstChild = child;
			lastChild = child;
		} else {
			firstChild->previousSibling = child;
			child->nextSibling = firstChild;
			firstChild = child;
		}

		return true;
	}

	bool NaryTreeNode::appendChild(NaryTreeNode* child) {
		if (child == nullptr) {
			return false;
		}

		child->parent = this;

		if (firstChild == nullptr) {
			firstChild = child;
			lastChild = child;
		} else {
			lastChild->nextSibling = child;
			child->previousSibling = lastChild;
			lastChild = child;
		}

		return true;
	}

	void NaryTreeNode::detachFromTree() {
		if (previousSibling == nullptr) {
			parent->firstChild = nextSibling;
		} else {
			previousSibling->nextSibling = nextSibling;
		}

		if (nextSibling == nullptr) {
			parent->lastChild = previousSibling;
		} else {
			nextSibling->previousSibling = previousSibling;
		}

		parent = nullptr;
		previousSibling = nullptr;
		nextSibling = nullptr;
	}

}
}
