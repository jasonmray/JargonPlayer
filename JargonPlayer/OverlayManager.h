#pragma once

#include <mpv/client.h>

#include <string>

class VideoWindow;

enum OverlayType {
	OverlayType_None,
	OverlayType_Playlist,
	OverlayType_Status,
	OverlayType_PerfStats,
};

class OverlayManager{
	public:
		OverlayManager(VideoWindow& videoWindow);
		~OverlayManager();

		void toggleOverlay(mpv_handle* mpv, OverlayType overlayType);
		void showOverlay(mpv_handle* mpv, OverlayType overlayType);
		void hideOverlay(mpv_handle* mpv);

		void notifyPlaylistUpdate(mpv_handle* mpv, const mpv_node& playlistNode);

	private:
		static const int64_t OverlayId = 5;

		VideoWindow& videoWindow;
		OverlayType currentOverlayType;

		void showAssOverlay(mpv_handle* mpv, const char* data);
};

