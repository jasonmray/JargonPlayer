#ifndef JARGON_SMARTPOINTER_H
#define JARGON_SMARTPOINTER_H

#include "Null.h"

#include <algorithm>
#include <atomic>
#include <cassert>


namespace Jargon{

	/**
		An interface for reference countable objects to be used with SmartPointer
	*/
	class ReferenceCountableObject{
		public:
			virtual ~ReferenceCountableObject();

			virtual size_t addReference() const = 0;
			virtual size_t removeReference() const = 0;
			virtual void dispose() const = 0;
	};

	/**
		A base class for non-thread-safe reference countable objects
	*/
	class ReferenceCountableBase : public ReferenceCountableObject{
		public:
			virtual ~ReferenceCountableBase();

			virtual size_t addReference() const;
			virtual size_t removeReference() const;
			virtual void dispose() const;

		protected:
			ReferenceCountableBase();
			ReferenceCountableBase(const ReferenceCountableBase &); // not implemented
			ReferenceCountableBase & operator=(const ReferenceCountableBase &); // not implemented

		private:
			mutable size_t m_count;
	};

	/**
		A base class for thread-safe reference countable objects
	*/
	class AtomicReferenceCountableBase : public ReferenceCountableObject {
		public:
			virtual ~AtomicReferenceCountableBase();

			virtual size_t addReference() const;
			virtual size_t removeReference() const;
			virtual void dispose() const;

		protected:
			AtomicReferenceCountableBase();
			AtomicReferenceCountableBase(const AtomicReferenceCountableBase &); // not implemented
			AtomicReferenceCountableBase & operator=(const AtomicReferenceCountableBase &); // not implemented

		private:
			mutable std::atomic_size_t m_count;
	};

	/**
		A default policy for destroying reference-counted objects.
		Calls the dispose() method on the object
	*/
	template <class T>
	class DisposingReleasePolicy{
		public:
			static void Release(T * toRelease){
				toRelease->dispose();
			}
	};

	/**
		A reference-counting smart pointer
	*/
	template<class ReferenceCountableClass, class ReleasePolicyClass = DisposingReleasePolicy<ReferenceCountableClass> >
	class SmartPointer{
		public:
			SmartPointer(){
				m_target = NULL;
			}

			SmartPointer( const SmartPointer<ReferenceCountableClass, ReleasePolicyClass> &toCopy ){
				acquireTarget(toCopy.getRaw());
			}

			template<class AnyClass>
			explicit SmartPointer( AnyClass * toCopy ){
				acquireTarget(toCopy);
			}

			template<class AnyClass, class AnyReleasePolicyClass>
			SmartPointer( const SmartPointer<AnyClass, AnyReleasePolicyClass> &toCopy ){
				acquireTarget(toCopy.getRaw());
			}


			~SmartPointer(){
				releaseTarget();
			}


			ReferenceCountableClass * getRaw() const{
				return m_target;
			}

			bool isNull() const{
				return m_target == NULL;
			}

			template<class T>
			void set(T * other){
				this->operator=(other);
			}

			template<class AnyReferenceCountableClass>
			void set(const SmartPointer<AnyReferenceCountableClass, ReleasePolicyClass> & other){
				this->operator=(other);
			}

			void swap(SmartPointer<ReferenceCountableClass, ReleasePolicyClass> & other){
				std::swap(m_target, other.m_target);
			}

			void reset(){
				swapTarget(NULL);
			}


			SmartPointer<ReferenceCountableClass, ReleasePolicyClass> & operator=( const SmartPointer<ReferenceCountableClass, ReleasePolicyClass> & toCopy ){
				swapTarget(toCopy.m_target);
				return *this;
			}

			template<class T>
			SmartPointer<ReferenceCountableClass, ReleasePolicyClass> & operator=( T * toCopy ){
				swapTarget(toCopy);
				return *this;
			}

			template<class AnyReferenceCountableClass>
			SmartPointer<ReferenceCountableClass, ReleasePolicyClass> & operator=( const SmartPointer<AnyReferenceCountableClass, ReleasePolicyClass> & toCopy ){
				swapTarget(toCopy.m_target);
				return *this;
			}


			ReferenceCountableClass & operator*() const{
				assert(m_target != NULL);
				return *m_target;
			}

			ReferenceCountableClass * operator->() const{
				assert(m_target != NULL);
				return m_target;
			}



			bool operator==(const SmartPointer & other) const {
				return m_target == other.m_target;
			}

			bool operator==(nullptr_t n) const {
				return m_target == n;
			}

			template<class T>
			bool operator==(const T * pointer) const {
				return m_target == pointer;
			}

			bool operator!=(const SmartPointer & other) const {
				return m_target != other.m_target;
			}

			bool operator!=(nullptr_t n) const {
				return m_target != n;
			}

			template<class T>
			bool operator!=(const T * pointer) const {
				return m_target != pointer;
			}

			bool operator<(const SmartPointer & other) const {
				return m_target < other.m_target;
			}

			template<class T>
			bool operator<(const T * pointer) const {
				return m_target < pointer;
			}

			bool operator>(const SmartPointer & other) const {
				return m_target > other.m_target;
			}

			template<class T>
			bool operator>(const T * pointer) const {
				return m_target > pointer;
			}

			bool operator<=(const SmartPointer & other) const {
				return m_target <= other.m_target;
			}

			template<class T>
			bool operator<=(const T * pointer) const {
				return m_target <= pointer;
			}

			bool operator>=(const SmartPointer & other) const {
				return m_target >= other.m_target;
			}

			template<class T>
			bool operator>=(const T * pointer) const {
				return m_target >= pointer;
			}

		private:
			ReferenceCountableClass * m_target;

			void acquireTarget(ReferenceCountableClass * target){
				if( target != NULL ){
					target->addReference();
				}
				this->m_target = target;
			}

			void releaseTarget(){
				if( m_target != NULL ){
					if( m_target->removeReference() == 0 ){
						ReleasePolicyClass::Release(m_target);
					}
					m_target = NULL;
				}
			}

			void swapTarget(ReferenceCountableClass * newTarget){
				if( newTarget != NULL ){
					newTarget->addReference();
				}
				releaseTarget();
				m_target = newTarget;
			}
	};

}

#endif
