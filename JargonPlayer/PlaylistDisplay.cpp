#include "PlaylistDisplay.h"
#include "Util.h"

#include "Jargon/FileSystem/Utilities.h"
#include "Jargon/StringUtilities.h"

#include <mpv/client.h>


class AssEventsBuilder {
	public:
		AssEventsBuilder(std::string& assEventsString) : 
			assEventsString(assEventsString)
		{
		}

		void emptyLine() {
			appendRaw(defaultFontString);
			assEventsString.append("\\N");
		}

		void writeNewline() {
			assEventsString.append("\\N");
		}

		void writeFontCommand(int fontSize) {
			appendRawFmt("{\\fs%d}", fontSize);
		}

		void writeFontCommand(int fontSize, bool bold) {
			appendRawFmt("{\\b%d\\fs%d}", bold ? 1 : 0, fontSize);
		}

		void appendRaw(const char* raw) {
			assEventsString.append(raw);
		}

		void appendLine(int fontSize, const char* formatString, ...) {
			va_list args;
			va_start(args, formatString);
			writeFontCommand(fontSize);
			appendRawVarArgs(formatString, args);
			writeNewline();
			va_end(args);
		}

		void appendLine(const char* formatString, ...) {
			va_list args;
			va_start(args, formatString);
			appendRaw(defaultFontString);
			appendRawVarArgs(formatString, args);
			writeNewline();
			va_end(args);
		}

	private:
		int defaultFontSize = 25;
		const char* defaultFontString = "{\\fs25}";
		std::string& assEventsString;

		void appendRawFmt(const char* formatString, ...) {
			va_list args;
			va_start(args, formatString);
			appendRawVarArgs(formatString, args);
			va_end(args);
		}

		void appendRawVarArgs(const char* formatString, va_list args) {
			std::string s = Jargon::StringUtilities::formatVarArgs(formatString, args);
			assEventsString.append(s);
		}
};

void PlaylistDisplay::BuildPlaylistData(mpv_handle* mpv, const mpv_node& playlistNode, std::string& assEventsOut) {
	assEventsOut.clear();

	AssEventsBuilder assEventsBuilder(assEventsOut);

	if (playlistNode.format == MPV_FORMAT_NODE_ARRAY) {
		const mpv_node_list* playlist = playlistNode.u.list;

		assEventsBuilder.emptyLine();
		assEventsBuilder.emptyLine();

		int64_t currentPlaylistPos = -1;
		mpv_get_property(mpv, "playlist-pos", MPV_FORMAT_INT64, &currentPlaylistPos);

		if (currentPlaylistPos < 0) {
			currentPlaylistPos = 0;
		}

		const int maxPlaylistDisplayItems = 20;
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
			assEventsBuilder.appendLine("No Items");
		} else {

			if (displayStartIndex > 0) {
				assEventsBuilder.appendLine("... %d more items ...", displayStartIndex);
			} else {
				assEventsBuilder.emptyLine();
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
						assEventsBuilder.writeFontCommand(25, isCurrent);

						if (isCurrent) {
							assEventsBuilder.appendRaw(" > ");
						}

						const std::string baseFilename(Jargon::FileSystem::getFilenameComponent(filename));
						assEventsBuilder.appendLine("[%d]  %s", playlistIndex, baseFilename.c_str());
					}
				}
			}

			if (displayEndIndex < playlistLength) {
				const int remainingItems = playlistLength - displayEndIndex;
				assEventsBuilder.appendLine("... %d more items ...", remainingItems);
			}
		}
	}
}
