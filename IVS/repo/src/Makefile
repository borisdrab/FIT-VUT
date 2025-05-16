# Definovanie premenných
DOTNET = dotnet
SRC_LIB = MathLib/MathLib.csproj
TEST_PROJECT = MathLib.Tests/MathLib.Tests.csproj
APP_PROJECT = CalcApp/CalcApp.csproj
STDDEV_PROJECT = StdDevCalculator/StdDevCalculator.csproj

# Ciele Makefile
.PHONY: all pack clean run doc stddev

all: run

# Zbalenie projektu
pack: clean
	@cmd /C "if not exist ..\\package mkdir ..\\package"
	@cmd /C "xcopy /E /I /Y . ..\\package\\"
	@cmd /C "copy /Y ..\\Doxyfile ..\\package\\"
	@cmd /C "copy /Y ..\\dokumentace.pdf ..\\package\\"
	@cmd /C "powershell -Command \"Compress-Archive -Path ..\\package\\* -DestinationPath ..\\project_package.zip\""
	@cmd /C "rmdir /s /q ..\\package"

# Čistenie build výstupov
clean:
	@cmd /C "if exist MathLib\\obj rmdir /s /q MathLib\\obj"
	@cmd /C "if exist MathLib\\bin rmdir /s /q MathLib\\bin"
	@cmd /C "if exist CalcApp\\obj rmdir /s /q CalcApp\\obj"
	@cmd /C "if exist CalcApp\\bin rmdir /s /q CalcApp\\bin"
	@cmd /C "if exist StdDevCalculator\\obj rmdir /s /q StdDevCalculator\\obj"
	@cmd /C "if exist StdDevCalculator\\bin rmdir /s /q StdDevCalculator\\bin"
	@cmd /C "if exist MathLib.Tests\\obj rmdir /s /q MathLib.Tests\\obj"
	@cmd /C "if exist MathLib.Tests\\bin rmdir /s /q MathLib.Tests\\bin"

# Spustenie hlavného GUI programu (CalcApp)
run:
	$(DOTNET) run --project $(APP_PROJECT) -c Release

# Generovanie dokumentácie
doc:
	doxygen ./Doxyfile

help:
	@echo Makefile pre projekt
	@echo Moznosti:
	@echo   all            - Spusti hlavny GUI program (CalcApp)
	@echo   pack           - Zbali projekt do zip archivu
	@echo   clean          - Vycisti build vystupy
	@echo   run            - Spusti hlavny GUI program (CalcApp)
	@echo   doc            - Vygeneruje dokumentaciu pomocou Doxygen
	@echo   stddev         - Vypocita vyberovou smerodatnou odchylku

# Výpočty výběrové směrodatné odchylky
stddev:
	$(DOTNET) run --project $(STDDEV_PROJECT) -c Release

stddev_benchmark: stddev
	dotnet run --project StdDevCalculator --configuration Release --benchmark