@echo off
cd premake
rm *.vcxproj*
cd ..
rm *.sdf
rm *.opensdf
rm *.sln
rm *.suo
rmdir /S /Q obj
rmdir /S /Q bin
pause