#include "model/LichSuThaoTac.h"

#include <cstring>
#include "storage/DocGhiDuLieu.h"

namespace {
ThongTinLopTinChiLichSu chupLopTinChi(const LopTinChi& lopTinChi) {
    ThongTinLopTinChiLichSu thongTin{};
    thongTin.maLopTC = lopTinChi.maLopTC;
    std::memcpy(thongTin.maMH, lopTinChi.maMH, sizeof(thongTin.maMH));
    std::memcpy(thongTin.nienKhoa, lopTinChi.nienKhoa, sizeof(thongTin.nienKhoa));
    thongTin.hocKy = lopTinChi.hocKy;
    thongTin.nhom = lopTinChi.nhom;
    thongTin.minSV = lopTinChi.minSV;
    thongTin.maxSV = lopTinChi.maxSV;
    thongTin.huyLop = lopTinChi.huyLop;
    return thongTin;
}

void khoiPhucThongTinLopTinChi(LopTinChi& lopTinChi, const ThongTinLopTinChiLichSu& thongTin) {
    lopTinChi.maLopTC = thongTin.maLopTC;
    std::memcpy(lopTinChi.maMH, thongTin.maMH, sizeof(lopTinChi.maMH));
    std::memcpy(lopTinChi.nienKhoa, thongTin.nienKhoa, sizeof(lopTinChi.nienKhoa));
    lopTinChi.hocKy = thongTin.hocKy;
    lopTinChi.nhom = thongTin.nhom;
    lopTinChi.minSV = thongTin.minSV;
    lopTinChi.maxSV = thongTin.maxSV;
    lopTinChi.huyLop = thongTin.huyLop;
}

void saoChepMaLop(char* dich, const char* nguon, int kichThuoc) {
    if (kichThuoc <= 0) return;
    std::memcpy(dich, nguon, static_cast<std::size_t>(kichThuoc));
    dich[kichThuoc - 1] = '\0';
}

bool xoaLopTinChiTai(DanhSachLopTinChi& danhSach, int viTri) {
    if (viTri < 0 || viTri >= danhSach.soLuong ||
        demTongDangKy(danhSach.lopTC[viTri]->dssvdk) > 0) {
        return false;
    }

    delete danhSach.lopTC[viTri];
    for (int i = viTri; i < danhSach.soLuong - 1; ++i) {
        danhSach.lopTC[i] = danhSach.lopTC[i + 1];
    }
    danhSach.lopTC[--danhSach.soLuong] = nullptr;
    return true;
}

bool themLopTinChiTai(DanhSachLopTinChi& danhSach,
                      const ThongTinLopTinChiLichSu& thongTin,
                      int viTri) {
    if (danhSach.soLuong >= MAX_LOP_TC ||
        timLopTinChiTheoMa(danhSach, thongTin.maLopTC) != -1 ||
        timLopTinChiTheoThamSo(danhSach, thongTin.nienKhoa, thongTin.hocKy,
                              thongTin.maMH, thongTin.nhom) != -1) {
        return false;
    }

    if (viTri < 0 || viTri > danhSach.soLuong) viTri = danhSach.soLuong;
    LopTinChi* lopTinChi = new LopTinChi();
    khoiPhucThongTinLopTinChi(*lopTinChi, thongTin);

    for (int i = danhSach.soLuong; i > viTri; --i) {
        danhSach.lopTC[i] = danhSach.lopTC[i - 1];
    }
    danhSach.lopTC[viTri] = lopTinChi;
    ++danhSach.soLuong;
    return true;
}

bool apDungHoanTac(const ThaoTacLichSu& thaoTac, PTRMH& root,
                   DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi,
                   std::string& thongBao) {
    const bool laThem = thaoTac.loaiThaoTac == LoaiThaoTacLichSu::Them;
    const bool laXoa = thaoTac.loaiThaoTac == LoaiThaoTacLichSu::Xoa;

    if (thaoTac.loaiDuLieu == LoaiDuLieuLichSu::Lop) {
        const ThongTinLopLichSu& thongTin = laThem ? thaoTac.lopMoi : thaoTac.lopCu;
        const int viTri = timLop(danhSachLop, thongTin.maLop);
        if (laThem) {
            if (viTri < 0 || danhSachLop.lop[viTri].dssv != nullptr) {
                thongBao = "Không thể hoàn tác: lớp đã có sinh viên.";
                return false;
            }
            for (int i = viTri; i < danhSachLop.soLuong - 1; ++i) danhSachLop.lop[i] = danhSachLop.lop[i + 1];
            danhSachLop.lop[--danhSachLop.soLuong] = {};
        } else if (laXoa) {
            if (viTri >= 0 || danhSachLop.soLuong >= MAX_LOP) return false;
            Lop lop = taoLop(thongTin.maLop, thongTin.tenLop);
            int viTriChen = thaoTac.viTriDanhSachLop;
            if (viTriChen < 0 || viTriChen > danhSachLop.soLuong) viTriChen = danhSachLop.soLuong;
            for (int i = danhSachLop.soLuong; i > viTriChen; --i) danhSachLop.lop[i] = danhSachLop.lop[i - 1];
            danhSachLop.lop[viTriChen] = lop;
            ++danhSachLop.soLuong;
        } else {
            if (viTri < 0) return false;
            std::memcpy(danhSachLop.lop[viTri].tenLop, thaoTac.lopCu.tenLop, MAX_TENLOP);
            danhSachLop.lop[viTri].tenLop[MAX_TENLOP - 1] = '\0';
        }
        thongBao = "Đã hoàn tác thao tác lớp.";
        return true;
    }

    if (thaoTac.loaiDuLieu == LoaiDuLieuLichSu::MonHoc) {
        const MonHoc& monHoc = laThem ? thaoTac.monHocMoi : thaoTac.monHocCu;
        if (laThem) {
            if (kiemTraMonHocDaMoLop(danhSachLopTinChi, monHoc.maMH)) {
                thongBao = "Không thể hoàn tác: môn học đã được mở lớp tín chỉ.";
                return false;
            }
            if (!xoaMonHoc(root, monHoc.maMH)) return false;
        } else if (laXoa) {
            if (timMonHoc(root, monHoc.maMH) != nullptr ||
                !themMonHoc(root, monHoc)) {
                return false;
            }
        } else {
            PTRMH nutMonHoc = timMonHoc(root, thaoTac.monHocCu.maMH);
            if (nutMonHoc == nullptr) return false;
            nutMonHoc->mh = thaoTac.monHocCu;
        }

        thongBao = "Đã hoàn tác thao tác môn học.";
        return true;
    }

    if (thaoTac.loaiDuLieu == LoaiDuLieuLichSu::SinhVien) {
        const SinhVien& sinhVien = laThem ? thaoTac.sinhVienMoi : thaoTac.sinhVienCu;
        const int viTriLop = timLop(danhSachLop, thaoTac.maLop);
        if (viTriLop < 0) return false;

        int viTriLopDangCo = -1;
        PTRSV sinhVienDangCo = timSinhVienToanTruong(
            danhSachLop, sinhVien.maSV, &viTriLopDangCo);

        if (laThem) {
            if (kiemTraSVDaDangKyLTC(danhSachLopTinChi, sinhVien.maSV)) {
                thongBao = "Không thể hoàn tác: sinh viên đã đăng ký lớp tín chỉ.";
                return false;
            }
            if (sinhVienDangCo == nullptr || viTriLopDangCo != viTriLop ||
                !xoaSVTheoMa(danhSachLop.lop[viTriLop].dssv, sinhVien.maSV)) {
                return false;
            }
        } else if (laXoa) {
            if (sinhVienDangCo != nullptr) return false;
            themSVCoThuTu(danhSachLop.lop[viTriLop].dssv, sinhVien);
        } else {
            if (sinhVienDangCo == nullptr || viTriLopDangCo != viTriLop) return false;
            if (!xoaSVTheoMa(danhSachLop.lop[viTriLop].dssv, thaoTac.sinhVienMoi.maSV)) {
                return false;
            }
            themSVCoThuTu(danhSachLop.lop[viTriLop].dssv, thaoTac.sinhVienCu);
        }

        thongBao = "Đã hoàn tác thao tác sinh viên.";
        return true;
    }

    const int maLopTC = laThem
        ? thaoTac.lopTinChiMoi.maLopTC
        : thaoTac.lopTinChiCu.maLopTC;
    const int viTriLopTC = timLopTinChiTheoMa(danhSachLopTinChi, maLopTC);

    if (laThem) {
        if (viTriLopTC < 0 ||
            !xoaLopTinChiTai(danhSachLopTinChi, viTriLopTC)) {
            thongBao = "Không thể hoàn tác: lớp tín chỉ đã có đăng ký.";
            return false;
        }
    } else if (laXoa) {
        if (!themLopTinChiTai(danhSachLopTinChi, thaoTac.lopTinChiCu,
                              thaoTac.viTriLopTinChi)) {
            return false;
        }
    } else {
        if (viTriLopTC < 0) return false;
        const int viTriTrung = timLopTinChiTheoThamSo(
            danhSachLopTinChi, thaoTac.lopTinChiCu.nienKhoa,
            thaoTac.lopTinChiCu.hocKy, thaoTac.lopTinChiCu.maMH,
            thaoTac.lopTinChiCu.nhom);
        if (viTriTrung >= 0 && viTriTrung != viTriLopTC) return false;
        khoiPhucThongTinLopTinChi(
            *danhSachLopTinChi.lopTC[viTriLopTC], thaoTac.lopTinChiCu);
    }

    thongBao = "Đã hoàn tác thao tác lớp tín chỉ.";
    return true;
}
} // namespace

