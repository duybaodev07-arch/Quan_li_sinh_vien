#include "quanLy.h"
#include "const.h"
#include "nhapLieu.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <utility>

namespace {
std::string hoTen(const SinhVien& sv) { return sv.getHo() + " " + sv.getTen(); }
bool nhapSinhVienMoi(SinhVien& sv, const std::string& maSV) {
    const std::string ho = docChuoiGioiHan("Nhap ho: ", MAX_HO);
    if (ho.empty()) return false;
    const std::string ten = docChuoiGioiHan("Nhap ten: ", MAX_TEN);
    if (ten.empty()) return false;
    std::string phai;
    while (true) {
        phai = docChuoi("Nhap phai (Nam/Nu): ");
        if (phai.empty()) return false;
        std::string thuong = phai;
        for (char& c : thuong) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (thuong == "nam") { phai = "Nam"; break; }
        if (thuong == "nu" || thuong == "nữ") { phai = "Nữ"; break; }
        std::cout << "Chi chap nhan Nam hoac Nu.\n";
    }
    std::string soDT;
    while (true) {
        soDT = docChuoi("Nhap so dien thoai: ");
        if (soDT.empty()) return false;
        bool hopLe = !soDT.empty() && soDT.size() <= MAX_SDT;
        for (std::size_t i = 0; i < soDT.size() && hopLe; ++i)
            if (!(std::isdigit(static_cast<unsigned char>(soDT[i])) || (i == 0 && soDT[i] == '+'))) hopLe = false;
        if (hopLe && soDT != "+") break;
        std::cout << "So dien thoai chi gom chu so, co the co + o dau, toi da " << MAX_SDT << " ky tu.\n";
    }
    sv = SinhVien(maSV, ho, ten, phai, soDT);
    return true;
}
bool dangKyTonTai(const DanhSachLopTinChi& dsLopTC, const std::string& maSV) {
    for (const auto& lopTC : dsLopTC.getDanhSach()) if (lopTC.timDangKyTheoMaSV(maSV)) return true;
    return false;
}
}

HeThong::HeThong(std::string thuMuc) : thuMucDuLieu(std::move(thuMuc)) {}
bool HeThong::khoiDong() {
    if (!docDuLieu(thuMucDuLieu, dsMonHoc, dsLop, dsLopTC)) {
        std::cerr << "Du lieu khong doc duoc. Hay kiem tra cac file trong thu muc data.\n";
        return false;
    }
    return true;
}
void HeThong::luuNeuCan() {
    if (tuDongLuu && !ghiDuLieu(thuMucDuLieu, dsMonHoc, dsLop, dsLopTC))
        std::cerr << "Canh bao: khong luu duoc du lieu.\n";
}

void HeThong::chay() {
    while (true) {
        std::cout << "\n========== QUAN LY DIEM SINH VIEN THEO TIN CHI ==========\n"
            << "1. Quan ly mon hoc\n2. Quan ly lop va sinh vien\n3. Mo lop tin chi\n"
            << "4. In sinh vien dang ky lop tin chi\n5. Dang ky / huy dang ky lop tin chi\n"
            << "6. Tu dong huy lop khong du si so\n7. Nhap / hieu chinh diem\n"
            << "8. In bang diem mon hoc cua lop tin chi\n9. Thong ke diem trung binh theo lop\n"
            << "10. Bang diem tong ket theo lop\n11. Luu du lieu\n0. Luu va thoat\n";
        int chon;
        if (!nhapSoNguyen(chon, 0, 11, "Chon chuc nang: ")) return;
        switch (chon) {
        case 1: menuMonHoc(); break;
        case 2: menuLop(); break;
        case 3: menuLopTinChi(); break;
        case 4: {
            auto* lopTC = nhapThongTinTimLopTinChi();
            if (!lopTC) break;
            inDanhSachDangKy(*lopTC);
            break;
        }
        case 5: menuDangKy(); break;
        case 6: {
            const std::string khoa = docChuoi("Nhap nien khoa: "); if (khoa.empty()) return;
            int ky; if (!nhapSoNguyen(ky, 1, 20, "Nhap hoc ky: ")) return;
            std::vector<LopTinChi*> dsCanHuy;
            for (auto& lop : dsLopTC.getDanhSach()) if (lop.getNienKhoa() == khoa && lop.getHocKy() == ky && !lop.getHuyLop() && lop.demSoSVDangKy() < lop.getMinSV()) dsCanHuy.push_back(&lop);
            if (dsCanHuy.empty()) { std::cout << "Khong co lop nao du dieu kien huy.\n"; break; }
            for (const auto* lop : dsCanHuy) inLopTinChi(*lop);
            if (xacNhan("Xac nhan huy cac lop tren?")) { for (auto* lop : dsCanHuy) lop->setHuyLop(true); luuNeuCan(); std::cout << "Da huy " << dsCanHuy.size() << " lop.\n"; }
            break;
        }
        case 7: {
            auto* lopTC = nhapThongTinTimLopTinChi(); if (lopTC) { nhapDiemLopTinChi(*lopTC); luuNeuCan(); }
            break;
        }
        case 8: {
            auto* lopTC = nhapThongTinTimLopTinChi(); if (lopTC) inBangDiemLopTinChi(*lopTC);
            break;
        }
        case 9:
        case 10: {
            const std::string maLop = docChuoi("Nhap ma lop: "); if (maLop.empty()) return;
            auto* lop = dsLop.timLopTheoMa(maLop); if (!lop) std::cout << "Khong tim thay lop.\n";
            else if (chon == 9) inDiemTrungBinhTheoLop(*lop); else inBangDiemTongKetTheoLop(*lop);
            break;
        }
        case 11: if (ghiDuLieu(thuMucDuLieu, dsMonHoc, dsLop, dsLopTC)) std::cout << "Da luu du lieu.\n"; else std::cout << "Luu du lieu that bai.\n"; break;
        case 0: luuNeuCan(); std::cout << "Da luu. Tam biet.\n"; return;
        }
        dungManHinh();
    }
}

