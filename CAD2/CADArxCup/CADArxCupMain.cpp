//////////////////////////////////////////////////////////////////////
// CADArxCupMain.cpp
// Der Einstiegspunkt f�r die AutoCAD-Erweiterung CADArxCup.arx
//////////////////////////////////////////////////////////////////////

// Damit mit Warnungsstufe 4 �bersetzt werden kann:
#pragma warning( disable : 4100 )
#pragma warning( disable : 4201 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4512 )


#include <aced.h>
#include <rxregsvc.h>
#include <tchar.h>

void CADArxCupCmd();

void InitApp()
{
    acedRegCmds->addCommand(_T("CADARX_COMMANDS"), _T("Cup"), _T("Pokal"),
                            ACRX_CMD_TRANSPARENT, CADArxCupCmd);
}

void UnloadApp()
{
    acedRegCmds->removeGroup(_T("CADARX_COMMANDS"));
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
