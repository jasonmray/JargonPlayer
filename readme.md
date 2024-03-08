# JargonPlayer

![JargonPlayer Logo](play_256x256.png)


## Overview
JargonPlayer is a minimalist media player based on libmpv. It's a pet project that I started hacking on after being frustrated by VLC's awkward hotkey support and flaky seeking.

Its featureset is mainly things I've personally needed at various times, but I'm releasing the code as a decent reference for using libmpv with SDL. The support for multiple video windows in the same process may also be helpful for people working with libmpv.

## Features
JargonPlayer can be driven entirely via the keyboard. I use it as the primary player on my living room PC, which I mostly use with a small wireless keyboard.

JargonPlayer supports multiple simultaneous play windows (that can be controlled sumultaneously). This is great for things like playing third-party commentary tracks or synchronizing multiple videos. The windows can be tiled across multiple monitors, and repositioned instantly via keyboard shortcuts.

Since it is built around libmpv JargonPlayer supports all the media formats of MPV, including images.

## Hotkeys

| key | action |
|-----|--------|
| caps-lock | When active, sends hotkeys to all open windows instead of just focused window. |
| ctrl + n | new window |
| space | play/pause |
| ctrl + space | play/pause all windows |
| left/right | seek 3s |
| alt + left/right | seek 1s |
| shift + left/right | seek 15s |
| pgUp/pgDown | seek 30s |
| shift + pgUp/pgDown | seek 60s |
| - | back 1 frame |
| = | forward 1 frame |
| home | seek to beginning of file |
| ctrl + left | prev file |
| ctrl + right | next file |
| tab | toggle playlist display
| F3 | shuffle playlist |
| f | enter fullscreen |
| esc | exit fullscreen |
| [ | 10% slower playback |
| ] | 10% faster playback |
| \ | reset playback speed |
| F2 | toggle slideshow mode for images |
| down | volume down  |
| up | volume up |
| m | toggle mute |
| , | decrease gamma & brightness |
| . | increase gamma & brightness |
| / | reset gamma & brightness |
| g | increase gamma |
| shift + g | decrease gamma |
| alt + g | reset gamma |
| ctrl + - | increase window transparency |
| ctrl + = | decrease window transparency |
| W, A, S, D | pan video up, left, down, right |
| q | zoom out |
| e | zoom in |
| r | reset pan & zoom |
| h | hide window chrome |
| k | pan audio left |
| l | pan audio right |
| t | next subtitles track |
| shift + t | prev subtitles track  |
| alt + t | toggle subtitle background color for visibility |
| y | next audio track |
| shift + y | prev audio track |
| ctrl + 9 | decrease audio frequency |
| ctrl + 0 | increase audio frequency |
| ctrl + 8 | reset audio frequency |
| i | toggle de-interlacing |
| o | cycle aspect ratio forward |
| shift + o | cycle aspect ratio backward |
| ctrl + o | reset aspect ratio |
| p | show perf stats and codec details |
| ctrl + c | copy path of current file to clipboard |
| ctrl + shift + c | copy current file to clipboard |
| ctrl + e | navigate to current file in Windows Explorer |
| ctrl + s | save raw video screenshot to photos folder |
| ctrl + shift + s | save raw video screenshot to same folder as current file |
| ctrl + alt + s | save rendered video screenshot (current size, color, transform, etc) to photos folder |
| ctrl + alt + shift + s | save rendered video screenshot (current size, color, transform, etc) to same folder as current file |
| n | minimize window |
| 1,2,3,4,5,6,7,8 | position the window in a quadrant of a monitor and hide chrome |
| alt + 1 | move window to fullscreen on monitor 1 |
| alt + 2 | move window to fullscreen on monitor 2 |
| ` | reset window to center of current monitor and show chrome |
| alt + = | resize window larger |
| alt + - | resize window smaller |
| ctrl + 1 | reset rotation |
| ctrl + 2 | set rotation to 90 degrees right |
| ctrl + 3 | set rotation to 180 degrees |
| ctrl + 4 | set rotation to 90 degrees left |
| ctrl + a | toggle always-on-top |
| ctrl + m | mirror video (may not work with hardware decoding on) |
| ctrl + f | flip video |
| ctrl + w | close window |
| ctrl + q | quit |

## Commandline options

| Commandline Option | Action |
|--------|--------|
| -tile | play files simultaneously, positioning windows each in a quarter of a monitor |
| -sort | sort list of files before playing (default) |
| -nosort | don't sort list of files before playing |
| -shuffle | shuffle list of files before playing |
| -skipimages | don't play image files |
| -skiparchives | don't try to open archive files |
| -noslideshow | don't auto-advance playlist, great for images |
| -disablehwdec | turn off hardware decoding |
| -webcam | display webcam |

## Building

The project should build cleanly with Visual Studio 2022.
1. Unzip the dependencies in 3rdParty.zip
2. Open `build\JargonPlayer.sln`
3. Build Release x64
4. Run `release.bat` in the root folder to collect the necessary binaries into a folder named `release`