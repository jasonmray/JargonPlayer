#pragma once

#include <vector>

class ProgramOptions{
	public:
		static ProgramOptions Instance;

		enum class OpenMode {
			Enqueue,
			Tile
		};

		ProgramOptions();
		~ProgramOptions();

		bool processOptions(int argc, const char *argv[]);

		OpenMode openMode;
		bool sortFiles;
		bool shuffleFiles;
		bool skipImages;
		bool useHardwareDecoding;
		std::vector<const char*> files;
};
