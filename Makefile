
OFILES = base/Matrix.o \
	 base/Volume.o \
	 base/LinearAlgebra.o \
	 base/Camera.o \
	 base/Color.o \
	 base/ImgProc.o \
	 base/Raymarcher.o \
	 base/StarterViewer.o
	 
AFILES = $(OFILES)

ROOTDIR = .
LIB = -L$(ROOTDIR)/lib -lstarter -lm 
GLLDFLAGS = -lglut -lGL -lm -lGLU -lOpenImageIO

CXX = g++ -g -O1 -fPIC -fopenmp -fopenmp -std=c++17

SWIGCXX = g++ -shared -g -O2 -fPIC -fopenmp -fopenmp -std=c++14

PYTHONINCLUDE = -I/usr/include/python3.8

SWIGEXEC = swig4.0

INCLUDES = -I ./include/ $(PYTHONINCLUDE) -I /usr/local/include -I /usr/include

test: $(AFILES) 
	ar rv ./lib/libstarter.a $?
	$(CXX) base/main.C $(INCLUDES) $(LIB) -o bin/test

base: $(AFILES) 
	ar rv ./lib/libstarter.a $?
	$(CXX) base/viewer.C $(INCLUDES) $(LIB) $(GLLDFLAGS) -o bin/viewer

.C.o: $<
	$(CXX) -c $(INCLUDES) $< -o $@

clean:
	rm -rf *.o bin/viewer bin/test base/*.o core ./lib/libstarter.a  *~ swig/*~ swig/*.so swig/*.o swig/*.cxx swig/*.pyc swig/bishop.py* doc/html doc/latex python/*.pyc 

genswig:	swig/bishop.i	$(OFILES)
	$(SWIGEXEC) -c++ -python -shadow -I./include/ swig/bishop.i
	$(SWIGCXX) -c swig/bishop_wrap.cxx  $(INCLUDES) -o swig/bishop_wrap.o
	$(SWIGCXX) swig/bishop_wrap.o $(LIB) -o swig/_bishop.so
