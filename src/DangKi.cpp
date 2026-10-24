#include "dangKi.h"
#include "const.h"
#include <utility>

DangKy::DangKy(std::string ma, double diemThi, bool daHuy)
    : maSV(std::move(ma)), diem(diemThi), huyDangKy(daHuy) {}
const std::string& DangKy::getMaSV() const { return maSV; }
double DangKy::getDiem() const { return diem; }
bool DangKy::getHuyDangKy() const { return huyDangKy; }
void DangKy::setDiem(double diemThi) { if (diemThi >= MIN_DIEM && diemThi <= MAX_DIEM) diem = diemThi; }
void DangKy::setHuyDangKy(bool daHuy) { huyDangKy = daHuy; }
