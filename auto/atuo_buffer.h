
#ifndef AUTOBUFFER_H
#define AUTOBUFFER_H

#include "../memorymgt/memorymgt.h"

namespace Auto
{
	template <class _Tp>
	class auto_buffer
	{
	public:
		typedef _Tp data_type;
		typedef _Tp & reference;
		typedef const _Tp & const_reference;
		typedef _Tp * pointer;
		typedef const _Tp* const_pointer;

		typedef size_t size_type;
		
		auto_buffer(size_type iSize, MemoryMgt::Cmemoryhandler* iHandler = NULL)
			: mMemoryHandler(iHandler)
		{
			mBoundry = iSize;
			_ASSERT(mBoundry >= 0);
			mMemoryHandler == NULL ? mpData = ::new data_type[mBoundry] :
				mpData = new (mMemoryHandler)data_type[mBoundry];
		}

		~auto_buffer()
		{
			clear();
		}

		pointer begin()
		{
			return mpData;
		}

		const_pointer end()
		{
			return (mpData + mBoundry + 1);
		}
		
		reference operator[](size_type iIndex)
		{
			return (*(mpData + iIndex));
		}

		const_reference operator[](size_type iIndex) const
		{
			return (*(mpData + iIndex));
		}

		 bool empty()
		 {
			 return mpData == NULL;
		 }

		 size_type size()
		 {
			 return mBoundry;
		 }

	private:
		void clear()
		{
			if (mpData)
			{
				mMemoryHandler == NULL ? ::delete[] mpData :
					operator delete[](mpData, mMemoryHandler);
			}
		}

	private:
		MemoryMgt::Cmemoryhandler* mMemoryHandler;
		size_type mCurrentIndex;
		size_type mBoundry;
		pointer mpData;

	};
}

#endif
