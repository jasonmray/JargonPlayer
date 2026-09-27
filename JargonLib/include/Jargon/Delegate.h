#pragma once

#include "Jargon/SmartPointer.h"


namespace Jargon{

	template<class ArgumentType, class ReturnType = void>
	class DelegateObject : public Jargon::ReferenceCountableBase {
		public:
			virtual ~DelegateObject(){
			}

			virtual ReturnType invoke(ArgumentType arguments) = 0;
	};

	template<class ArgumentType>
	class DelegateObject<ArgumentType, void> : public Jargon::ReferenceCountableBase {
		public:
			virtual ~DelegateObject(){
			}

			virtual void invoke(ArgumentType arguments) = 0;
	};

	template<>
	class DelegateObject<void, void> : public Jargon::ReferenceCountableBase {
		public:
			virtual ~DelegateObject() {
			}

			virtual void invoke() = 0;
	};

	template<class TargetType, class ArgumentType, class ReturnType = void>
	class Delegate : public DelegateObject<ArgumentType, ReturnType> {
		public:
			typedef ReturnType (TargetType::*DelegatedMethod)(ArgumentType);

			Delegate() = delete;

			Delegate(TargetType* targetObject, DelegatedMethod delegatedMethod) {
				this->targetObject = targetObject;
				this->delegatedMethod = delegatedMethod;
			}

			~Delegate() {
			}

			ReturnType invoke(ArgumentType argument) {
				return (targetObject->*delegatedMethod)(argument);
			}
		private:
			TargetType* targetObject;
			DelegatedMethod delegatedMethod;
	};

	template<class TargetType, class ArgumentType>
	class Delegate<TargetType, ArgumentType, void> : public DelegateObject<ArgumentType, void> {
		public:
			typedef void (TargetType::*DelegatedMethod)(ArgumentType);

			Delegate() = delete;

			Delegate(TargetType* targetObject, DelegatedMethod delegatedMethod) {
				this->targetObject = targetObject;
				this->delegatedMethod = delegatedMethod;
			}

			~Delegate() {
			}

			void invoke(ArgumentType argument) {
				(targetObject->*delegatedMethod)(argument);
			}
		private:
			TargetType* targetObject;
			DelegatedMethod delegatedMethod;
	};

	template<class TargetType>
	class Delegate<TargetType, void, void> : public DelegateObject<void, void> {
		public:
			typedef void (TargetType::* DelegatedMethod)();

			Delegate() = delete;

			Delegate(TargetType* targetObject, DelegatedMethod delegatedMethod) {
				this->targetObject = targetObject;
				this->delegatedMethod = delegatedMethod;
			}

			~Delegate() {
			}

			void invoke() {
				(targetObject->*delegatedMethod)();
			}
		private:
			TargetType* targetObject;
			DelegatedMethod delegatedMethod;
	};

	template<class ArgumentType, class ReturnType = void>
	class FunctionDelegate : public DelegateObject<ArgumentType, ReturnType> {
		public:
			typedef ReturnType (*DelegatedMethod)(ArgumentType);

			FunctionDelegate() = delete;

			FunctionDelegate(DelegatedMethod delegatedMethod) {
				this->delegatedMethod = delegatedMethod;
			}

			~FunctionDelegate() {
			}

			ReturnType invoke(ArgumentType argument) {
				return (*delegatedMethod)(argument);
			}

		private:
			DelegatedMethod delegatedMethod;
	};

	template<class ArgumentType>
	class FunctionDelegate<ArgumentType, void> : public DelegateObject<ArgumentType, void> {
		public:
			typedef void (*DelegatedMethod)(ArgumentType);

			FunctionDelegate() = delete;

			FunctionDelegate(DelegatedMethod delegatedMethod) {
				this->delegatedMethod = delegatedMethod;
			}

			~FunctionDelegate() {
			}

			void invoke(ArgumentType argument) {
				(*delegatedMethod)(argument);
			}

		private:
			DelegatedMethod delegatedMethod;
	};

	template<>
	class FunctionDelegate<void, void> : public DelegateObject<void, void> {
		public:
			typedef void (*DelegatedMethod)();

			FunctionDelegate() = delete;

			FunctionDelegate(DelegatedMethod delegatedMethod) {
				this->delegatedMethod = delegatedMethod;
			}

			~FunctionDelegate() {
			}

			void invoke() {
				(*delegatedMethod)();
			}

		private:
			DelegatedMethod delegatedMethod;
	};

}

