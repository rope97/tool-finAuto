#include "lib/AutomataView.hpp"
#include "lib/AutomataGraph.hpp"
#include "lib/FiniteAutomata.hpp"
#include "lib/InfoPopup.hpp"
#include "ui_MainWindow.h"
#include <string>
#include <algorithm>
#include <iterator>
#include <ios>
#include <fstream>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextLayout>
#include <QInputEvent>
#include <QShortcut>

AutomataView::AutomataView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mAutomataDisplay(new AutomataDisplay(this)),
      automataController(AutomataController::GetController())
{
    ui->setupUi(this);
    mAutomataDisplay->setSceneRect(-16000, -16000, 32000, 32000);
    ui->gvAutomataDisplay->setScene(mAutomataDisplay);

    ConnectUIButtons();
    ConnectShortcuts();
}

AutomataView::~AutomataView()
{
    delete ui;
}

void AutomataView::ConnectUIButtons()
{
    connect(ui->pbCreateNFA, &QPushButton::clicked, this, &AutomataView::OnCreateNFAClicked);
    connect(ui->pbCreateDFA, &QPushButton::clicked, this, &AutomataView::OnCreateDFAClicked);
    connect(ui->pbCreateMDFA, &QPushButton::clicked, this, &AutomataView::OnCreateMDFAClicked);
    connect(ui->pbGenerateRegex, &QPushButton::clicked, this, &AutomataView::OnGenerateRegexClicked);
    connect(ui->pbDeterminize, &QPushButton::clicked, this, &AutomataView::OnDeterminizeClicked);
    connect(ui->pbMinimize, &QPushButton::clicked, this, &AutomataView::OnMinimizeClicked);
    connect(ui->pbComplete, &QPushButton::clicked, this, &AutomataView::OnCompleteClicked);
    connect(ui->pbReverse, &QPushButton::clicked, this, &AutomataView::OnReverseClicked);
    connect(ui->pbComplement, &QPushButton::clicked, this, &AutomataView::OnComplementClicked);
    connect(ui->pbUnion, &QPushButton::clicked, this, &AutomataView::OnUnionClicked);
    connect(ui->pbIntersection, &QPushButton::clicked, this, &AutomataView::OnIntersectionClicked);
    connect(ui->pbDifference, &QPushButton::clicked, this, &AutomataView::OnDifferenceClicked);
    connect(ui->pbClone, &QPushButton::clicked, this, &AutomataView::OnCloneClicked);
    connect(ui->pbDelete, &QPushButton::clicked, this, &AutomataView::OnDeleteClicked);
    connect(ui->pbAcceptableString, &QPushButton::clicked, this, &AutomataView::OnGenerateAcceptedClicked);
    connect(ui->pbUnacceptableString, &QPushButton::clicked, this, &AutomataView::OnGenerateUnacceptedClicked);
    connect(ui->pbRandomString, &QPushButton::clicked, this, &AutomataView::OnGenerateRandomClicked);
    connect(ui->actionNew, &QAction::triggered, this, &AutomataView::OnNewClicked);
    connect(ui->actionOpen, &QAction::triggered, this, &AutomataView::OnOpenClicked);
    connect(ui->actionSave, &QAction::triggered, this, &AutomataView::OnSaveClicked);
    connect(ui->actionSaveAs, &QAction::triggered, this, &AutomataView::OnSaveAsClicked);
    connect(ui->actionInfo, &QAction::triggered, this, &AutomataView::OnInfoClicked);
    connect(ui->pbStart, &QPushButton::clicked, this, &AutomataView::OnStartClicked);
    connect(ui->pbStepForward, &QPushButton::clicked, this, &AutomataView::OnForwardClicked);
    connect(ui->pbStepBack, &QPushButton::clicked, this, &AutomataView::OnBackClicked);
    connect(ui->pbReset, &QPushButton::clicked, this, &AutomataView::OnResetClicked);
    connect(ui->pbSkip, &QPushButton::clicked, this, &AutomataView::OnSkipClicked);
}

void AutomataView::ConnectShortcuts()
{
    QShortcut *scCopy = new QShortcut(QKeySequence("Ctrl+D"), this);
    QObject::connect(scCopy, SIGNAL(activated()), this, SLOT(OnCloneClicked()));
    QShortcut *scDelete = new QShortcut(QKeySequence("Del"), this);
    QObject::connect(scDelete, SIGNAL(activated()), this, SLOT(OnDeleteClicked()));
}

