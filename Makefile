CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude
SOURCES := src/main.cpp src/storage/DocGhiDuLieu.cpp src/ui/GiaoDienConsole.cpp src/ui/Menu.cpp src/features/QuanLyLopSinhVien.cpp src/features/QuanLyMonHocLopTinChi.cpp src/features/QuanLyDiemBaoCao.cpp
ifeq ($(OS),Windows_NT)
TARGET := QuanLyDiemSV.exe
RM := del /Q /F
RUN := $(TARGET)
else
TARGET := QuanLyDiemSV
RM := rm -f
RUN := ./$(TARGET)
endif
.PHONY: all run clean
all: $(TARGET)
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)
run: $(TARGET)
	$(RUN)
clean:
	$(RM) $(TARGET)

