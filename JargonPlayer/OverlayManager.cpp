#include "OverlayManager.h"
#include "DeviceStatus.h"
#include "MpvCommands.h"
#include "PlaylistDisplay.h"

#include <cassert>



static void SetMpvOverlayAssData(mpv_handle* mpv, int64_t overlayId, const char* assEventData) {
	mpv_node node = { 0 };
	mpv_node_list list = { 0 };
	char* keys[4] = { 0 };
	mpv_node values[4] = { 0 };
	mpv_node result = { 0 };

	node.format = MPV_FORMAT_NODE_MAP;
	node.u.list = &list;

	list.values = values;
	list.keys = keys;
	list.num = 4;

	keys[0] = (char*)"name";
	values[0].format = MPV_FORMAT_STRING;
	values[0].u.string = (char*)"osd-overlay";

	keys[1] = (char*)"id";
	values[1].format = MPV_FORMAT_INT64;
	values[1].u.int64 = overlayId;

	keys[2] = (char*)"format";
	values[2].format = MPV_FORMAT_STRING;
	values[2].u.string = (char*)"ass-events";

	keys[3] = (char*)"data";
	values[3].format = MPV_FORMAT_STRING;
	values[3].u.string = (char*)assEventData;

	mpv_command_node(mpv, &node, &result);
}

static void HideMpvOverlay(mpv_handle* mpv, int64_t overlayId) {
	mpv_node node = { 0 };
	mpv_node_list list = { 0 };
	char* keys[4] = { 0 };
	mpv_node values[4] = { 0 };
	mpv_node result = { 0 };

	node.format = MPV_FORMAT_NODE_MAP;
	node.u.list = &list;

	list.values = values;
	list.keys = keys;
	list.num = 4;

	keys[0] = (char*)"name";
	values[0].format = MPV_FORMAT_STRING;
	values[0].u.string = (char*)"osd-overlay";

	keys[1] = (char*)"id";
	values[1].format = MPV_FORMAT_INT64;
	values[1].u.int64 = overlayId;

	keys[2] = (char*)"format";
	values[2].format = MPV_FORMAT_STRING;
	values[2].u.string = (char*)"none";

	keys[3] = (char*)"data";
	values[3].format = MPV_FORMAT_STRING;
	values[3].u.string = (char*)"";

	mpv_command_node(mpv, &node, &result);
}

OverlayManager::OverlayManager(VideoWindow& videoWindow) :
	videoWindow(videoWindow),
	currentOverlayType(OverlayType_None)
{
}

OverlayManager::~OverlayManager(){
}

void OverlayManager::toggleOverlay(mpv_handle* mpv, OverlayType overlayType) {
	if (overlayType == currentOverlayType) {
		hideOverlay(mpv);
	} else {
		hideOverlay(mpv);
		showOverlay(mpv, overlayType);
	}
}

void OverlayManager::showOverlay(mpv_handle* mpv, OverlayType overlayType) {
	if (overlayType == OverlayType_None) {
		// nothing to do
	} else if (overlayType == OverlayType_Playlist) {
		mpv_node playlistNode;
		if (mpv_get_property(mpv, "playlist", MPV_FORMAT_NODE, &playlistNode) >= 0) {
			currentOverlayType = OverlayType_Playlist;

			notifyPlaylistUpdate(mpv, playlistNode);
			mpv_free_node_contents(&playlistNode);
		}
	} else if (overlayType == OverlayType_Status) {
		const std::string deviceStatus = DeviceStatus::BuildDeviceStatusString(videoWindow);
		showAssOverlay(mpv, deviceStatus.c_str());
	} else if (overlayType == OverlayType_PerfStats) {
		mpv_command(mpv, MpvCommands::TogglePerfStats);
	} else {
		assert(false);
	}
	currentOverlayType = overlayType;
}

void OverlayManager::hideOverlay(mpv_handle* mpv) {
	if (currentOverlayType == OverlayType_None) {
		// nothing to do
	} else if (currentOverlayType == OverlayType_Playlist) {
		HideMpvOverlay(mpv, OverlayId);
	} else if (currentOverlayType == OverlayType_Status) {
		HideMpvOverlay(mpv, OverlayId);
	} else if (currentOverlayType == OverlayType_PerfStats) {
		mpv_command(mpv, MpvCommands::TogglePerfStats);
	} else {
		assert(false);
	}
	currentOverlayType = OverlayType_None;
}

void OverlayManager::notifyPlaylistUpdate(mpv_handle* mpv, const mpv_node& playlistNode) {
	if (currentOverlayType != OverlayType_Playlist) {
		return;
	}

	std::string playlistData;
	PlaylistDisplay::BuildPlaylistData(mpv, playlistNode, playlistData);
	showAssOverlay(mpv, playlistData.c_str());
}

void OverlayManager::showAssOverlay(mpv_handle* mpv, const char* data) {
	SetMpvOverlayAssData(mpv, OverlayId, data);
}