void AutomataView::OnCreateNFAClicked()
{
    CreationAutomataOperation(std::bind(&AutomataController::CreateNFA, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnCreateDFAClicked()
{
    CreationAutomataOperation(std::bind(&AutomataController::CreateDFA, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnCreateMDFAClicked()
{
    CreationAutomataOperation(std::bind(&AutomataController::CreateMDFA, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnGenerateRegexClicked()
{
    auto selected = mAutomataDisplay->GetSelectedAutomata();
    if (selected.size() == 1) {
        FiniteAutomata tempAutomata = *automataModel.GetAutomata(selected[0]->GetID());
        QString param = param.fromStdString(tempAutomata.BuildRegex());
        ui->leAutomata->setText(param);
    }
    else
        QMessageBox::information(this, tr("Error"), "Select one automaton to generate regex");
}

void AutomataView::OnDeterminizeClicked()
{
    UnaryAutomataOperaiton(std::bind(&AutomataController::ToDFA, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnMinimizeClicked()
{
    UnaryAutomataOperaiton(std::bind(&AutomataController::ToMDFA, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnCompleteClicked()
{
    UnaryAutomataOperaiton(std::bind(&AutomataController::Complete, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnReverseClicked()
{
    UnaryAutomataOperaiton(std::bind(&AutomataController::Reverse, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnComplementClicked()
{
    UnaryAutomataOperaiton(std::bind(&AutomataController::Complement, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnUnionClicked()
{
    BinaryAutomataOperaiton(std::bind(&AutomataController::Union, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnIntersectionClicked()
{
    BinaryAutomataOperaiton(std::bind(&AutomataController::Intersection, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnDifferenceClicked()
{
    BinaryAutomataOperaiton(std::bind(&AutomataController::Difference, &automataController, std::placeholders::_1, std::placeholders::_2));
}

void AutomataView::OnCloneClicked()
{
    auto selected = mAutomataDisplay->GetSelectedAutomata();
    for (auto *item : selected)
    {
        unsigned automataID = automataController.CloneAutomata(automataModel, item->GetID());
        QPointF centerPos = item->GetCenterPos();
        mAutomataDisplay->AddAutomata(*automataModel.GetAutomata(automataID), automataID, centerPos + QPoint(80, 80));
        item->setSelected(false);
        mAutomataDisplay->GetAutomataByID(automataID)->setSelected(true);
    }
}

void AutomataView::OnDeleteClicked()
{
    auto selected = mAutomataDisplay->GetSelectedAutomata();
    for (auto *item : selected) {
        automataController.RemoveAutomata(automataModel, item->GetID());
        mAutomataDisplay->removeItem(item);
        delete item;
    }
}

void AutomataView::OnGenerateRandomClicked()
{
    auto selected = mAutomataDisplay->GetSelectedAutomata();
    if(selected.size() == 1) {
        auto generated = automataController.GetRandom(automataModel, selected[0]->GetID());
        QString inputString;
        if (generated)
            inputString = QString::fromStdString(*generated);
        else
            inputString = "No string can be generated";
        ui->leTestInput->setText(inputString);
    }
    else
        QMessageBox::information(this, tr("Error"), "Select one automaton to generate string on");
}

void AutomataView::OnGenerateAcceptedClicked()
{
    auto selected = mAutomataDisplay->GetSelectedAutomata();
    if(selected.size() == 1) {
        auto generated = automataController.GetAccepted(automataModel, selected[0]->GetID());
        QString inputString;
        if (generated)
            inputString = QString::fromStdString(*generated);
        else
            inputString = "No acceptable string exists";
        ui->leTestInput->setText(inputString);
    }
    else
        QMessageBox::information(this, tr("Error"), "Select one automaton to generate string on");
}

void AutomataView::OnGenerateUnacceptedClicked()
{
    auto selected = mAutomataDisplay->GetSelectedAutomata();
    if(selected.size() == 1) {
        auto generated = automataController.GetUnaccepted(automataModel, selected[0]->GetID());
        QString inputString;
        if (generated)
            inputString = QString::fromStdString(*generated);
        else
            inputString = "No unacceptable string exists";
        ui->leTestInput->setText(inputString);
    }
    else
        QMessageBox::information(this, tr("Error"), "Select one automaton to generate string on");
}

void AutomataView::OnNewClicked()
{
   // std::string file;
   // std::string scene;
   // TODO: add check if scene is in sync with file in path
    auto automatas = automataModel.GetAllAutomata();
    for (const auto &automata : automatas) {
        auto graph = mAutomataDisplay->GetAutomataByID(automata.first);
        mAutomataDisplay->removeItem(graph);
        delete graph;
    }
    automataModel.RemoveAllAutomata();
    path = "";
}

void AutomataView::OnOpenClicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", "FAT files (*.fat)");
    std::string filename = file.toStdString();
    std::ifstream openFile;
    path = filename;
    openFile.open(filename);
    if (!openFile.fail()){
        auto automatas = automataModel.GetAllAutomata();
        for (const auto &automata : automatas) {
            auto graph = mAutomataDisplay->GetAutomataByID(automata.first);
            mAutomataDisplay->removeItem(graph);
            delete graph;
        }
        automataModel.RemoveAllAutomata();

        unsigned counter;
        openFile >> counter;

        for (unsigned i = 0; i < counter; i++){
            FiniteAutomata read;
            openFile >> read;
            unsigned automataID = automataController.CreateA(automataModel, read);
            qreal x;
            openFile >> x;
            qreal y;
            openFile >> y;
            QPointF centerPos = QPointF(x, y);
            mAutomataDisplay->AddAutomata(*automataModel.GetAutomata(automataID), automataID, centerPos);
        }
    }
    openFile.close();
}

void AutomataView::OnSaveClicked()
{
    if (path == ""){
        if (automataModel.GetAllAutomata().size() == 0){
            QMessageBox::information(this, tr("Error"), "The scene is empty");
        }
        else{
            QString file = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/", "FAT files (*.fat)");
            if (!file.isEmpty()){
                std::string filename = file.toStdString();
                if (filename.substr(filename.size()-4).compare(".fat")!=0){
                    filename+=".fat";
                }
                path = filename;
                std::ofstream saveFile;
                saveFile.open(filename);
                auto automatas = automataModel.GetAllAutomata();
                saveFile << automatas.size() << " ";
                for (const auto &automata: automatas){
                    saveFile << automata.second;
                    QPointF centerPos = mAutomataDisplay->GetAutomataByID(automata.first)->GetCenterPos();
                    saveFile << centerPos.x() << " " << centerPos.y() << " ";
                }
                saveFile.close();
            }
        }
    }
    else {
        std::ofstream saveFile;
        saveFile.open(path);
        auto automatas = automataModel.GetAllAutomata();
        saveFile << automatas.size() << " ";
        for (const auto &automata: automatas){
            saveFile << automata.second;
            QPointF centerPos = mAutomataDisplay->GetAutomataByID(automata.first)->GetCenterPos();
            saveFile << centerPos.x() << " " << centerPos.y() << " ";
        }
        saveFile.close();
    }
}

void AutomataView::OnSaveAsClicked()
{
    if (automataModel.GetAllAutomata().size() == 0){
        QMessageBox::information(this, tr("Error"), "The scene is empty");
    }
    else{
        QString file = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/", "FAT files (*.fat)");
        if (!file.isEmpty()){
            std::string filename = file.toStdString();
            if (filename.substr(filename.size()-4).compare(".fat")!=0){
                filename+=".fat";
            }
            path = filename;
            std::ofstream saveFile;
            saveFile.open(filename);
            auto automatas = automataModel.GetAllAutomata();
            saveFile << automatas.size() << " ";
            for (const auto &automata: automatas){
                saveFile << automata.second;
                QPointF centerPos = mAutomataDisplay->GetAutomataByID(automata.first)->GetCenterPos();
                saveFile << centerPos.x() << " " << centerPos.y() << " ";
            }
            saveFile.close();
        }
    }
}

void AutomataView::CreatePopup(){
    Popup *popup = new Popup(this);
    popup->show();
}

void AutomataView::OnInfoClicked()
{
    CreatePopup();
}

void AutomataView::OnStartClicked()
{
    mCurrentSimulationStep = 0;
    if (mSimulationActive) {
        ui->pbStart->setText("Start");
        this->ClearTestStringMarking();
        auto simulated = mAutomataDisplay->GetSimulatedAutomata();
        for (auto& item : simulated)
            item->StopMatchSimulation();
    }
    else {
        ui->pbStart->setText("Stop");
        this->MarkTestStringSegment(mCurrentSimulationStep);
        auto selected = mAutomataDisplay->GetSelectedAutomata();
        for (auto& item : selected) {
            unsigned automataID = item->GetID();
            FiniteAutomata automata = *automataModel.GetAutomata(automataID);
            auto steps = automata.MatchSimulation(ui->leTestInput->text().toStdString());
            item->StartMatchSimulation(steps);
        }
    }

    ui->pbReset->setEnabled(!mSimulationActive);
    ui->pbStepBack->setEnabled(!mSimulationActive);
    ui->pbStepForward->setEnabled(!mSimulationActive);
    ui->pbSkip->setEnabled(!mSimulationActive);
    ui->leTestInput->setEnabled(mSimulationActive);
    ui->pbRandomString->setEnabled(mSimulationActive);
    ui->pbAcceptableString->setEnabled(mSimulationActive);
    ui->pbUnacceptableString->setEnabled(mSimulationActive);
    mSimulationActive = !mSimulationActive;
}

void AutomataView::OnForwardClicked()
{
    auto simulated = mAutomataDisplay->GetSimulatedAutomata();
    for (auto& item : simulated)
        item->MatchSimulationForward();
    unsigned testStringLength = ui->leTestInput->text().length();
    if (mCurrentSimulationStep < testStringLength)
        mCurrentSimulationStep++;
    this->MarkTestStringSegment(mCurrentSimulationStep);
}

void AutomataView::OnBackClicked()
{
    auto simulated = mAutomataDisplay->GetSimulatedAutomata();
    for (auto& item : simulated)
        item->MatchSimulationBack();
    if (mCurrentSimulationStep > 0)
        mCurrentSimulationStep--;
    this->MarkTestStringSegment(mCurrentSimulationStep);
}

void AutomataView::OnResetClicked()
{
    auto simulated = mAutomataDisplay->GetSimulatedAutomata();
    for (auto& item : simulated)
        item->MatchSimulationReset();
    mCurrentSimulationStep = 0;
    this->MarkTestStringSegment(mCurrentSimulationStep);
}

void AutomataView::OnSkipClicked()
{
    auto simulated = mAutomataDisplay->GetSimulatedAutomata();
    for (auto& item : simulated)
        item->MatchSimulationSkip();
    unsigned testStringLength = ui->leTestInput->text().length();
    mCurrentSimulationStep = testStringLength;
    this->MarkTestStringSegment(mCurrentSimulationStep);
}

void AutomataView::MarkTestStringSegment(unsigned pos, unsigned len)
{
    QTextCharFormat f;
    f.setBackground(Qt::darkGreen);
    f.setForeground(Qt::white);

    QTextLayout::FormatRange fr;
    QList<QInputMethodEvent::Attribute> attributes;
    QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
    int start = pos - ui->leTestInput->cursorPosition();
    int length = len;
    attributes.append(QInputMethodEvent::Attribute(type, start, length, f));
    QInputMethodEvent event(QString(), attributes);
    QCoreApplication::sendEvent(ui->leTestInput, &event);
}

void AutomataView::ClearTestStringMarking()
{
    MarkTestStringSegment(0, 0);
}

void AutomataView::CreationAutomataOperation(std::function<unsigned (AutomataModel&, std::string&)> controllerFunc)
{
    QString param = ui->leRegExp->text();
    std::string paramStr = param.toStdString();

    RegexDriver regexDriver;
    if (regexDriver.Parse(paramStr) == nullptr) {
        QMessageBox::information(this, tr("Error"), "Invalid regex");
        return;
    }

    unsigned automataID = controllerFunc(automataModel, paramStr);
    QPointF viewCenter = ui->gvAutomataDisplay->mapToScene(ui->gvAutomataDisplay->rect().center());
    mAutomataDisplay->AddAutomata(*automataModel.GetAutomata(automataID), automataID, viewCenter);

    auto selected = mAutomataDisplay->GetSelectedAutomata();
    for(auto item : selected)
        item->setSelected(false);
    mAutomataDisplay->GetAutomataByID(automataID)->setSelected(true);
}

void AutomataView::UnaryAutomataOperaiton(std::function<unsigned (AutomataModel&, unsigned)> controllerFunc)
{
    auto selected = mAutomataDisplay->GetSelectedAutomata();

    for (auto *item : selected) {
        unsigned automataID = item->GetID();
        QPointF centerPos = item->GetCenterPos();

        automataID = controllerFunc(automataModel, automataID);
        mAutomataDisplay->removeItem(item);
        delete item;
        mAutomataDisplay->AddAutomata(*automataModel.GetAutomata(automataID), automataID, centerPos);
        mAutomataDisplay->GetAutomataByID(automataID)->setSelected(true);
    }
}

void AutomataView::BinaryAutomataOperaiton(std::function<unsigned (AutomataModel&, std::vector<unsigned>&)> controllerFunc)
{
    auto selected = mAutomataDisplay->GetSelectedAutomata();
    if(selected.size() < 2)
        return;

    QPointF viewCenter = ui->gvAutomataDisplay->mapToScene(ui->gvAutomataDisplay->rect().center());

    std::vector<unsigned> selectedID = mAutomataDisplay->GetSelectedIDs(selected);

    unsigned automataID = controllerFunc(automataModel, selectedID);
    for (auto *item : selected) {
        mAutomataDisplay->removeItem(item);
        delete item;
    }

    mAutomataDisplay->AddAutomata(*automataModel.GetAutomata(automataID), automataID, viewCenter);
    mAutomataDisplay->GetAutomataByID(automataID)->setSelected(true);
}
