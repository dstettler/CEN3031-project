# Hurrigators Project

Hurricane tracking program using Qt with OpenStreetMap and NOAA data.

## Setup Process

Ensure you recursively clone the repo to ensure you also get the libosmscout repo cloned as well. Download the [Qt Open Source edition installer](https://www.qt.io/download-qt-installer). Check that you are working on an open source project, and that you are an individual. Once you have reached the component selection screen, ensure you have Qt 5.15.2 (this is the Qt5 LTS) checked. All of the other defaults should also be fine. 
Next, install Python 3.11 from [the python downloads page](https://www.python.org/downloads), this is needed for our NOAA crawling script. 

### On Windows:
Install [MSYS2](https://www.msys2.org), and ensure you have the msys64/mingw64/bin directory added to your PATH. From here, run:

`pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-libtool mingw-w64-x86_64-libiconv mingw-w64-x86_64-cmake mingw-w64-x86_64-extra-cmake-modules mingw-w64-x86_64-ninja mingw-w64-x86_64-protobuf mingw-w64-x86_64-libxml2 mingw-w64-x86_64-cairo mingw-w64-x86_64-pango mingw-w64-x86_64-qt5 mingw-w64-x86_64-glew mingw-w64-x86_64-glfw mingw-w64-x86_64-glm mingw-w64-x86_64-qt5-xmlpatterns mingw-w64-x86_64-qt5-scxml`

This will install the dependencies required to build Hurri and OSMScout, the backend library we use to render the map.

### On Mac:
Install XCode and run it for the first time to ensure it fully sets up its environment. Ensure you have all C++ dependencies installed (it should do this by default). Install [homebrew](https://brew.sh). Restart the computer or refresh your PATH. 
Next, run:

`brew install openjdk protobuf protobuf-c qt5 cairo libxml2 gettext pango glfw3 glew glm pkgconfig`

To install the dependencies required to build Hurri and OSMScout, the backend library we use to render the map

### All Platforms
Once you have the dependencies installed, you need to modify the `CMakeLists.txt` file to point to your Qt install. On Windows this will be something to the effect of `C:/Qt/5.15.2/mingw81_64/lib/cmake`, and on Mac it will be something to the effect of `/Users/YOUR_USER/Qt/5.15.2/macos/lib/cmake`. This path will need to go in the `CMAKE_PREFIX_PATH` variable located on line 20 in the `CMakeLists.txt`.
From here, CMake is set up. You can now run:
`cmake -B build -G “Ninja”`
to build the CMake cache, and:
`cmake --build build`
to compile Hurri.

Once Hurri and OSMScout have been compiled, you will need to copy some things into the build directory for Hurri to run. First, you will need to copy the dynamic libraries for OSMScout, OSMScout_map, and OSMScout_map_qt next to the executable, like so (on Windows):

![An image showing a Windows Explorer window, with 4 files: HurrigatorsProject.exe, libosmscout.dll, libosmscout_map.dll, and libosmscout_map_qt.dll](https://cdn.discordapp.com/attachments/1021489757596950568/1049847507393458236/image.png)

These are located in the `build/lib/libosmscout/libosmscout`, `build/lib/libosmscout/libosmscout-map`, and `build/lib/libosmscout/libosmscout-map-qt` directories, respectively.

Finally, you need to copy the contents of the `hurrigators-data.7z` archive into the installation directory. This includes the map database we have already generated from OSM data, the images, and the stylesheets required for rendering. Once completed, the build directory will look something like this:

![An image showing the full directory of the build](https://cdn.discordapp.com/attachments/1021489757596950568/1049847518130868274/image.png)