void HeThong::menuMonHoc() {
    while (true) {
        std::cout << "\n--- QUAN LY MON HOC ---\n1. Them\n2. Xoa\n3. Hieu chinh\n4. In theo ten\n0. Quay lai\n";
        int chon; if (!nhapSoNguyen(chon, 0, 4, "Chon: ")) return;
        if (chon == 0) return;
        if (chon == 4) { inDanhSachMonHocTheoTen(dsMonHoc); continue; }
        std::string ma; if (!nhapMa(ma, static_cast<int>(MAX_MAMH), "Nhap ma mon hoc: ")) return;
        if (chon == 2) {
            bool daMo = false; for (const auto& ltc : dsLopTC.getDanhSach()) if (ltc.getMaMH() == ma) daMo = true;
            if (daMo) std::cout << "Khong the xoa mon hoc da duoc mo lop tin chi.\n";
            else if (xoaMonHoc(dsMonHoc, ma)) { luuNeuCan(); std::cout << "Da xoa mon hoc.\n"; } else std::cout << "Khong tim thay mon hoc.\n";
            continue;
        }
        MonHoc* mh = timMonHocTheoMa(dsMonHoc, ma);
        if (chon == 3 && !mh) { std::cout << "Khong tim thay mon hoc.\n"; continue; }
        if (chon == 2) continue;
        const std::string ten = docChuoiGioiHan("Nhap ten mon hoc: ", MAX_TEN); if (ten.empty()) return;
        int tclt, tcth;
        if (!nhapSoNguyen(tclt, 0, static_cast<int>(MAX_SO_TCLT), "Nhap so tin chi ly thuyet (0-30): ") ||
            !nhapSoNguyen(tcth, 0, static_cast<int>(MAX_SO_TCTH), "Nhap so tin chi thuc hanh (0-30): ")) return;
        if (tclt + tcth == 0) { std::cout << "Tong so tin chi phai lon hon 0.\n"; continue; }
        if (chon == 1) {
            if (themMonHoc(dsMonHoc, MonHoc(ma, ten, static_cast<unsigned>(tclt), static_cast<unsigned>(tcth)))) { luuNeuCan(); std::cout << "Da them mon hoc.\n"; }
            else std::cout << "Ma mon hoc da ton tai.\n";
        } else {
            mh->setTenMH(ten); mh->setSoTCLT(static_cast<unsigned>(tclt)); mh->setSoTCTH(static_cast<unsigned>(tcth)); luuNeuCan(); std::cout << "Da cap nhat mon hoc.\n";
        }
    }
}

