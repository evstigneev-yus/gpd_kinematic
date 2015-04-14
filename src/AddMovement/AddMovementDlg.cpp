
#include "AddMovementDlg.h"

#include <Gepard/Visualization/GCamera.h>
#include <Gepard/Visualization/GPDGeometryRender.h>

using namespace Gepard::Visualization;
using namespace Gepard::Callbacks;
using namespace Gepard::Topology_Geometry;



AddMovementDlg::AddMovementDlg(int steps, QWidget *parent) 
    : QDialog(parent)    
{
	stepCount = steps;

	this->setWindowFlags(Qt::WindowStaysOnTopHint);

	//�������
	redPalette = new QPalette();
	redPalette->setColor(QPalette::Base, QColor(255,0,0,120));
	greenPalette = new QPalette();
	greenPalette->setColor(QPalette::Base, QColor(0, 255, 0, 120));

	//���������� ������� 
	
	mainLabel = new QLabel(tr("Chiose part"));                //����� ������
	partLabel = new QLabel(tr("Part: "));                //������:
	moveTypeLabel = new QLabel(tr("Move Type"));            //��� ��������:
	moveNameLabel = new QLabel(tr("Name: "));            //���: (��������)

	shiftLabel = new QLabel(tr("Shift: "));               //��������: ��� ����:

	axisLabel = new QLabel(tr("Axis: "));                //������:
	startStepLabel = new QLabel(tr("Start step from 0 "));           //��������� ���:
	endStepLabel = new QLabel(tr("End step to ").append(QString::number(stepCount)));  //�������� ���:
	moveListLabel = new QLabel(tr("Move list"));            //������ ��������

	linearRadio = new QRadioButton(tr("Linear"));        //��������
	radialRadio = new QRadioButton(tr("Radial"));         //������������

	partNameOutput = new QLineEdit();        //����� ������
	partNameOutput->setReadOnly(true);
	partNameOutput->setPalette(*redPalette);
	moveNameInput = new QLineEdit();        //���� ��������
	shiftInput = new QLineEdit();            //���� ��������
	axisOutput = new QLineEdit();            //����� ���
	axisOutput->setReadOnly(true);
	axisOutput->setPalette(*redPalette);
	startStepInput = new QLineEdit();        //���� ���������� ����
	endStepInput = new QLineEdit();          //���� ��������� ����

	partAddButton = new QPushButton(tr("+"));       //������� ������
	partDropButton = new QPushButton(tr("-"));      //�������� ������
	axisAddButton = new QPushButton(tr("+"));       //�������� ���
	axisDropButton = new QPushButton(tr("-"));      //�������� ���
	moveAddButton = new QPushButton(tr("Add"));       //�������� ��������
	moveDropButton = new QPushButton(tr("Drop"));      //�������� ������ �������

	moveList = new QTableWidget(1,3);
	moveListHeader = new QHeaderView(Qt::Horizontal);
	moveList->setHorizontalHeader(moveListHeader);
	moveListLabels << "Movement" << "Type" << "Shift/Angle�";
	moveList->setHorizontalHeaderLabels(moveListLabels);
	moveList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);


	//���������
	mainLay = new QGridLayout(this);

	//������ 0
	mainLay->addWidget(mainLabel, 0, 0, 1, 4);
	//������ 1
	mainLay->addWidget(partLabel, 1, 0, 1, 1);
	mainLay->addWidget(partNameOutput, 1, 1, 1, 1);
	mainLay->addWidget(partAddButton, 1, 2, 1, 1);
	mainLay->addWidget(partDropButton, 1, 3, 1, 1);
	//������ 2
	mainLay->addWidget(moveTypeLabel, 2, 0, 1, 1);
	//������ 3
	mainLay->addWidget(linearRadio, 3, 0, 1, 2);
	mainLay->addWidget(radialRadio, 3, 2, 1, 2);
	//������ 4
	mainLay->addWidget(moveNameLabel, 4, 0, 1, 1);
	mainLay->addWidget(moveNameInput, 4, 1, 1, 1);
	//������ 5
	mainLay->addWidget(shiftLabel, 5, 0, 1, 1);
	mainLay->addWidget(shiftInput, 5, 1, 1, 1);
	//������ 6
	mainLay->addWidget(axisLabel, 6, 0, 1, 1);
	mainLay->addWidget(axisOutput, 6, 1, 1, 1);
	mainLay->addWidget(axisAddButton, 6, 2, 1, 1);
	mainLay->addWidget(axisDropButton, 6, 3, 1, 1);
	//������ 7
	mainLay->addWidget(startStepLabel, 7, 0, 1, 1);
	mainLay->addWidget(startStepInput, 7, 1, 1, 1);
	//������ 8
	mainLay->addWidget(endStepLabel, 8, 0, 1, 1);
	mainLay->addWidget(endStepInput, 8, 1, 1, 1);
	//������ 9
	mainLay->addWidget(moveAddButton, 9, 0, 1, 1);
	mainLay->addWidget(moveDropButton, 9, 1, 1, 1);
	//������ 10
	mainLay->addWidget(moveListLabel, 10, 0, 1, 1);
	//������ 11
	mainLay->addWidget(moveList, 11, 0, 1, 4);

	this->setLayout(mainLay);

	//������ ��������
	this->hideAll();

	linearRadio->setChecked(true);
	startStepInput->setText(tr("0"));
	endStepInput->setText(tr("1"));

	connect(partAddButton, SIGNAL(clicked()), this, SLOT(flagPart()));
	connect(axisAddButton, SIGNAL(clicked()), this, SLOT(flagFace()));
	connect(linearRadio, SIGNAL(clicked()), this, SLOT(setLinear()));
	connect(radialRadio, SIGNAL(clicked()), this, SLOT(setRadial()));
	connect(partNameOutput, SIGNAL(textChanged(const QString&)), this, SLOT(showAll()));
	connect(partNameOutput, SIGNAL(textChanged(const QString&)), this, SLOT(partNameOutputGreen()));
	connect(axisOutput, SIGNAL(textChanged(const QString&)), this, SLOT(axisOutputGreen()));
	//connect(axisAddButton, SIGNAL(clicked()), this, SLOT(showAxis()));

	//changePalette(partNameOutput, greenPalette);

	currentMode = NOTHING;
}


