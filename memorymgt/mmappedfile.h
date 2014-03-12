
#ifndef MMAP_H
#define MMAP_H

#include <Windows.h>
#include <iostream>

namespace MemoryMgt
{
	class CMMappedFile
	{
	public:
		CMMappedFile(wchar_t* iPath);
		~CMMappedFile();

		void* GetBinary();

		void SeekToBegin();
		void SeekToEnd();
		void SetPosition(size_t iNewPosition);

	private:
		HANDLE mMappedFile;
		std::wstring mFilePath;
	};

}


#endif
