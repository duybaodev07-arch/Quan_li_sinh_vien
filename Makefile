CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude

rwildcard = $(foreach dir,$(wildcard $(1)/*),$(call rwildcard,$(dir),$(2)) $(filter $(subst *,%,$(2)),$(dir)))
SOURCES := $(call rwildcard,src,*.cpp)
HEADERS := $(call rwildcard,include,*.h)

ifeq ($(OS),Windows_NT)
TARGET := QuanLyDiemSV.exe
RUN := ./$(TARGET)
RM := del /Q /F
else
TARGET := QuanLyDiemSV
RUN := ./$(TARGET)
RM := rm -f
endif

.PHONY: all run clean
all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	$(RUN)

clean:
	$(RM) $(TARGET)