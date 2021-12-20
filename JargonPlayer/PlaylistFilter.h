#pragma once

struct mpv_handle;
struct mpv_node;

#include <set>
#include <string>

class PlaylistFilter{
	public:
		PlaylistFilter();
		~PlaylistFilter();

		void handlePlaylistChange(mpv_handle* mpv, const mpv_node& propertyEventData);

	private:
		bool isType(const std::string& ext, const std::set<std::string>& extensions);

		std::set<std::string> imageExtensions;
		std::set<std::string> archiveExtensions;
};