void HeThong::menuLop() {
    while (true) {
        std::cout << "\n--- QUAN LY LOP / SINH VIEN ---\n1. Them lop\n2. Xoa lop\n3. In lop\n4. Quan ly sinh vien trong lop\n0. Quay lai\n";
        int chon; if (!nhapSoNguyen(chon, 0, 4, "Chon: ")) return;
        if (chon == 0) return;
        if (chon == 3) { inDanhSachLop(dsLop); continue; }
        if (chon == 1) {
            std::string ma; if (!nhapMa(ma, static_cast<int>(MAX_MALOP), "Nhap ma lop: ")) return;
            const std::string ten = docChuoiGioiHan("Nhap ten lop: ", MAX_TEN); if (ten.empty()) return;
            if (dsLop.themLop(Lop(ma, ten))) { luuNeuCan(); std::cout << "Da them lop.\n"; } else std::cout << "Ma lop trung hoac da dat gioi han.\n";
            continue;
        }
        std::string maLop; if (!nhapMa(maLop, static_cast<int>(MAX_MALOP), "Nhap ma lop: ")) return;
        Lop* lop = dsLop.timLopTheoMa(maLop); if (!lop) { std::cout << "Khong tim thay lop.\n"; continue; }
        if (chon == 2) {
            if (demSoSinhVien(lop->getDSSV()) != 0) { std::cout << "Hay xoa sinh vien trong lop truoc.\n"; continue; }
            if (dsLop.xoaLop(maLop)) { luuNeuCan(); std::cout << "Da xoa lop.\n"; }
            continue;
        }
        while (true) {
            std::cout << "\nLop " << lop->getTenLop() << " (" << lop->getMaLop() << ")\n1. Nhap sinh vien moi\n2. Hieu chinh sinh vien\n3. Xoa sinh vien\n4. In theo ten\n5. In theo ma SV\n0. Quay lai\n";
            int thaoTac; if (!nhapSoNguyen(thaoTac, 0, 5, "Chon: ")) return;
            if (thaoTac == 0) break;
            if (thaoTac == 4) { inDanhSachSinhVien(lop->getDSSV()); continue; }
            if (thaoTac == 5) { inDanhSachSinhVienTheoMa(lop->getDSSV()); continue; }
            std::string maSV;
            if (!nhapMa(maSV, static_cast<int>(MAX_MASV), thaoTac == 1 ? "Nhap ma SV (Enter ket thuc): " : "Nhap ma SV: ", thaoTac == 1)) { if (thaoTac == 1) break; return; }
            if (thaoTac == 1) {
                if (dangKyTonTai(dsLopTC, maSV)) { std::cout << "Ma SV da co lich su dang ky; khong the tao lai.\n"; continue; }
                if (dsLop.timLopTheoMa(maLop) == nullptr) break;
                if (timSinhVien(lop->getDSSV(), maSV)) { std::cout << "Ma SV da ton tai trong lop nay.\n"; continue; }
                SinhVien sv; if (!nhapSinhVienMoi(sv, maSV)) return;
                bool trung = false;
                for (const auto& lopKhac : dsLop.getDanhSach()) if (lopKhac.getMaLop() != maLop && daTonTaiSinhVien(lopKhac.getDSSV(), maSV)) trung = true;
                if (trung) { std::cout << "Ma SV da duoc su dung trong lop khac.\n"; continue; }
                lop->themSinhVien(sv); luuNeuCan(); std::cout << "Da them sinh vien.\n";
            } else if (thaoTac == 2) {
                PTRSV node = timSinhVien(lop->getDSSV(), maSV); if (!node) { std::cout << "Khong tim thay sinh vien.\n"; continue; }
                SinhVien sv; if (!nhapSinhVienMoi(sv, maSV)) return;
                lop->capNhatSinhVien(maSV, sv); luuNeuCan(); std::cout << "Da cap nhat sinh vien.\n";
            } else {
                if (dangKyTonTai(dsLopTC, maSV)) { std::cout << "Khong the xoa sinh vien co lich su dang ky lop tin chi.\n"; continue; }
                if (lop->xoaSinhVien(maSV)) { luuNeuCan(); std::cout << "Da xoa sinh vien.\n"; } else std::cout << "Khong tim thay sinh vien.\n";
            }
        }
    }
}

