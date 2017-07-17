CPP_COMPILER = g++
CPP_FLAGS = -Wall -std=c++11

BUILD_DIR = ./bin

SIMH = ./sim/headers
SIMC = ./sim/src

all: dif exi grd ph2 det

dif: $(BUILD_DIR)/dif.exe 
exi: $(BUILD_DIR)/exiAlpha.exe
det: $(BUILD_DIR)/detGauss.exe
grd: $(BUILD_DIR)/grd.exe
ph2: $(BUILD_DIR)/ph2.exe

$(BUILD_DIR)/dif.exe : ./dif/headers/*.hpp ./dif/src/*.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I ./dif/headers/ -I $(SIMH) ./dif/src/*.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/dif.exe

$(BUILD_DIR)/exiAlpha.exe: ./exi/exiAlpha.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./exi/exiAlpha.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/exiAlpha.exe

$(BUILD_DIR)/grd.exe: ./grd/grd.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./grd/grd.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/grd.exe

$(BUILD_DIR)/ph2.exe: ./ph2/headers/*.hpp ./ph2/src/*.cpp $(SIMH)/*.hpp $(SIMC)/*cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I ./ph2/headers -I $(SIMH) ./ph2/src/*.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/ph2.exe

$(BUILD_DIR)/detGauss.exe: ./det/detGauss.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./det/detGauss.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/detGauss.exe