SET AIR_SDK=C:\sdk\flex_sdk_4.6.0.23201B_air_3.2
SET WINRAR=C:\Program Files\WinRAR\
call "%WINRAR%\winrar" e FRESteamWorksLib.swc
call "%AIR_SDK%\bin\adt" -package -target ane FRESteamWorks.ane descriptor.xml -swc FRESteamWorksLib.swc -platform Windows-x86 library.swf FRESteamWorks.dll -platform MacOS-x86 library.swf FRESteamWorks.framework
del library.swf
del catalog.xml

:error
pause

:end