// ����������� ������� ��������� ������ - ������� �� �������� ������������

void AddMovementDlg::renderCallbackEvent(Gepard::Visualization::GCallbackMessage _message)
{
    if (!isVisible()) return;
    if (!_message._Object) return;

	if (currentMode == NOTHING) return;

	
		if (_message._Object->_type == GODT_FACE)
		{
			GPDFace* _facePtr = (GPDFace*)_message._Object->ObjectData;
			if (!_facePtr) return;

			if (currentMode == FACE) 
			{
				axisOutput->setText(GetFaceName(_facePtr));

				fReper = _facePtr->GetFaceReper();
				
				shift = shiftInput->text().toInt();

				shiftStart = fReper.R;
				shiftEnd = fReper.R+fReper.E2*shift;

				showAxis();

				currentMode = NOTHING;

			}
			else if (currentMode == PART)
			{
				GPDSolid *solidPtr = _facePtr->parentSolidPtr;

				//�������� ����:
				GCamera* cam0 = GeometryRenderManager::GetCamera(0);
				GPDGeometryRender* render0 = (GPDGeometryRender*)cam0;
				render0->SetSolidSelection(solidPtr);	
				
				//_facePtr->GetFaceReper();
				//��� ��� ����� ������� ������ ����������:
				//��������� ��� �� �������� "�������� ��������"
				//emit addMovementSignal(solidPtr);

				qDebug() << "Solid name:" << GetSolidName(solidPtr) << " = " << solidPtr;

				partNameOutput->setText(GetSolidName(solidPtr));
				//ui.lineEdit_Object->setText(GetSolidName(solidPtr));

				currentMode = NOTHING;
			}


			

		

		}//if GODT_FACE
	
}

