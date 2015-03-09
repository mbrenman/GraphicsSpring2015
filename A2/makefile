BCC = g++
DLIBS = -L/h/mshah08/Desktop/comp175/lib -lGL -lGLU -lglut -lglui
#SLIBS = /h/mshah08/Desktop/comp175/lib/libGLEW.a
INC = -I./ -I/h/mshah08/Desktop/comp175/include
EXE = Assignment2

$(EXE) : *.cpp
	$(BCC) $(INC) $< -o $@ $(DLIBS) $(SLIBS)

clean:
	rm -rf *.o *~ $(EXE)
