//////////////////////////////////////////////////////////////////////
// CADArxLetterCmd.cpp
// Ein Kommando zum Erzeugen eines Buchstabenprofils
//////////////////////////////////////////////////////////////////////

#include "CADArxLetter.h"

void CADArxLetterCmd()
{
    CADArxLetter letter;
    if (letter.DataInput())
    {
        letter.Create();
    }
}

