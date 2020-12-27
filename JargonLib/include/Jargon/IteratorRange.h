
#ifndef JARGON_ITERATORRANGE_H
#define JARGON_ITERATORRANGE_H


namespace Jargon{

	template<class IteratorType>
	class IteratorRange{
		public:
			IteratorRange(const IteratorType & begin, const IteratorType & end):
				m_begin(begin),
				m_end(end)
			{
			}

			IteratorType begin() const{
				return m_begin;
			}

			IteratorType end() const{
				return m_end;
			}

		private:
			IteratorType m_begin;
			IteratorType m_end;

	};

}

#endif
