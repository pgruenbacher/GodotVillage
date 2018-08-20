/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "UI.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "GL_Widget.h"
#include "./Utils/HashTable.h"
#include <string>
#include <iostream>


UI::UI(Data*     data)
    : _data(data)
{
    QWidget* dock = new QWidget(this);
    _toolContainer = new QVBoxLayout(dock);
    _toolContainer->setSpacing(0);
    setFixedWidth(400);

    //--------------------------------------------------------
    //H layout
    //--------------------------------------------------------
    QWidget* dockh = new QWidget();
    QHBoxLayout* hlayout = new QHBoxLayout();
    dockh->setLayout(hlayout);
    _toolContainer->addWidget(dockh);
    hlayout->setSpacing(0);

    QPushButton* generation = new QPushButton(tr("Gen"));
    QPushButton* scenarioEdit = new QPushButton(tr("Scen"));
    QPushButton* atlasEdit = new QPushButton(tr("Atlas"));
    QPushButton* roadsEdit = new QPushButton(tr("Roads"));
    QPushButton* buildEdit = new QPushButton(tr("Build"));
    QPushButton* geoEdit = new QPushButton(tr("Geom"));
    QPushButton* terrainEdit = new QPushButton(tr("Land"));

    generation->setFixedSize(40,40);
    scenarioEdit->setFixedSize(40,40);
    atlasEdit->setFixedSize(40,40);
    roadsEdit->setFixedSize(40,40);
    buildEdit->setFixedSize(40,40);
    geoEdit->setFixedSize(40,40);
    terrainEdit->setFixedSize(40,40);

    connect(scenarioEdit, SIGNAL(clicked()), this, SLOT(slot_State0()));
    connect(atlasEdit, SIGNAL(clicked()), this, SLOT(slot_State1()));
    connect(roadsEdit, SIGNAL(clicked()), this, SLOT(slot_State2()));
    connect(buildEdit, SIGNAL(clicked()), this, SLOT(slot_State3()));
    connect(geoEdit, SIGNAL(clicked()), this, SLOT(slot_State4()));
    connect(terrainEdit, SIGNAL(clicked()), this, SLOT(slot_State5()));
    connect(generation, SIGNAL(clicked()), this, SLOT(slot_State6()));

    hlayout->addWidget(generation);
    //hlayout->addWidget(scenarioEdit);
    hlayout->addWidget(atlasEdit);
    hlayout->addWidget(roadsEdit);
    hlayout->addWidget(buildEdit);
    hlayout->addWidget(geoEdit);
    hlayout->addWidget(terrainEdit);


    _toolBox = new QFrame();
    _toolContainer->addWidget(_toolBox);

    setState(MainWindow::GENERATION_DEFAULT);

    _toolBoxStyle = new QString("QFrame         { background-color: navy; }\n"
                                "QFrame:hover   { background-color: green; }\n"
                                "QFrame:pressed { background-color: red; }\n"
                               );

}

void UI::setState(MainWindow::State state)
{
    switch (state)
    {
    case MainWindow::SCENARIO_DEFAULT :
        loadScenarioEdit();
        break;

    case MainWindow::ATLAS_DEFAULT :
        loadAtlasEdit();
        break;

    case MainWindow::ROADS_DEFAULT :
        loadRoadsEdit();
        break;

    case MainWindow::BUILDINGS_DEFAULT :
        loadBuildingEdit();
        break;

    case MainWindow::GEOMETRY_DEFAULT :
        loadGeometryEdit();
        break;

    case MainWindow::LANDSCAPE_DEFAULT :
        loadLandscapeEdit();
        break;

    case MainWindow::GENERATION_DEFAULT :
        loadVillageGen();
        break;
    }
}

void UI::slot_Step()
{
    if (!_data)
        return;
    if (!_data->system)
        return;
    int nb =  nbSteps->text().toDouble();
    for (unsigned int i = 0; i < nb; ++i)
    {
        //_data->system->plastep();
        if (_renderer)
            _renderer->updateTextures();
        _GL_Widget->updateGL();
    }
}

void UI::slot_ReloadShaders()
{
    if (!_renderer)
        return;
    _renderer->reloadShaders();
    _GL_Widget->updateGL();
}

void UI::slot_Reset()
{
    slot_ClearRoads();
    slot_ClearBuildings();
    slot_ClearWalls();
}

void UI::slot_ClearWalls()
{
    if (!_data)
        return;
    if (!_data->system)
        return;
    _data->system->clearWalls();
    if (_renderer)
        _renderer->updateTextures(6);
    _GL_Widget->updateGL();
}

void UI::slot_ClearRoads()
{
    if (!_data)
        return;
    if (!_data->system)
        return;
    _data->system->clearRoads();
    if (_renderer)
        _renderer->updateTextures(2);
    _GL_Widget->updateGL();
}

void UI::slot_CreateRoad()
{
    if (!_data)
        return;
    if (!_data->system)
        return;

    std::cout << "Finding path... ";
    fflush(stdout);

    //selectedRAtlas.updateParameters(_road, selectedRoadType);
    _data->system->_roadsAtlas = selectedRAtlas;
    LinkedList<Vector2I> path = _data->system->generatePath(Vector2I(x1->text().toInt(),
                                y1->text().toInt()),
                                Vector2I(x2->text().toInt(),
                                         y2->text().toInt()),
                                selectedRoadType);
    Road road(selectedRoadType, path);
    _data->system->addRoad(road);
    if (_renderer)
        _renderer->updateTextures(2);
    _GL_Widget->updateGL();


    std::cout << "done" << std::endl;

}

void UI::slot_ClearBuildings()
{
    if (!_data)
        return;
    if (!_data->system)
        return;

    _data->system->clearBuildings();

    if (_renderer)
        _renderer->updateTextures(5);
    _GL_Widget->updateGL();
}

void UI::slot_CreateBuilding()
{
    if (!_data)
        return;
    if (!_data->system)
        return;

    selectedAtlas.updateParameters(_building, selectedBuildingType);
    _data->system->positioningBuilding(_building);

    if (_renderer)
        _renderer->updateTextures(5);
    _GL_Widget->updateGL();
}

void UI::slot_AddBuilding()
{

    if (!_data)
        return;
    if (!_data->system)
        return;
    selectedAtlas.updateParameters(_building, selectedBuildingType);
    Vector3F position;
    position.x = xI->text().toDouble();
    position.z = yI->text().toDouble();
    position.y = 0;
    _building.getNode().setPosition(position);
    _data->system->addBuilding(_building);

    if (_renderer)
        _renderer->updateTextures();
    _GL_Widget->updateGL();
}


void UI::slot_SetScale()
{
    if (!_data)
        return;
    if (!_data->system)
        return;
}