void HeThong::menuLopTinChi() {
    while (true) {
        std::cout << "\n--- MO / QUAN LY LOP TIN CHI ---\n1. Mo lop tin chi\n2. Hieu chinh\n3. Xoa\n4. In danh sach\n0. Quay lai\n";
        int chon; if (!nhapSoNguyen(chon, 0, 4, "Chon: ")) return;
        if (chon == 0) return;
        if (chon == 4) { inDanhSachLopTinChi(dsLopTC); continue; }
        if (chon == 1) {
            std::string maMH; if (!nhapMa(maMH, static_cast<int>(MAX_MAMH), "Nhap ma mon hoc: ")) return;
            if (!timMonHocTheoMa(dsMonHoc, maMH)) { std::cout << "Mon hoc chua ton tai.\n"; continue; }
            const std::string khoa = docChuoiGioiHan("Nhap nien khoa (vi du 2025-2026): ", MAX_NIENKHOA); if (khoa.empty()) { std::cout << "Nien khoa khong hop le.\n"; continue; }
            int ky, nhom, minSV, maxSV;
            if (!nhapSoNguyen(ky, 1, 20, "Nhap hoc ky: ") || !nhapSoNguyen(nhom, 1, 1000, "Nhap nhom: ") ||
                !nhapSoNguyen(minSV, 1, 10000, "Nhap so SV toi thieu: ") || !nhapSoNguyen(maxSV, minSV, 10000, "Nhap so SV toi da: ")) return;
            const int maLopTC = dsLopTC.taoMaLopTiepTheo();
            LopTinChi lopTC(maLopTC, maMH, khoa, ky, nhom, minSV, maxSV);
            if (dsLopTC.themLopTinChi(lopTC)) { luuNeuCan(); std::cout << "Da mo lop tin chi. Ma lop tin chi: " << maLopTC << '\n'; }
            else std::cout << "Lop tin chi trung mon hoc/nien khoa/hoc ky/nhom.\n";
            continue;
        }
        int ma; if (!nhapSoNguyen(ma, 1, 2147483647, "Nhap ma lop tin chi: ")) return;
        LopTinChi* lopTC = dsLopTC.timLopTinChi(ma); if (!lopTC) { std::cout << "Khong tim thay lop tin chi.\n"; continue; }
        if (chon == 3) {
            if (!lopTC->getDSSVDK().empty()) { std::cout << "Khong the xoa lop da co lich su dang ky; hay dung trang thai huy lop.\n"; continue; }
            if (dsLopTC.xoaLopTinChi(ma)) { luuNeuCan(); std::cout << "Da xoa lop tin chi.\n"; }
        } else {
            if (!lopTC->getDSSVDK().empty()) { std::cout << "Khong sua ma mon hoc, nien khoa, hoc ky hoac nhom sau khi co dang ky.\n"; continue; }
            std::string maMH; if (!nhapMa(maMH, static_cast<int>(MAX_MAMH), "Nhap ma mon hoc moi: ")) return;
            if (!timMonHocTheoMa(dsMonHoc, maMH)) { std::cout << "Mon hoc khong ton tai.\n"; continue; }
            const std::string khoa = docChuoiGioiHan("Nhap nien khoa: ", MAX_NIENKHOA); if (khoa.empty()) continue;
            int ky, nhom, minSV, maxSV;
            if (!nhapSoNguyen(ky, 1, 20, "Nhap hoc ky: ") || !nhapSoNguyen(nhom, 1, 1000, "Nhap nhom: ") ||
                !nhapSoNguyen(minSV, 1, 10000, "Nhap so SV toi thieu: ") || !nhapSoNguyen(maxSV, minSV, 10000, "Nhap so SV toi da: ")) return;
            auto* trung = dsLopTC.timLopTinChiTheoMonHoc(maMH, khoa, ky, nhom);
            if (trung && trung != lopTC) { std::cout << "Thong tin moi bi trung voi lop tin chi khac.\n"; continue; }
            lopTC->setMaMH(maMH); lopTC->setNienKhoa(khoa); lopTC->setHocKy(ky); lopTC->setNhom(nhom); lopTC->setMinSV(minSV); lopTC->setMaxSV(maxSV);
            luuNeuCan(); std::cout << "Da hieu chinh lop tin chi.\n";
        }
    }
}

