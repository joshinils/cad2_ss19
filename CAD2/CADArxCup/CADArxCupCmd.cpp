//////////////////////////////////////////////////////////////////////
// CADArxCupCmd.cpp
// Ein Kommando zum Erzeugen eines Pokals
//////////////////////////////////////////////////////////////////////

#include "CADArxCup.h"

void CADArxCupCmd()
{
	try
	{
		CADArxCup cup;
		if ( cup.DataInput() && cup.Calc() )
			cup.Create();
	}
	catch (const std::exception&)
	{ }
}