void UI::slot_Paint()
{
    if (elementToPaint == std::string("Altitude"))
    {
        _GL_Widget->toPaint     = _data->system->_terrain->getHeightMap();
    }
    else if (elementToPaint == std::string("Vegetation"))
    {
        _GL_Widget->toPaint     = _data->system->_terrain->getVegetation();
    }
    else if (elementToPaint == std::string("Water"))
    {
        _GL_Widget->toPaint     = _data->system->_terrain->getWater();
    }
    else if (elementToPaint == std::string("Rock"))
    {
        _GL_Widget->toPaint     = _data->system->_terrain->getRock();
    }
    else
    {
        _GL_Widget->toPaint     = _data->system->_terrain->getHeightMap();
    }



    if (_GL_Widget->_state != GL_Widget::Painting)
    {
        _window->setCursor(Qt::CrossCursor);
        _GL_Widget->paintRadius = brushSizeE->text().toDouble();
        _GL_Widget->paintCoeff  = brushCoeffE->text().toDouble();
        _GL_Widget->_state = GL_Widget::Painting;
    }
    else
    {
        _GL_Widget->_state = GL_Widget::Navigation;
        _window->setCursor(Qt::ArrowCursor);
    }
}


void UI::slot_Pick1()
{
    if (_GL_Widget->_state != GL_Widget::Picking)
    {
        _window->setCursor(Qt::CrossCursor);
        _GL_Widget->_state = GL_Widget::Picking;
        _GL_Widget->pickEx = x1;
        _GL_Widget->pickEy = y1;
    }
    else
    {
        _GL_Widget->_state = GL_Widget::Navigation;
        _window->setCursor(Qt::ArrowCursor);
    }
}

void UI::slot_Pick2()
{
    if (_GL_Widget->_state != GL_Widget::Picking)
    {
        _window->setCursor(Qt::CrossCursor);
        _GL_Widget->_state = GL_Widget::Picking;
        _GL_Widget->pickEx = x2;
        _GL_Widget->pickEy = y2;
    }
    else
    {
        _GL_Widget->_state = GL_Widget::Navigation;
        _window->setCursor(Qt::ArrowCursor);
    }
}

void UI::slot_PickI()
{
    if (_GL_Widget->_state != GL_Widget::Picking)
    {
        _window->setCursor(Qt::CrossCursor);
        _GL_Widget->_state = GL_Widget::Picking;
        _GL_Widget->pickEx = xI;
        _GL_Widget->pickEy = yI;
    }
    else
    {
        _GL_Widget->_state = GL_Widget::Navigation;
        _window->setCursor(Qt::ArrowCursor);
    }
}




void UI::slot_State0()
{
    _window->setState(MainWindow::SCENARIO_DEFAULT);
    setState(MainWindow::SCENARIO_DEFAULT);
}

void UI::slot_State1()
{
    _window->setState(MainWindow::ATLAS_DEFAULT);
    setState(MainWindow::ATLAS_DEFAULT);
}

void UI::slot_State2()
{
    _window->setState(MainWindow::ROADS_DEFAULT);
    setState(MainWindow::ROADS_DEFAULT);
}

void UI::slot_State3()
{
    _window->setState(MainWindow::BUILDINGS_DEFAULT);
    setState(MainWindow::BUILDINGS_DEFAULT);
}

void UI::slot_State4()
{
    _window->setState(MainWindow::GEOMETRY_DEFAULT);
    setState(MainWindow::GEOMETRY_DEFAULT);
}

void UI::slot_State5()
{
    _window->setState(MainWindow::LANDSCAPE_DEFAULT);
    setState(MainWindow::LANDSCAPE_DEFAULT);
}

void UI::slot_State6()
{
    _window->setState(MainWindow::GENERATION_DEFAULT);
    setState(MainWindow::GENERATION_DEFAULT);
}



void UI::initToolBox()
{
    delete _toolBox;
    _toolBox = new QFrame();
    _toolBox->setFixedSize(400,780);
    _toolContainer->addWidget(_toolBox);
    _toolBox->setFrameStyle(QFrame::StyledPanel);
    _toolBox->setStyleSheet("QFrame {background-color: #eeeeee; margin: 10px; border: 1px solid #aaaaaa; padding: 10px;}");

    /*
    QPalette palette = _toolBox->palette();
        palette.setColor( backgroundRole(), QColor( 200, 200, 200 ) );
        _toolBox->setPalette( palette );
        _toolBox->setAutoFillBackground( true );
        */
}


