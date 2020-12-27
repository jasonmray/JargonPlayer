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
		bool isImageType(const std::string& ext);

		std::set<std::string> imageExtensions;
};
