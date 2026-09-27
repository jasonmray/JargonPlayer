#pragma once

#include <string>
#include <vector>

class ProgramOptions{
	public:
		static ProgramOptions Instance;

		enum class OpenMode {
			Enqueue,
			Tile
		};

		enum class WindowMode {
			Normal,
			Fullscreen,
			Maximized
		};

		ProgramOptions();
		~ProgramOptions();

		bool processOptions(int argc, const char *argv[]);

		OpenMode openMode;
		WindowMode windowMode;
		bool sortFiles;
		bool shuffleFiles;
		bool skipImages;
		bool skipArchives;
		bool slideshowEnabled;
		bool useHardwareDecoding;
		
		std::vector<std::string> files;
};
