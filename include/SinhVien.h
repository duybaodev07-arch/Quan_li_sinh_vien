#pragma once

#include <string>
#include <vector>

class SinhVien {
private:
    std::string maSV;
    std::string ho;
    std::string ten;
    std::string phai;
    std::string soDT;
public:
    SinhVien() = default;
    SinhVien(std::string ma, std::string hoSV, std::string tenSV, std::string gioiTinh, std::string dienThoai);
    const std::string& getMaSV() const;
    const std::string& getHo() const;
    const std::string& getTen() const;
    const std::string& getPhai() const;
    const std::string& getSoDT() const;
    void setMaSV(const std::string& ma);
    void setHo(const std::string& hoSV);
    void setTen(const std::string& tenSV);
    void setPhai(const std::string& gioiTinh);
    void setSoDT(const std::string& dienThoai);
};

class NodeSV {
private:
    SinhVien sv;
    NodeSV* next = nullptr;
public:
    explicit NodeSV(const SinhVien& sinhVien);
    const SinhVien& getSinhVien() const;
    NodeSV* getNext() const;
    void setNext(NodeSV* node);
};
using PTRSV = NodeSV*;

PTRSV timSinhVien(PTRSV head, const std::string& maSV);
bool daTonTaiSinhVien(PTRSV head, const std::string& maSV);
bool sinhVienDungTruoc(const SinhVien& a, const SinhVien& b);
void themSinhVienVaoDSSV(PTRSV& head, PTRSV p);
bool xoaSinhVien(PTRSV& head, const std::string& maSV);
int demSoSinhVien(PTRSV head);
void inSinhVien(const SinhVien& sv);
void inDanhSachSinhVien(PTRSV head);
void inDanhSachSinhVienTheoMa(PTRSV head);
std::vector<SinhVien> taoMangSinhVien(PTRSV head);
void xoaTatCaSinhVien(PTRSV& head);
