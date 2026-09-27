#pragma once

#include <array>
#include <cassert>
#include <vector>

namespace Jargon{
namespace DataStructures{

	template<class T, size_t PageSize>
	class PagedArray{
		public:
			typedef std::array<T, PageSize> Page;

			PagedArray(){
				lastPageSize = 0;
			}

			virtual ~PagedArray() {
			}

			void clear() {
				lastPageSize = 0;
				pages.clear();
			}

			size_t getSize() const {
				size_t fullPages = 0;
				if (pages.size() > 0) {
					fullPages = pages.size() - 1;
				}
				return fullPages * PageSize + lastPageSize;
			}

			T& operator[](size_t position) {
				assert(position < getSize());
				const size_t pageIndex = position / PageSize;
				const size_t subIndex = position % PageSize;

				return pages[pageIndex][subIndex];
			}

			const T& operator[](size_t position) const {
				assert(position < getSize());
				const size_t pageIndex = position / PageSize;
				const size_t subIndex = position % PageSize;

				return pages[pageIndex][subIndex];
			}

			void append(const T& element) {
				if (lastPageSize == 0 || lastPageSize == PageSize) {
					pages.push_back({});
					lastPageSize = 0;
				}
				pages.back()[lastPageSize] = element;

				lastPageSize = lastPageSize + 1;
			}

		private:
			size_t lastPageSize;
			std::vector<Page> pages;
	};
}
}
