CXXFLAGS = -std=c++17 -O2 
LIBS = -lncursesw

all: gra gra_cytaty

clean:
	-rm -f gra gra_cytaty

gra: 
	g++ $(CXXFLAGS) gra.cpp -o gra $(LIBS) 

gra_cytaty: 
	g++ $(CXXFLAGS) gra_cytaty.cpp -o gra_cytaty $(LIBS) 

.PHONY: all clean