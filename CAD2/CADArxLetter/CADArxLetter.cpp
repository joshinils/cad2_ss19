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

//#define _USE_MATH_DEFINES
#include <math.h> // has M_PI if above is defined
#include <stdio.h>
#include <tchar.h>

#include <dbsymtb.h>
#include <string>


// Zum Testen ist das "Epsilon" relativ groß gewählt.
#define CADArx_Length_Eps  0.001

const ads_real CADArxLetter::_rPi = /* M_PI; //*/ 3.14159265359;

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

    acedInitGet(RSG_NONEG, NULL);
    iRet = acedGetReal(_T("\nBreite: "), &_rWidth);
    if ( RTNORM == iRet )
    {
        acedInitGet(RSG_NONEG, NULL);
        iRet = acedGetReal(_T("\nHöhe: "), &_rHeight);
    }
    if ( RTNORM == iRet )
    {
        acedInitGet(RSG_NONEG, NULL);
        iRet = acedGetReal(_T("\nAbstand: "), &_rDist);
    }
    ads_point ptRef;
    if (RTNORM == iRet)
    {
        iRet = acedGetPoint(NULL, _T("\nLinke untere Ecke (X, Y, Z): "), ptRef);
    }
    if (RTNORM == iRet)
    {
        _ptRef[X] = ptRef[X];
        _ptRef[Y] = ptRef[Y];
        _ptRef[Z] = ptRef[Z];
        _bInitialized = true;
    }

    return _bInitialized;
}

// from https://stackoverflow.com/questions/10737644/convert-const-char-to-wstring
std::wstring s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

//prints std::string converted to wstring in autocad
void printMessage(std::string const& s)
{
    acutPrintf(s2ws(s).c_str());
}

// do all the stuff at once
// ignores AcDbObjectId pOutputId
Acad::ErrorStatus appendAcDbEntityAtOnce(AcDbEntity* pEntity)
{
    AcDbBlockTable* pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);

    AcDbBlockTableRecord* pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

    // Nun können die einzelnen Entities hinzugefügt werden.
    AcDbObjectId pOutputId; // to give as reference
    Acad::ErrorStatus es = pBlockTableRecord->appendAcDbEntity(pOutputId, pEntity);

    pEntity->close();
    pBlockTableRecord->close();
    pBlockTable->close();

    return es;
}


void CADArxLetter::Create(void)
{
    if (!_bInitialized)
    {
        return;
    }

    // Fügen Sie hier den Code zur Berechnung Speicherung
	// der AcDbLine- bzw. AcDbArc-Elemente ein.

    //left lower is _ptRef

    // left upper
    AcGePoint3d lu(_ptRef);
    lu.y += _rHeight;

    // right upper
    AcGePoint3d ru(lu);
    ru.x += _rWidth;

    // right lower
    AcGePoint3d rl(_ptRef);
    rl.x += _rWidth;

    std::string msg;
    msg += "\n";
    msg += __FUNCTION__;
    msg += " _ptRef: " + std::to_string(_ptRef.x) + " " + std::to_string(_ptRef.y) + " " + std::to_string(_ptRef.z);
    msg += "     lu: " + std::to_string(lu.x) + " " + std::to_string(lu.y) + " " + std::to_string(lu.z);

    printMessage(msg);

    // define bounding rectangle
    appendAcDbEntityAtOnce(new AcDbLine(_ptRef, lu));
    appendAcDbEntityAtOnce(new AcDbLine(lu, ru));
    appendAcDbEntityAtOnce(new AcDbLine(ru, rl));
    appendAcDbEntityAtOnce(new AcDbLine(rl, _ptRef));

    double w10 = _rWidth  / 10;
    double h10 = _rHeight / 10;

    AcGePoint3d jlu(lu);
    jlu.x += w10;
    jlu.y -= h10;

    AcGePoint3d jru(ru);
    jru.x -= w10;
    jru.y -= h10;

    // upper horizontal stroke
    appendAcDbEntityAtOnce(new AcDbLine(jlu, jru));

    AcGePoint3d jll(jlu);
    jll.y -= _rDist;

    AcGePoint3d jrl(jru);
    jrl.y -= _rDist;
    jrl.x -= _rDist;

    // lower horizontal stroke
    appendAcDbEntityAtOnce(new AcDbLine(jll, jrl));
    // cap of upper part on the left
    appendAcDbEntityAtOnce(new AcDbLine(jlu, jll));

    AcGePoint3d center(_ptRef);
    center.x += _rWidth / 2;
    double radius = _rWidth / 2 - w10 - _rDist;
    center.y += radius + h10;

    // is the letter tall enough for only one center point?
    if (center.y >= _rHeight / 3)
    {
        // draw two quarter circles in each corner, connected at the bottom

        // draw left corner first
        // outer circle quadrant first
        center.y = _rHeight / 3;
        double radiusY = center.y - h10;

        center.x = center.y;
        double radiusX = center.x - w10;

        radius = min(radiusX, radiusY);

        // inner arc
        appendAcDbEntityAtOnce(new AcDbArc(center, radius, _rPi, _rPi/2));
        // outer arc
        appendAcDbEntityAtOnce(new AcDbArc(center, radius + _rDist, _rPi, _rPi / 2));

        AcGePoint3d capl(center);
        capl.x -= radius + _rDist;

        AcGePoint3d capr(center);
        capr.x -= radius;

        // cap of arcs at the left
        appendAcDbEntityAtOnce(new AcDbLine(capl, capr));

        //todo move caps to right
        capl.x += _rWidth - capr.x;
        capr.x += capl.x + _rDist;

        // vertical stroke inner
        appendAcDbEntityAtOnce(new AcDbLine(capl, jrl));
        // vertical stroke outer
        appendAcDbEntityAtOnce(new AcDbLine(capr, jru));

        // copy center for later
        auto centerLeft = center;
        // move center to the right
        center.x = _rWidth - center.x;

        // inner arc
        appendAcDbEntityAtOnce(new AcDbArc(center, radius, _rPi / 2, 0));
        // outer arc
        appendAcDbEntityAtOnce(new AcDbArc(center, radius + _rDist, _rPi / 2, 0));

        // draw lower connection of corners
        center.y -= radius;
        centerLeft.y -= radius;

        // upper horizontal connection
        appendAcDbEntityAtOnce(new AcDbLine(center, centerLeft));

        center.y -= _rDist;
        centerLeft.y -= _rDist;

        // lower horizontal connection
        appendAcDbEntityAtOnce(new AcDbLine(center, centerLeft));
    }
    // use one center point to draw one half circle
    else
    {
        // inner arc
        appendAcDbEntityAtOnce(new AcDbArc(center, radius         , _rPi, 0));
        // outer arc
        appendAcDbEntityAtOnce(new AcDbArc(center, radius + _rDist, _rPi, 0));

        AcGePoint3d capl(center);
        capl.x -= radius + _rDist;

        AcGePoint3d capr(center);
        capr.x -= radius;

        // cap of arcs at the left
        appendAcDbEntityAtOnce(new AcDbLine(capl, capr));

        capr.x += radius * 2 + _rDist;
        capl.x += radius * 2 + _rDist;

        // vertical stroke inner
        appendAcDbEntityAtOnce(new AcDbLine(capl, jrl));
        // vertical stroke outer
        appendAcDbEntityAtOnce(new AcDbLine(capr, jru));
    }
}


