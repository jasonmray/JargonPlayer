#pragma once

struct mpv_handle;
struct mpv_node;

class PlaylistDisplay {
	public:
		static const int PlaylistOverlayId = 99;
		static void DisplayPlaylist(mpv_handle* mpv, const mpv_node& playlistNode);
		static void HidePlaylist(mpv_handle* mpv);

	private:

};
