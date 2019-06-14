//////////////////////////////////////////////////////////////////////
// CADArxCup.h
// Schnittstelle der Klasse CADArxCup
// Aufgabe: Beschreibung, Berechnung und Erstellung eines Pokals
//          mit der objectARX-API
//////////////////////////////////////////////////////////////////////

#pragma once

#include <adsdef.h>
#include <gepnt3d.h>
#include <gepnt2d.h>
class AcDb3dSolid;


// zum testweise anzeigen
#include <adsdef.h>
#include <gepnt3d.h>
class AcGePoint2d;
class AcDbBlockTableRecord;

#include <dbsymtb.h>


class CADArxCup
{
public:
    CADArxCup(void);
    virtual ~CADArxCup(void);

    bool DataInput(void);
    bool Calc(void);
    void Create(void);

private:
    ads_real    _rDiameter;
    ads_real    _rHeight;
    ads_real    _rWidth;
    ads_real    _rRadiusSphere;
    int         _nSphereNumber;
    AcGePoint3d _ptRef;

    bool _bInitialized;

	// Zwischenergebnisse der Methode CADArxCup::Calc
	ads_real _rHorWidth;           // Dicke der schrägen Wand in x-Richtung gemessen
    ads_real _rSphereRadiusOnCup;  // Pokalradius auf dem die Kugeln liegen
    ads_real _rInnerStemHeight;    // Höhe des Stiels innen

	// Das geschlossene Ausgangsprofil für den Rotationskörper
	// (Zwischenergebniss der Methode CADArxCup::Calc)
	// Um die Schleife für die Linienerzeugung für das Profil zu vereinfachen
	// Anfangs- bzw. Endpunkt doppelt mit den gleichen Koordinaten speichern.
	AcGePoint2d _pPtsProfile[10];

    // Hilfsmethode: Erzeugen einer Kugel.
    AcDb3dSolid* CreateSphere(double dPhi);

    static const ads_real _rPi;


	// stolen from gear:
	void CreateLine(AcDbBlockTableRecord* pBlockTableRecord,
		const AcGePoint2d&    ptStart,
		const AcGePoint2d&    ptEnd);

	AcDbBlockTable* _pBlockTable;
	AcDbBlockTableRecord* _pBlockTableRecord;
};
