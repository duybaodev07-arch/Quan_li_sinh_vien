#pragma once

#include <string>

std::string docChuoi(const std::string& loiNhac, bool choPhepRong = false);
std::string docChuoiGioiHan(const std::string& loiNhac, std::size_t doDaiToiDa);
std::string chuanHoaKhoangTrang(const std::string& chuoi);
bool nhapMa(std::string& ma, int doDaiToiDa, const std::string& loiNhac, bool choPhepRong = false);
bool nhapSoNguyen(int& giaTri, int min, int max, const std::string& loiNhac);
bool nhapSoThuc(double& giaTri, double min, double max, const std::string& loiNhac);
bool xacNhan(const std::string& loiNhac);
void dungManHinh();
