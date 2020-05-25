[![Build Status](https://travis-ci.org/piti-diablotin/qAgate.svg?branch=master)](https://travis-ci.org/piti-diablotin/qAgate)

# qAgate
qAgate is the new Qt interface for [agate](https://github.com/piti-diablotin/abiout).
It includes all the features of Agate with a new user friendly look.
It also includes sommes "external" tools to help some users with fast analysis.

# Installation
First you need to install [agate](https://github.com/piti-diablotin/abiout).
Then you can choose to install qAgate with the following PPA for ubuntu or to compile it yourself.
No matter how you installed agate, make sure to be up to date with both agate and qAgate while the API of agate is not fully finished.

## Personal Repository
  The PPA contains package for all maintained Ubuntu distributions.
  Simply run 
  ```
  sudo add-apt-repository ppa:piti-diablotin/abiout
  sudo apt-get update
  sudo apt-get install abiout qagate
  ```
  and you are done !
  Simply execute `qAgate` in a terminal.
  Please not the capital *A* which is the difference with the old depreciated qagate !
  
  
## From the source
  First you need to install some dependancies (works for all maintained Ubuntu versions)
  ```
  sudo apt-get install libfreetype6-dev libboost-dev qtbase5-dev qt5-qmake qt5-default libqt5opengl5-dev libssh-dev git
  ```
  Note that you can install abiout in those dependencies.
  ```
  sudo apt install abiout
  ```
  Then you clone this repository and compile
  ```
  git clone https://github.com/piti-diablotin/qAgate.git
  cd qAgate
  qmake qAgate.pro
  make
  sudo make install
  ```
  That's it.
  You can now run it with `qAgate`
  
  *Tips*: If agate, spglib or freetype are installed in a custom repository you can specify this directory with the environement variable
  `PREFIX_AGATE PREFIX_SPGLIB PREFIX_FREETYPE`.
  You can set the installation prefix with the variable `PREFIX`
