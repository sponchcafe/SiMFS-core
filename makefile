CPP_COMPILER = g++
CPP_FLAGS = -Wall -std=c++11

BUILD_DIR = ./bin

SIMH = ./sim/headers
SIMC = ./sim/src

all: dif exi grd ph2 det mix sum bkg

dif: $(BUILD_DIR)/dif
exi: $(BUILD_DIR)/exiAlpha
det: $(BUILD_DIR)/detGauss
grd: $(BUILD_DIR)/grd
ph2: $(BUILD_DIR)/ph2
mix: $(BUILD_DIR)/mix
sum: $(BUILD_DIR)/sum
bkg: $(BUILD_DIR)/bkg

$(BUILD_DIR)/dif: ./dif/headers/*.hpp ./dif/src/*.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I ./dif/headers/ -I $(SIMH) ./dif/src/*.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_dif

$(BUILD_DIR)/exiAlpha: ./exi/exiAlpha.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./exi/exiAlpha.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_exiAlpha

$(BUILD_DIR)/grd: ./grd/grd.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./grd/grd.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_grd

$(BUILD_DIR)/ph2: ./ph2/headers/*.hpp ./ph2/src/*.cpp $(SIMH)/*.hpp $(SIMC)/*cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I ./ph2/headers -I $(SIMH) ./ph2/src/*.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_ph2

$(BUILD_DIR)/detGauss: ./det/detGauss.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./det/detGauss.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_detGauss
	
$(BUILD_DIR)/mix: ./mix/mix.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./mix/mix.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_mix

$(BUILD_DIR)/sum: ./sum/sum.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./sum/sum.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_sum

$(BUILD_DIR)/bkg: ./bkg/bkg.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./bkg/bkg.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_bkg