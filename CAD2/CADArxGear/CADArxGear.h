//////////////////////////////////////////////////////////////////////
// CADArxGear.h
// Schnittstelle der Klasse CADArxGear
// Aufgabe: Beschreibung, Berechnung und Erstellung eines Umrisses für ein Evolventenzahrad
//          mit der objectARX-API
//////////////////////////////////////////////////////////////////////

#pragma once

#include <adsdef.h>
#include <gepnt3d.h>
class AcGePoint2d;
class AcDbBlockTableRecord;

#include <dbsymtb.h>

class CADArxGear
{
public:
    CADArxGear(void);
    virtual ~CADArxGear(void);

    bool DataInput(void);
    bool Calc(void);
    void Create(void);

private:
    ads_real    _rModul;
    int         _nTeethNumber;
    int         _nPointNumber;
    AcGePoint3d _ptCenter;

    bool      _bInitialized;
    bool      _bIsLengthWarning;

    AcDbBlockTable* _pBlockTable;
    AcDbBlockTableRecord* _pBlockTableRecord;

    AcGePoint2d* _pPts;

	// Zwischenergebnisse der Methode CADArxGear::Calc
    ads_real _rR0;      // Teilkreisradius
    ads_real _rRb;      // Grundkreisradius
    ads_real _rRf;      // Fußkreisradius
    ads_real _rRa;      // Kopfkreisradius
    ads_real _rDelta;   // Zahnspitzenwinkel

    // Hilfsmethode zum Speichern einer Linie.
    void CreateLine(AcDbBlockTableRecord* pBlockTableRecord,
                    const AcGePoint2d&    ptStart,
                    const AcGePoint2d&    ptEnd);

    // Hilfsmethode zum Speichern eines Bogenstücks.
    void CreateArc(AcDbBlockTableRecord* pBlockTableRecord,
                   const AcGePoint2d&    ptStart,
                   const AcGePoint2d&    ptEnd,
                   ads_real              rR);

    // Einige notwendige konstante Grundgrößen:
    static const ads_real _rPi;
    static const ads_real _rAlpha0;
    static const ads_real _rTanAlpha0;
    static const ads_real _rCosAlpha0;
};
