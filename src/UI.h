/***************************************************************************
 *   Copyright (C) 2010  Arnaud Emilien
 **************************************************************************/

#ifndef _UI_H
#define _UI_H

#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QFont>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QImage>
#include <QScrollArea>
#include <QTabWidget>

#include <QPalette>
#include <QLayoutIterator>
#include <QSpacerItem>
#include <QListWidget>
#include <QTableWidget>
#include <QGroupBox>
#include <QTreeWidget>
#include <QSpinBox>

#include "Renderer.h"
#include "Data.h"
#include "GL_Widget.h"
#include "MainWindow.h"
#include "./Generator/Scenario.h"
#include "./Generator/BuildingsAtlas.h"
#include "./Generator/RoadsAtlas.h"



class UI : public QWidget
{
private :
    Q_OBJECT

public :
    // SCENARIO
    QListWidget *sequencesList;
    Scenario     selectedScenario;
    Sequence     selectedSequence;
    QTableWidget* sequencesTable;
    BuildingsAtlas    selectedAtlas;
    Building::Type    selectedBuildingType;
    Renderer::DISPLAY selectedDisplayType;

    RoadsAtlas    selectedRAtlas;
    Road::Type    selectedRoadType;

    // ATLAS
    QTreeWidget* rAtT;
    QTreeWidget* bAtT;

    // ROADS
    QLineEdit   *x1;
    QLineEdit   *y1;
    QLineEdit   *x2;
    QLineEdit   *y2;
    QLineEdit   *curvatureWeight;
    QLineEdit   *slopeWeight;
    QLineEdit   *slopeMax;
    QLineEdit   *travelWeight;
    QLineEdit   *waterWeight;
    QLineEdit   *roadsWeight;

    // INTEREST
    QLineEdit   *xI;
    QLineEdit   *yI;
    QLineEdit   *socialII;
    QLineEdit   *roadsII;
    QLineEdit   *defenseII;
    QLineEdit   *waterII;
    QLineEdit   *dominationII;
    QLineEdit   *cultII;
    QLineEdit   *altitudeII;
    QLineEdit   *sunII;

    // SYSTEM
    QLineEdit   *scaleGG;
    QLineEdit   *nbSteps;
    QLineEdit   *scaleEdit;
    QListWidget *patternEdit;

    // TERRAIN
    QLineEdit   *brushSizeE;
    QLineEdit   *brushCoeffE;


    // FEASABILITY

    // UI
    QTabWidget  *tab1;
    QScrollArea *scroll1;
    QScrollArea *scroll2;
    QScrollArea *scroll3;
    QScrollArea *scroll4;
    QScrollArea *scroll5;

    Road _road;
    Building _building;

public :
    Renderer* _renderer;
    Data*     _data;
    GL_Widget* _GL_Widget;
    MainWindow* _window;

    QLayout* _toolContainer;
    QFrame*  _toolBox;
    QString* _toolBoxStyle;

    std::string elementToPaint;

    UI(Data*     data);

    void initToolBox();

    void setState(MainWindow::State state);

    void loadVillageGen();
    void loadScenarioEdit();
    void loadAtlasEdit();
    void loadRoadsEdit();
    void loadBuildingEdit();
    void loadGeometryEdit();
    void loadLandscapeEdit();

    QListWidget*  scenarioListFromHashScenario(HashTable<std::string, Scenario>& scenarios);
    //QTableWidget* sequenceTableFromScenario(Scenario& scenario);

    QListWidget*  roadsAtlasListFromHashAtlas(HashTable<std::string, RoadsAtlas>& atlas);
    QListWidget*  buildingsAtlasListFromHashAtlas(HashTable<std::string, BuildingsAtlas>& atlas);

    QTreeWidget*  roadsAtlasTreeFromAtlas(RoadsAtlas& atlas);
    QTreeWidget*  buildingsAtlasTreeFromAtlas(BuildingsAtlas& atlas);

    QTableWidget* roadsAtlasTableFromAtlas(RoadsAtlas& atlas);
    QTableWidget* buildingsAtlasTableFromAtlas(BuildingsAtlas& atlas);

    QListWidget*  roadsTypeList();
    QListWidget*  buildingsTypeList();
    QListWidget*  displayTypeList();

