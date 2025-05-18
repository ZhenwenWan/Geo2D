set Octave_dir=C:\Octave-7.2.0\mingw64\bin
set mxi_dir=C:\DongJun\EW7_20221206\app\x64

set Current_dir=%cd%
set path=%PATH%;%Octave_dir%

cd %Current_dir%\Build
del dash.cpp.o
del dashTrain.cpp.o
del dashLoop.cpp.o
del base2d.cpp.o
del polygon.cpp.o
del main.cpp.o
del filelist.txt
del test.exe

%mxi_dir%\mxi %Current_dir%\buildDemo.mx
copy test.exe %Current_dir%\lib
cd ..
cmd