void HeThong::inDanhSachDangKy(const LopTinChi& lopTC) const {
    const MonHoc* mh = timMonHocTheoMa(dsMonHoc, lopTC.getMaMH());
    std::cout << "Lop TC " << lopTC.getMaLopTC() << " | " << (mh ? mh->getTenMH() : lopTC.getMaMH())
        << " | " << lopTC.getNienKhoa() << " | Ky " << lopTC.getHocKy() << " | Nhom " << lopTC.getNhom() << '\n';
    std::cout << std::left << std::setw(17) << "MA SV" << std::setw(45) << "HO TEN" << "TRANG THAI\n";
    std::vector<std::pair<SinhVien, bool>> ds;
    for (const auto& dk : lopTC.getDSSVDK()) {
        const Lop* lopChu = nullptr;
        for (const auto& lop : dsLop.getDanhSach()) if ((lopChu = &lop) && timSinhVien(lop.getDSSV(), dk.getMaSV())) break; else lopChu = nullptr;
        if (lopChu) { PTRSV p = timSinhVien(lopChu->getDSSV(), dk.getMaSV()); ds.emplace_back(p->getSinhVien(), dk.getHuyDangKy()); }
        else ds.emplace_back(SinhVien(dk.getMaSV(), "[Khong tim thay]", "", "", ""), dk.getHuyDangKy());
    }
    std::sort(ds.begin(), ds.end(), [](const auto& a, const auto& b) { return a.first.getMaSV() < b.first.getMaSV(); });
    for (const auto& item : ds) std::cout << std::left << std::setw(17) << item.first.getMaSV() << std::setw(45) << hoTen(item.first)
        << (item.second ? "Da huy" : "Con hieu luc") << '\n';
    std::cout << "Tong dang ky con hieu luc: " << lopTC.demSoSVDangKy() << '\n';
}

void HeThong::menuDangKy() {
    std::string maSV; if (!nhapMa(maSV, static_cast<int>(MAX_MASV), "Nhap ma sinh vien: ")) return;
    Lop* lopChu = nullptr;
    for (auto& lop : dsLop.getDanhSach()) if (timSinhVien(lop.getDSSV(), maSV)) { lopChu = &lop; break; }
    if (!lopChu) { std::cout << "Khong tim thay sinh vien.\n"; return; }
    std::cout << "Sinh vien: " << maSV << " - " << hoTen(timSinhVien(lopChu->getDSSV(), maSV)->getSinhVien()) << "\n";
    int tacVu; if (!nhapSoNguyen(tacVu, 1, 2, "1. Dang ky, 2. Huy dang ky: ")) return;
    const std::string khoa = docChuoiGioiHan("Nhap nien khoa: ", MAX_NIENKHOA); if (khoa.empty()) return;
    int ky; if (!nhapSoNguyen(ky, 1, 20, "Nhap hoc ky: ")) return;
    std::vector<LopTinChi*> dsMo;
    for (auto& lop : dsLopTC.getDanhSach()) if (lop.getNienKhoa() == khoa && lop.getHocKy() == ky && !lop.getHuyLop() && (tacVu == 2 ? lop.timDangKyTheoMaSV(maSV) && !lop.timDangKyTheoMaSV(maSV)->getHuyDangKy() : lop.demSoSVDangKy() < lop.getMaxSV())) dsMo.push_back(&lop);
    if (dsMo.empty()) { std::cout << "Khong co lop tin chi phu hop.\n"; return; }
    std::cout << std::left << std::setw(8) << "MA LTC" << std::setw(13) << "MA MH" << std::setw(32) << "TEN MON HOC"
        << std::setw(13) << "NIEN KHOA" << std::setw(8) << "KY" << std::setw(8) << "NHOM"
        << std::setw(12) << "DA DANG KY" << "CHO TRONG\n";
    for (const auto* lop : dsMo) {
        const MonHoc* mh = timMonHocTheoMa(dsMonHoc, lop->getMaMH());
        std::cout << std::left << std::setw(8) << lop->getMaLopTC() << std::setw(13) << lop->getMaMH()
            << std::setw(32) << (mh ? mh->getTenMH() : "[Khong co mon hoc]") << std::setw(13) << lop->getNienKhoa()
            << std::setw(8) << lop->getHocKy() << std::setw(8) << lop->getNhom() << std::setw(12) << lop->demSoSVDangKy()
            << (lop->getMaxSV() - lop->demSoSVDangKy()) << '\n';
    }
    int ma; if (!nhapSoNguyen(ma, 1, 2147483647, "Nhap ma lop tin chi muon chon: ")) return;
    auto it = std::find_if(dsMo.begin(), dsMo.end(), [ma](const LopTinChi* ltc) { return ltc->getMaLopTC() == ma; });
    if (it == dsMo.end()) { std::cout << "Lop khong thuoc danh sach du dieu kien.\n"; return; }
    const bool thanhCong = tacVu == 1 ? (*it)->themDangKy(maSV) : (*it)->huyDangKyTheoMaSV(maSV);
    if (thanhCong) { luuNeuCan(); std::cout << (tacVu == 1 ? "Dang ky thanh cong.\n" : "Da huy dang ky.\n"); }
    else std::cout << "Khong the cap nhat dang ky (co the da du, da dang ky, hoac khong con hieu luc).\n";
}

