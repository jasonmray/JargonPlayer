
#ifndef JARGON_PROVIDEROBJECT_H
#define JARGON_PROVIDEROBJECT_H


namespace Jargon{

	template<class ProvidedType>
	class ProviderObject{
		public:
			virtual ~ProviderObject() = default;

			virtual bool hasNext() const = 0;
			virtual ProvidedType getNext() = 0;
	};

}

#endif
