QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/AutomataModel.cpp \
    src/InfoPopup.cpp \
    src/AutomataController.cpp \
    src/AutomataView.cpp \
    src/FiniteAutomata.cpp \
    src/TLSAutomata.cpp \
    src/RegexAST.cpp \
    src/RegexDriver.cpp \
    src/ZoomableGraphicsView.cpp \
    src/AutomataDisplay.cpp \
    src/AutomataEdge.cpp \
    src/AutomataGraph.cpp \
    src/AutomataNode.cpp \
    src/main.cpp

HEADERS += \
    lib/AutomataModel.hpp \
    lib/InfoPopup.hpp \
    lib/AutomataController.hpp \
    lib/AutomataView.hpp \
    lib/FiniteAutomata.hpp \
    lib/RegexAST.hpp \
    lib/RegexDriver.hpp \
    lib/ZoomableGraphicsView.hpp \
    lib/AutomataDisplay.hpp \
    lib/AutomataEdge.hpp \
    lib/AutomataGraph.hpp \
    lib/AutomataNode.hpp \
    lib/TLSAutomata.hpp
LIBS += -lgvc -lcgraph -lcdt

FORMS += \
    InfoPopup.ui \
    MainWindow.ui

FLEXSOURCES += \
    src/RegexLexer.lex

BISONSOURCES += \
    src/RegexParser.ypp

DEFINES += SRCDIR=\"$$PWD\"
INCLUDEPATH += /opt/homebrew/opt/graphviz/include
LIBS += -L/opt/homebrew/opt/graphviz/lib

flex.input = FLEXSOURCES
flex.output = ${QMAKE_FILE_BASE}.cpp
flex.commands = flex -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
flex.variable_out = SOURCES
flex.clean = ${QMAKE_FILE_IN_BASE}.cpp
flex.name = flex
flex.CONFIG += target_predeps

bisonsource.input = BISONSOURCES
bisonsource.output = ${QMAKE_FILE_BASE}.tab.cpp
bisonsource.commands = /opt/homebrew/opt/bison/bin/bison -d -v -o ${QMAKE_FILE_BASE}.tab.cpp ${QMAKE_FILE_IN}
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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