void LichSuThaoTac::dayLichSu(QuanLyLichSu& lichSu, const ThaoTacLichSu& thaoTac) {
    StackLichSu& nganXep = (thaoTac.loaiDuLieu == LoaiDuLieuLichSu::SinhVien || thaoTac.loaiDuLieu == LoaiDuLieuLichSu::Lop)
        ? lichSu.undoSinhVienLop : lichSu.undo;
    nganXep.top = new NodeLichSu{thaoTac, nganXep.top};
}

namespace {
ThongTinLopLichSu chupThongTinLop(const Lop& lop) {
    ThongTinLopLichSu thongTin{};
    saoChepMaLop(thongTin.maLop, lop.maLop, MAX_MALOP);
    saoChepMaLop(thongTin.tenLop, lop.tenLop, MAX_TENLOP);
    return thongTin;
}
}

void LichSuThaoTac::ghiNhanThemLop(QuanLyLichSu& lichSu, const Lop& lop, int viTri) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::Lop;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Them;
    thaoTac.viTriDanhSachLop = viTri;
    thaoTac.lopMoi = chupThongTinLop(lop);
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanXoaLop(QuanLyLichSu& lichSu, const Lop& lop, int viTri) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::Lop;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Xoa;
    thaoTac.viTriDanhSachLop = viTri;
    thaoTac.lopCu = chupThongTinLop(lop);
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanSuaLop(QuanLyLichSu& lichSu, const Lop& truoc, const Lop& sau) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::Lop;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Sua;
    thaoTac.lopCu = chupThongTinLop(truoc);
    thaoTac.lopMoi = chupThongTinLop(sau);
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanThemLopTinChi(QuanLyLichSu& lichSu, const LopTinChi& lopTinChi, int viTri) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::LopTinChi;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Them;
    thaoTac.viTriLopTinChi = viTri;
    thaoTac.lopTinChiMoi = chupLopTinChi(lopTinChi);
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanXoaLopTinChi(QuanLyLichSu& lichSu, const LopTinChi& lopTinChi, int viTri) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::LopTinChi;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Xoa;
    thaoTac.viTriLopTinChi = viTri;
    thaoTac.lopTinChiCu = chupLopTinChi(lopTinChi);
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanSuaLopTinChi(QuanLyLichSu& lichSu,
                         const ThongTinLopTinChiLichSu& truoc,
                         const LopTinChi& sau) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::LopTinChi;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Sua;
    thaoTac.lopTinChiCu = truoc;
    thaoTac.lopTinChiMoi = chupLopTinChi(sau);
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanThemSinhVien(QuanLyLichSu& lichSu, const char* maLop,
                         const SinhVien& sinhVien) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::SinhVien;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Them;
    saoChepMaLop(thaoTac.maLop, maLop, MAX_MALOP);
    thaoTac.sinhVienMoi = sinhVien;
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanXoaSinhVien(QuanLyLichSu& lichSu, const char* maLop,
                        const SinhVien& sinhVien) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::SinhVien;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Xoa;
    saoChepMaLop(thaoTac.maLop, maLop, MAX_MALOP);
    thaoTac.sinhVienCu = sinhVien;
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanSuaSinhVien(QuanLyLichSu& lichSu, const char* maLop,
                        const SinhVien& truoc, const SinhVien& sau) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::SinhVien;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Sua;
    saoChepMaLop(thaoTac.maLop, maLop, MAX_MALOP);
    thaoTac.sinhVienCu = truoc;
    thaoTac.sinhVienMoi = sau;
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanThemMonHoc(QuanLyLichSu& lichSu, const MonHoc& monHoc) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::MonHoc;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Them;
    thaoTac.monHocMoi = monHoc;
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanXoaMonHoc(QuanLyLichSu& lichSu, const MonHoc& monHoc) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::MonHoc;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Xoa;
    thaoTac.monHocCu = monHoc;
    dayLichSu(lichSu, thaoTac);
}