LopTinChi* HeThong::nhapThongTinTimLopTinChi() {
    const std::string khoa = docChuoiGioiHan("Nhap nien khoa: ", MAX_NIENKHOA);
    if (khoa.empty()) return nullptr;
    int ky;
    if (!nhapSoNguyen(ky, 1, 20, "Nhap hoc ky: ")) return nullptr;
    std::string maMH;
    if (!nhapMa(maMH, static_cast<int>(MAX_MAMH), "Nhap ma mon hoc: ")) return nullptr;
    int nhom;
    if (!nhapSoNguyen(nhom, 1, 1000, "Nhap nhom: ")) return nullptr;
    LopTinChi* lopTC = dsLopTC.timLopTinChiTheoMonHoc(maMH, khoa, ky, nhom);
    if (!lopTC) std::cout << "Khong tim thay lop tin chi voi cac thong tin vua nhap.\n";
    return lopTC;
}

void HeThong::nhapDiemLopTinChi(LopTinChi& lopTC) {
    if (lopTC.getHuyLop()) { std::cout << "Lop tin chi da bi huy.\n"; return; }
    std::vector<DangKy*> ds;
    for (auto& dk : lopTC.getDSSVDK()) if (!dk.getHuyDangKy()) ds.push_back(&dk);
    if (ds.empty()) { std::cout << "Lop chua co sinh vien dang ky con hieu luc.\n"; return; }
    const MonHoc* mh = timMonHocTheoMa(dsMonHoc, lopTC.getMaMH());
    std::cout << "Nhap diem thi - " << (mh ? mh->getTenMH() : lopTC.getMaMH()) << '\n';
    std::cout << std::left << std::setw(6) << "STT" << std::setw(17) << "MA SV" << std::setw(27) << "HO" << std::setw(17) << "TEN" << "DIEM\n";
    for (std::size_t i = 0; i < ds.size(); ++i) {
        const SinhVien* sv = nullptr;
        for (const auto& lop : dsLop.getDanhSach()) { PTRSV p = timSinhVien(lop.getDSSV(), ds[i]->getMaSV()); if (p) { sv = &p->getSinhVien(); break; } }
        if (!sv) continue;
        std::cout << std::left << std::setw(6) << (i + 1) << std::setw(17) << sv->getMaSV() << std::setw(27) << sv->getHo() << std::setw(17) << sv->getTen();
        if (ds[i]->getDiem() >= 0) std::cout << std::fixed << std::setprecision(2) << ds[i]->getDiem(); else std::cout << "--";
        std::cout << '\n';
        double diem;
        if (!nhapSoThuc(diem, MIN_DIEM, MAX_DIEM, "Nhap diem (0-10): ")) return;
        ds[i]->setDiem(diem);
    }
}

void HeThong::inBangDiemLopTinChi(const LopTinChi& lopTC) const {
    const MonHoc* mh = timMonHocTheoMa(dsMonHoc, lopTC.getMaMH());
    std::cout << "BANG DIEM MON HOC: " << (mh ? mh->getTenMH() : lopTC.getMaMH()) << "\nNien khoa: " << lopTC.getNienKhoa()
        << " | Hoc ky: " << lopTC.getHocKy() << " | Nhom: " << lopTC.getNhom() << '\n';
    std::cout << std::left << std::setw(6) << "STT" << std::setw(17) << "MA SV" << std::setw(27) << "HO" << std::setw(17) << "TEN" << "DIEM\n";
    int stt = 0;
    for (const auto& dk : lopTC.getDSSVDK()) {
        if (dk.getHuyDangKy()) continue;
        for (const auto& lop : dsLop.getDanhSach()) {
            PTRSV p = timSinhVien(lop.getDSSV(), dk.getMaSV()); if (!p) continue;
            const auto& sv = p->getSinhVien(); std::cout << std::left << std::setw(6) << ++stt << std::setw(17) << sv.getMaSV()
                << std::setw(27) << sv.getHo() << std::setw(17) << sv.getTen();
            if (dk.getDiem() >= 0) std::cout << std::fixed << std::setprecision(2) << dk.getDiem(); else std::cout << "Chua co diem";
            std::cout << '\n'; break;
        }
    }
}

