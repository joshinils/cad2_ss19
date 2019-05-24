//////////////////////////////////////////////////////////////////////
// CADArxGearCmd.cpp
// Ein Kommando zum Erzeugen eines Zahnradprofils
//////////////////////////////////////////////////////////////////////

#include "CADArxGear.h"

void CADArxGearCmd()
{
    try
    {
        CADArxGear gear;
        if (gear.DataInput() && gear.Calc())
            gear.Create();
    }
    catch (const std::exception&) { }
}