void LichSuThaoTac::ghiNhanSuaMonHoc(QuanLyLichSu& lichSu, const MonHoc& truoc, const MonHoc& sau) {
    ThaoTacLichSu thaoTac{};
    thaoTac.loaiDuLieu = LoaiDuLieuLichSu::MonHoc;
    thaoTac.loaiThaoTac = LoaiThaoTacLichSu::Sua;
    thaoTac.monHocCu = truoc;
    thaoTac.monHocMoi = sau;
    dayLichSu(lichSu, thaoTac);
}

bool LichSuThaoTac::hoanTacThaoTac(QuanLyLichSu& lichSu, PTRMH& root,
                    DanhSachLop& danhSachLop,
                    DanhSachLopTinChi& danhSachLopTinChi,
                    std::string& thongBao) {
    thongBao.clear();
    if (lichSu.undo.top == nullptr) {
        thongBao = "Chưa có thao tác nào để hoàn tác.";
        return false;
    }

    NodeLichSu* nutLichSu = lichSu.undo.top;
    lichSu.undo.top = nutLichSu->next;
    const ThaoTacLichSu thaoTac = nutLichSu->thaoTac;
    delete nutLichSu;

    if (!apDungHoanTac(thaoTac, root, danhSachLop, danhSachLopTinChi, thongBao)) {
        dayLichSu(lichSu, thaoTac);
        if (thongBao.empty()) {
            thongBao = "Không thể hoàn tác thao tác này trong trạng thái dữ liệu hiện tại.";
        }
        return false;
    }

    if (!luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi)) {
        thongBao += " Dữ liệu đã đổi trong bộ nhớ nhưng không ghi được file.";
        return false;
    }
    return true;
}

