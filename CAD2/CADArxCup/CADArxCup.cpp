//////////////////////////////////////////////////////////////////////
// CADArxCup.cpp
// Klasse CADArxCup
// Aufgabe: Beschreibung, Berechnung und Erstellung eines Pokals
//          mit der objectARX-API
//////////////////////////////////////////////////////////////////////

#include "CADArxCup.h"

// Damit mit Warnungsstufe 4 übersetzt werden kann:
#pragma warning( disable : 4100 )
#pragma warning( disable : 4201 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4512 )

#include <aced.h>
#include <dbents.h>
#include <dbregion.h>
#include <dbsol3d.h>
#include <dbgroup.h>
#include <dbapserv.h>
#include <adscodes.h>
#include <acedads.h>
#include <math.h>
#include <stdio.h>
#include <tchar.h>

// Das "Epsilon" zum Testen der Dicke:
#define CADArx_Width_Eps  0.01

const ads_real CADArxCup::_rPi = 3.14159265359;

CADArxCup::CADArxCup(void)
{
    _bInitialized = false;

	// Diese Werte werden nur wegen der Testausgabe initialisiert.
	_rHorWidth = 0.0;
	_rSphereRadiusOnCup = 0.0;
    _rInnerStemHeight = 0.0;
}

CADArxCup::~CADArxCup(void)
{
}

bool CADArxCup::DataInput(void)
{
    _bInitialized = false;

    int iRet;

    // Für den Durchmesser, Höhe, Dicke und Kugelradius sind nur positive Zahlen erlaubt:
    acedInitGet(RSG_NONULL,NULL);
    iRet = acedGetReal(_T("\ngoßer Durchmesser: "),&_rDiameter);
    if ( RTNORM == iRet )
    {
        acedInitGet(RSG_NONULL,NULL);
        iRet = acedGetReal(_T("\nGesamthöhe: "),&_rHeight);
    }
    if ( RTNORM == iRet )
    {
        acedInitGet(RSG_NONULL,NULL);
        iRet = acedGetReal(_T("\nDicke: "),&_rWidth);
    }
    if ( RTNORM == iRet )
    {
        acedInitGet(RSG_NONEG,NULL);
        iRet = acedGetInt(_T("\nAnzahl der Kugeln: "),&_nSphereNumber);
    }
    if ( RTNORM == iRet && _nSphereNumber > 0 )
	{
        acedInitGet(RSG_NONULL,NULL);
        iRet = acedGetReal(_T("\nKugelradius: "),&_rRadiusSphere);
	}

    ads_point ptRef;
    if ( RTNORM == iRet )
        iRet = acedGetPoint(NULL, _T("\nBezugspunkt (X,Y,Z): "), ptRef);
    if ( RTNORM == iRet )
    {
        _ptRef[X] = ptRef[X];
        _ptRef[Y] = ptRef[Y];
        _ptRef[Z] = ptRef[Z];
        _bInitialized = true;
    }

    return _bInitialized;
}

bool CADArxCup::Calc(void)
{
    if ( !_bInitialized )
        return false;

    // Fügen Sie hier den Code zur Prüfung der Eingabedaten sowie
    // zur Berechnung der Zwischenergebnisse und des Ausgangsprofils
    // für den Rotationskörper ein.
    // Zwischenergebnisse: _rHorWidth, _rSphereRadiusOnCup (siehe CADArxCup.h)
    // AcGePoint2d-Feld für das Profil: _pPtsProfile[10] (siehe CADArxCup.h)

    _pPtsProfile[0] = _pPtsProfile[9] = AcGePoint2d();
    _pPtsProfile[1] = AcGePoint2d( _rDiameter * 3.0f / 8.0f, 0.0f);
    _pPtsProfile[2] = AcGePoint2d( _rDiameter / 8.0f, _rHeight / 8.0f );
    _pPtsProfile[3] = AcGePoint2d( _rDiameter / 8.0f, _rHeight / 4.0f );
    _pPtsProfile[4] = AcGePoint2d( _rDiameter / 2.0f, _rHeight );
    _pPtsProfile[5] = AcGePoint2d( _rDiameter / 2.0f - _rHorWidth, _rHeight);
    _pPtsProfile[6] = AcGePoint2d( _rDiameter / 8.0f - _rWidth, _rInnerStemHeight);
    _pPtsProfile[7] = AcGePoint2d( _rDiameter / 8.0f - _rWidth, _rHeight / 8.0f);
    _pPtsProfile[8] = AcGePoint2d( 0, _rHeight / 8.0f);

	// Zum Testen und für die Bewertung
    // die berechneten Werte ausgeben:
	acutPrintf(_T("\nHorizontal Width     : %8.6f"), _rHorWidth);
	acutPrintf(_T("\nSphere Radius on Cup : %8.6f"), _rSphereRadiusOnCup);
	acutPrintf(_T("\nInner Stem Height    : %8.6f"), _rInnerStemHeight);
    return true;
}

AcDb3dSolid* CADArxCup::CreateSphere(double dPhi)
{
    // Fügen Sie hier den Code zum Erzeugen einer Kugel ein.

    return NULL;
}

void CADArxCup::Create(void)
{
    // Fuegen Sie hier dn Code zur Erzeugung und Speicherung des Pokals ein.
    // Achten Sie darauf, dass für alle erzeugten Elemente (die Linien,
    // die Fläche, ...) die close-Methode aufgerufen wird.

}

