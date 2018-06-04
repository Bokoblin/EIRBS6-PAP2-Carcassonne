# Projet S6 - Carcassonne

[![Build Status](https://travis-ci.com/Bokoblin/EIRBS6-PAP2-ProjetCarcassonne.svg?token=G5wZLAQfSsj9KcoZsDs4&branch=master)](https://travis-ci.com/Bokoblin/EIRBS6-PAP2-ProjetCarcassonne)
[![CodeFactor](https://www.codefactor.io/repository/github/bokoblin/eirbs6-pap2-projetcarcassonne/badge)](https://www.codefactor.io/repository/github/bokoblin/eirbs6-pap2-projetcarcassonne)

### Description

This project is the second programming project at ENSEIRB.
It consists in programming a french board game named Carcassonne using the C language. <br>
It was carried out from March 23rd to May 18th, 2018.


### Authors

* BEN ABDELJELIL Hadhami [hbenabdelje@enseirb-matmeca.fr](mailto:hbenabdelje@enseirb-matmeca.fr)
* JOLIVET Arthur [ajolivet@enseirb-matmeca.fr](mailto:ajolivet@enseirb-matmeca.fr)
* OSTERTAG Marie [mostertag@enseirb-matmeca.fr](mailto:mostertag@enseirb-matmeca.fr)
* ROSEAU Baptiste [broseau@enseirb-matmeca.fr](mailto:broseau@enseirb-matmeca.fr)


### Tools used during the project

- IDEs : CLion, Visual Studio Code, QT Creator, emacs
- Language: C
- Libraries:
    - SDL 2.0 for the optional UI


### Documentation

The documentation can be generated using Doxygen.
A web-accessible documentation is planned.


### Report

The report is located in the "report" repository in PDF format.

### How to build

#### Requirements

To build project, the following programs must be installed and reachable from `PATH`:
- make
- gcc / clang

You can optionally need those programs:
- cmake
- doxygen

#### Instructions

You can build the project using either make or CMake.

By using make, the following commands are available in the `Makefile`:

```
make [help]  #To show the available commands
make build   #To build the project
make test    #To build and start the tests
make vtest   #To build and start the tests with valgrind check
make ctest   #To build and start the tests with coverage
make install #To install the project in /install
make clean   #To remove objects files, executables and installation
make run     #To run program once installed (make install before required)
make vrun    #To run program once installed (make install before required) with valgrind check
make grun    #To run program once installed (make install before required) with the UI
make docs    #To generate documentation with doxygen in /doc (open /doc/html/index.html in your browser)
```
    
By using CMake, please execute the following commands:

```
mkdir build
cd build
cmake .. && make
```


### Licence

TODO

### Code conventions

Please see `CONVENTIONS.md` file
