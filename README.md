**UPDATE:** Please refer to CNLohr's video on OpenVR from C instead: https://www.youtube.com/watch?v=r6kM3tR03g4

# tdm-gcc-openvr
Get VR working from C with the smallest possible amount of code

This snippet features loading `openvr_api.dll` using WinAPI's LoadLibrary() function.

I couldn't get anything from https://github.com/ValveSoftware/openvr/issues/133 to work and decided to go an alternative route. The method here does not require patching headers with python scripts or messing with the linker in any way, while also using the most up to date version of the OpenVR dll.

Take `openvr_api.dll` from `steamapps/common/SteamVR/bin/win64/`