void UI::loadVillageGen()
{

    initToolBox();
    QVBoxLayout* layout4= new QVBoxLayout(_toolBox);
    layout4->setSpacing(0);

    QLabel *title = new QLabel("Village Generation");
    title->setStyleSheet("QLabel {color: #333333;font-size:14pt;font-weight:bold; \
	                      padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    title->setFixedHeight(15);
    layout4->addWidget(title);
    layout4->addSpacing(20);


    if (!_data)
    {
        layout4->addSpacing(500);
        return;
    }

    // PROPERTIES

    QLabel* p1 = new QLabel("Properties :");
    p1->setStyleSheet("QLabel {color: #333333;font-size:10pt;font-weight:bold; \
	                   padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    layout4->addWidget(p1);

    QWidget* ww1 = new QWidget();
    QHBoxLayout* hh1 = new QHBoxLayout(ww1);
    QLabel* scalel  = new QLabel("Scale");
    scalel->setFixedWidth(100);
    scalel->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    scaleEdit = new QLineEdit(QString().setNum(_data->system->getScale()));
    hh1->addWidget(scalel);
    hh1->addWidget(scaleEdit);

    connect(scaleEdit, SIGNAL(textEdited(QString)), this, SLOT(slot_ChangeScale(QString)));

    QWidget* ww2 = new QWidget();
    QHBoxLayout* hh2 = new QHBoxLayout(ww2);
    QLabel* pattern = new QLabel("Pattern");
    pattern->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    pattern->setFixedWidth(100);
    patternEdit = navigationPattern();
    patternEdit->setFixedHeight(100);
    hh2->addWidget(pattern);
    hh2->addWidget(patternEdit);

    connect(patternEdit, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_ChangePattern(QListWidgetItem *)));

    layout4->addWidget(ww1);
    layout4->addWidget(ww2);


    // LIST

    QLabel* p2 = new QLabel("Generation :");
    p2->setStyleSheet("QLabel {color: #333333;font-size:10pt;font-weight:bold; \
	                   padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    layout4->addWidget(p2);

    QListWidget* list = scenarioListFromHashScenario(_data->scenarios);
    list->setFixedHeight(150);
    layout4->addWidget(list);

    connect(list, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_PlayScenarioSelection(QListWidgetItem *)));



    // SEQUENCES
    sequencesList = new QListWidget();
    sequencesList->setFixedHeight(280);
    layout4->addWidget(sequencesList);

    // ACTIONS
    QPushButton* playScenario = new QPushButton(tr("Play scenario"));
    connect(playScenario, SIGNAL(clicked()), this, SLOT(slot_VillagePlayScenario()));
    layout4->addWidget(playScenario);


    QPushButton* playSeq = new QPushButton(tr("Play sequence"));
    connect(playSeq, SIGNAL(clicked()), this, SLOT(slot_VillagePlaySequence()));
    layout4->addWidget(playSeq);


    QPushButton *reset = new QPushButton(tr("Empty Village"));
    layout4->addWidget(reset);
    connect(reset, SIGNAL(clicked()), this, SLOT(slot_Reset()));


}

void UI::loadScenarioEdit()
{
    initToolBox();
    QVBoxLayout* layout4= new QVBoxLayout(_toolBox);
    layout4->setSpacing(0);

    QLabel *title = new QLabel("Scenario Editor");
    title->setStyleSheet("QLabel {color: #333333;font-size:14pt;font-weight:bold; \
	                      padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    title->setFixedHeight(15);
    layout4->addWidget(title);
    layout4->addSpacing(20);


    if (!_data)
    {
        layout4->addSpacing(500);
        return;
    }

    // LIST
    QListWidget* list = scenarioListFromHashScenario(_data->scenarios);
    layout4->addWidget(list);

    connect(list, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_ScenarioSelection(QListWidgetItem *)));

    // ACTIONS
    QWidget* dockh = new QWidget();
    QHBoxLayout* hlayout = new QHBoxLayout();
    dockh->setLayout(hlayout);
    layout4->addWidget(dockh);
    hlayout->setSpacing(0);

    // SEQUENCES
    sequencesTable = new QTableWidget(20,3);
    sequencesTable->setFixedHeight(500);
    for (unsigned int i = 0; i < 20; ++i)
    {
        for (unsigned int j = 0; j < 3; ++j)
        {
            sequencesTable->setItem(i,j, new QTableWidgetItem());
        }
    }
    layout4->addWidget(sequencesTable);
}

void UI::loadAtlasEdit()
{
    initToolBox();
    QVBoxLayout* layout4= new QVBoxLayout(_toolBox);
    layout4->setSpacing(0);

    QLabel *title = new QLabel("Atlas Editor");
    title->setStyleSheet("QLabel {color: #333333;font-size:14pt;font-weight:bold; \
	                      padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    title->setFixedHeight(15);
    layout4->addWidget(title);
    layout4->addSpacing(20);

    if (!_data)
    {
        layout4->addSpacing(500);
        return;
    }


    QTabWidget* tab = new QTabWidget();
    tab->setFixedSize(360,700);
    QWidget* roadsA = new QWidget();
    QVBoxLayout* layoutR = new QVBoxLayout(roadsA);
    QWidget* buildsA = new QWidget();
    QVBoxLayout* layoutB = new QVBoxLayout(buildsA);
    QWidget* display = new QWidget();
    QVBoxLayout* layoutD = new QVBoxLayout(display);
    tab->addTab(roadsA, "Roads Atlas");
    tab->addTab(buildsA, "Buildings Atlas");
    tab->addTab(display, "Display");
    layout4->addWidget(tab);

    tab->setStyleSheet("QTabWidget {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    roadsA->setStyleSheet("QWidget {padding: 0px; margin:0px; border: 1px solid #aaaaaa;}");
    buildsA->setStyleSheet("QWidget {padding: 0px; margin:0px; border: 1px solid #aaaaaa;}");


    QListWidget*  ratlas = roadsAtlasListFromHashAtlas(_data->system->_roadsAtlasList);
    layoutR->addWidget(ratlas);

    connect(ratlas, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_RoadsAtlasSelected(QListWidgetItem *)));

    rAtT = new QTreeWidget();
    rAtT->setHeaderHidden(true);
    rAtT->setFixedHeight(500);
    layoutR->addWidget(rAtT);


    QListWidget*  batlas = buildingsAtlasListFromHashAtlas(_data->system->_buildingsAtlasList);
    layoutB->addWidget(batlas);

    connect(batlas, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_BuildingsAtlasSelected(QListWidgetItem *)));
    bAtT = new QTreeWidget();
    bAtT->setHeaderHidden(true);
    bAtT->setFixedHeight(500);
    layoutB->addWidget(bAtT);


    QLabel* lab1 = new QLabel("Building Atlas :");
    lab1->setStyleSheet("QWidget {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    lab1->setFixedHeight(12);
    QListWidget* listBB = buildingsAtlasListFromHashAtlas(_data->system->_buildingsAtlasList);
    connect(listBB, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_DisplayAtlasSelected(QListWidgetItem *)));
    listBB->setFixedHeight(120);

    QLabel* lab2 = new QLabel("Building Type :");
    lab2->setFixedHeight(12);
    lab2->setStyleSheet("QWidget {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    QListWidget* listBT = buildingsTypeList();
    connect(listBT, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_DisplayBTypeSelected(QListWidgetItem *)));
    listBT->setFixedHeight(190);

    QLabel* lab3 = new QLabel("Display Type :");
    lab3->setFixedHeight(12);
    lab3->setStyleSheet("QWidget {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    QListWidget* listDT = displayTypeList();
    connect(listDT, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_DisplayTypeSelected(QListWidgetItem *)));
    listDT->setFixedHeight(190);

    QPushButton *displ = new QPushButton("Display");
    connect(displ, SIGNAL(clicked()), this, SLOT(slot_Display()));
    layoutD->addWidget(lab1);
    layoutD->addWidget(listBB);
    layoutD->addWidget(lab2);
    layoutD->addWidget(listBT);
    layoutD->addWidget(lab3);
    layoutD->addWidget(listDT);
    layoutD->addWidget(displ);



    layout4->addSpacing(500);
}

void UI::loadRoadsEdit()
{
    initToolBox();
    QVBoxLayout* layout4= new QVBoxLayout(_toolBox);
    layout4->setSpacing(0);

    QLabel *title = new QLabel("Roads Editor");
    title->setStyleSheet("QLabel {color: #333333;font-size:14pt;font-weight:bold; \
	                      padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    title->setFixedHeight(15);
    layout4->addWidget(title);
    layout4->addSpacing(20);

    if (!_data)
    {
        layout4->addSpacing(500);
        return;
    }


    QLabel* p1 = new QLabel("Generation :");
    p1->setStyleSheet("QLabel {color: #333333;font-size:10pt;font-weight:bold; \
	                   padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    layout4->addWidget(p1);

    QWidget *w1_1 = new QWidget();
    layout4->addWidget(w1_1);
    QHBoxLayout *line1_1 = new QHBoxLayout(w1_1);
    QLabel *xy = new QLabel("Start (X, Y) :");
    xy->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    xy->setFixedWidth(100);
    line1_1->addWidget(xy);
    x1 = new QLineEdit(QString("700"));
    line1_1->addWidget(x1);
    y1 = new QLineEdit(QString("950"));
    line1_1->addWidget(y1);
    QPushButton *pick1 = new QPushButton(tr("Pick Coords"));
    line1_1->addWidget(pick1);
    connect(pick1, SIGNAL(clicked()), this, SLOT(slot_Pick1()));

    QWidget *w1_2 = new QWidget();
    layout4->addWidget(w1_2);
    QHBoxLayout *line1_2 = new QHBoxLayout(w1_2);
    QLabel *xy2 = new QLabel("Goal (X, Y)");
    xy2->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    xy2->setFixedWidth(100);
    line1_2->addWidget(xy2);
    x2 = new QLineEdit(QString("1030"));
    line1_2->addWidget(x2);
    y2 = new QLineEdit(QString("1010"));
    line1_2->addWidget(y2);

    QPushButton *pick2 = new QPushButton(tr("Pick Coords"));
    line1_2->addWidget(pick2);
    connect(pick2, SIGNAL(clicked()), this, SLOT(slot_Pick2()));

    QLabel      *atlas = new QLabel("Road Atlas");
    atlas->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    layout4->addWidget(atlas);


    QListWidget*  ratlas = roadsAtlasListFromHashAtlas(_data->system->_roadsAtlasList);
    ratlas->setFixedHeight(200);
    layout4->addWidget(ratlas);


    QLabel      *type = new QLabel("Road Type");
    type->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    layout4->addWidget(type);

    QListWidget*  roads = roadsTypeList();
    roads->setFixedHeight(200);
    layout4->addWidget(roads);

    QPushButton *createRoad = new QPushButton(tr("Generate Road"));
    layout4->addWidget(createRoad);
    connect(createRoad, SIGNAL(clicked()), this, SLOT(slot_CreateRoad()));


    connect(roads, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_RoadTypeSelected(QListWidgetItem *)));
    connect(ratlas, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_RoadAtlasSelected(QListWidgetItem *)));


    QPushButton *clearRoad1 = new QPushButton(tr("Shortcut"));
    layout4->addWidget(clearRoad1);
    connect(clearRoad1, SIGNAL(clicked()), this, SLOT(slot_Shortcuts()));


    QPushButton *clearRoad2 = new QPushButton(tr("Beta Skeleton"));
    layout4->addWidget(clearRoad2);
    connect(clearRoad2, SIGNAL(clicked()), this, SLOT(slot_BetaSkeleton()));

    // BEZIER
    QPushButton *clearRoad11 = new QPushButton(tr("Bezier"));
    layout4->addWidget(clearRoad11);
    connect(clearRoad11, SIGNAL(clicked()), this, SLOT(slot_Bezier()));

    // CLEAR ROADS
    QPushButton *clearRoad = new QPushButton(tr("Clear Roads"));
    layout4->addWidget(clearRoad);
    connect(clearRoad, SIGNAL(clicked()), this, SLOT(slot_ClearRoads()));




}

void UI::loadBuildingEdit()
{
    initToolBox();
    QVBoxLayout* layout4= new QVBoxLayout(_toolBox);
    layout4->setSpacing(0);

    QLabel *title = new QLabel("Building Editor");
    title->setStyleSheet("QLabel {color: #333333;font-size:14pt;font-weight:bold; \
	                      padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    title->setFixedHeight(15);
    layout4->addWidget(title);
    layout4->addSpacing(20);

    if (!_data)
    {
        layout4->addSpacing(500);
        return;
    }

    QLabel* p1 = new QLabel("Creation :");
    p1->setStyleSheet("QLabel {color: #333333;font-size:10pt;font-weight:bold; \
	                   padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    layout4->addWidget(p1);

    QWidget *w2_1 = new QWidget();
    layout4->addWidget(w2_1);
    QHBoxLayout *line2_1 = new QHBoxLayout(w2_1);

    QLabel *xyI = new QLabel("X, Y");
    xyI->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    line2_1->addWidget(xyI);
    xI = new QLineEdit(QString("700"));
    line2_1->addWidget(xI);
    yI = new QLineEdit(QString("950"));
    line2_1->addWidget(yI);
    QPushButton *pickI = new QPushButton(tr("Pick Coords"));
    line2_1->addWidget(pickI);
    connect(pickI, SIGNAL(clicked()), this, SLOT(slot_PickI()));

    QLabel      *type2 = new QLabel("Building Type");
    type2->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    QListWidget* listBT2 = buildingsTypeList();
    listBT2->setFixedHeight(150);
    layout4->addWidget(type2);
    layout4->addWidget(listBT2);



    connect(listBT2, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_BuildingTypeSelected(QListWidgetItem *)));

    QPushButton *addBuilding = new QPushButton(tr("Add Building"));
    layout4->addWidget(addBuilding);
    connect(addBuilding, SIGNAL(clicked()), this, SLOT(slot_AddBuilding()));



    QLabel* p2 = new QLabel("Generation :");
    p2->setStyleSheet("QLabel {color: #333333;font-size:10pt;font-weight:bold; \
	                   padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    layout4->addWidget(p2);


    QLabel      *atlas = new QLabel("Building Atlas");
    atlas->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    QListWidget*  batlas = buildingsAtlasListFromHashAtlas(_data->system->_buildingsAtlasList);
    batlas->setFixedHeight(100);
    layout4->addWidget(atlas);
    layout4->addWidget(batlas);

    QLabel      *type = new QLabel("Building Type");
    type->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    QListWidget* listBT = buildingsTypeList();
    listBT->setFixedHeight(150);
    layout4->addWidget(type);
    layout4->addWidget(listBT);


    connect(batlas, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_BuildingAtlasSelected(QListWidgetItem *)));
    connect(listBT, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_BuildingTypeSelected(QListWidgetItem *)));

    QPushButton *createBuilding = new QPushButton(tr("Generate Building"));
    layout4->addWidget(createBuilding);
    connect(createBuilding, SIGNAL(clicked()), this, SLOT(slot_CreateBuilding()));


    QPushButton *createBuilding2 = new QPushButton(tr("Watershed"));
    layout4->addWidget(createBuilding2);
    connect(createBuilding2, SIGNAL(clicked()), this, SLOT(slot_Watershed()));


    QPushButton *clearBuilding = new QPushButton(tr("Clear Building"));
    layout4->addWidget(clearBuilding);
    connect(clearBuilding, SIGNAL(clicked()), this, SLOT(slot_ClearBuildings()));
}

void UI::loadGeometryEdit()
{
    initToolBox();
    QVBoxLayout* layout4= new QVBoxLayout(_toolBox);
    layout4->setSpacing(0);

    QLabel *title = new QLabel("Geometry Editor");
    title->setStyleSheet("QLabel {color: #333333;font-size:14pt;font-weight:bold; \
	                      padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    title->setFixedHeight(15);
    layout4->addWidget(title);
    layout4->addSpacing(20);

    QPushButton *generateGeometry = new QPushButton(tr("Generate Geometry"));
    layout4->addWidget(generateGeometry);
    connect(generateGeometry, SIGNAL(clicked()), this, SLOT(slot_GenerateGeometry()));

    if (!_data)
    {
        layout4->addSpacing(500);
        return;
    }

    layout4->addSpacing(700);
}

void UI::loadLandscapeEdit()
{
    initToolBox();

    QVBoxLayout* layout4= new QVBoxLayout(_toolBox);
    layout4->setSpacing(0);

    QLabel *title = new QLabel("Landscape Editor");
    title->setStyleSheet("QLabel {color: #333333;font-size:14pt;font-weight:bold; \
	                      padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    title->setFixedHeight(15);
    layout4->addWidget(title);
    layout4->addSpacing(20);

    if (!_data)
    {
        layout4->addSpacing(500);
        return;
    }

    QLabel* p2 = new QLabel("Properties :");
    p2->setStyleSheet("QLabel {color: #333333;font-size:10pt;font-weight:bold; \
	                   padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    layout4->addWidget(p2);

    QWidget* ww1 = new QWidget();
    QHBoxLayout* hhh1 = new QHBoxLayout(ww1);
    QLabel* scalel  = new QLabel("Max elevation");
    scalel->setFixedWidth(100);
    scalel->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    scaleEdit = new QLineEdit(QString().setNum(_data->system->_terrain->getMaxElevation()));
    hhh1->addWidget(scalel);
    hhh1->addWidget(scaleEdit);

    layout4->addWidget(ww1);

    connect(scaleEdit, SIGNAL(textEdited(QString)), this, SLOT(slot_ChangeElevation(QString)));




    QLabel* p1 = new QLabel("Gaussian painting :");
    p1->setStyleSheet("QLabel {color: #333333;font-size:10pt;font-weight:bold; \
	                   padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    layout4->addWidget(p1);

    QListWidget* elements = editableTerrainElementList();
    elements->setFixedHeight(200);
    layout4->addWidget(elements);
    connect(elements, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_PaintTerrainSelection(QListWidgetItem *)));

    QWidget *widget1 = new QWidget();
    QHBoxLayout *hh1 = new QHBoxLayout(widget1);
    QLabel *brushSize = new QLabel("Brush size");
    brushSize->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    brushSize->setFixedSize(100,15);
    hh1->addWidget(brushSize);
    brushSizeE = new QLineEdit(QString("75.0"));
    hh1->addWidget(brushSizeE);
    layout4->addWidget(widget1);

    QWidget *widget2 = new QWidget();
    QHBoxLayout *hh2 = new QHBoxLayout(widget2);
    QLabel *brushCoeff = new QLabel("Coeff");
    brushCoeff->setStyleSheet("QLabel {padding: 0px; margin:0px; border: 0px solid #aaaaaa;}");
    brushCoeff->setFixedSize(100,15);
    hh2->addWidget(brushCoeff);
    brushCoeffE = new QLineEdit(QString("0.1"));
    hh2->addWidget(brushCoeffE);
    layout4->addWidget(widget2);

    QPushButton *paint = new QPushButton(tr("Paint"));
    layout4->addWidget(paint);
    connect(paint, SIGNAL(clicked()), this, SLOT(slot_Paint()));

    layout4->addSpacing(350);





}

std::string enumToName(Building::Type type)
{
    switch (type)
    {
    case Building::HOUSE :
        return std::string("House");
        break;
    case Building::FIELD :
        return std::string("Field");
        break;
    case Building::CHURCH :
        return std::string("Church");
        break;
    case Building::TRADE :
        return std::string("Trade");
        break;
    case Building::STATUE :
        return std::string("Statue");
        break;
    case Building::VILLA :
        return std::string("Villa");
        break;
    }

}

void UI::slot_PlayScenarioSelection(QListWidgetItem *item)
{
    std::string scenarioName = QString(item->text()).toStdString();
    std::cout << "Selection of : " <<scenarioName << std::endl;
    Scenario scenario = _data->scenarios[scenarioName];
    selectedScenario = scenario;

    // Empty sequences list
    while (sequencesList->count())
    {
        QListWidgetItem *i = sequencesList->item(0);
        delete i;
    }


    LinkedList<Sequence> sequences = scenario.getSequences();
    LinkedList<Sequence>::LinkedListIterator it(& sequences);
    while (it.hasNext())
    {
        it.next();
        std::string name;
        switch (it.value().getType())
        {
        case Sequence::BUILDING_GENERATION :
            name = "Building-Gen : ";
            name += enumToName(it.value().getGenerationType());
            name += ", ";
            name += QString().setNum(it.value().getGenerationNumber()).toStdString();
            name += ".";

            break;
        case Sequence::BUILDINGS_ATLAS_SELECTION :
            name = "Atlas-Buil-Selec : ";
            name += std::string(it.value().getSelectionName());
            name += ".";
            break;
        case Sequence::ROADS_ATLAS_SELECTION :
            name = "Atlas-Road-Selec : ";
            name += std::string(it.value().getSelectionName());
            name += ".";
            break;
        case Sequence::DIRECT_BUILDING_MODIFICATION:
            name = "Building-Creation : ";
            name += enumToName(it.value().getCreationType());
            name += " at ";
            name += QString().setNum(it.value().getCreationX()).toStdString();
            name += ",";
            name += QString().setNum(it.value().getCreationY()).toStdString();
            name += ".";
            break;
        case Sequence::WALL_CREATION:
            name = "Wall creation";
            name += " at ";
            name += QString().setNum(it.value().getWallX()).toStdString();
            name += ",";
            name += QString().setNum(it.value().getWallY()).toStdString();
            name += " r = ";
            name += QString().setNum(it.value().getWallRadius()).toStdString();
            name += ".";
            break;
        }
        sequencesList->addItem(QString(name.c_str()));
    }

    //connect(sequencesList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_PlaySequenceSelection(QListWidgetItem *)));

}
void UI::slot_PlaySequenceSelection(QListWidgetItem *item)
{
    /*
    std::string sequenceName = QString(item->text()).toStdString();
    std::cout << "Play of : " <<sequenceName << std::endl;
    */
}

void UI::slot_ScenarioSelection(QListWidgetItem *item)
{
    std::string scenarioName = QString(item->text()).toStdString();
    std::cout << "Selection of : " <<scenarioName << std::endl;
    Scenario scenario = _data->scenarios[scenarioName];



//        QTableWidget* table = sequenceTableFromScenario(scenario);


    /*

    // Empty sequences list
    while (sequencesList->count())
    {
            QListWidgetItem *i = sequencesList->item(0);
            delete i;
    }


    LinkedList<Sequence> sequences = scenario.getSequences();
    LinkedList<Sequence>::LinkedListIterator it(& sequences);
    while (it.hasNext())
    {
            it.next();
            std::string name;
            switch (it.value().getType())
            {
                    case Sequence::BUILDING_GENERATION :
                            name = "Buil-Gen";
                    break;
            }
            sequencesList->addItem(QString(name.c_str()));
    }
    */

}

void UI::slot_ScenarioNew()
{
}

void UI::slot_ScenarioDelete()
{
}

void UI::slot_ScenarioEdit()
{
}



void UI::slot_VillagePlayScenario()
{
    std::cout << "Play scenario " << std::endl;
    if (!_data)
        return;

    _data->system->playAScenario(selectedScenario);
    if (_renderer)
        _renderer->updateTextures();
    _GL_Widget->updateGL();
}

void UI::slot_VillagePlaySequence()
{
    std::cout << "Play sequence " << std::endl;
}


QListWidget*  UI::scenarioListFromHashScenario(HashTable<std::string, Scenario>& scenarios)
{
    QListWidget* list = new QListWidget();
    HashTable<std::string, Scenario>::HashTableIterator it(& _data->scenarios);
    while (it.hasNext())
    {
        it.next();
        std::string name = it.value().getKey();
        list->addItem(QString(name.c_str()));
    }
    return list;
}
/*
QTableWidget* UI::sequenceTableFromScenario(Scenario& scenario)
{
        QTableWidget* table = sequencesTable;

        sequencesTable->setFixedHeight(500);
	for (unsigned int i = 0; i < 20; ++i)
	{
	        for (unsigned int j = 0; j < 3; ++j)
	        {
                	sequencesTable->setItem(i,j, new QTableWidgetItem());
                }
        }


        LinkedList<Sequence> sequences = scenario.getSequences();
	LinkedList<Sequence>::LinkedListIterator it(& sequences);



        QLabel *type;
        QLabel *typeBuild;
        QLabel *typeNumber;
        QTableWidgetItem *item;

        int i = 0;
	while (it.hasNext())
	{
	        it.next();
	        std::string name;
	        switch (it.value().getType())
	        {
	               case Sequence::BUILDING_GENERATION :
	               {
	                        name = "Buil-Gen";
	               }
	               break;
	               case Sequence::DIRECT_ROAD_MODIFICATION :
	                        name = "Road-Direct";
	               break;
	               case Sequence::DIRECT_BUILDING_MODIFICATION :
	                        name = "Buil-Direct";
	               break;
	               case Sequence::ROADS_ATLAS_MODIFICATION :
	                        name = "Atlas-Road";
	               break;
	               case Sequence::BUILDINGS_ATLAS_MODIFICATION :
	                        name = "Atlas-Buil";
	               break;
	               case Sequence::BUILDINGS_ATLAS_SELECTION :
	                        name = "Atlas-Buil-Selec";
	               break;
	               case Sequence::ROADS_ATLAS_SELECTION :
	                        name = "Atlas-Road-Selec";
	               break;
	        }
	        item = new QTableWidgetItem();
	        table->setItem(i, 0,item);
	        item->setText(QString(name.c_str()));
	        ++i;
	}
	//table->setEditTriggers(QAbstractItemView::AllEditTriggers);
}

QTableWidget* UI::roadsAtlasTableFromAtlas(RoadsAtlas& atlas)
{
}

QTableWidget* UI::buildingsAtlasTableFromAtlas(BuildingsAtlas& atlas)
{
}

QTreeWidget*  UI::roadsAtlasTreeFromAtlas(RoadsAtlas& atlas)
{
}

QTreeWidget*  UI::buildingsAtlasTreeFromAtlas(BuildingsAtlas& atlas)
{
}*/


QListWidget*  UI::roadsAtlasListFromHashAtlas(HashTable<std::string, RoadsAtlas>& atlas)
{
    QListWidget* list = new QListWidget();
    HashTable<std::string, RoadsAtlas>::HashTableIterator it(& _data->system->_roadsAtlasList);
    while (it.hasNext())
    {
        it.next();
        std::string name = it.value().getKey();
        list->addItem(QString(name.c_str()));
    }
    return list;
}

QListWidget*  UI::buildingsAtlasListFromHashAtlas(HashTable<std::string, BuildingsAtlas>& atlas)
{
    QListWidget* list = new QListWidget();
    HashTable<std::string, BuildingsAtlas>::HashTableIterator it(& _data->system->_buildingsAtlasList);
    while (it.hasNext())
    {
        it.next();
        std::string name = it.value().getKey();
        list->addItem(QString(name.c_str()));
    }
    return list;
}




void  UI::slot_RoadsAtlasSelected(QListWidgetItem *item)
{
    std::string name = QString(item->text()).toStdString();
    std::cout << "Selection of : " << name << std::endl;
    RoadsAtlas atlas = _data->system->_roadsAtlasList[name];

    // Delete all elements
    while( int nb = rAtT->topLevelItemCount () )
    {
        delete rAtT->takeTopLevelItem( nb - 1 );
    }



    rAtT->setStyleSheet("QTreeWidget::item {\
                        margin:0px;\
                        padding:2px;\
                        height:13px;\
                        border-bottom: 0px solid lightGray;\
                        }"
                       );


    QTreeWidgetItem* root = new QTreeWidgetItem();
    root->setText(0, name.c_str());
    rAtT->addTopLevelItem(root);
    rAtT->expandItem(root);
    rAtT->setHeaderHidden(true);

    QTreeWidgetItem* current= NULL;
    for (unsigned int i = 0; i < Road::CUSTOM; ++i)
    {
        current = new QTreeWidgetItem();
        std::string currentName;
        switch ((Road::Type)i)
        {
        case Road::TRAIL :
            currentName = "Trail";
            break;
        case Road::STAIRS :
            currentName = "Stairs";
            break;
        case Road::STREET :
            currentName = "Street";
            break;
        case Road::DIRT_ROAD :
            currentName = "Dirty road";
            break;
        case Road::ROAD :
            currentName = "Road";
            break;
        }
        current->setText(0, currentName.c_str());
        //current->setFlags(Qt::ItemIsEditable);
        root->addChild(current);
        rAtT->expandItem(current);

        for (unsigned int j = 0; j < RoadsAtlas::_roadsWeight+1; ++j)
        {
            QTreeWidgetItem* paramW = new QTreeWidgetItem();
            current->addChild(paramW);
            QWidget* line = new QWidget();
            line->setStyleSheet("QWidget {padding: 0px; margin:0px;}");
            QHBoxLayout* hl = new QHBoxLayout(line);
            hl->setSpacing(0);
            line->setFixedHeight(30);
            rAtT->setItemWidget(paramW, 0, line);
            std::string param;
            switch((RoadsAtlas::Params)j)
            {
            case RoadsAtlas::_travelWeight :
                param = "travelWeight";
                break;
            case RoadsAtlas::_curvatureWeight :
                param = "curvatureWeight";
                break;
            case RoadsAtlas::_curvatureMax :
                param = "curvatureMax";
                break;
            case RoadsAtlas::_slopeWeight :
                param = "slopeWeight";
                break;
            case RoadsAtlas::_slopeMax :
                param = "slopeMax";
                break;
            case RoadsAtlas::_waterWeight :
                param = "waterWeight";
                break;
            case RoadsAtlas::_roadsWeight :
                param = "roadsWeight";
                break;
            }
            QLabel* label = new QLabel(param.c_str());
            label->setStyleSheet("QLabel {color: #000000;font-size:9pt;font-weight:normal; \
	                                      padding: 0px; margin:00px; border: 0px solid #aaaaaa;}");
            label->setFixedSize(100,20);
            hl->addWidget(label);

            QString values = QString().setNum(atlas.getParameter((Road::Type)i, (RoadsAtlas::Params)j));
            //QLineEdit* value = new QLineEdit(values);
            MyTreeQLineEdit* value = new MyTreeQLineEdit(QString(name.c_str()),(Road::Type)i,(RoadsAtlas::Params)j,values);
            connect(value, SIGNAL(NewRoadParamValue (QString, QString, Road::Type, RoadsAtlas::Params)),
                    this,  SLOT(slot_NewRoadParamValue (QString, QString, Road::Type, RoadsAtlas::Params)));
            value->setFixedSize(90,20);
            hl->addWidget(value);

        }
    }


}

void  UI::slot_BuildingsAtlasSelected(QListWidgetItem *item)
{
    std::string name = QString(item->text()).toStdString();
    std::cout << "Selection of : " << name << std::endl;
    BuildingsAtlas atlas = _data->system->_buildingsAtlasList[name];

    // Delete all elements
    while( int nb = bAtT->topLevelItemCount () )
    {
        delete bAtT->takeTopLevelItem( nb - 1 );
    }



    bAtT->setStyleSheet("QTreeWidget::item {\
                        margin:0px;\
                        padding:2px;\
                        height:13px;\
                        border-bottom: 0px solid lightGray;\
                        }"
                       );


    QTreeWidgetItem* root = new QTreeWidgetItem();
    root->setText(0, name.c_str());
    bAtT->addTopLevelItem(root);
    bAtT->expandItem(root);
    bAtT->setHeaderHidden(true);

    QTreeWidgetItem* current= NULL;
    for (unsigned int i = 0; i < Building::CUSTOM; ++i)
    {
        current = new QTreeWidgetItem();
        std::string currentName;
        switch ((Building::Type)i)
        {
        case Building::HOUSE :
            currentName = "House";
            break;
        case Building::VILLA :
            currentName = "Villa";
            break;
        case Building::GARDEN :
            currentName = "Garden";
            break;
        case Building::TRADE :
            currentName = "Trade";
            break;
        case Building::CHURCH :
            currentName = "Church";
            break;
        case Building::STATUE :
            currentName = "Statue";
            break;
        case Building::FIELD :
            currentName = "Field";
            break;
        case Building::WATER_MILL :
            currentName = "Water Mill";
            break;
        case Building::WIND_MILL :
            currentName = "Wind Mill";
            break;
        case Building::TOWER_DEFENSE :
            currentName = "Tower";
            break;
        case Building::CASTLE :
            currentName = "Castle";
            break;
        case Building::WALL :
            currentName = "Wall";
            break;
        }
        current->setText(0, currentName.c_str());
        root->addChild(current);
        bAtT->expandItem(current);

        for (unsigned int j = 0; j < BuildingsAtlas::_vegetationFeasability+1; ++j)
        {
            QTreeWidgetItem* paramW = new QTreeWidgetItem();
            current->addChild(paramW);
            QWidget* line = new QWidget();
            line->setStyleSheet("QWidget {padding: 0px; margin:0px;}");
            QHBoxLayout* hl = new QHBoxLayout(line);
            hl->setSpacing(0);
            line->setFixedHeight(30);
            bAtT->setItemWidget(paramW, 0, line);
            std::string param;
            switch((BuildingsAtlas::Params)j)
            {
            case BuildingsAtlas::_sociabilityWeight :
                param = "sociabilityWeight";
                break;
            case BuildingsAtlas::_sociabilityMin :
                param = "sociabilityMin";
                break;
            case BuildingsAtlas::_sociabilityBest :
                param = "sociabilityBest";
                break;
            case BuildingsAtlas::_sociabilityMax :
                param = "sociabilityMax";
                break;

            case BuildingsAtlas::_roadsWeight :
                param = "roadsWeight";
                break;
            case BuildingsAtlas::_roadsMax :
                param = "roadsMax";
                break;

            case BuildingsAtlas::_altitudeWeight :
                param = "altitudeWeight";
                break;
            case BuildingsAtlas::_altitudeMax :
                param = "altitudeMax";
                break;

            case BuildingsAtlas::_dominationWeight :
                param = "dominationWeight";
                break;
            case BuildingsAtlas::_dominationR :
                param = "dominationR";
                break;

            case BuildingsAtlas::_sunWeight :
                param = "sunWeight";
                break;

            case BuildingsAtlas::_fortificationWeight :
                param = "fortificationWeight";
                break;
            case BuildingsAtlas::_fortificationRmax :
                param = "fortificationRmax";
                break;

            case BuildingsAtlas::_militaryWeight :
                param = "militaryWeight";
                break;
            case BuildingsAtlas::_militaryRmax :
                param = "militaryRmax";
                break;

            case BuildingsAtlas::_waterWeight :
                param = "waterWeight";
                break;
            case BuildingsAtlas::_waterRmax :
                param = "waterRmax";
                break;

            case BuildingsAtlas::_cultWeight :
                param = "cultWeight";
                break;
            case BuildingsAtlas::_cultRmax :
                param = "cultRmax";
                break;

            case BuildingsAtlas::_spaceMin :
                param = "spaceMin";
                break;
            case BuildingsAtlas::_slopeVarMax :
                param = "slopeVarMax";
                break;
            case BuildingsAtlas::_slopeMax :
                param = "slopeMax";
                break;
            case BuildingsAtlas::_waterFeasability :
                param = "waterFeasability";
                break;
            case BuildingsAtlas::_vegetationFeasability :
                param = "vegetationFeasability";
                break;
            }
            QLabel* label = new QLabel(param.c_str());
            label->setStyleSheet("QLabel {color: #000000;font-size:9pt;font-weight:normal; \
	                                      padding: 0px; margin:00px; border: 0px solid #aaaaaa;}");
            label->setFixedSize(100,20);
            hl->addWidget(label);

            QString values = QString().setNum(atlas.getParameter((Building::Type)i, (BuildingsAtlas::Params)j));
            //QLineEdit* value = new QLineEdit(values);

            MyBTreeQLineEdit* value = new MyBTreeQLineEdit(QString(name.c_str()),
                    (Building::Type)i,(BuildingsAtlas::Params)j,values);
            connect(value, SIGNAL(NewBuildingParamValue (QString, QString, Building::Type, BuildingsAtlas::Params)),
                    this,  SLOT(slot_NewBuildingParamValue (QString, QString, Building::Type, BuildingsAtlas::Params)));

            value->setFixedSize(90,20);
            hl->addWidget(value);

        }
    }
}


QListWidget*  UI::buildingsTypeList()
{
    QListWidget* list = new QListWidget();
    list->addItem("House");
    list->addItem("Villa");
    list->addItem("Garden");
    list->addItem("Trade");
    list->addItem("Church");
    list->addItem("Statue");
    list->addItem("Field");
    list->addItem("Water Mill");
    list->addItem("Wind Mill");
    list->addItem("Tower");
    list->addItem("Castle");
    list->addItem("Wall");
    return list;
}

QListWidget*  UI::roadsTypeList()
{
    QListWidget* list = new QListWidget();
    list->addItem("Trail");
    list->addItem("Stairs");
    list->addItem("Dirty road");
    list->addItem("Road");
    list->addItem("Street");
    return list;
}


QListWidget*  UI::displayTypeList()
{
    QListWidget* list = new QListWidget();
    list->addItem("Aggregation probability");
    list->addItem("Global interest");
    list->addItem("sociability");
    list->addItem("roads");
    list->addItem("altitude");
    list->addItem("domination");
    list->addItem("sun");
    list->addItem("fortification");
    list->addItem("military");
    list->addItem("water");
    list->addItem("cult");
    list->addItem("Global feasability");
    list->addItem("slope feasability");
    list->addItem("space feasability");
    list->addItem("water feasability");
    list->addItem("vegetation feasability");
    list->addItem("impossible");
    return list;
}


QListWidget*  UI::editableTerrainElementList()
{
    QListWidget* list = new QListWidget();
    list->addItem("Altitude");
    list->addItem("Vegetation");
    list->addItem("Water");
    list->addItem("Rock");
    return list;
}

void UI::slot_PaintTerrainSelection(QListWidgetItem *item)
{
    elementToPaint = QString(item->text()).toStdString();

}

void UI::slot_DisplayAtlasSelected(QListWidgetItem *item)
{
    std::string name = QString(item->text()).toStdString();
    selectedAtlas = _data->system->_buildingsAtlasList[name];
}


Building::Type UI::stringToBuildingType(std::string name)
{
    if (name == std::string("House"))
    {
        return Building::HOUSE;
    }
    else if (name == std::string("Villa"))
    {
        return Building::VILLA;
    }
    else if (name == std::string("Garden"))
    {
        return Building::GARDEN;
    }
    else if (name == std::string("Trade"))
    {
        return Building::TRADE;
    }
    else if (name == std::string("Church"))
    {
        return Building::CHURCH;
    }
    else if (name == std::string("Statue"))
    {
        return Building::STATUE;
    }
    else if (name == std::string("Field"))
    {
        return Building::FIELD;
    }
    else if (name == std::string("Water Mill"))
    {
        return Building::WATER_MILL;
    }
    else if (name == std::string("Wind Mill"))
    {
        return Building::WIND_MILL;
    }
    else if (name == std::string("Tower"))
    {
        return Building::TOWER_DEFENSE;
    }
    else if (name == std::string("Castle"))
    {
        return Building::CASTLE;
    }
    else if (name == std::string("Wall"))
    {
        return Building::WALL;
    }
}

Road::Type UI::stringToRoadType(std::string name)
{
    if (name == std::string("Trail"))
    {
        return Road::TRAIL;
    }
    else if (name == std::string("Stairs"))
    {
        return Road::STAIRS;
    }
    else if (name == std::string("Dirty road"))
    {
        return Road::DIRT_ROAD;
    }
    else if (name == std::string("Road"))
    {
        return Road::ROAD;
    }
    else if (name == std::string("Street"))
    {
        return Road::STREET;
    }
}

Renderer::DISPLAY UI::stringToDisplayType(std::string name)
{
    if (name == std::string("Aggregation probability"))
    {
        return Renderer::AGGREGATION;
    }
    else if (name == std::string("Global interest"))
    {
        return Renderer::GLOBAL_INTEREST;
    }
    else if (name == std::string("sociability"))
    {
        return Renderer::SOCIAL_INTEREST;
    }
    else if (name == std::string("roads"))
    {
        return Renderer::ROADS_INTEREST;
    }
    else if (name == std::string("altitude"))
    {
        return Renderer::ALTITUDE_INTEREST;
    }
    else if (name == std::string("domination"))
    {
        return Renderer::DOMINATION_INTEREST;
    }
    else if (name == std::string("sun"))
    {
        return Renderer::SUN_INTEREST;
    }
    else if (name == std::string("fortification"))
    {
        return Renderer::FORTIFICATION_INTEREST;
    }
    else if (name == std::string("military"))
    {
        return Renderer::MILITARY_INTEREST;
    }
    else if (name == std::string("water"))
    {
        return Renderer::WATER_INTEREST;
    }
    else if (name == std::string("cult"))
    {
        return Renderer::CULT_INTEREST;
    }
    else if (name == std::string("Global feasability"))
    {
        return Renderer::GLOBAL_FEASABILITY;
    }
    else if (name == std::string("slope feasability"))
    {
        return Renderer::SLOPE_FEASABILITY;
    }
    else if (name == std::string("water feasability"))
    {
        return Renderer::WATER_FEASABILITY;
    }
    else if (name == std::string("space feasability"))
    {
        return Renderer::SPACE_FEASABILITY;
    }
    else if (name == std::string("vegetation feasability"))
    {
        return Renderer::VEGETATION_FEASABILITY;
    }
    else if (name == std::string("impossible"))
    {
        return Renderer::NON_FEASABILITY;
    }
}




void UI::slot_DisplayBTypeSelected(QListWidgetItem *item)
{
    std::string name = QString(item->text()).toStdString();
    selectedBuildingType = stringToBuildingType(name);
}

void UI::slot_DisplayTypeSelected(QListWidgetItem *item)
{
    std::string name = QString(item->text()).toStdString();
    selectedDisplayType = stringToDisplayType(name);
}

void UI::slot_Display()
{
    _GL_Widget->_renderer->setDisplayMode(selectedAtlas, selectedBuildingType, selectedDisplayType);
    _GL_Widget->paintGL();
    _GL_Widget->updateGL();
}



void UI::slot_ChangeScale(QString scale)
{
    _data->system->setScale(scale.toInt());
}

void UI::slot_ChangePattern(QListWidgetItem * item)
{
    std::string name = QString(item->text()).toStdString();
    if (name == "Four")
    {
        _data->system->setNeighbourhood(FOUR);
    }
    else if (name == "Eight")
    {
        _data->system->setNeighbourhood(EIGHT);
    }
    else if (name == "Sixteen")
    {
        _data->system->setNeighbourhood(SIXTEEN);
    }
    else if (name == "Thirty-two")
    {
        _data->system->setNeighbourhood(THIRTY_TWO);
    }
}

QListWidget*  UI::navigationPattern()
{
    QListWidget* list = new QListWidget();
    list->addItem("Four");
    list->addItem("Eight");
    list->addItem("Sixteen");
    list->addItem("Thirty-two");
    return list;
}

void UI::slot_ChangeElevation(QString scale)
{
    _data->system->_terrain->setMaxElevation(scale.toDouble());
}


void UI::slot_RoadAtlasSelected(QListWidgetItem *item)
{
    std::string name = QString(item->text()).toStdString();
    selectedRAtlas = _data->system->_roadsAtlasList[name];
}

void UI::slot_BuildingAtlasSelected(QListWidgetItem *item)
{
    std::string name = QString(item->text()).toStdString();
    selectedAtlas = _data->system->_buildingsAtlasList[name];
}

void UI::slot_RoadTypeSelected(QListWidgetItem *item)
{
    std::string name = QString(item->text()).toStdString();
    selectedRoadType = stringToRoadType(name);
}


void UI::slot_BuildingTypeSelected(QListWidgetItem *item)
{
    std::string name = QString(item->text()).toStdString();
    selectedBuildingType = stringToBuildingType(name);
}


void UI::slot_NewRoadParamValue (QString value, QString atlasName, Road::Type type, RoadsAtlas::Params param)
{
    _data->system->_roadsAtlasList[atlasName.toStdString()].setParameters(type, param, value.toDouble());
}

void UI::slot_NewBuildingParamValue (QString value, QString atlasName, Building::Type type, BuildingsAtlas::Params param)
{
    _data->system->_buildingsAtlasList[atlasName.toStdString()].setParameters(type, param, value.toDouble());
}


void UI::slot_GenerateGeometry()
{
    _data->system->generateGeometry();
}

void UI::slot_Shortcuts()
{
    _data->system->shortcuts();
}

void UI::slot_BetaSkeleton()
{
    _data->system->betaSkeleton();
    if (_renderer)
        _renderer->updateTextures(2);
    _GL_Widget->updateGL();
}



void UI::slot_Watershed()
{
    _data->system->watershed();
    if (_renderer)
        _renderer->updateTextures();
    _GL_Widget->updateGL();

}

void UI::slot_Bezier()
{
    _data->system->_bezier = ! _data->system->_bezier;
}
