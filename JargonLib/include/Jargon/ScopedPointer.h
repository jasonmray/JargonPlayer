#ifndef JARGON_SCOPEDPOINTER_H
#define JARGON_SCOPEDPOINTER_H

#include <algorithm>
#include <atomic>
#include <cassert>


namespace Jargon{

	/**
		A release policy that just calls delete
		Calls the dispose() method on the object
	*/
	template <class T>
	class DeletingReleasePolicy{
		public:
			static void Release(T * toRelease){
				delete toRelease;
			}
	};

	template <class T>
	class ArrayDeletingReleasePolicy{
		public:
			static void Release(T * toRelease){
				delete[] toRelease;
			}
	};

	/**
		Just a simple scoped pointer that deletes a thing upon destruction, or when given a new thing to hold.

		Don't use this with C-style array types without changing the release policy to ArrayDeletingReleasePolicy.
		There's probably some way to do that automatically with templates, but it isn't worth the hassle since
		you shouldn't be using C arrays anyway.
	*/
	template<class TargetClass, class ReleasePolicyClass = DeletingReleasePolicy<TargetClass> >
	class ScopedPointer{
		public:
			ScopedPointer(){
				m_target = nullptr;
			}

			ScopedPointer(std::nullptr_t n){
				m_target = nullptr;
			}

			template<class T>
			explicit ScopedPointer(T * target){
				this->m_target = target;
			}

			~ScopedPointer(){
				releaseTarget();
			}

			TargetClass * getRaw() const{
				return m_target;
			}

			TargetClass*& getRef() {
				return m_target;
			}

			TargetClass** addressOf() {
				return &m_target;
			}

			bool isNull() const{
				return m_target == nullptr;
			}

			operator bool() const {
				return !isNull();
			}

			template<class T>
			void set(T * other){
				this->operator=(other);
			}

			void swap(ScopedPointer<TargetClass, ReleasePolicyClass> & other){
				std::swap(m_target, other.m_target);
			}

			TargetClass* releaseOwnership() {
				TargetClass* toReturn = m_target;
				m_target = nullptr;
				return toReturn;
			}

			void reset(){
				swapTarget(nullptr);
			}

			template<class T>
			ScopedPointer<TargetClass, ReleasePolicyClass> & operator=( T * toCopy ){
				swapTarget(toCopy);
				return *this;
			}

			TargetClass & operator*() const{
				assert(m_target != nullptr);
				return *m_target;
			}

			TargetClass * operator->() const{
				assert(m_target != nullptr);
				return m_target;
			}

			bool operator==(const ScopedPointer & other) const {
				return m_target == other.m_target;
			}

			template<class T>
			bool operator==(const T * pointer) const {
				return m_target == pointer;
			}

			bool operator!=(const ScopedPointer & other) const {
				return m_target != other.m_target;
			}

			template<class T>
			bool operator!=(const T * pointer) const {
				return m_target != pointer;
			}

			bool operator<(const ScopedPointer & other) const {
				return m_target < other.m_target;
			}

			template<class T>
			bool operator<(const T * pointer) const {
				return m_target < pointer;
			}

			bool operator>(const ScopedPointer & other) const {
				return m_target > other.m_target;
			}

			template<class T>
			bool operator>(const T * pointer) const {
				return m_target > pointer;
			}

			bool operator<=(const ScopedPointer & other) const {
				return m_target <= other.m_target;
			}

			template<class T>
			bool operator<=(const T * pointer) const {
				return m_target <= pointer;
			}

			bool operator>=(const ScopedPointer & other) const {
				return m_target >= other.m_target;
			}

			template<class T>
			bool operator>=(const T * pointer) const {
				return m_target >= pointer;
			}

		private:
			TargetClass * m_target;

			void releaseTarget(){
				if( m_target != nullptr ){
					ReleasePolicyClass::Release(m_target);
					m_target = nullptr;
				}
			}

			void swapTarget(TargetClass * newTarget){
				releaseTarget();
				m_target = newTarget;
			}
	};

	template<class ReleasePolicyClass>
	class ScopedPointer<void, ReleasePolicyClass>{
		public:
			ScopedPointer(){
				m_target = nullptr;
			}

			ScopedPointer(std::nullptr_t n){
				m_target = nullptr;
			}

			template<class T>
			explicit ScopedPointer(T * target){
				this->m_target = target;
			}

			~ScopedPointer(){
				releaseTarget();
			}

			void * getRaw() const{
				return m_target;
			}

			bool isNull() const{
				return m_target == nullptr;
			}

			template<class T>
			void set(T * other){
				this->operator=(other);
			}

			void swap(ScopedPointer<void, ReleasePolicyClass> & other){
				std::swap(m_target, other.m_target);
			}

			void reset(){
				swapTarget(nullptr);
			}

			template<class T>
			ScopedPointer<void, ReleasePolicyClass> & operator=( T * toCopy ){
				swapTarget(toCopy);
				return *this;
			}

			void* operator->() const{
				assert(m_target != nullptr);
				return m_target;
			}

			bool operator==(const ScopedPointer & other) const {
				return m_target == other.m_target;
			}

			template<class T>
			bool operator==(const T * pointer) const {
				return m_target == pointer;
			}

			bool operator!=(const ScopedPointer & other) const {
				return m_target != other.m_target;
			}

			template<class T>
			bool operator!=(const T * pointer) const {
				return m_target != pointer;
			}

			bool operator<(const ScopedPointer & other) const {
				return m_target < other.m_target;
			}

			template<class T>
			bool operator<(const T * pointer) const {
				return m_target < pointer;
			}

			bool operator>(const ScopedPointer & other) const {
				return m_target > other.m_target;
			}

			template<class T>
			bool operator>(const T * pointer) const {
				return m_target > pointer;
			}

			bool operator<=(const ScopedPointer & other) const {
				return m_target <= other.m_target;
			}

			template<class T>
			bool operator<=(const T * pointer) const {
				return m_target <= pointer;
			}

			bool operator>=(const ScopedPointer & other) const {
				return m_target >= other.m_target;
			}

			template<class T>
			bool operator>=(const T * pointer) const {
				return m_target >= pointer;
			}

		private:
			void* m_target;

			void releaseTarget(){
				if( m_target != nullptr ){
					ReleasePolicyClass::Release(m_target);
					m_target = nullptr;
				}
			}

			void swapTarget(void* newTarget){
				releaseTarget();
				m_target = newTarget;
			}
	};

}

#endif
