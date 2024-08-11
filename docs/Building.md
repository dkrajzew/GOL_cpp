# Building optionslib_cpp
Different ways to build the library are supported, please find them below.

Please notice that only the MSVC build supports XML configuration parsing. To enable it, you have to set the define USE_XERCES_XML and You have to include paths to XERCES includes and libraries. If you have any patches for the current make files, please let me know.

## MSVC
### MSVC 2013
You may find a MSVC2013 project file in the folder msvc/msvc2013/options.sln. Open it in your MSVC2013 and build the solution.

### MSVC 2017
You may find a MSVC2017 project file in the folder msvc/msvc2017/options.sln. Open it in your MSVC2017 and build the solution.

## GNU autoconf/automake
In the libraries' root folder do the following:
```
make -f Makefile.cvs
./configure
make
```

This should build the static version of the library named liboptions.a and located in ./utils/options as well as the example applications.

## CMake
Open the root folder and let CMake configure the project.

## Borland c++ compiler
Let's assume, you have installed your Borland c++-Tools into the folder C:\Borland\BCC55. First, set the path in <ROOT>\Makefile.bc properly, somewhere at "Paths of extra headers and libraries:" (my installation is in C:\MehrProgramme\Borland\BCC55).

Then use the following to build the library and the example / test binaries:
```
set PATH=%PATH%;C:\MehrProgramme\Borland\BCC55\Bin
make -fMakefile.bc
```
The library will be located in the folder <ROOT>\utils\options as "options.lib". The executables in &lt;ROOT&gt; as "example.exe" and "tester.exe".



