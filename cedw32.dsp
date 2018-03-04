# Microsoft Developer Studio Project File - Name="cedw32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=cedw32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cedw32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cedw32.mak" CFG="cedw32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cedw32 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "cedw32 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cedw32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1c09 /d "NDEBUG"
# ADD RSC /l 0x1c09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /machine:I386 /libpath:"c:\Program Files\Microsoft Platform SDK\Lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=cver cedw.ver
# End Special Build Tool

!ELSEIF  "$(CFG)" == "cedw32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W1 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1c09 /d "_DEBUG"
# ADD RSC /l 0x1c09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"c:\Program Files\Microsoft Platform SDK\Lib"

!ENDIF 

# Begin Target

# Name "cedw32 - Win32 Release"
# Name "cedw32 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\blck.c
# End Source File
# Begin Source File

SOURCE=.\bmrk.c
# End Source File
# Begin Source File

SOURCE=.\cedw32.rc
# End Source File
# Begin Source File

SOURCE=.\cmnd.c
# End Source File
# Begin Source File

SOURCE=.\crma.c
# End Source File
# Begin Source File

SOURCE=.\dlg_file.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_options.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_prompts.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_search.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgs_win.c
# End Source File
# Begin Source File

SOURCE=.\edit.c
# End Source File
# Begin Source File

SOURCE=.\file.c
# End Source File
# Begin Source File

SOURCE=.\file_w32.c
# End Source File
# Begin Source File

SOURCE=.\find.c
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\scrn.c
# End Source File
# Begin Source File

SOURCE=.\string2.cpp
# End Source File
# Begin Source File

SOURCE=.\tree.cpp
# End Source File
# Begin Source File

SOURCE=.\undo.c
# End Source File
# Begin Source File

SOURCE=.\vdeo_win.c
# End Source File
# Begin Source File

SOURCE=.\wnd_clipboard.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd_controls.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd_edit.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd_init.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd_kbd.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd_main.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd_mouse.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd_print.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd_status.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd_tree.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ced.h
# End Source File
# Begin Source File

SOURCE=.\cedw.h
# End Source File
# Begin Source File

SOURCE=.\string2.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00018.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00019.bmp
# End Source File
# Begin Source File

SOURCE=.\c_class.bmp
# End Source File
# Begin Source File

SOURCE=.\cedw.ico
# End Source File
# Begin Source File

SOURCE=.\mssqk.bmp
# End Source File
# Begin Source File

SOURCE=.\ora_func.bmp
# End Source File
# End Group
# End Target
# End Project
