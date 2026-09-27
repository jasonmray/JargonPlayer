#include "ProgramOptions.h"
#include "WebcamEnumerator.h"
#include "Util.h"

#include "Jargon/FileSystem/Utilities.h"
#include "Jargon/StringUtilities.h"

#include <algorithm>
#include <ctime>
#include <random>

ProgramOptions ProgramOptions::Instance;

ProgramOptions::ProgramOptions() :
	openMode(OpenMode::Enqueue),
	windowMode(WindowMode::Normal),
	sortFiles(true),
	shuffleFiles(false),
	skipImages(false),
	skipArchives(false),
	slideshowEnabled(true),
	useHardwareDecoding(true)
{
}

ProgramOptions::~ProgramOptions(){
}

bool ProgramOptions::processOptions(int argc, const char *argv[]) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' || argv[i][0] == '/') {
			const char* optionName = &(argv[i][1]);
			if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "tile")) {
				this->openMode = OpenMode::Tile;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "enqueue")) {
				this->openMode = OpenMode::Enqueue;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "sort")) {
				this->sortFiles = true;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "nosort")) {
				this->sortFiles = false;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "shuffle")) {
				this->sortFiles = false;
				this->shuffleFiles = true;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "skipimages")) {
				this->skipImages = true;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "skiparchives")) {
				this->skipArchives = true;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "noslideshow")) {
				this->slideshowEnabled = false;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "disablehwdec")) {
				this->useHardwareDecoding = false;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "fullscreen")) {
				this->windowMode = WindowMode::Fullscreen;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "maximize")) {
				this->windowMode = WindowMode::Maximized;
			} else if (Jargon::StringUtilities::stringEqualCaseInsensitive(optionName, "webcam")) {
				WebcamEnumerator webcamEnumerator;
				webcamEnumerator.enumerateWebcamUrls(this->files);
			} else {
				return false;
			}
		} else {
			const char * filename = argv[i];
			if (strchr(filename, '*') != nullptr || strchr(filename, '?') != nullptr) {
				Jargon::FileSystem::globFiles(filename, this->files);
			} else{
				this->files.push_back(filename);
			}
		}
	}

	if (this->sortFiles) {
		std::sort(this->files.begin(), this->files.end(), Jargon::StringUtilities::caseInsensitiveSortFunctor);
	} else if (this->shuffleFiles) {
		std::random_device randomDevice;
		std::default_random_engine randomGenerator(randomDevice());
		std::shuffle(this->files.begin(), this->files.end(), randomGenerator);
	}

	return true;
}
