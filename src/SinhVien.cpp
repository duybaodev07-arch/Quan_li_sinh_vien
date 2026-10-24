#include "sinhVien.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <tuple>

namespace {
std::string khoaThuong(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}
}

SinhVien::SinhVien(std::string ma, std::string hoSV, std::string tenSV, std::string gioiTinh, std::string dienThoai)
    : maSV(std::move(ma)), ho(std::move(hoSV)), ten(std::move(tenSV)), phai(std::move(gioiTinh)), soDT(std::move(dienThoai)) {}
const std::string& SinhVien::getMaSV() const { return maSV; }
const std::string& SinhVien::getHo() const { return ho; }
const std::string& SinhVien::getTen() const { return ten; }
const std::string& SinhVien::getPhai() const { return phai; }
const std::string& SinhVien::getSoDT() const { return soDT; }
void SinhVien::setMaSV(const std::string& ma) { maSV = ma; }
void SinhVien::setHo(const std::string& hoSV) { ho = hoSV; }
void SinhVien::setTen(const std::string& tenSV) { ten = tenSV; }
void SinhVien::setPhai(const std::string& gioiTinh) { phai = gioiTinh; }
void SinhVien::setSoDT(const std::string& dienThoai) { soDT = dienThoai; }

NodeSV::NodeSV(const SinhVien& sinhVien) : sv(sinhVien) {}
const SinhVien& NodeSV::getSinhVien() const { return sv; }
NodeSV* NodeSV::getNext() const { return next; }
void NodeSV::setNext(NodeSV* node) { next = node; }
PTRSV timSinhVien(PTRSV head, const std::string& maSV) {
    for (PTRSV p = head; p; p = p->getNext()) if (p->getSinhVien().getMaSV() == maSV) return p;
    return nullptr;
}
bool daTonTaiSinhVien(PTRSV head, const std::string& maSV) { return timSinhVien(head, maSV) != nullptr; }
bool sinhVienDungTruoc(const SinhVien& a, const SinhVien& b) {
    const auto ka = std::make_tuple(khoaThuong(a.getTen()), khoaThuong(a.getHo()), a.getMaSV());
    const auto kb = std::make_tuple(khoaThuong(b.getTen()), khoaThuong(b.getHo()), b.getMaSV());
    return ka < kb;
}
void themSinhVienVaoDSSV(PTRSV& head, PTRSV p) {
    if (!p) return;
    PTRSV truoc = nullptr;
    PTRSV hienTai = head;
    while (hienTai && !sinhVienDungTruoc(p->getSinhVien(), hienTai->getSinhVien())) {
        truoc = hienTai;
        hienTai = hienTai->getNext();
    }
    p->setNext(hienTai);
    if (truoc) truoc->setNext(p); else head = p;
}
bool xoaSinhVien(PTRSV& head, const std::string& maSV) {
    PTRSV truoc = nullptr;
    PTRSV p = head;
    while (p && p->getSinhVien().getMaSV() != maSV) { truoc = p; p = p->getNext(); }
    if (!p) return false;
    if (truoc) truoc->setNext(p->getNext()); else head = p->getNext();
    delete p;
    return true;
}
int demSoSinhVien(PTRSV head) { int n = 0; for (; head; head = head->getNext()) ++n; return n; }
void inSinhVien(const SinhVien& sv) {
    std::cout << std::left << std::setw(17) << sv.getMaSV() << std::setw(27) << sv.getHo()
              << std::setw(17) << sv.getTen() << std::setw(12) << sv.getPhai() << sv.getSoDT() << '\n';
}
void inDanhSachSinhVien(PTRSV head) {
    std::cout << std::left << std::setw(17) << "MA SV" << std::setw(27) << "HO" << std::setw(17)
              << "TEN" << std::setw(12) << "PHAI" << "SO DT\n";
    for (PTRSV p = head; p; p = p->getNext()) inSinhVien(p->getSinhVien());
}
std::vector<SinhVien> taoMangSinhVien(PTRSV head) {
    std::vector<SinhVien> ds;
    for (PTRSV p = head; p; p = p->getNext()) ds.push_back(p->getSinhVien());
    return ds;
}
void inDanhSachSinhVienTheoMa(PTRSV head) {
    auto ds = taoMangSinhVien(head);
    std::sort(ds.begin(), ds.end(), [](const SinhVien& a, const SinhVien& b) { return a.getMaSV() < b.getMaSV(); });
    std::cout << std::left << std::setw(17) << "MA SV" << std::setw(27) << "HO" << std::setw(17) << "TEN" << "PHAI\n";
    for (const auto& sv : ds) std::cout << std::left << std::setw(17) << sv.getMaSV() << std::setw(27)
        << sv.getHo() << std::setw(17) << sv.getTen() << sv.getPhai() << '\n';
}
void xoaTatCaSinhVien(PTRSV& head) { while (head) { PTRSV p = head; head = head->getNext(); delete p; } }
