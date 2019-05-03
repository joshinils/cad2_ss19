//////////////////////////////////////////////////////////////////////
// CADArxLetter.h
// Schnittstelle der Klasse CADArxLetter
// Aufgabe: Beschreibung, Berechnung und Erstellung eines Buchstabenumrisses
//          mit der objectARX-API
//////////////////////////////////////////////////////////////////////

#pragma once

#include <adsdef.h>
#include <gepnt3d.h>
#include <dbsymtb.h>


class AcGePoint2d;
class AcDbBlockTableRecord;

class CADArxLetter
{
public:
    CADArxLetter(void);
    virtual ~CADArxLetter(void);

	bool DataInput(void);
	void Create(void);

private:
    ads_real    _rWidth;
    ads_real    _rHeight;
    ads_real    _rDist;
    AcGePoint3d _ptRef;
    AcDbBlockTable* _pBlockTable;
    AcDbBlockTableRecord* _pBlockTableRecord;
    bool _creationOk;

    bool _bInitialized;

    static const ads_real _rPi;
    void appendAcDbEntityAtOnce(AcDbEntity* pEntity);
    void checkParameters
    ();
};
