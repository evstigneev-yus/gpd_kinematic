#ifndef CMOVEMENTS_H
#define CMOVEMENTS_H

#include "..\includes\gepard.h"
#include <string>
using namespace std;


//using namespace Gepard::BasicMath;


 //	������������ ����� ��������
 
enum EMovementTypes
{
    LINEAR,
    CIRCULAR
};

//	����� ��������
 
class CMovements
{
private:
    //��� �������� �� enum EMovementTypes
    EMovementTypes m_moveType;
    //��� ��������
    string m_name;
    //�������� ��� ���� �� ������� ���� ��������� ������
    double m_shift; 
    //��� ����� ������� ����� ����������� �������� ��� ��� ������ ������� ����� ��������� ����
    Gepard::BasicMath::GPDVector m_axis;
    //����� ������ ������� ���� ������� ����
    Gepard::BasicMath::GPDPoint m_point;
    //������ �������� �� ���������
    int m_start;
    //����� �������� �� ���������
    int m_end;
public:
    
     /*
      *  �����������, ���������������� ��� ��������� �� ���������
      *  ���� start<end, �� ������ �������� �� ���������
      */
    CMovements(EMovementTypes = EMovementTypes::LINEAR,
                Gepard::BasicMath::GPDPoint = Gepard::BasicMath::GPDPoint(0,0,0),
                string = " ",
                Gepard::BasicMath::GPDVector = Gepard::BasicMath::GPDVector(0, 0, 0),
                double = 0.0,
                int = 0,
                int = 1);
    /*
     *  �����������, ���������������� ��� ��������� �� ���������
     *  ���� start<end, �� ������ �������� �� ���������
     */
//     CMovements(EMovementTypes = EMovementTypes::CIRCULAR,
//         Gepard::BasicMath::GPDPoint = Gepard::BasicMath::GPDPoint(0, 0, 0),
//         QString = " ",
//         Gepard::BasicMath::GPDVector = Gepard::BasicMath::GPDVector(0, 0, 0),
//         double = 0.0,
//         int = 0,
//         int = 1);

    /*
     *	����������� �����������
     */
    CMovements(const CMovements&);
    virtual ~CMovements();
    /*
     *	������ ��� ��������
     */ 
    void SetMovementType(EMovementTypes);

    EMovementTypes GetMovementType();
    /*
     *	������ �������� ��� ���� � ����������� �� ���� ��������
     */
    void SetShift(double);
    /*
     *	�������� �������� ��� ���� � ����������� �� ���� ��������
     */
    double GetShift();

    // ������ ���
    void SetMoveName(string);

    //�������� ���

    string GetMoveName();
    
    /*
     *	������ ���
     */
    void SetAxis(Gepard::BasicMath::GPDVector);
    /*
     *	�������� ���
     */
    Gepard::BasicMath::GPDVector GetAxis();

    /*
     *	������ ������ �������� �� ���������
     *  ���� �������� ������ m_end, ������ �� ����������
     */
    void SetStart(int);
    
    /*
     *	�������� ������ �������� �� ���������
     */	
     
    int GetStart();
    
     //	������ ����� �������� �� ���������
     // ���� �������� ������ m_start, ������ �� ����������
     
    void SetEnd(int);
    
     //	�������� ����� �������� �� ���������
     
    int GetEnd();
    
     //	�������� ����� �� ���� ���
     
    double GetMovePerStep();
    
     //	�������� ���������� �����
     
    int GetStepsCnt();
    /*
     *	������ �����
     */
    Gepard::BasicMath::GPDPoint GetPoint();
    /*
     *	�������� �����
     */
    void SetPoint(Gepard::BasicMath::GPDPoint);
};

#endif //CMOVEMENTS_H