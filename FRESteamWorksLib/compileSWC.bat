SET AIR_SDK=C:\sdk\flex_sdk_4.6.0.23201B_air_3.2
set location=C:\projects\fresteamworks\fresteamworkslib\

set filename=FRESteamWorksLib
call "%AIR_SDK%\bin\acompc.bat" -swf-version 13 -source-path "%location%src" -include-sources "%location%src"  -optimize -output "%location%bin\%filename%.swc"

:error
pause

:end