    QListWidget*  editableTerrainElementList();

    QListWidget*  navigationPattern();



public slots:


    void slot_Step();
    void slot_ClearRoads();
    void slot_CreateRoad();
    void slot_ClearWalls();
    void slot_AddBuilding();
    void slot_ClearBuildings();
    void slot_CreateBuilding();


    void slot_ReloadShaders();
    void slot_SetScale();
    void slot_Reset();

    void slot_Paint();
    void slot_Pick1();
    void slot_Pick2();
    void slot_PickI();


    void slot_State0();
    void slot_State1();
    void slot_State2();
    void slot_State3();
    void slot_State4();
    void slot_State5();
    void slot_State6();

    void slot_ScenarioNew();
    void slot_ScenarioDelete();
    void slot_ScenarioEdit();
    void slot_ScenarioSelection(QListWidgetItem *item);

    void slot_VillagePlayScenario();
    void slot_VillagePlaySequence();
    void slot_PlayScenarioSelection(QListWidgetItem *item);
    void slot_PlaySequenceSelection(QListWidgetItem *item);

    void slot_RoadsAtlasSelected(QListWidgetItem *item);
    void slot_BuildingsAtlasSelected(QListWidgetItem *item);


    void slot_PaintTerrainSelection(QListWidgetItem *item);

    void slot_DisplayAtlasSelected(QListWidgetItem *);
    void slot_DisplayBTypeSelected(QListWidgetItem *);
    void slot_DisplayTypeSelected(QListWidgetItem *);
    void slot_Display();

    void slot_ChangeElevation(QString);
    void slot_ChangeScale(QString);
    void slot_ChangePattern(QListWidgetItem *);


    Building::Type stringToBuildingType(std::string name);
    Road::Type stringToRoadType(std::string name);
    Renderer::DISPLAY stringToDisplayType(std::string name);

    void slot_RoadAtlasSelected(QListWidgetItem *);
    void slot_RoadTypeSelected(QListWidgetItem *);

    void slot_BuildingAtlasSelected(QListWidgetItem *);
    void slot_BuildingTypeSelected(QListWidgetItem *);

    void slot_NewRoadParamValue (QString value, QString atlasName, Road::Type type, RoadsAtlas::Params param);
    void slot_NewBuildingParamValue (QString value, QString atlasName, Building::Type type, BuildingsAtlas::Params param);

    void slot_GenerateGeometry();

    void slot_Shortcuts();
    void slot_BetaSkeleton();
    void slot_Watershed();
    void slot_Bezier();
};

class MyTreeQLineEdit : public QLineEdit
{
    Q_OBJECT
private :
    QString _atlasName;
    Road::Type _type;
    RoadsAtlas::Params _param;

public :
    MyTreeQLineEdit(QString atlasName, Road::Type type,
                    RoadsAtlas::Params param, QString value)
        : QLineEdit(value)
        , _atlasName(atlasName)
        , _type(type)
        , _param(param)
    {
        connect(this, SIGNAL(textEdited(QString)), this, SLOT(slot_QLineChanded(QString)));
    }

public slots:
    void slot_QLineChanded(QString change)
    {
        emit NewRoadParamValue(change, _atlasName, _type, _param);
    }

signals:
    void NewRoadParamValue (QString value, QString atlasName, Road::Type type, RoadsAtlas::Params param);
};

class MyBTreeQLineEdit : public QLineEdit
{
    Q_OBJECT
private :
    QString _atlasName;
    Building::Type _type;
    BuildingsAtlas::Params _param;

public :
    MyBTreeQLineEdit(QString atlasName, Building::Type type,
                     BuildingsAtlas::Params param, QString value)
        : QLineEdit(value)
        , _atlasName(atlasName)
        , _type(type)
        , _param(param)
    {
        connect(this, SIGNAL(textEdited(QString)), this, SLOT(slot_QLineChanded(QString)));
    }

public slots:
    void slot_QLineChanded(QString change)
    {
        emit NewBuildingParamValue(change, _atlasName, _type, _param);
    }

signals:
    void NewBuildingParamValue (QString value, QString atlasName, Building::Type type, BuildingsAtlas::Params param);
};


#endif // _UI_H
