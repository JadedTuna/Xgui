SOURCES=$(shell find ./src/ -name "*.cpp")
TARGET=xgui_textview
SOURCES+=tests/$(TARGET).cpp
OBJECTS=$(SOURCES:%.cpp=%.o)

DEFINES=-D_XGUI_USE_XDBE
CPPFLAGS=`pkg-config --cflags x11` `pkg-config --cflags xft` `pkg-config --cflags xrender` `pkg-config --cflags xext` -std=c++0x -Isrc -Wall -Wextra -g
CPPFLAGS+=$(DEFINES)
LIBS=`pkg-config --libs x11` `pkg-config --libs xft` `pkg-config --libs xrender` `pkg-config --libs xext` `pkg-config --libs freetype2`
CXX=g++

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK.cpp) $^ $(CPPFLAGS) $(LIBS) -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)