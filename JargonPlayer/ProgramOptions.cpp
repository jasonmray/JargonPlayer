#include "ProgramOptions.h"
#include "Util.h"

#include "Jargon/System/Utilities.h"

#include <algorithm>
#include <ctime>
#include <random>

ProgramOptions ProgramOptions::Instance;

ProgramOptions::ProgramOptions() :
	openMode(OpenMode::Enqueue),
	sortFiles(true),
	shuffleFiles(false),
	skipImages(false),
	skipArchives(false),
	useHardwareDecoding(true)
{
}

ProgramOptions::~ProgramOptions(){
}

bool ProgramOptions::processOptions(int argc, const char *argv[]) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' || argv[i][0] == '/') {
			const char* optionName = &(argv[i][1]);
			if (Util::stringEqualCaseInsensitive(optionName, "tile")) {
				this->openMode = OpenMode::Tile;
			} else if (Util::stringEqualCaseInsensitive(optionName, "enqueue")) {
				this->openMode = OpenMode::Enqueue;
			} else if (Util::stringEqualCaseInsensitive(optionName, "sort")) {
				this->sortFiles = true;
			} else if (Util::stringEqualCaseInsensitive(optionName, "nosort")) {
				this->sortFiles = false;
			} else if (Util::stringEqualCaseInsensitive(optionName, "shuffle")) {
				this->sortFiles = false;
				this->shuffleFiles = true;
			} else if (Util::stringEqualCaseInsensitive(optionName, "skipimages")) {
				this->skipImages = true;
			} else if (Util::stringEqualCaseInsensitive(optionName, "skiparchives")) {
				this->skipArchives = true;
			} else if (Util::stringEqualCaseInsensitive(optionName, "disablehwdec")) {
				this->useHardwareDecoding = false;
			} else {
				return false;
			}
		} else {
			const char * filename = argv[i];
			if (strchr(filename, '*') != nullptr || strchr(filename, '?') != nullptr) {
				Jargon::System::globFiles(filename, this->files);
			} else{
				this->files.push_back(filename);
			}

		}
	}

	if (this->sortFiles) {
		std::sort(this->files.begin(), this->files.end());
	} else if (this->shuffleFiles) {
		std::random_device randomDevice;
		std::srand(randomDevice());
		std::random_shuffle(this->files.begin(), this->files.end());
	}

	return true;
}
