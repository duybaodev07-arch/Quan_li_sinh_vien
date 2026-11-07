CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude

rwildcard = $(foreach dir,$(wildcard $(1)/*),$(call rwildcard,$(dir),$(2)) $(filter $(subst *,%,$(2)),$(dir)))
SOURCES := $(call rwildcard,src,*.cpp)
HEADERS := $(call rwildcard,include,*.h)

TARGET := QuanLyDiemSV.exe
PLATFORM_SOURCE := src/ui/GiaoDienConsole_NenTang.cpp
RUN := ./$(TARGET)
RM := del /Q /F
SOURCES := $(filter-out src/ui/GiaoDienConsole_NenTang.cpp,$(SOURCES)) $(PLATFORM_SOURCE)

.PHONY: all run clean
all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	$(RUN)

clean:
	$(RM) $(TARGET)
