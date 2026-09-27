# JargonPlayer

![JargonPlayer Logo](Resources/play_256x256.png)


## Overview
JargonPlayer is a minimalist media player based on libmpv. It's a pet project that I started hacking on after being frustrated by VLC's awkward hotkey support and flaky seeking.

Its featureset is mainly things I've personally needed at various times, but I'm releasing the code as a decent reference for using libmpv with SDL. The support for multiple video windows in the same process may also be helpful for people working with libmpv.

## Features
JargonPlayer can be driven entirely via the keyboard. I use it as the primary player on my living room PC, which I operate with a small wireless keyboard.

JargonPlayer supports multiple simultaneous play windows (that can be controlled simultaneously if you like). This is great for things like playing third-party commentary tracks, synchronizing videos, or just playing multiple videos at the same time. The windows can be tiled across multiple monitors, and repositioned instantly via keyboard shortcuts.

Since it is built around libmpv JargonPlayer supports all the media formats of MPV, including images. It can also load media from compressed archives like ZIP.

## Keyboard Operation

| Basic Actions | |
|-----|--------|
| Spacebar | Play/Pause |
| f | Enter Fullscreen |
| esc | Exit Fullscreen |
| Down Arrow | Volume Down  |
| Up Arrow | Volume Up |
| m | Toggle Mute |
| Ctrl + q | Quit |
| Ctrl + Left Arrow | Previous File in Playlist |
| Ctrl + Right Arrow | Next File in Playlist |
| Ctrl + Home | Go to First File in Playlist |
| Tab | Toggle Playlist Display
| F3 | Shuffle Playlist |

| Seeking | |
|-----|--------|
| Home | Seek to Beginning of File |
| Left Arrow | Seek Back 3 Seconds |
| Right Arrow | Seek Forward 3 Seconds |
| Alt + Left Arrow | Seek Back 1 Second |
| Alt + Right Arrow | Seek Forward 1 Second |
| Shift + Left Arrow | Seek Back 15 Seconds |
| Shift + Right Arrow | Seek Forward 15 Seconds |
| Page Up | Seek Back 30 Seconds |
| Page Down | Seek Forward 30 Seconds |
| Shift + Page Up | Seek Back 1 Minute |
| Shift + Page Down | Seek Forward 1 Minute |
| Ctrl + Shift + Page Up | Seek Back 5 minutes |
| Ctrl + Shift + Page Down | Seek Forward 5 minutes |
| - | Step Back 1 Frame |
| = | Step Forward 1 Frame |

