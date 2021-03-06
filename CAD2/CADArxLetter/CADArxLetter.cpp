//////////////////////////////////////////////////////////////////////
// CADArxLetter.cpp
// Klasse CADArxLetter
// Aufgabe: Beschreibung, Berechnung und Erstellung eines Evolventenzahrades
//          mit der objectARX-API
//////////////////////////////////////////////////////////////////////

#include "CADArxLetter.h"

// Damit mit Warnungsstufe 4 �bersetzt werden kann:
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


// Zum Testen ist das "Epsilon" relativ gro� gew�hlt.
#define CADArx_Length_Eps  0.001

const ads_real CADArxLetter::_rPi = /* M_PI; //*/ 3.14159265359;

CADArxLetter::CADArxLetter(void)
{
    _bInitialized = false;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(_pBlockTable, AcDb::kForRead);
    _pBlockTable->getAt(ACDB_MODEL_SPACE, _pBlockTableRecord, AcDb::kForWrite);
    _creationOk = true;
}

CADArxLetter::~CADArxLetter(void)
{
    _pBlockTableRecord->close();
    _pBlockTable->close();
}

bool CADArxLetter::DataInput(void)
{
    _bInitialized = false;

    int iRet;

    acedInitGet(RSG_NONEG, NULL);
    iRet = acedGetReal(_T("\nBreite: "), &_rWidth);
    if (RTNORM == iRet)
    {
        acedInitGet(RSG_NONEG, NULL);
        iRet = acedGetReal(_T("\nH�he: "), &_rHeight);
    }
    if (RTNORM == iRet)
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
void CADArxLetter::appendAcDbEntityAtOnce(AcDbEntity* pEntity)
{
    if (typeid(*pEntity) == typeid(AcDbLine))
    {
        // check if line is long enough
        AcDbLine* line = dynamic_cast<AcDbLine*>(pEntity);
        AcGePoint3d start = line->startPoint();
        AcGePoint3d end = line->endPoint();
        double length = (start.x - end.x) * (start.x - end.x);
        length += (start.y - end.y) * (start.y - end.y);
        length += (start.z - end.z) * (start.z - end.z);
        if (sqrt(length) < 1.0f)
        {
            if (_creationOk)
            {
                printMessage("a length is less than 1!\n");
            }
            _creationOk = false;
        }
    }
    else if (typeid(*pEntity) == typeid(AcDbArc))
    {
        // check if radius is large enough
        AcDbArc* arc = dynamic_cast<AcDbArc*>(pEntity);
        if (arc->radius() < 1.0f)
        {
            if (_creationOk)
            {
                printMessage("a radius is less than 1!\n");
            }
            _creationOk = false;
        }
    }

    // Nun k�nnen die einzelnen Entities hinzugef�gt werden.
    AcDbObjectId pOutputId; // to give as reference and thereafter ignore
    Acad::ErrorStatus es = _pBlockTableRecord->appendAcDbEntity(pOutputId, pEntity);

    pEntity->close();

    if (es != Acad::ErrorStatus::eOk || !_creationOk)
    {
        throw(std::runtime_error("CADarxLetter:: an error occured, i can not continue!"));
    }
}

void CADArxLetter::checkParameters()
{
    if (_rDist < 1.0f)
    {
        _creationOk = false;
    }

    if (_rWidth < _rWidth / 5 + _rDist * 2 + 2/*min radius*/)
    {
        _creationOk = false;
    }

    if (_rHeight < _rHeight / 5 + _rDist * 2 + 2/*min radius*/)
    {
        _creationOk = false;
    }

    double radiusY = _rHeight / 3 - _rHeight / 10 - _rDist;
    double radiusX = _rHeight / 3 - _rWidth / 10 - _rDist;
    if (radiusY < 1.0f || radiusX < 1.0f)
    {
        _creationOk = false;
    }

    if (_creationOk == false)
    {
        printMessage("CADarxLetter:: values too small, can not create\n");
    }
}


void CADArxLetter::Create(void)
{
    if (!_bInitialized)
    {
        return;
    }

    checkParameters();
    if (!_creationOk)
    {
        return;
    }

    // F�gen Sie hier den Code zur Berechnung Speicherung
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

    //std::string msg;
    //msg += "\n";
    //msg += __FUNCTION__;
    //msg += " _ptRef: " + std::to_string(_ptRef.x) + " " + std::to_string(_ptRef.y) + " " + std::to_string(_ptRef.z);
    //msg += "     lu: " + std::to_string(lu.x) + " " + std::to_string(lu.y) + " " + std::to_string(lu.z);

    //printMessage(msg);
    try
    {
        // define bounding rectangle
        appendAcDbEntityAtOnce(new AcDbLine(_ptRef, lu));
        appendAcDbEntityAtOnce(new AcDbLine(lu, ru));
        appendAcDbEntityAtOnce(new AcDbLine(ru, rl));
        appendAcDbEntityAtOnce(new AcDbLine(rl, _ptRef));

        double w10 = _rWidth / 10;
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
        center.y += radius + h10 + _rDist;

        // is the letter tall enough for only one center point?
        if (center.y >= _ptRef.y + _rHeight / 3)
        {
            // draw two quarter circles in each corner, connected at the bottom

            // draw left corner first
            // outer circle quadrant first
            center.y = _rHeight / 3;
            double radiusY = center.y - h10 - _rDist;

            center.x = center.y;
            double radiusX = center.x - w10 - _rDist;

            center.x += _ptRef.x;
            center.y += _ptRef.y;
            center.z += _ptRef.z;

            radius = min(radiusX, radiusY);

            // inner arc
            appendAcDbEntityAtOnce(new AcDbArc(center, radius, _rPi, 3 * _rPi / 2));
            // outer arc
            appendAcDbEntityAtOnce(new AcDbArc(center, radius + _rDist, _rPi, 3 * _rPi / 2));

            AcGePoint3d capl(center);
            capl.x -= radius + _rDist;

            AcGePoint3d capr(center);
            capr.x -= radius;

            // cap of arcs at the left
            appendAcDbEntityAtOnce(new AcDbLine(capl, capr));


            // copy center for later
            auto centerLeft = center;
            // move center to the right
            center.x = _ptRef.x + _rWidth - (_rDist + radius + w10);

            //todo move caps to right
            capl.x = center.x + radius;
            capr.x = center.x + radius + _rDist;

            // vertical stroke inner
            appendAcDbEntityAtOnce(new AcDbLine(capl, jrl));
            // vertical stroke outer
            appendAcDbEntityAtOnce(new AcDbLine(capr, jru));


            // inner arc
            appendAcDbEntityAtOnce(new AcDbArc(center, radius, 3 * _rPi / 2, 0));
            // outer arc
            appendAcDbEntityAtOnce(new AcDbArc(center, radius + _rDist, 3 * _rPi / 2, 0));

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
            appendAcDbEntityAtOnce(new AcDbArc(center, radius, _rPi, 0));
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
    catch (const std::exception&)
    {
        // error out without notice, lets see how that goes...
    }

}


