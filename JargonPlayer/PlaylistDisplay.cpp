#include "PlaylistDisplay.h"
#include "Util.h"

#include "Jargon/StringUtilities.h"

#include <libmpv/include/client.h>

#include <string>


void PlaylistDisplay::DisplayPlaylist(mpv_handle* mpv, const mpv_node& playlistNode) {

	if (playlistNode.format == MPV_FORMAT_NODE_ARRAY) {
		const mpv_node_list* playlist = playlistNode.u.list;

		std::string listString;
		listString.append("{\\fs25} \\N");
		listString.append("{\\fs25} \\N");

		int64_t currentPlaylistPos = -1;
		mpv_get_property(mpv, "playlist-pos", MPV_FORMAT_INT64, &currentPlaylistPos);

		if (currentPlaylistPos < 0) {
			currentPlaylistPos = 0;
		}

		const int maxPlaylistDisplayItems = 12;
		const int playlistLength = playlist->num;

		int displayStartIndex = 0;
		if (playlistLength > maxPlaylistDisplayItems) {
			if (currentPlaylistPos >= 3) {
				displayStartIndex = (int)currentPlaylistPos - 3;

				const int remainingItems = playlistLength - displayStartIndex;
				if (remainingItems < maxPlaylistDisplayItems) {
					displayStartIndex = playlistLength - maxPlaylistDisplayItems;
				}
			}
		}

		int numItemsToDisplay = 0;
		if (playlistLength > 0) {
			const int remainingItems = playlistLength - displayStartIndex;
			numItemsToDisplay = std::min(remainingItems, maxPlaylistDisplayItems);
		}

		const int displayEndIndex = displayStartIndex + numItemsToDisplay;

		if (playlistLength == 0) {
			listString.append("No Items");
		}
		else {


			if (displayStartIndex > 0) {
				const std::string precedingString = Jargon::StringUtilities::format("{\\fs25}... %d more items ...\\N", displayStartIndex);
				listString.append(precedingString);
			}
			else {
				listString.append("{\\fs25} \\N");
			}

			for (int i = 0; i < numItemsToDisplay; i++) {
				const int playlistIndex = displayStartIndex + i;

				const mpv_node& playlistEntry = playlist->values[playlistIndex];

				if (playlistEntry.format == MPV_FORMAT_NODE_MAP) {
					const mpv_node_list* entryProperties = playlistEntry.u.list;

					const char* filename = nullptr;
					bool isCurrent = false;

					for (int propertyIndex = 0; propertyIndex < entryProperties->num; propertyIndex++) {
						const char* propertyName = entryProperties->keys[propertyIndex];
						const mpv_node& propertyValue = entryProperties->values[propertyIndex];

						if (std::string("filename") == propertyName) {
							filename = propertyValue.u.string;
						}

						if (std::string("current") == propertyName) {
							isCurrent = propertyValue.u.flag != 0;
						}
					}

					if (filename != nullptr) {
						if (isCurrent) {
							listString.append("{\\b1\\fs25} > ");
						}
						else {
							listString.append("{\\b0\\fs25}");
						}

						const std::string number = Jargon::StringUtilities::format("[%d]  ", playlistIndex);
						listString.append(number);

						const std::string baseFilename = Util::getBaseFilename(filename);
						listString.append(baseFilename);
						listString.append("\n");
					}
				}
			}

			if (displayEndIndex < playlistLength) {
				const int remainingItems = playlistLength - displayEndIndex;
				const std::string remainingString = Jargon::StringUtilities::format("{\\fs25}... %d more items ...\\N", remainingItems);
				listString.append(remainingString);
			}
		}

		//		const char* command[] = {"osd-overlay", "99", "ass-events", listString.c_str(), 0};
		//		mpv_command(mpv, command);

		mpv_node        node;
		mpv_node_list   list;
		char* keys[4];
		mpv_node        values[4];
		mpv_node        result;

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
		values[1].u.int64 = PlaylistOverlayId;

		keys[2] = (char*)"format";
		values[2].format = MPV_FORMAT_STRING;
		values[2].u.string = (char*)"ass-events";

		keys[3] = (char*)"data";
		values[3].format = MPV_FORMAT_STRING;
		values[3].u.string = (char*)listString.c_str();

		mpv_command_node(mpv, &node, &result);

	}
}


void PlaylistDisplay::HidePlaylist(mpv_handle* mpv) {
	//const char* command[] = { "osd-overlay", "99", "none", 0 };
	//mpv_command(mpv, command);
	//
	const char* command2[] = { "overlay-remove", "99", 0 };
	mpv_command(mpv, command2);


	mpv_node        node;
	mpv_node_list   list;
	char* keys[5];
	mpv_node        values[5];
	mpv_node        result;

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
	values[1].u.int64 = PlaylistOverlayId;

	keys[2] = (char*)"format";
	values[2].format = MPV_FORMAT_STRING;
	values[2].u.string = (char*)"ass-events";

	keys[3] = (char*)"data";
	values[3].format = MPV_FORMAT_STRING;
	values[3].u.string = (char*)"";

	mpv_command_node(mpv, &node, &result);
}