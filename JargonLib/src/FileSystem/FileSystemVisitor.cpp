
#include "Jargon/FileSystem/FileSystemVisitor.h"

namespace Jargon{
namespace FileSystem{

	FileSystemVisitor::FileSystemVisitor(){
	}

	FileSystemVisitor::~FileSystemVisitor(){
	}

	bool FileSystemVisitor::visitFile(const char* filename) {
		return true;
	}

	bool FileSystemVisitor::beginFolder(const char* path) {
		return true;
	}

	bool FileSystemVisitor::endFolder(const char* path) {
		return true;
	}

}
}
