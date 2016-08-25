# Microsoft Developer Studio Project File - Name="PDImagem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PDImagem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PDImagem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PDImagem.mak" CFG="PDImagem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PDImagem - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PDImagem - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PDImagem - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x416 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "PDImagem - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x416 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x416 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "PDImagem - Win32 Release"
# Name "PDImagem - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgElemEstr1.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgElemEstr2.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFatorZoom.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHistograma.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLimiarManual.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOpLogica.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPRDistancia.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PDIBase.cpp
# End Source File
# Begin Source File

SOURCE=.\PDImagem.cpp
# End Source File
# Begin Source File

SOURCE=.\PDImagem.rc
# End Source File
# Begin Source File

SOURCE=.\PDImagemDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PDImagemView.cpp
# End Source File
# Begin Source File

SOURCE=.\PDIMorfb.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\DlgElemEstr1.h
# End Source File
# Begin Source File

SOURCE=.\DlgElemEstr2.h
# End Source File
# Begin Source File

SOURCE=.\DlgFatorZoom.h
# End Source File
# Begin Source File

SOURCE=.\DlgHistograma.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgLimiarManual.h
# End Source File
# Begin Source File

SOURCE=.\DlgOpLogica.h
# End Source File
# Begin Source File

SOURCE=.\DlgPRDistancia.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PDIBase.h
# End Source File
# Begin Source File

SOURCE=.\PDImagem.h
# End Source File
# Begin Source File

SOURCE=.\PDImagemDoc.h
# End Source File
# Begin Source File

SOURCE=.\PDImagemView.h
# End Source File
# Begin Source File

SOURCE=.\PDIMorfb.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Bmphi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bmphs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bmpsi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cruz.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Linhah.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Linhav.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PDImaDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\PDImagem.ico
# End Source File
# Begin Source File

SOURCE=.\res\PDImagem.rc2
# End Source File
# Begin Source File

SOURCE=.\res\PDImagemDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Rombus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Tres.bmp
# End Source File
# End Group
# End Target
# End Project