void HeThong::inDiemTrungBinhTheoLop(const Lop& lop) const {
    std::cout << "BANG THONG KE DIEM TRUNG BINH KHOA HOC - LOP " << lop.getMaLop() << "\n";
    std::cout << std::left << std::setw(6) << "STT" << std::setw(17) << "MA SV" << std::setw(28) << "HO" << std::setw(17) << "TEN" << "DIEM TB\n";
    auto dsSV = taoMangSinhVien(lop.getDSSV());
    for (std::size_t i = 0; i < dsSV.size(); ++i) {
        double tongDiem = 0; unsigned long long tongTC = 0;
        for (const auto& lopTC : dsLopTC.getDanhSach()) {
            if (lopTC.getHuyLop()) continue;
            const DangKy* dk = lopTC.timDangKyTheoMaSV(dsSV[i].getMaSV());
            const MonHoc* mh = timMonHocTheoMa(dsMonHoc, lopTC.getMaMH());
            if (!dk || dk->getHuyDangKy() || dk->getDiem() < 0 || !mh) continue;
            const unsigned tc = mh->getSoTCLT() + mh->getSoTCTH();
            if (tc == 0) continue;
            tongDiem += dk->getDiem() * tc; tongTC += tc;
        }
        std::cout << std::left << std::setw(6) << (i + 1) << std::setw(17) << dsSV[i].getMaSV() << std::setw(28) << dsSV[i].getHo()
            << std::setw(17) << dsSV[i].getTen();
        if (tongTC) std::cout << std::fixed << std::setprecision(2) << tongDiem / tongTC; else std::cout << "Chua du diem";
        std::cout << '\n';
    }
}

void HeThong::inBangDiemTongKetTheoLop(const Lop& lop) const {
    std::map<std::string, std::string> tenMon;
    std::map<std::string, std::map<std::string, double>> diemCaoNhat;
    for (const auto& mh : dsMonHoc) tenMon[mh.getMaMH()] = mh.getTenMH();
    for (const auto& lopTC : dsLopTC.getDanhSach()) {
        if (lopTC.getHuyLop()) continue;
        for (const auto& dk : lopTC.getDSSVDK()) {
            if (dk.getHuyDangKy() || dk.getDiem() < 0) continue;
            bool thuocLop = false;
            for (PTRSV p = lop.getDSSV(); p; p = p->getNext()) if (p->getSinhVien().getMaSV() == dk.getMaSV()) { thuocLop = true; break; }
            if (!thuocLop) continue;
            auto& diem = diemCaoNhat[dk.getMaSV()][lopTC.getMaMH()];
            if (diem == 0.0 || dk.getDiem() > diem) diem = dk.getDiem();
        }
    }
    std::set<std::string> dsMaMH;
    for (const auto& dong : diemCaoNhat) for (const auto& mon : dong.second) dsMaMH.insert(mon.first);
    std::cout << "BANG DIEM TONG KET - LOP " << lop.getMaLop() << "\n";
    std::cout << std::left << std::setw(6) << "STT" << std::setw(17) << "MA SV" << std::setw(40) << "HO TEN";
    for (const auto& maMH : dsMaMH) std::cout << std::setw(14) << maMH;
    std::cout << '\n';
    auto dsSV = taoMangSinhVien(lop.getDSSV());
    std::sort(dsSV.begin(), dsSV.end(), [](const SinhVien& a, const SinhVien& b) { return a.getMaSV() < b.getMaSV(); });
    int stt = 0;
    for (const auto& sv : dsSV) {
        std::cout << std::left << std::setw(6) << ++stt << std::setw(17) << sv.getMaSV() << std::setw(40) << hoTen(sv);
        for (const auto& maMH : dsMaMH) {
            const auto itSV = diemCaoNhat.find(sv.getMaSV());
            if (itSV != diemCaoNhat.end() && itSV->second.count(maMH)) std::cout << std::setw(14) << std::fixed << std::setprecision(2) << itSV->second.at(maMH);
            else std::cout << std::setw(14) << "--";
        }
        std::cout << '\n';
    }
    if (dsMaMH.empty()) std::cout << "Chua co diem thi de tong ket.\n";
}
