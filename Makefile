#
#  Makefile for fltk applications
#

# set local paths
FLTK =
GLEW =

INCLUDE = -I$(FLTK) -I$(FLTK)/png -I$(FLTK)/jpeg -I$(GLEW)/include
LIBDIR = -L$(FLTK)/lib

CFLAGS = -O2 -g -fpermissive -std=c++11 -DANIMATOR
LIBS = -lfltk -lfltk_gl -lm -lfltk_png -lfltk_jpeg -ldl -lpthread $(GLEW)/lib/libGLEW.a

ifeq ($(PLAT),OSX)
	CFLAGS += -Dcimg_OS=0 -Dcimg_display_type=0 
	LIBS += -lz -framework OpenGL -framework AGL -framework Cocoa
else
	LIBS += -lfltk_z -lX11 -lXext -lXfixes -lGL -lGLU
endif

CC = g++

.SUFFIXES: .o .cpp .cxx

.cpp.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

.cxx.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

ALL.O = curve.o curveevaluator.o beziercurveevaluator.o catmullromcurveevaluator.o \
	bsplinecurveevaluator.o linearcurveevaluator.o camera.o graphwidget.o indicatorwindow.o \
	modelerdraw.o modelerui.o modelerview.o imageio.o point.o rect.o vault.o vect.o cameras.o \
	model.o particleSystem.o properties.o \
	rijndael.o shaders.o trackball.o sample.o \
	particle.o force.o cloth.o clothParticle.o constraint.o

modeler: $(ALL.O)
	$(CC) $(CFLAGS) -o $@ $(ALL.O) $(INCLUDE) $(LIBDIR) $(LIBS)

clean:
	rm -f $(ALL.O) modeler
