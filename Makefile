CXX=clang++
CXXFLAGS=-W -Wall -Wno-long-long -pedantic -pipe -g -O0 -D_DEBUG -D_GNU_SOURCE=1 -D_REENTRANT
#CXXFLAGS=-W -Wall -Wno-long-long -pedantic -pipe -O2 -D_GNU_SOURCE=1 -D_REENTRANT
INCLUDE = -I/usr/include/SDL -I/usr/include/python2.7

LD=clang++
LDFLAGS=
#LDFLAGS=-Wl,-O1
LIBS = -lSDL -lSDL_gfx -lSDL_ttf -ldl -lutil -lm -lpython2.7 -ltinyxml

RM = rm -f

############################

cxxsrc = config.cpp \
	item.cpp \
	living.cpp \
	movable.cpp \
	playable.cpp \
	memory.cpp \
	mineral.cpp \
	base.cpp \
	miningship.cpp \
	fighter.cpp \
	missile.cpp \
	item_manager.cpp \
	stat_manager.cpp \
	game_manager.cpp \
	handler_dummy.cpp \
	handler_example.cpp \
	python_wrapper.cpp \
	python_handler.cpp \
	renderer_dummy.cpp \
	renderer_sdl.cpp \
	renderer_sdl_draw.cpp \
	renderer_sdl_console.cpp \
	renderer_summary.cpp \
	main.cpp

objects = $(cxxsrc:.cpp=.o)
deps = $(cxxsrc:.cpp=.d)
target = AIWar

############################

all: $(target)

module:
	python setup.py build

$(target): $(objects)
	$(LD) -o $@ $(LDFLAGS) $^ $(LIBS)

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) -MMD -MF $*.d $(INCLUDE) $<

.PHONY: clean

clean:
	$(RM) $(deps)
	$(RM) $(objects)

distclean: clean
	$(RM) *~
	$(RM) $(target)
	python setup.py clean

############################

-include $(deps)