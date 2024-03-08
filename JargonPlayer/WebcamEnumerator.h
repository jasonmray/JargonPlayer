#pragma once

#include <string>
#include <vector>

/*
	This class enumerates video source devices and returns the proper names & urls to open those 
	devices with mpv. It's a relatively heavyweight feature because it initializes COM and pulls
	in OLE & OLEAUT, but it's relegated to this class so it's easy to disable
*/
class WebcamEnumerator{
	public:
		WebcamEnumerator();
		~WebcamEnumerator();

		bool enumerateWebcamNames(std::vector<std::string>& webcamNamesOut);
		std::string buildWebcamFileUrl(const char* webcamName);

		bool enumerateWebcamUrls(std::vector<std::string>& webcamUrlsOut);

	private:
		bool initialized = false;
};
