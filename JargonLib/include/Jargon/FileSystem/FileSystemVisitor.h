#pragma once


namespace Jargon{
namespace FileSystem{

	class FileSystemVisitor{
		public:
			FileSystemVisitor();
			virtual ~FileSystemVisitor();

			virtual bool visitFile(const char* filename);
			virtual bool beginFolder(const char* path);
			virtual bool endFolder(const char* path);
		private:

	};

}
}

