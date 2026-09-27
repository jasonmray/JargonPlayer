#pragma once

#include <string>

struct mpv_handle;
struct mpv_node;

class PlaylistDisplay {
	public:
		static void BuildPlaylistData(mpv_handle* mpv, const mpv_node& playlistNode, std::string& assEventsOut);

	private:

};
