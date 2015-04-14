#include "Mover.h"

using namespace Gepard::Topology_Geometry;
using namespace Gepard::BasicMath;

CMover::CMover(GPDSolid *part)
    : m_part(part)
{
}

CMover::~CMover(void)
{
}

void CMover::SetPart(GPDSolid *part)
{
    m_part=part;
}

int CMover::AddMovement( CMovements movement)
{
m_movementsVector.push_back(movement);
return m_movementsVector.size();
}

int CMover::AddMovement(EMovementTypes movType, QString name, GPDVector axis, double shift, int start, int end)
{
    m_movementsVector.push_back(CMovements(movType,name,axis,shift,start,end));
    return m_movementsVector.size();
}

void CMover::MoveIt(int pos)
{
    CMovements move = m_movementsVector.at(pos);
    GPDReper rep = m_part->SolidReper;
    if (move.GetMovementType()==EMovementTypes::CIRCULAR)
    {
       GPDReper mReper;
       mReper.R.setCoords(move.GetPoint().nx,move.GetPoint().ny,move.GetPoint().nz);
       mReper.morphByAngleAndAxis(move.GetAxis(),move.GetShift());
       rep.Transform(GPDReper::getGlobalReper(), mReper);
       
    }
    else
        if (move.GetMovementType()==EMovementTypes::LINEAR)
        {
            GPDVector vec = move.GetAxis()*move.GetShift();
            rep.R += vec;
            //rep.moveOnVector(vec);
            //mReper.morphByAngleAndAxis(move.GetAxis(),move.GetShift());
            //rep.Transform(GPDReper::getGlobalReper(), mReper);
        }
    m_part->UpdateSolidPosition(rep);
}

/////////////////////////////////////
// Method:    GetMovementAt
// FullName:  CMover::GetMovementAt
// Access:    public 
// Returns:   CMovements*
// Qualifier:
// Parameter: int pos
/////////////////////////////////////
CMovements* CMover::GetMovementAt( int pos )
{
    return &m_movementsVector.at(pos);
}

CMovements* CMover::GetFirstMovement()
{
    return &m_movementsVector.at(0);
}

int CMover::GetSizeOfmovementsVector()
{
    return m_movementsVector.size();
}
