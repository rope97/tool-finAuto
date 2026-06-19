#ifndef AUTOMATA_VIEW_HPP
#define AUTOMATA_VIEW_HPP

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <functional>
#include "AutomataDisplay.hpp"
#include "AutomataController.hpp"
#include "AutomataModel.hpp"
#include "RegexDriver.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class AutomataView : public QMainWindow
{
    Q_OBJECT

public:
    AutomataView(QWidget *parent = nullptr);
    ~AutomataView();

    void ConnectUIButtons();
    void ConnectShortcuts();

private slots:
    void OnNewClicked();
    void OnOpenClicked();
    void OnSaveClicked();
    void OnSaveAsClicked();
    void OnInfoClicked();
    void OnCreateNFAClicked();
    void OnCreateDFAClicked();
    void OnCreateMDFAClicked();
    void OnGenerateRegexClicked();
    void OnDeterminizeClicked();
    void OnMinimizeClicked();
    void OnCompleteClicked();
    void OnReverseClicked();
    void OnComplementClicked();
    void OnUnionClicked();
    void OnIntersectionClicked();
    void OnDifferenceClicked();
    void OnCloneClicked();
    void OnDeleteClicked();
    void OnGenerateRandomClicked();
    void OnGenerateAcceptedClicked();
    void OnGenerateUnacceptedClicked();
    void OnStartClicked();
    void OnForwardClicked();
    void OnBackClicked();
    void OnResetClicked();
    void OnSkipClicked();
    void OnCreateTLSClicked();
private:
    Ui::MainWindow *ui;
    AutomataDisplay *mAutomataDisplay;
    AutomataController& automataController;
    AutomataModel automataModel;
    void CreatePopup();

    // Used for checking whether string match
    // simulation has been started.
    bool mSimulationActive = false;
    unsigned mCurrentSimulationStep = 0;

    void MarkTestStringSegment(unsigned pos, unsigned len = 1);
    void ClearTestStringMarking();
    std::string path = "";

    void CreationAutomataOperation(std::function<unsigned (AutomataModel&, std::string&)> controllerFunc);
    void UnaryAutomataOperaiton(std::function<unsigned (AutomataModel&, unsigned)> controllerFunc);
    void BinaryAutomataOperaiton(std::function<unsigned (AutomataModel&, std::vector<unsigned>&)> controllerFunc);
};

#endif // AUTOMATA_VIEW_HPP
