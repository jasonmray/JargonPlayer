#include "PlaylistFilter.h"
#include "ProgramOptions.h"
#include "Util.h"

#include "Jargon/StringUtilities.h"

#include <libmpv/include/client.h>
#include <algorithm>
#include <cctype>
#include <vector>


PlaylistFilter::PlaylistFilter(){
	imageExtensions.insert("jpg");
	imageExtensions.insert("jpeg");
	imageExtensions.insert("gif");
	imageExtensions.insert("png");
	imageExtensions.insert("bmp");

	archiveExtensions.insert("zip");
	archiveExtensions.insert("rar");
}

PlaylistFilter::~PlaylistFilter(){
}

void PlaylistFilter::handlePlaylistChange(mpv_handle* mpv, const mpv_node& propertyEventData){
	if (ProgramOptions::Instance.skipImages || ProgramOptions::Instance.skipArchives){
		std::vector<int> toRemove;

		if (propertyEventData.format == MPV_FORMAT_NODE_ARRAY) {
			const mpv_node_list* playlistEntries = propertyEventData.u.list;

			for (int playlistIndex = 0; playlistIndex < playlistEntries->num; playlistIndex++) {
				const mpv_node& playlistEntry = playlistEntries->values[playlistIndex];

				if (playlistEntry.format == MPV_FORMAT_NODE_MAP) {
					const mpv_node_list* entryProperties = playlistEntry.u.list;

					for (int propertyIndex = 0; propertyIndex < entryProperties->num; propertyIndex++) {
						const char* propertyName = entryProperties->keys[propertyIndex];
						const mpv_node& propertyValue = entryProperties->values[propertyIndex];

						Util::debugLog(propertyName, propertyValue);
						if (std::string("filename") == propertyName) {
							const char* filename = propertyValue.u.string;
							std::string ext = Util::getFileExtension(filename);

							if (ProgramOptions::Instance.skipImages && isType(ext, imageExtensions)){
								Util::log("toRemove: %d\n", playlistIndex);
								toRemove.push_back(playlistIndex);
							}

							if (ProgramOptions::Instance.skipArchives && isType(ext, archiveExtensions)) {
								Util::log("toRemove: %d\n", playlistIndex);
								toRemove.push_back(playlistIndex);
							}
						}
					}
				}
			}
		}

		for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it) {
			std::string indexString = Jargon::StringUtilities::format("%i", *it);
			const char* command[] = {"playlist-remove", indexString.c_str(), 0};
			Util::log("removing playlist item %d\n", *it);
			mpv_command(mpv, command);
		}
	}
}

bool PlaylistFilter::isType(const std::string& ext, const std::set<std::string>& extensions) {
	std::string lowerCaseExt = ext;
	std::transform(lowerCaseExt.begin(), lowerCaseExt.end(), lowerCaseExt.begin(),
		[](unsigned char c) { return std::tolower(c); });

	if (extensions.find(lowerCaseExt) != extensions.end()) {
		return true;
	}

	return false;
}
