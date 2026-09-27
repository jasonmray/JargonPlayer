
#include "Jargon/FileSystem/FileSystemWalker.h"
#include "Jargon/FileSystem/Utilities.h"
#include "Jargon/FileSystem/WindowsFileFinder.h"
#include "Jargon/StringUtilities.h"


namespace Jargon{
namespace FileSystem{

	FileSystemWalker::FileSystemWalker(FileSystemVisitor& fileSystemVisitor) :
		fileSystemVisitor(fileSystemVisitor)
	{
	}

	FileSystemWalker::~FileSystemWalker(){
	}

	bool FileSystemWalker::walk(const char* path) {
		std::string canonicalizedName;

		if (!Jargon::FileSystem::canonicalizePathName(path, canonicalizedName)) {
			// just continue with the original path
			canonicalizedName = path;
		}

		if (Jargon::FileSystem::isAbsolutePath(canonicalizedName.c_str())) {
			canonicalizedName = Jargon::FileSystem::ensureLongPathPrefix(canonicalizedName);
		}

		return walkUnknown(canonicalizedName);
	}

	bool FileSystemWalker::walkUnknown(const std::string& path) {
		if (Jargon::FileSystem::isDirectory(path.c_str())) {
			return walkFolder(path);
		} else {
			return walkFile(path);
		}
	}

	bool FileSystemWalker::walkFile(const std::string& filePath) {
		const char* pathnameToVisit = Jargon::FileSystem::skipLongPathPrefix(filePath);
		return fileSystemVisitor.visitFile(pathnameToVisit);
	}

	bool FileSystemWalker::walkFolder(const std::string& folderPath) {
		const char* pathnameToVisit = Jargon::FileSystem::skipLongPathPrefix(folderPath);
		if (!fileSystemVisitor.beginFolder(pathnameToVisit)) {
			return false;
		}

		const bool needSlash = !Jargon::StringUtilities::stringEndsWith(folderPath, '\\');

		std::string findExpression = folderPath;
		if (needSlash) {
			findExpression += "\\*";
		} else {
			findExpression += "*";
		}

		WindowsFileFinder fileFinder;
		if (!fileFinder.start(findExpression)) {
			setLastErrorFrom(fileFinder);
			return false;
		}

		std::string filename;
		while (fileFinder.getCurrentFilename(filename)) {
			std::string combinedPath = needSlash ? folderPath + "\\" + filename : folderPath + filename;
			if (!walkUnknown(combinedPath)) {
				return false;
			}

			if (!fileFinder.advance()) {
				break;
			}
		}
		
		if(fileFinder.hasError()) {
			setLastErrorFrom(fileFinder);
			return false;
		}

		if (!fileSystemVisitor.endFolder(pathnameToVisit)) {
			return false;
		}

		return true;
	}

}
}
