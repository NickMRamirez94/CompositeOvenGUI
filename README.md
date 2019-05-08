# CompositeOvenGUI
The CureCycle application is part of the User Programmable Composite Oven project. This project was completed by Nicholas Ramirez (Lead Designer of this desktop application), Chad Palmer, and David Brassfield. The application is meant to be used alongside the [Oven Controller](https://github.com/NickMRamirez94/CompositeOvenEmbeddedControl). The desktop application provides the user with a way to create, edit, and save cure cycles to be run on the composite oven. Further, the application provides the user with a way to view the temperature data received from the oven. 

## Requirements

* Ubuntu 16.04+
* GCC 5.5.0
* GNU Make 4.1
* Qt 5.9.6
* QMake 3.1
* Qt Creator 4.7.0
* Git

## Installation of requirements
### Using Web Installer

[Qt Download](https://www.qt.io/download-qt-installer?hsCtaTracking=9f6a2170-a938-42df-a8e2-a9f0b1d6cdce%7C6cb0de4f-9bb5-4778-ab02-bfb62735f3e5) - For Qt library and Qt Creator (IDE)

Download all Qt 5.9.6 library files, Qt Creator, and only Desktop related files.

### Using Terminal

```
sudo apt-get install qtcreator
```

```
sudo apt install qt5-default
```

## Run the Application

Clone the application directory.

```
git clone https://github.com/NickMRamirez94/CompositeOvenGUI.git
```

### Through Qt Creator

Open the Qt Creator. Select Open Project. Select CureCycle.pro file. Qt Creator will import files. The application can then be run from there.

If there is an error between version try cleaning the project and rebuilding.

### Through Terminal

In terminal Navigate to the CureCycle directory. Then run:

```
qmake CureCycle.pro
```

Followed by:

```
make
```

An executable should have been created. So you can then run:

```
./CureCycle
```

If qmake is not found by default you'll need to specifiy the path to the executable. In general it is found in:

```
/home/{user}/Qt/{version}/gcc_64/bin/qmake
```

## How to Use


