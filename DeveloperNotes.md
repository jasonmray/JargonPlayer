## Upgrading MPV

I'm using Windows builds from zhongfly:

https://github.com/zhongfly/mpv-winbuild/releases

Grab the asset named:

	mpv-dev-x86_64-YYYYMMDD-git-xxxxxxxxxx.7z

You can use the 'v3' release if you have a more modern CPU:

	mpv-dev-x86_64-v3-YYYYMMDD-git-xxxxxxxxxx.7z


After extracting the archive with 7-zip:

	move <extracted>\include\mpv\*.* 3rdParty\libmpv\include\mpv\
	move <extracted>\libmpv-2.dll 3rdParty\libmpv\bin\x64\mpv-2.dll
	move <extracted>\libmpv.dll.a 3rdParty\libmpv\lib\x64\libmpv.dll.a

Note: do not delete `3rdParty\libmpv\x64\mpv.def`

Generate `mpv.lib` from `libmpv.dll.a` using Visual Studio's `lib.exe`:

	pushd 3rdParty\libmpv\lib\x64
	create_lib.bat
	popd