bool LichSuThaoTac::hoanTacThaoTacC(QuanLyLichSu& lichSu, PTRMH& root,
                    DanhSachLop& danhSachLop,
                    DanhSachLopTinChi& danhSachLopTinChi,
                    std::string& thongBao) {
    thongBao.clear();
    if (lichSu.undoSinhVienLop.top == nullptr) {
        thongBao = "Câu c chưa có thao tác nào để hoàn tác.";
        return false;
    }
    NodeLichSu* nutLichSu = lichSu.undoSinhVienLop.top;
    lichSu.undoSinhVienLop.top = nutLichSu->next;
    const ThaoTacLichSu thaoTac = nutLichSu->thaoTac;
    delete nutLichSu;
    if (!apDungHoanTac(thaoTac, root, danhSachLop, danhSachLopTinChi, thongBao)) {
        lichSu.undoSinhVienLop.top = new NodeLichSu{thaoTac, lichSu.undoSinhVienLop.top};
        if (thongBao.empty()) thongBao = "Không thể hoàn tác thao tác này trong trạng thái dữ liệu hiện tại.";
        return false;
    }
    if (!luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi)) {
        thongBao += " Dữ liệu đã đổi trong bộ nhớ nhưng không ghi được file.";
        return false;
    }
    return true;
}

