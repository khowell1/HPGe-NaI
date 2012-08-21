



FLAGS  = -O3 -Wall -Wno-sign-compare
INCS = -I/usr/include $(shell root-config --cflags)
LIBS = $(shell root-config --glibs)
OBJ= $(SRC:%.cc=%.o)
SRC= Master.cc RPhotonSource.cc main.cc Photon.cc RLinearInterpolant.cc Cylinder.cc
EXE=main.ex

all: $(EXE)

$(EXE): $(OBJ) $(SRC) 
	g++ $(OBJ) -o $@ $(FLAGS) $(INCS) $(LIBS)

$(OBJ): %.o: %.cc Makefile
	g++ $(FLAGS) -c $(INCS) -o $@ $<

.PHONY : clean
clean: 
	rm -f $(OBJ) $(EXE) 
