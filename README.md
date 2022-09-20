# Hurrigators Project

Hurricane tracking program using Qt with OpenStreetMap and NOAA data.

## Setup Process

Download the Qt Open Source edition installer [here](https://www.qt.io/download-qt-installer).
Check that you are working on an open source project, and that you are individual
Once you have reached the component selection screen, ensure you have Qt 6.3.2 and Qt Creator checked.

[![Image showing the Qt installer](https://cdn.discordapp.com/attachments/1021489757596950568/1021549389728190514/unknown.png)](https://cdn.discordapp.com/attachments/1021489757596950568/1021549389728190514/unknown.png)
Like shown.

All of the other defaults should also be fine.

It could take >1 hour to download and install so get some popcorn.

#### On Windows
Ensure you have  MinGW and `mingw32-make` installed ([MinGW's configuration installer can be downloaded here](https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/)). In Qt Creator, it should autodetect MinGW and set up a desktop build kit automatically. Select it by double-clicking "Desktop Qt 6.2.3 MinGW 64-bit" in the side panel, at which point Build and Run icons should appear below the option. From here, Qt Creator will automatically scan the `.pro` file and you can build with `Ctrl + Shift + B`, run with `Ctrl + R`, and debug with `F5`.

#### On Mac
Install XCode and run it for the first time to ensure it fully sets up its environment. Ensure you have all C++ dependencies installed (it should do this by default). Restart the computer or refresh your PATH. From here Qt Creator should be able to autodetect the GCC compiler, and scan the `.pro` file upon restarting.

## Adding New Classes/Windows
Project code is sorted by type. Headers go in the headers directory, `.cpp` files go in the source directory, and `.ui` forms go in the forms directory. Qt Creator will not automatically do this when creating them, so make sure to sort them appropriately once you have created them. It is also important to note that due to the way the qmake linker creates the Makefile, you must also append a `source/`, `headers/`, etc. before the name of the file in the `HurrigatorsProject.pro` to ensure the file is linked properly, as well as include `headers/` before the header when `#include`'ing anything.