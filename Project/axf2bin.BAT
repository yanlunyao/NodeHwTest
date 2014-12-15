@echo off

md ..\Release

C:\Keil\ARM\BIN40\fromelf.exe --bin -o ./OUTPUT/Program.bin ./OUTPUT/Program.axf

echo copy necessary files to release folder
copy .\OUTPUT\Program.bin ..\Release\NodeHwTest" "V01-C20130705-PCB.bin
copy .\OUTPUT\NodeHwTest.hex ..\Release\NodeHwTest" "V01-C20130705-PCB.hex




