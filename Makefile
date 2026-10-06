
OFILES = base/Matrix.o \
	 base/Volume.o \
	 base/ImplicitFields.o\
	 base/BinaryOperators.o\
	 base/UnaryOperators.o\
	 base/FieldInterface.o\
	 base/VolumeGrid.o \
	 base/Mesh.o \
	 base/Light.o \
	 base/LinearAlgebra.o \
	 base/Camera.o \
	 base/Color.o \
	 base/ImgProc.o \
	 base/Raymarcher.o \
	 base/StarterViewer.o
	 
AFILES = $(OFILES)

ROOTDIR = .
LIB = -L$(ROOTDIR)/lib -lstarter -lm
GLLDFLAGS = -L /opt/homebrew/lib -lglut -lGL -lm -lGLU -lopenvdb -lOpenImageIO -lOpenImageIO_Util -ltbb -lz

CXX = clang++ -g -fPIC -fopenmp -fopenmp -std=c++17

SWIGCXX = g++ -shared -g -O2 -fPIC -fopenmp -fopenmp -std=c++14

PYTHONINCLUDE = -I/usr/include/python3.8

SWIGEXEC = swig4.0

INCLUDES = -I /opt/homebrew/include -I ./include/ $(PYTHONINCLUDE) -I /usr/local/include -I /usr/include -I ./ext/include

test: $(AFILES) 
	ar rv ./lib/libstarter.a $?
	$(CXX) base/gridTest.C $(INCLUDES) $(LIB) $(GLLDFLAGS) -o bin/gridTest

base: $(AFILES) 
	ar rv ./lib/libstarter.a $?
	$(CXX) base/viewer.C $(INCLUDES) $(LIB) $(GLLDFLAGS) -o bin/viewer

.C.o: $<
	$(CXX) -c $(INCLUDES) $< -o $@

clean:
	rm -rf *.o bin/viewer bin/gridTest bin/*.dSYM base/*.o core ./lib/libstarter.a  *~ swig/*~ swig/*.so swig/*.o swig/*.cxx swig/*.pyc swig/bishop.py* doc/html doc/latex python/*.pyc 

genswig:	swig/bishop.i	$(OFILES)
	$(SWIGEXEC) -c++ -python -shadow -I./include/ swig/bishop.i
	$(SWIGCXX) -c swig/bishop_wrap.cxx  $(INCLUDES) -o swig/bishop_wrap.o
	$(SWIGCXX) swig/bishop_wrap.o $(LIB) -o swig/_bishop.so
