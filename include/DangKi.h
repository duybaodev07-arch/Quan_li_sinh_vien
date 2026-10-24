#pragma once

#include <string>

class DangKy {
private:
    std::string maSV;
    double diem = -1.0;
    bool huyDangKy = false;
public:
    DangKy() = default;
    explicit DangKy(std::string ma, double diemThi = -1.0, bool daHuy = false);
    const std::string& getMaSV() const;
    double getDiem() const;
    bool getHuyDangKy() const;
    void setDiem(double diemThi);
    void setHuyDangKy(bool daHuy);
};
