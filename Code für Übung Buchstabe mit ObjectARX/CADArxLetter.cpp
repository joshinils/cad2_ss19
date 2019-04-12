//////////////////////////////////////////////////////////////////////
// CADArxLetter.cpp
// Klasse CADArxLetter
// Aufgabe: Beschreibung, Berechnung und Erstellung eines Evolventenzahrades
//          mit der objectARX-API
//////////////////////////////////////////////////////////////////////

#include "CADArxLetter.h"

// Damit mit Warnungsstufe 4 übersetzt werden kann:
#pragma warning( disable : 4100 )
#pragma warning( disable : 4201 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4512 )

#include <aced.h>
#include <dbents.h>
#include <dbgroup.h>
#include <dbapserv.h>
#include <adscodes.h>
#include <acedads.h>
#include <math.h>
#include <stdio.h>
#include <tchar.h>

// Zum Testen ist das "Epsilon" relativ groß gewählt.
#define CADArx_Length_Eps  0.001

const ads_real CADArxLetter::_rPi = 3.14159265359;

CADArxLetter::CADArxLetter(void)
{
    _bInitialized = false;
}

CADArxLetter::~CADArxLetter(void)
{
}

bool CADArxLetter::DataInput(void)
{
    _bInitialized = false;

    int iRet;

    acedInitGet(RSG_NONEG,NULL);
    iRet = acedGetReal(_T("\nBreite: "),&_rWidth);
    if ( RTNORM == iRet )
    {
        acedInitGet(RSG_NONEG,NULL);
        iRet = acedGetReal(_T("\nHöhe: "),&_rHeight);
    }    
    if ( RTNORM == iRet )
    {
        acedInitGet(RSG_NONEG,NULL);
        iRet = acedGetReal(_T("\nAbstand: "),&_rDist);
    }    
    ads_point ptRef;
    if ( RTNORM == iRet )
        iRet = acedGetPoint(NULL, _T("\nLinke untere Ecke (X,Y,Z): "), ptRef);
    if ( RTNORM == iRet )
    {
        _ptRef[X] = ptRef[X];
        _ptRef[Y] = ptRef[Y];
        _ptRef[Z] = ptRef[Z];
        _bInitialized = true;
    }

    return _bInitialized;
}

void CADArxLetter::Create(void)
{
    if ( !_bInitialized )
        return;

    // Fügen Sie hier den Code zur Berechnung Speicherung 
	// der AcDbLine- bzw. AcDbArc-Elemente ein.
}