QString AddMovementDlg::GetSolidName(Gepard::Topology_Geometry::GPDSolid *_solidPtr)
{
	QString solName = _solidPtr->GetSolidName();

	QString _messageStr = solName.isEmpty() ? QDialog::tr("Solid") : solName;

	_messageStr += " (";
	_messageStr += QString::number(_solidPtr->GetSolidUIN());
	_messageStr += ")";

	return _messageStr;
}

QString AddMovementDlg::GetFaceName(Gepard::Topology_Geometry::GPDFace * _facePrt)
{
	QString faceName = QString::fromStdString(_facePrt->GetFaceTypeStr());
	faceName.append(tr(" "));
	faceName.append(QString::number(_facePrt->GetUIN()));

	return faceName;
}

void AddMovementDlg::setLinear()
{
	shiftLabel->setText(tr("Shift"));
}

void AddMovementDlg::setRadial()
{
	shiftLabel->setText(tr("Angle�"));
}

void AddMovementDlg::hideAll()
{
	//������ ��������
	moveTypeLabel->hide();
	linearRadio->hide();
	radialRadio->hide();
	moveNameLabel->hide();
	moveNameInput->hide();
	shiftLabel->hide();
	shiftInput->hide();
	axisLabel->hide();
	axisOutput->hide();
	axisAddButton->hide();
	axisDropButton->hide();
	startStepLabel->hide();
	startStepInput->hide();
	endStepLabel->hide();
	endStepInput->hide();
	moveAddButton->hide();
	moveDropButton->hide();
	moveListLabel->hide();
	moveList->hide();
}

void AddMovementDlg::showAll()
{
	moveTypeLabel->show();
	linearRadio->show();
	radialRadio->show();
	moveNameLabel->show();
	moveNameInput->show();
	shiftLabel->show();
	shiftInput->show();
	axisLabel->show();
	axisOutput->show();
	axisAddButton->show();
	axisDropButton->show();
	startStepLabel->show();
	startStepInput->show();
	endStepLabel->show();
	endStepInput->show();
	moveAddButton->show();
	moveDropButton->show();
	moveListLabel->show();
	moveList->show();
}

void AddMovementDlg::partNameOutputRed()
{
	partNameOutput->setPalette(*redPalette);
}

void AddMovementDlg::partNameOutputGreen()
{
	partNameOutput->setPalette(*greenPalette);
}

void AddMovementDlg::axisOutputRed()
{
	axisOutput->setPalette(*redPalette);
}

void AddMovementDlg::axisOutputGreen()
{
	axisOutput->setPalette(*greenPalette);
}

void AddMovementDlg::showAxis()
{
	GCamera * cam0 = GeometryRenderManager::GetCamera(0);
	GPDGeometryRender * cam0Render = dynamic_cast<GPDGeometryRender*>(cam0);

	cam0Render->DeleteSpecialGObject((SpecialGObject*)newAxis);

	newAxis = cam0Render->DrawAxis(shiftStart.x, shiftStart.y, shiftStart.z, shiftEnd.x, shiftEnd.y, shiftEnd.z, GColor(0, 0, 1), "Axis");
}

void AddMovementDlg::deleteAxis(GAxis *Axis)
{
	GCamera * cam0 = GeometryRenderManager::GetCamera(0);
	GPDGeometryRender * cam0Render = dynamic_cast<GPDGeometryRender*>(cam0);

	cam0Render->DeleteSpecialGObject((SpecialGObject*)Axis);
}

void AddMovementDlg::flagPart()
{
	currentMode = PART;
}

void AddMovementDlg::flagFace()
{
	currentMode = FACE;
}

void AddMovementDlg::flagNothing()
{
	currentMode = NOTHING;
}
