//////////////////////////////////////////////////////////////////////
// CADArxGear.cpp
// Klasse CADArxGear
// Aufgabe: Beschreibung, Berechnung und Erstellung eines Umrisses für ein Evolventenzahrad
//          mit der objectARX-API
//////////////////////////////////////////////////////////////////////

#include "CADArxGear.h"

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

// Das "Epsilon" zum Testen der Bogenlänge vor dem Speicher der Linie bzw. des Bogens:
// (Zum Testen ist das "Epsilon" relativ groß gewählt.)
#define CADArx_Length_Eps  0.001

// Einige notwendige konstante Grundgrößen:
const ads_real CADArxGear::_rPi = 3.14159265359;
const ads_real CADArxGear::_rAlpha0 = 20.0 * CADArxGear::_rPi / 180.;
const ads_real CADArxGear::_rTanAlpha0 = tan(CADArxGear::_rAlpha0);
const ads_real CADArxGear::_rCosAlpha0 = cos(CADArxGear::_rAlpha0);

CADArxGear::CADArxGear(void)
{
    _bInitialized = false;
    _pPts = NULL;

    _rR0 = 0.0;
    _rRb = 0.0;
    _rRf = 0.0;
    _rRa = 0.0;
    _rDelta = 0.0;
}

CADArxGear::~CADArxGear(void)
{
    delete[] _pPts;
}

bool CADArxGear::DataInput(void)
{
    delete[] _pPts;
    _pPts = NULL;
    _bInitialized = false;
    _bIsLengthWarning = false;

    int iRet;

    // Für den Modul, Zähnezahl und Punktanzahl sind nur positive Zahlen erlaubt:
    acedInitGet(RSG_NONEG,NULL);
    iRet = acedGetReal(_T("\nModul: "),&_rModul);
    if ( RTNORM == iRet )
    {
        acedInitGet(RSG_NONEG,NULL);
        iRet = acedGetInt(_T("\nZähnezahl: "),&_nTeethNumber);
    }    
    while ( RTNORM == iRet )
    {
        acedInitGet(RSG_NONEG,NULL);
        iRet = acedGetInt(_T("\nAnzahl der Punkte: "),&_nPointNumber);
        if ( RTNORM == iRet && _nPointNumber < 3 )
            acutPrintf(_T("\nEs müssen mindestens 3 Punkte sein! Bitte geben Sie einen Wert >= 3 ein!"));
        else
            break;
    }                                   
    ads_point ptCenter;
    if ( RTNORM == iRet )
        iRet = acedGetPoint(NULL, _T("\nMittelpunkt (X,Y,Z): "), ptCenter);
    if ( RTNORM == iRet )
    {
        _ptCenter[X] = ptCenter[X];
        _ptCenter[Y] = ptCenter[Y];
        _ptCenter[Z] = ptCenter[Z];
        _bInitialized = true;
    }

    return _bInitialized;
}

bool CADArxGear::Calc(void)
{
    if ( !_bInitialized )
        return false;

    // Fügen Sie hier den Code zur Berechnung der _nPointNumber Flankenpunkte ein
    // und speichern Sie die Daten in einem entsprechenden _pPts-Array.

	// Zum Testen und für die Bewertung die berechneten Zwischenergebnisse ausgeben:
	acutPrintf(_T("\n r0 = %6.2f"),_rR0);
	acutPrintf(_T("\n rb = %10.6f"),_rRb);
	acutPrintf(_T("\n rf = %6.2f"),_rRf);
	acutPrintf(_T("\n ra = %6.2f"),_rRa);
	acutPrintf(_T("\n delta = %10.6f"),_rDelta);

    return true;
}

void CADArxGear::CreateLine(AcDbBlockTableRecord* pBlockTableRecord,
                            const AcGePoint2d&    ptStart,
                            const AcGePoint2d&    ptEnd)
{
    // Fügen Sie hier den Code zum Speichern einer Linie ein 
    // und zum Prüfen der Länge vor dem Speichern.
}

void CADArxGear::CreateArc(AcDbBlockTableRecord* pBlockTableRecord,
                           const AcGePoint2d&    ptStart,
                           const AcGePoint2d&    ptEnd, 
                           ads_real              rR)
{
    // Fügen Sie hier den Code zum Speichern eines Bogenstücks ein 
    // und zum Prüfen der Bogenlänge vor dem Speichern.
}

void CADArxGear::Create(void)
{
    if ( NULL == _pPts )
        return;

    // Fügen Sie hier den Code zur Speicherung der AcDbLine- bzw. AcDbArc-Elemente ein.
    // Zur Ermittlung der Punkte der jeweils gedrehten Flanke die Methode können Sie
    // die Methode AcGePoint2d::rotateBy. Informieren Sie sich in der Online-Hilfe über 
    // diese Methode.
}