| Window Operations | |
|---------|--------|
| Caps Lock | When active, keyboard actions are sent to all open windows instead of just focused window |
| Ctrl + n | New Window |
| Ctrl + w | Close Window |
| Ctrl + a | Toggle Always-On-Top |
| Ctrl + Spacebar | Play/Pause All Windows |
| n | Minimize Window |
| h | Hide Titlebar |
| Alt + - | Resize Window Smaller |
| Alt + = | Resize Window Larger |
| Ctrl + - | Make Window More Transparent |
| Ctrl + = | Make Window More Opaque |
| 1, 2, 3, 4, 5, 6, 7, 8 | Position the window in a quadrant of a monitor and hide titlebar |
| Alt + 1 | Move window to fullscreen on monitor 1 |
| Alt + 2 | Move window to fullscreen on monitor 2 |
| ` | Reset window to center of current monitor and show titlebar |

| Playback Operations | |
|-----|--------|
| t | Cycle to Next Subtitle Track |
| Shift + t | Cycle to Previous Subtitles Track  |
| y | Cycle to Next Audio Track |
| Shift + y | Cycle to Previous Audio Track |
| [ | 10% Slower Playback |
| ] | 10% Faster Playback |
| \ | Reset Playback Speed to Normal |
| F2 | Toggle Auto-Advance for Images |
| Ctrl + [ | Decrease Image Display Duration 1 Second |
| Ctrl + ] | Increase Image Display Duration 1 Second |
| \ | Reset Image Display Duration to Default |

| Display Operations | |
|----------------------------|--------|
| p | Toggle perf stats and codec details |
| ' | Toggle full filename and system status display |
| w, a, s, d | Pan Video/Image Display |
| q | Zoom Out |
| e | Zoom In |
| r | Reset Pan & Zoom |
| i | Toggle De-interlacing |
| o | Cycle forward through common aspect ratios |
| Shift + o | Cycle backward through common aspect ratios |
| Ctrl + o | Reset to Default Aspect Ratio |
| Ctrl + m | Mirror video (may not work with hardware decoding on) |
| Ctrl + f | Flip video (may not work with hardware decoding on) |
| Ctrl + 2 | Set rotation to 90 degrees right |
| Ctrl + 3 | Set rotation to 180 degrees |
| Ctrl + 4 | Set rotation to 90 degrees left |
| Ctrl + 1 | Reset to default rotation |
| , | Decrease Gamma |
| . | Increase Gamma |
| / | Reset Gamma to Default |
| Alt + t | Toggle subtitle background color for visibility |
| k | Pan Audio Left |
| l | Pan Audio Right |
| Shift + k | Reset Audio Pan |
| Shift + l | Reset Audio Pan |
| Ctrl + 9 | Decrease Audio Frequency |
| Ctrl + 0 | Increase Audio Frequency |
| Ctrl + 8 | Reset Audio Frequency |

| Misc Operations | |
|-----|--------|
| Ctrl + c | Copy path of current file to clipboard |
| Ctrl + Shift + c | Copy current file to clipboard |
| Ctrl + Shift + x | Cut current file to clipboard |
| Ctrl + e | Navigate to current file in Windows Explorer |
| Ctrl + s | Save video screenshot to Windows Screenshots folder |
| Ctrl + Shift + s | Save video screenshot to same folder as current file |

## Mouse Actions
| Mouse Actions | |
|-----|--------|
| Double Click | Toggle fullscreen |
| Drag | Move window |
| Mouse Wheel | Increase/Decrease Volume |

## Game Controller Actions
| Gamepad Actions | |
|-----|--------|
| A Button | Play/Pause |
| B Button | Toggle Fullscreen |
| Start Button | Toggle Playlist Display |
| Left Shoulder | Previous Playlist Item |
| Right Shoulder | Next Playlist Item |
| Left Stick | Seek Forward/Backward | 
| Right Stick | Pan | 
| Left Trigger | Zoom Out | 
| Right Trigger | Zoom In | 
| X Button | Reset Pan & Zoom |
| D-pad Up / D-pad Down | Cycle Subtitle Tracks |
| D-pad Left / D-pad Right | Cycle Audio Tracks |

## Command Line options

`JargonPlayer.exe [options] <files or folders to play>`

| Option | Action |
|--------|--------|
| -fullscreen | Start with fullscreen window (no titlebar) |
| -maximize | Start with maximized window (with titlebar) |
| -tile | Play given files/folders simultaneously, each in its own window, tiling the windows across displays |
| -sort | Sort list of files before playing (default) |
| -nosort | Don't sort list of files before playing |
| -shuffle | Shuffle list of files before playing |
| -skipimages | Don't play image files |
| -skiparchives | Don't try to open archive files |
| -noslideshow | Don't auto-advance playlist, great for images |
| -disablehwdec | Turn off hardware decoding |
| -webcam | Display webcam |

## Ini Config File
A few options can be configured via `JargonPlayer.ini`

This file is optional. It should be placed in the same folder as `JargonPlayer.exe`

```
[JargonPlayer]

# If enabled, allows mpv lib to load config files from the folder that contains JargonPlayer.exe
EnableMpvConfig = false

# If set, overrides default location to save screenshots. Note: folder must exist
ScreenshotsFolder = c:\screenshots

# If enabled, player will automatically step to next frame after taking a screenshot.
# This allows rapid capture of frame sequences with ctrl+s
AutoAdvanceAfterScreenshot = true

# File type for screenshots
# Must be a type supported by mpv: png, jpg, jpeg, webp, jxl, avif
ScreenshotFiletype = jpg
```


## Local Webcam Viewing
Launch with `JargonPlayer.exe -webcam` to display video from the system camera. If your system has multiple cameras, they should appear in the playlist. You can cycle through them with normal playlist operations such as **Ctrl + Right Arrow** and **Ctrl + Left Arrow**

## Building

The project should build cleanly with Visual Studio 2022.
1. Unzip the dependencies in 3rdParty.zip
2. Open `JargonPlayer.sln`
3. Build Release x64
4. Run `release.bat` in the root folder to collect the necessary binaries into a folder named `release`