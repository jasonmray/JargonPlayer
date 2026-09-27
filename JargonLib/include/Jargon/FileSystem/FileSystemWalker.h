#pragma once

#include "Jargon/ErrorContext.h"
#include "Jargon/FileSystem/FileSystemVisitor.h"

#include <string>


namespace Jargon{
namespace FileSystem{

	class FileSystemWalker : public ErrorContext {
		public:
			FileSystemWalker(FileSystemVisitor& fileSystemVisitor);
			~FileSystemWalker();

			bool walk(const char* path);
		private:
			FileSystemVisitor& fileSystemVisitor;

			bool walkUnknown(const std::string& path);
			bool walkFile(const std::string& filePath);
			bool walkFolder(const std::string& folderPath);
	};

}
}

