rem copy exe, dll, pdb to run and copy lib to lib
rem copy.bat "$(ProjectDir)" "$(Configuration)" "$(ProjectName)" "$(SolutionDir)"

title copy binary 

@echo OFF

rem there is no space setting variable
set projectdir=%1
set projectdir=%projectdir:~0,-1%
set configuration=%2
set configuration=%configuration:~1,-1%
set projectname=%3
set projectname=%projectname:~1,-1%

set solutiondir=%4
set solutiondir=%solutiondir:~0,-1%

@echo ON
if exist %projectdir%%configuration%\%projectname%.dll" (
	@echo copy %projectdir%%configuration%\%projectname%.dll" to %solutiondir%run"
	xcopy %projectdir%%configuration%\%projectname%.dll" %solutiondir%run"
)

if exist %projectdir%%configuration%\%projectname%.exe" (
	@echo copy %projectdir%%configuration%\%projectname%.exe" to %solutiondir%run"
	xcopy %projectdir%%configuration%\%projectname%.exe" %solutiondir%run"
)

if exist %projectdir%%configuration%\%projectname%.pdb" (
	@echo copy %projectdir%%configuration%\%projectname%.pdb" to %solutiondir%run"
	xcopy %projectdir%%configuration%\%projectname%.pdb" %solutiondir%run"
)

if exist %projectdir%%configuration%\%projectname%.lib" (
	@echo copy %projectdir%%configuration%\%projectname%.lib" to %solutiondir%lib"
	xcopy %projectdir%%configuration%\%projectname%.lib" %solutiondir%lib"
)

exit 0