//////////////////////////////////////////////////////////////////////
// TestFigureCmd.cpp
// Ein Kommando zum Erzeugen einer Figur mit Linien und Bögen
//////////////////////////////////////////////////////////////////////

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
#include "tchar.h"

void TestFigureCmd() 
{ 
    ads_point ptRef;
    ads_real  rWidth = 10.;
    ads_real  rHeight = 10.;
    int iRet;

    // Zunächst wird der Benutzer zur Eingabe von Breite, Höhe und einem Punkt in 
    // dieser Reihenfolge aufgefordert.
    acedInitGet(RSG_NONEG,NULL);
    iRet = acedGetReal(_T("\nBreite: "),&rWidth);
    // Es wurde nicht ESC gedrückt und alles OK --> dann weiter:
    if ( RTNORM == iRet )
    {
        acedInitGet(RSG_NONEG,NULL);
        iRet = acedGetReal(_T("\nHöhe: "),&rHeight);
    }                                   
    // Achtung: Der Punkt kann sowohl durch Eingabe über Tastatur und durch Anclicken
    // am Bildschirm erzeugt werden.
    if ( RTNORM == iRet )
        iRet = acedGetPoint(NULL, _T("\nLinke untere Ecke (X,Y,Z): "), ptRef);
    if ( RTNORM == iRet )
    {
        // Zunächst werden vier Linien und Bögen mit den entsprechenden
        // geometrischen Daten erzeugt werden.
        AcGePoint3d pt1(ptRef[X],          ptRef[Y],           ptRef[Z]);
        AcGePoint3d pt2(ptRef[X] + rWidth, ptRef[Y],           ptRef[Z]);
        AcGePoint3d pt3(ptRef[X] + rWidth, ptRef[Y] + rHeight, ptRef[Z]);
        AcGePoint3d pt4(ptRef[X],          ptRef[Y] + rHeight, ptRef[Z]);

        double r = rHeight > rWidth ? rWidth : rHeight;
        const double dPi = 3.14159265;
        r *= 0.5;
        AcDbLine* pLine1 = new AcDbLine(pt1, pt2);
        AcDbLine* pLine2 = new AcDbLine(pt2, pt3);
        AcDbLine* pLine3 = new AcDbLine(pt3, pt4);
        AcDbLine* pLine4 = new AcDbLine(pt4, pt1);
        AcDbArc*  pArc1  = new AcDbArc (pt1, r, 0.0      , 0.5 * dPi);
        AcDbArc*  pArc2  = new AcDbArc (pt2, r, 0.5 * dPi,       dPi);
        AcDbArc*  pArc3  = new AcDbArc (pt3, r,       dPi, 1.5 * dPi);
        AcDbArc*  pArc4  = new AcDbArc (pt4, r, 1.5 * dPi, 2.0 * dPi);

        // Alle Entities müssen innerhalb eines Blocks abgespeichert werden, 
        // z.B. im "aktuellen" Block: 
        AcDbBlockTable* pBlockTable;
        acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);

        AcDbBlockTableRecord* pBlockTableRecord;
        pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);

        // Nun können die einzelnen Entities hinzugefügt werden.
        AcDbObjectId lineId1;
        AcDbObjectId lineId2;
        AcDbObjectId lineId3;
        AcDbObjectId lineId4;
        AcDbObjectId arcId1;
        AcDbObjectId arcId2;
        AcDbObjectId arcId3;
        AcDbObjectId arcId4;
        pBlockTableRecord->appendAcDbEntity(lineId1, pLine1);
        pBlockTableRecord->appendAcDbEntity(lineId2, pLine2);
        pBlockTableRecord->appendAcDbEntity(lineId3, pLine3);
        pBlockTableRecord->appendAcDbEntity(lineId4, pLine4);
        pBlockTableRecord->appendAcDbEntity(arcId1,  pArc1 );
        pBlockTableRecord->appendAcDbEntity(arcId2,  pArc2 );
        pBlockTableRecord->appendAcDbEntity(arcId3,  pArc3 );
        pBlockTableRecord->appendAcDbEntity(arcId4,  pArc4 );

        pLine1->close();
        pLine2->close();
        pLine3->close();
        pLine4->close();
        pArc1->close();
        pArc2->close();
        pArc3->close();
        pArc4->close();
        pBlockTableRecord->close();
        pBlockTable->close();
    }
}