std::string LichSuThaoTac::moTaThaoTacGanNhat(const QuanLyLichSu& lichSu) {
    const NodeLichSu* nut = lichSu.undo.top;
    if (nut == nullptr) return "Chưa có thao tác";

    const ThaoTacLichSu& thaoTac = nut->thaoTac;
    const char* hanhDong = thaoTac.loaiThaoTac == LoaiThaoTacLichSu::Them ? "Thêm" :
                           thaoTac.loaiThaoTac == LoaiThaoTacLichSu::Sua ? "Sửa" : "Xóa";
    if (thaoTac.loaiDuLieu == LoaiDuLieuLichSu::MonHoc) {
        const MonHoc& monHoc = thaoTac.loaiThaoTac == LoaiThaoTacLichSu::Them
            ? thaoTac.monHocMoi : thaoTac.monHocCu;
        return std::string(hanhDong) + " môn " + monHoc.maMH;
    }
    if (thaoTac.loaiDuLieu == LoaiDuLieuLichSu::SinhVien) {
        const SinhVien& sinhVien = thaoTac.loaiThaoTac == LoaiThaoTacLichSu::Them
            ? thaoTac.sinhVienMoi : thaoTac.sinhVienCu;
        return std::string(hanhDong) + " SV " + sinhVien.maSV + " lớp " + thaoTac.maLop;
    }
    if (thaoTac.loaiDuLieu == LoaiDuLieuLichSu::Lop) {
        const ThongTinLopLichSu& lop = thaoTac.loaiThaoTac == LoaiThaoTacLichSu::Them
            ? thaoTac.lopMoi : thaoTac.lopCu;
        return std::string(hanhDong) + " lớp " + lop.maLop;
    }

    const ThongTinLopTinChiLichSu& lopTinChi = thaoTac.loaiThaoTac == LoaiThaoTacLichSu::Them
        ? thaoTac.lopTinChiMoi : thaoTac.lopTinChiCu;
    return std::string(hanhDong) + " lớp TC #" + std::to_string(lopTinChi.maLopTC) +
           " (" + lopTinChi.maMH + ")";
}

std::string LichSuThaoTac::moTaThaoTacCGanNhat(const QuanLyLichSu& lichSu) {
    QuanLyLichSu banSao{};
    banSao.undo.top = lichSu.undoSinhVienLop.top;
    return moTaThaoTacGanNhat(banSao);
}

void LichSuThaoTac::giaiPhongLichSu(QuanLyLichSu& lichSu) {
    auto giaiPhongStack = [](StackLichSu& nganXep) {
        while (nganXep.top != nullptr) {
            NodeLichSu* nutLichSu = nganXep.top;
            nganXep.top = nutLichSu->next;
            delete nutLichSu;
        }
    };
    giaiPhongStack(lichSu.undo);
    giaiPhongStack(lichSu.undoSinhVienLop);
}
