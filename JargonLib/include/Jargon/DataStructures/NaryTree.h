#pragma once


namespace Jargon{
namespace DataStructures{

	class NaryTreeNode {
		public:
			NaryTreeNode();
			virtual ~NaryTreeNode();

		protected:
			NaryTreeNode* getParent();
			const NaryTreeNode* getParent() const;

			NaryTreeNode* getPreviousSibling();
			const NaryTreeNode* getPreviousSibling() const;

			NaryTreeNode* getNextSibling();
			const NaryTreeNode* getNextSibling() const;

			NaryTreeNode* getFirstChild();
			const NaryTreeNode* getFirstChild() const;

			NaryTreeNode* getLastChild();
			const NaryTreeNode* getLastChild() const;

			bool attachPreviousSibling(NaryTreeNode* sibling);
			bool attachNextSibling(NaryTreeNode* sibling);
			bool appendChild(NaryTreeNode* child);
			bool prependChild(NaryTreeNode* child);
			void detachFromTree();

		private:
			NaryTreeNode* parent = nullptr;
			NaryTreeNode* previousSibling = nullptr;
			NaryTreeNode* nextSibling = nullptr;
			NaryTreeNode* firstChild = nullptr;
			NaryTreeNode* lastChild = nullptr;
	};

	template<class Subclass>
	class SimpleNaryTreeNode : public NaryTreeNode {
		public:
			virtual ~SimpleNaryTreeNode() {
			}

			virtual Subclass* getParent() {
				return (Subclass*)NaryTreeNode::getParent();
			}

			virtual const Subclass* getParent() const {
				return (const Subclass*)NaryTreeNode::getParent();
			}

			virtual Subclass* getPreviousSibling() {
				return (Subclass*)NaryTreeNode::getPreviousSibling();
			}

			virtual const Subclass* getPreviousSibling() const {
				return (const Subclass*)NaryTreeNode::getPreviousSibling();
			}

			virtual Subclass* getNextSibling() {
				return (Subclass*)NaryTreeNode::getNextSibling();
			}

			virtual const Subclass* getNextSibling() const {
				return (const Subclass*)NaryTreeNode::getNextSibling();
			}

			virtual Subclass* getFirstChild() {
				return (Subclass*)NaryTreeNode::getFirstChild();
			}
			virtual const Subclass* getFirstChild() const {
				return (const Subclass*)NaryTreeNode::getFirstChild();
			}

			virtual Subclass* getLastChild() {
				return (Subclass*)NaryTreeNode::getLastChild();
			}

			virtual const Subclass* getLastChild() const {
				return (const Subclass*)NaryTreeNode::getLastChild();
			}

			virtual bool appendChild(Subclass* child) {
				return NaryTreeNode::appendChild(child);
			}

			virtual bool prependChild(Subclass* child) {
				return NaryTreeNode::prependChild(child);
			}

			virtual bool attachPreviousSibling(Subclass* sibling) {
				return NaryTreeNode::attachPreviousSibling(sibling);
			}

			virtual bool attachNextSibling(Subclass* sibling) {
				return NaryTreeNode::attachNextSibling(sibling);
			}

			virtual void detachFromTree() {
				NaryTreeNode::detachFromTree();
			}
	};

}
}
