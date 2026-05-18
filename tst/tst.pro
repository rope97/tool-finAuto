#TEMPLATE = aux

CONFIG += c++17

QT       += core gui
QT       += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


isEmpty(CATCH_INCLUDE_DIR): CATCH_INCLUDE_DIR=$$(CATCH_INCLUDE_DIR)
# set by Qt Creator wizard
isEmpty(CATCH_INCLUDE_DIR): CATCH_INCLUDE_DIR=$$PWD/../3rd/Catch2/single_include
!isEmpty(CATCH_INCLUDE_DIR): INCLUDEPATH *= $${CATCH_INCLUDE_DIR}

isEmpty(CATCH_INCLUDE_DIR): {
    message("CATCH_INCLUDE_DIR is not set, assuming Catch2 can be found automatically in your system")
}

PROJECT_DIR = $$PWD/../app

INCLUDEPATH += $$PROJECT_DIR

SRC_DIR = $$PWD/../app/src
HDR_DIR = $$PWD/../app/lib

LIBS += \
    -lgvc \
    -lcgraph \
    -lcdt

FORMS += \
    $$PROJECT_DIR/InfoPopup.ui \
    $$PROJECT_DIR/MainWindow.ui

DEFINES += SRCDIR=\\\"$$PWD/\\\"

FLEXSOURCES += \
    $$SRC_DIR/RegexLexer.lex

BISONSOURCES += \
    $$SRC_DIR/RegexParser.ypp

flex.input = FLEXSOURCES
flex.output = ${QMAKE_FILE_BASE}.cpp
flex.commands = flex -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
flex.variable_out = SOURCES
flex.clean = ${QMAKE_FILE_IN_BASE}.cpp
flex.name = flex
flex.CONFIG += target_predeps

bisonsource.input = BISONSOURCES
bisonsource.output = ${QMAKE_FILE_BASE}.tab.cpp
bisonsource.commands = bison -d -v -o ${QMAKE_FILE_BASE}.tab.cpp ${QMAKE_FILE_IN}
bisonsource.variable_out = SOURCES
bisonsource.name = bisonsource
bisonsource.CONFIG += target_predeps

bisonheader.input = BISONSOURCES
bisonheader.output = ${QMAKE_FILE_BASE}.tab.hpp
bisonheader.commands = @true
bisonheader.variable_out = HEADERS
bisonheader.name = bisonsource
bisonheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += \
    flex \
    bisonsource \
    bisonheader

PROJECT_SOURCES = \
    $$SRC_DIR/InfoPopup.cpp \
    $$SRC_DIR/AutomataController.cpp \
    $$SRC_DIR/AutomataView.cpp \
    $$SRC_DIR/AutomataModel.cpp \
    $$SRC_DIR/FiniteAutomata.cpp \
    $$SRC_DIR/ZoomableGraphicsView.cpp \
    $$SRC_DIR/AutomataDisplay.cpp \
    $$SRC_DIR/AutomataEdge.cpp \
    $$SRC_DIR/AutomataGraph.cpp \
    $$SRC_DIR/AutomataNode.cpp \
    $$SRC_DIR/RegexDriver.cpp \
    $$SRC_DIR/RegexAST.cpp \

PROJECT_HEADERS = \
    $$HDR_DIR/InfoPopup.hpp \
    $$HDR_DIR/AutomataController.hpp \
    $$HDR_DIR/AutomataView.hpp \
    $$HDR_DIR/AutomataModel.hpp \
    $$HDR_DIR/FiniteAutomata.hpp \
    $$HDR_DIR/ZoomableGraphicsView.hpp \
    $$HDR_DIR/AutomataDisplay.hpp \
    $$HDR_DIR/AutomataEdge.hpp \
    $$HDR_DIR/AutomataGraph.hpp \
    $$HDR_DIR/AutomataNode.hpp \
    $$HDR_DIR/RegexDriver.hpp \
    $$HDR_DIR/RegexAST.hpp

SOURCES += \
    main.cpp \
    $$PROJECT_SOURCES \
    tests/tst_AutomataModel.cpp \
    tests/tst_FiniteAutomata_Alphabet.cpp \
    tests/tst_FiniteAutomata_BuildRegex.cpp \
    tests/tst_FiniteAutomata_Complement.cpp \
    tests/tst_FiniteAutomata_Completion.cpp \
    tests/tst_FiniteAutomata_CompletionUnderAlphabet.cpp \
    tests/tst_FiniteAutomata_Determinization.cpp \
    tests/tst_FiniteAutomata_Difference.cpp \
    tests/tst_FiniteAutomata_InputStream.cpp \
    tests/tst_FiniteAutomata_Intersection.cpp \
    tests/tst_FiniteAutomata_MatchSimulation.cpp \
    tests/tst_FiniteAutomata_Minimization.cpp \
    tests/tst_FiniteAutomata_OutputStream.cpp \
    tests/tst_FiniteAutomata_Reversal.cpp \
    tests/tst_FiniteAutomata_States.cpp \
    tests/tst_FiniteAutomata_Union.cpp \
    tests/tst_RegexAST_Compile_and_SubClasses.cpp \
    tests/tst_RegexDriver_Parse.cpp

HEADERS += \
    $$PROJECT_HEADERS

