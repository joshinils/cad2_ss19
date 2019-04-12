//////////////////////////////////////////////////////////////////////
// TestARX.cpp
// Der Einstiegspunkt für die AutoCAD-Erweiterung TestARX
//////////////////////////////////////////////////////////////////////

// Damit mit Warnungsstufe 4 übersetzt werden kann:
#pragma warning( disable : 4100 )
#pragma warning( disable : 4201 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4512 )


#include <aced.h> 
#include <rxregsvc.h>
#include "tchar.h"

void TestARXCmd() 
{ 
    acutPrintf(_T("\nTestARX command!")); 
}

void InitApp() 
{ 
    acedRegCmds->addCommand(_T("TESTARX_COMMANDS"), _T("TestARX"), _T("TesteARX"), 
                            ACRX_CMD_TRANSPARENT, TestARXCmd); 
}

void UnloadApp() 
{ 
    acedRegCmds->removeGroup(_T("TESTARX_COMMANDS")); 
}

extern "C" AcRx::AppRetCode 
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt) 
{ 
    switch (msg) 
    { 
        case AcRx::kInitAppMsg: 
            acrxDynamicLinker->unlockApplication(pkt); 
            acrxRegisterAppMDIAware(pkt); 
            InitApp(); 
            break; 
        case AcRx::kUnloadAppMsg: 
            UnloadApp(); 
            break; 
        default: 
            break; 
    }
    return AcRx::kRetOK;
}
