#pragma once

#include <string>
#include <vector>

class MonHoc {
private:
    std::string maMH;
    std::string tenMH;
    unsigned soTCLT = 0;
    unsigned soTCTH = 0;
public:
    MonHoc() = default;
    MonHoc(std::string ma, std::string ten, unsigned tclt, unsigned tcth);
    const std::string& getMaMH() const;
    const std::string& getTenMH() const;
    unsigned getSoTCLT() const;
    unsigned getSoTCTH() const;
    void setMaMH(const std::string& maMH);
    void setTenMH(const std::string& tenMH);
    void setSoTCLT(unsigned soTCLT);
    void setSoTCTH(unsigned soTCTH);
};

bool themMonHoc(std::vector<MonHoc>& dsMonHoc, const MonHoc& mh);
bool xoaMonHoc(std::vector<MonHoc>& dsMonHoc, const std::string& maMH);
MonHoc* timMonHocTheoMa(std::vector<MonHoc>& dsMonHoc, const std::string& maMH);
const MonHoc* timMonHocTheoMa(const std::vector<MonHoc>& dsMonHoc, const std::string& maMH);
void inMonHoc(const MonHoc& mh);
void inDanhSachMonHocTheoTen(const std::vector<MonHoc>& dsMonHoc);
