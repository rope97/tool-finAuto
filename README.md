# Finite Automata Toolbox (Qt5 + C++)

## Overview

This project is a Qt-based graphical tool for working with **finite state automata (FSA)** and regular expressions.

It provides an interactive environment for:
- constructing automata from regular expressions
- visualizing automata as graphs
- applying classical automata algorithms
- testing string acceptance
- converting between automata and regular expressions

---

## ✨ Implemented Features

### Automata Construction
- Build NFA from regular expressions
- Determinization (NFA → DFA)
- Minimization (Brzozowski algorithm)
- Completion of automata

### Automata Operations
- Union
- Intersection
- Difference
- Complement
- Reversal
- Cloning and deletion

### Regular Expression Support
- Regex → NFA conversion via AST parser
- NFA → Regex conversion

### Testing Utilities
- Test if automaton accepts a string
- Step-by-step simulation of string matching
- Random accepted/unaccepted string generation

---

##New Feature: TLS Automaton Model

A custom **TLS handshake finite automaton** was added as an extension of the toolbox.

### Description

The TLS automaton models a simplified protocol handshake as a finite state machine for mine final exam:
States:
- C → ClientHello
- S → ServerHello
- K → Key Exchange
- V → Verification
- F → Finished
- A → Error state

Transitions simulate a simplified handshake flow:
- C → S → K → V → F (normal handshake)
- alternative transitions simulate session reuse paths
- error transitions lead to a sink state

### Purpose

This extension demonstrates how **communication protocols can be modeled using finite automata**, which is useful for:
- protocol verification
- formal modeling of communication systems
- state-based security analysis

---

##UI Features

The graphical interface includes:

- Automata creation panel (NFA / DFA / MDFA)
- Operation tools (determinize, minimize, union, etc.)
- TLS automaton generation button (custom feature)
- Simulation mode with step-by-step execution
- Graph visualization of automata states and transitions

  
- C++17
- Qt 5
- Flex (lexer)
- Bison (parser)
- Graphviz (graph rendering)

---

 Build Instructions

```bash
qmake app.pro
make
./app


Project currently tested on Linux/macOS
TLS automaton is an educational model, not a real cryptographic implementation

Danica Dimitrijevic

