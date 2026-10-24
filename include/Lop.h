#pragma once

#include "sinhVien.h"
#include <string>
#include <vector>

class Lop {
private:
    std::string maLop;
    std::string tenLop;
    PTRSV dssv = nullptr;
public:
    Lop() = default;
    Lop(std::string ma, std::string ten);
    Lop(const Lop& other);
    Lop& operator=(const Lop& other);
    Lop(Lop&& other) noexcept;
    Lop& operator=(Lop&& other) noexcept;
    ~Lop();
    const std::string& getMaLop() const;
    const std::string& getTenLop() const;
    PTRSV getDSSV() const;
    void setTenLop(const std::string& ten);
    bool themSinhVien(const SinhVien& sv);
    bool capNhatSinhVien(const std::string& maSV, const SinhVien& svMoi);
    bool xoaSinhVien(const std::string& maSV);
};

class DanhSachLop {
private:
    std::vector<Lop> dsLop;
public:
    std::vector<Lop>& getDanhSach();
    const std::vector<Lop>& getDanhSach() const;
    Lop* timLopTheoMa(const std::string& maLop);
    const Lop* timLopTheoMa(const std::string& maLop) const;
    bool themLop(const Lop& lop);
    bool xoaLop(const std::string& maLop);
};

void inLop(const Lop& lop);
void inDanhSachLop(const DanhSachLop& dsLop);
