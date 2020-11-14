[![Build Status](https://travis-ci.org/piti-diablotin/qAgate.svg?branch=master)](https://travis-ci.org/piti-diablotin/qAgate)
[![Downloads](https://img.shields.io/github/downloads/piti-diablotin/qAgate/latest/total.svg)](https://github.com/piti-diablotin/qAgate/releases/latest)

# qAgate
qAgate is the new Qt interface for [agate](https://github.com/piti-diablotin/agate).
It includes all the features of Agate with a new user friendly look.
It also includes sommes "external" tools to help some users with fast analysis.

# Installation
First you need to install [agate](https://github.com/piti-diablotin/agate).
Then you can choose to install qAgate with the following PPA for ubuntu or to compile it yourself.
No matter how you installed agate, make sure to be up to date with both agate and qAgate while the API of agate is not fully finished.

## Ubuntu

### Personal Repository
The PPA contains package for all maintained Ubuntu distributions.
Simply run 
```
sudo add-apt-repository ppa:piti-diablotin/abiout
sudo apt-get update
sudo apt-get install libagate-dev qagate
```
and you are done !
Simply execute `qAgate` in a terminal.
Please not the capital *A* which is the difference with the old depreciated qagate !


## From sources
First you need to install some dependancies (works for all maintained Ubuntu versions)
  ```
  sudo apt-get install libfreetype6-dev libboost-dev qtbase5-dev qt5-qmake qt5-default libqt5opengl5-dev libssh-dev git
  ```
  Note that you can install libagate-dev in those dependencies.
  ```
  sudo apt install libagate-dev
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

## MacOS

### DMG
  A `.dmg` file is now available in the [release page](https://github.com/piti-diablotin/qAgate/releases)
  Download the `.dmg` file, open it and *drag-and-drop* the app in you `Applications` folder or in your dock.

### Homebrew
  If you have installed `agate` with Homebrew following the [HowTo](https://github.com/piti-diablotin/agate/blob/master/README.md) you can then just install `qAgate with brew like
  ```
  brew install qagate
  ```
  The `.app` will be place in `/usr/local/Cellar/qagate/X.X.X/bin`
  You can then make a symlink to `/Applications/.

  For instance for version 1.1.1 `ln -s /usr/local/Cellar/qagate/1.1.1/bin/qAgate.app /Applications/`.
  The app will then appear in the launchpad.

### From sources
  `qAgate` can be compiled with homebrew. Here is an example supposing `agate` has been compiled and installed in `/usr/local` following this [HowTo](https://github.com/piti-diablotin/agate/blob/master/README.md)
  ```
  brew install freetype git libssh qt
  git clone https://github.com/piti-diablotin/qAgate.git
  cd qAagate
  qmake "PREFIX=/usr/local" "PREFIX_AGATE=/usr/local" "PREFIX_FREETYPE=/usr/local" "PREFIX_SSH=/usr/local"
  make
  sudo make install
  ```

  *Tips*: If agate, libssg or freetype are installed in a custom repository you can specify this directory with the environement variable
  `PREFIX_AGATE PREFIX_SSH PREFIX_FREETYPE`.
  You can set the installation prefix with the variable `PREFIX`
