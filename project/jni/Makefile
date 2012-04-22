TARGET   = gears2-sdl
CLASSES  = gears_renderer gear
SOURCE   = $(TARGET).c $(CLASSES:%=%.c)
OBJECTS  = $(TARGET).o $(CLASSES:%=%.o)
HFILES   = $(CLASSES:%=%.h)
OPT      = -O2 -Wall
CFLAGS   = $(OPT) -I. `sdl-config --cflags` -DA3D_GL2
LDFLAGS  = -lGLEW -lGL -lGLU -lm -L/usr/lib `sdl-config --libs` -La3d -la3d
CCC      = gcc

all: $(TARGET)

$(TARGET): $(OBJECTS) a3d
	$(CCC) $(OPT) $(OBJECTS) -o $@ $(LDFLAGS)

.PHONY: a3d

a3d:
	$(MAKE) -C a3d

clean:
	rm -f $(OBJECTS) *~ \#*\# $(TARGET)
	$(MAKE) -C a3d clean

$(OBJECTS): $(HFILES)
