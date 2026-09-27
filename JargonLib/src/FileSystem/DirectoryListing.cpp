
#include "Jargon/FileSystem/DirectoryListing.h"
#include "Jargon/FileSystem/Utilities.h"
#include "Jargon/StringUtilities.h"


namespace Jargon{
namespace FileSystem{

	DirectoryListing::DirectoryListing(){
	}

	DirectoryListing::~DirectoryListing(){
		close();
	}

	bool DirectoryListing::open(const char* path) {
		return open(std::string_view(path));
	}

	bool DirectoryListing::open(const std::string& path) {
		return open(std::string_view(path));
	}

	bool DirectoryListing::open(const std::string_view& path) {
		std::string findExpression;
		if (!Jargon::FileSystem::canonicalizePathName(path, findExpression)) {
			setLastError("failed to canonicalize path");
			return false;
		}

		if (!Jargon::StringUtilities::stringEndsWith(findExpression, '\\')) {
			findExpression += "\\*";
		} else {
			findExpression += "*";
		}

		if (!fileFinder.start(findExpression)) {
			setLastErrorFrom(fileFinder);
			return false;
		}

		return true;
	}

	const std::string& DirectoryListing::getPath() const {
		return path;
	}

	bool DirectoryListing::getCurrentFilename(std::string& filenameOut) {
		return fileFinder.getCurrentFilename(filenameOut);
	}

	bool DirectoryListing::advance() {
		return fileFinder.advance();
	}

	void DirectoryListing::close() {
		fileFinder.stop();
		path.clear();
	}
}
}
