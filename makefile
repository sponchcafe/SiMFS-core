CPP_COMPILER = g++
CPP_FLAGS = -Wall -Wextra -std=c++11 -O3 -march=native

BUILD_DIR = ./bin

SIMH = ./sim/headers
SIMC = ./sim/src

all: dif exi grd ph2 det mix sum bkg

dif: $(BUILD_DIR)/dif
exi: $(BUILD_DIR)/exiAlpha
detGauss: $(BUILD_DIR)/detGauss
detAlpha: $(BUILD_DIR)/detAlpha
shift: $(BUILD_DIR)/shift
grd: $(BUILD_DIR)/grd
ph2: $(BUILD_DIR)/ph2
mix: $(BUILD_DIR)/mix
sum: $(BUILD_DIR)/sum
bkg: $(BUILD_DIR)/bkg
pass: $(BUILD_DIR)/pass
linsp: $(BUILD_DIR)/linsp

$(BUILD_DIR)/dif: ./dif/headers/*.hpp ./dif/src/*.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I ./dif/headers/ -I $(SIMH) ./dif/src/*.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_dif

$(BUILD_DIR)/exiAlpha: ./exi/exiAlpha.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./exi/exiAlpha.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_exiAlpha

$(BUILD_DIR)/ph2: ./ph2/headers/*.hpp ./ph2/src/*.cpp $(SIMH)/*.hpp $(SIMC)/*cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I ./ph2/headers -I $(SIMH) ./ph2/src/*.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_ph2

$(BUILD_DIR)/detGauss: ./det/detGauss.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./det/detGauss.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_detGauss

$(BUILD_DIR)/detAlpha: ./det/detAlpha.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./det/detAlpha.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_detAlpha
		
$(BUILD_DIR)/mix: ./mix/mix.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./mix/mix.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_mix

$(BUILD_DIR)/sum: ./sum/sum.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./sum/sum.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_sum

$(BUILD_DIR)/bkg: ./bkg/bkg.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./bkg/bkg.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_bkg



$(BUILD_DIR)/grd: ./utl/grd.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./utl/grd.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_grd

$(BUILD_DIR)/shift: ./utl/shift.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./utl/shift.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_shift

$(BUILD_DIR)/pass: ./utl/pass.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./utl/pass.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_pass

$(BUILD_DIR)/linsp: ./utl/linspace.cpp $(SIMH)/*.hpp $(SIMC)/*.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -I $(SIMH) ./utl/linspace.cpp $(SIMC)/*.cpp -o $(BUILD_DIR)/sim_linsp
