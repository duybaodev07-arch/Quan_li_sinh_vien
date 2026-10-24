#include "ui/GiaoDienConsole.h"
#include "features/QuanLyMonHocLopTinChi.h"
#include "features/HamDungChung.h"

#include "storage/DocGhiDuLieu.h"


#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <cstring>

using namespace std;

// ============================================================================
void chucNangA_MoLopTinChi(PTRMH cayMonHoc, DanhSachLopTinChi& danhSachLopTinChi, const DanhSachLop& danhSachLop) {
    int mucDangChon = 0;
    int soDongMoiTrang = 14;
    string thongBao = "";
    bool coLoi = false;
    bool canVeLaiToanBo = true;
    int mucChonTruoc = -1;
    int trangTruoc = -1;
    int nutDangTro = -1;
    int nutTruocDo = -1;

    int soNut = 4;
    NutChuNhat* cacNut = new NutChuNhat[soNut];
    cacNut[0] = { 6,  23, 24, 1, 0, "[ + THÊM MỚI (Ins) ]" };
    cacNut[1] = { 34, 23, 24, 1, 1, "[ HIỆU CHỈNH (Enter) ]" };
    cacNut[2] = { 62, 23, 22, 1, 2, "[ XÓA LỚP (Del) ]" };
    cacNut[3] = { 88, 23, 22, 1, 3, "[ QUAY LẠI (ESC) ]" };

    auto veMotDong = [&](int idx, int viTriDau, bool dangDuocChon) {
        int r = idx - viTriDau;
        if (r < 0 || r >= soDongMoiTrang) return;
        int toaDoDong = 7 + r;
        GiaoDienConsole::diDenToaDo(4, toaDoDong);
        if (idx >= 0 && idx < danhSachLopTinChi.soLuong) {
            LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[idx];
            NodeMH* mh = timMonHoc(cayMonHoc, lopTinChi->maMH);
            string tenMonHoc = mh ? mh->mh.tenMH : "(Không rõ)";
            int soDangKy = demSVDangKyHieuLuc(lopTinChi->dssvdk);
            GiaoDienConsole::datMau(dangDuocChon ? DEN : (lopTinChi->huyLop ? DO_NHAT : TRANG), dangDuocChon ? VANG : DEN);
            cout << GiaoDienConsole::canPhai(" " + to_string(lopTinChi->maLopTC), 7)
                 << GiaoDienConsole::canPhai(lopTinChi->maMH, 10)
                 << GiaoDienConsole::canPhai(tenMonHoc, 32)
                 << GiaoDienConsole::canPhai(lopTinChi->nienKhoa, 12)
                 << GiaoDienConsole::canPhai(to_string(lopTinChi->hocKy), 5)
                 << GiaoDienConsole::canPhai(to_string(lopTinChi->nhom), 6)
                 << GiaoDienConsole::canPhai(to_string(lopTinChi->minSV), 8)
                 << GiaoDienConsole::canPhai(to_string(lopTinChi->maxSV), 8)
                 << GiaoDienConsole::canPhai(to_string(soDangKy), 8)
                 << GiaoDienConsole::canPhai(lopTinChi->huyLop ? "Đã hủy" : "Đang mở", 12);
        } else {
            GiaoDienConsole::datMau(XAM_NHAT, DEN);
            cout << string(108, ' ');
        }
    };

    while (true) {
        if (mucDangChon >= danhSachLopTinChi.soLuong) mucDangChon = (danhSachLopTinChi.soLuong > 0) ? danhSachLopTinChi.soLuong - 1 : 0;
        int tongSoTrang = (danhSachLopTinChi.soLuong == 0) ? 1 : (danhSachLopTinChi.soLuong + soDongMoiTrang - 1) / soDongMoiTrang;
        int trangHienTai = mucDangChon / soDongMoiTrang;
        int viTriDau = trangHienTai * soDongMoiTrang;
        int viTriCuoi = (viTriDau + soDongMoiTrang < danhSachLopTinChi.soLuong) ? (viTriDau + soDongMoiTrang) : danhSachLopTinChi.soLuong;

        if (trangHienTai != trangTruoc) canVeLaiToanBo = true;

        if (canVeLaiToanBo) {
            GiaoDienConsole::xoaManHinh();
            GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH LỚP TÍN CHỈ");

            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 5);
            cout << GiaoDienConsole::canPhai("MÃ LTC", 7)
                 << GiaoDienConsole::canPhai("MÃ MH", 10)
                 << GiaoDienConsole::canPhai("TÊN MÔN HỌC", 32)
                 << GiaoDienConsole::canPhai("NIÊN KHÓA", 12)
                 << GiaoDienConsole::canPhai("HK", 5)
                 << GiaoDienConsole::canPhai("NHÓM", 6)
                 << GiaoDienConsole::canPhai("SV MIN", 8)
                 << GiaoDienConsole::canPhai("SV MAX", 8)
                 << GiaoDienConsole::canPhai("ĐÃ ĐK", 8)
                 << GiaoDienConsole::canPhai("TRẠNG THÁI", 12);

            for (int r = 0; r < soDongMoiTrang; ++r) {
                int idx = viTriDau + r;
                veMotDong(idx, viTriDau, idx == mucDangChon);
            }

            for (int i = 0; i < soNut; ++i) {
                GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
            }

            GiaoDienConsole::veThanhTrangThai("Ins: Mở lớp mới | Enter: Sửa lớp đang chọn | Del: Xóa lớp | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang) + " | ESC: Quay lại", thongBao, coLoi);
            thongBao = "";
            canVeLaiToanBo = false;
            mucChonTruoc = mucDangChon;
            trangTruoc = trangHienTai;
            nutTruocDo = nutDangTro;
        } else {
            if (mucDangChon != mucChonTruoc) {
                veMotDong(mucChonTruoc, viTriDau, false);
                veMotDong(mucDangChon, viTriDau, true);
                mucChonTruoc = mucDangChon;
            }
            if (nutDangTro != nutTruocDo) {
                for (int i = 0; i < soNut; ++i) {
                    GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
                }
                nutTruocDo = nutDangTro;
            }
        }

        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        int hanhDong = -1;

        if (suKien.type == SK_CHUOT_DI_CHUYEN) {
            nutDangTro = -1;
            for (int i = 0; i < soNut; ++i) {
                if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                    nutDangTro = i;
                    break;
                }
            }
            if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                int idx = viTriDau + (suKien.toaDoChuotY - 7);
                if (idx < viTriCuoi) mucDangChon = idx;
            }
            continue;
        } else if (suKien.type == SK_CHUOT_TRAI) {
            for (int i = 0; i < soNut; ++i) {
                if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                    hanhDong = cacNut[i].id;
                    break;
                }
            }
            if (hanhDong == -1 && suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                int idx = viTriDau + (suKien.toaDoChuotY - 7);
                if (idx < viTriCuoi) {
                    if (mucDangChon == idx) hanhDong = 1;
                    else mucDangChon = idx;
                }
            }
        }

        if (suKien.type == SK_PHIM_ESC || hanhDong == 3) {
            luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
            delete[] cacNut;
            break;
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
            if (danhSachLopTinChi.soLuong > 0) mucDangChon = (mucDangChon - 1 + danhSachLopTinChi.soLuong) % danhSachLopTinChi.soLuong;
        } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
            if (danhSachLopTinChi.soLuong > 0) mucDangChon = (mucDangChon + 1) % danhSachLopTinChi.soLuong;
        } else if (suKien.type == EVT_KEY_INSERT || hanhDong == 0) {
            if (danhSachLopTinChi.soLuong >= MAX_LOP_TC) {
                thongBao = "Danh sách lớp tín chỉ đã đầy (tối đa 10000 lớp)!";
                coLoi = true;
                canVeLaiToanBo = true;
                continue;
            }
            TruongNhap* f = new TruongNhap[7];
            f[0] = { "Mã Lớp TC (Tự động)", to_string(layMaLopTCKeTiep(danhSachLopTinChi)), 10, TRUONG_CHI_DOC, 0, 0, 0 };
            f[1] = { "Mã môn học", "", 10, TRUONG_MA, 0, 0, 0 };
            f[2] = { "Niên khóa (VD: 2025-2026)", "2025-2026", 11, TRUONG_MA, 0, 0, 0 };
            f[3] = { "Học kỳ (1, 2, 3)", "1", 1, TRUONG_SO, 0, 0, 0 };
            f[4] = { "Nhóm (1..99)", "1", 2, TRUONG_SO, 0, 0, 0 };
            f[5] = { "Số SV tối thiểu (Min)", "15", 4, TRUONG_SO, 0, 0, 0 };
            f[6] = { "Số SV tối đa (Max)", "60", 4, TRUONG_SO, 0, 0, 0 };

            string thongBaoLoi = "";
            while (true) {
                if (!chayBieuMau("MỞ LỚP TÍN CHỈ MỚI", f, 7, thongBaoLoi)) break;

                string maMH = GiaoDienConsole::vietHoa(f[1].value);
                string nienKhoa = GiaoDienConsole::catKhoangTrang(f[2].value);
                int hocKy = atoi(f[3].value.c_str());
                int nhom = atoi(f[4].value.c_str());
                int soSinhVienToiThieu = atoi(f[5].value.c_str());
                int soSinhVienToiDa = atoi(f[6].value.c_str());

                if (maMH.empty() || nienKhoa.empty()) {
                    thongBaoLoi = "Mã môn học và Niên khóa không được để trống!";
                    continue;
                }
                if (timMonHoc(cayMonHoc, maMH.c_str()) == nullptr) {
                    thongBaoLoi = "Mã môn học '" + maMH + "' không tồn tại trong danh sách môn học!";
                    continue;
                }
                if (hocKy < 1 || hocKy > 3) {
                    thongBaoLoi = "Học kỳ chỉ nhận giá trị 1, 2 hoặc 3!";
                    continue;
                }
                if (nhom < 1) {
                    thongBaoLoi = "Nhóm phải là số nguyên dương (>= 1)!";
                    continue;
                }
                if (soSinhVienToiThieu <= 0 || soSinhVienToiDa < soSinhVienToiThieu) {
                    thongBaoLoi = "Điều kiện: Số SV Min > 0 và Số SV Max >= Số SV Min!";
                    continue;
                }
                if (timLopTinChiTheoThamSo(danhSachLopTinChi, nienKhoa.c_str(), hocKy, maMH.c_str(), nhom) != -1) {
                    thongBaoLoi = "Lớp tín chỉ (Mã MH + Niên khóa + Học kỳ + Nhóm) này đã tồn tại!";
                    continue;
                }

                LopTinChi* lopTinChi = new LopTinChi();
                lopTinChi->maLopTC = layMaLopTCKeTiep(danhSachLopTinChi);
                strncpy(lopTinChi->maMH, maMH.c_str(), MAX_MAMH - 1); lopTinChi->maMH[MAX_MAMH - 1] = '\0';
                strncpy(lopTinChi->nienKhoa, nienKhoa.c_str(), MAX_NIENKHOA - 1); lopTinChi->nienKhoa[MAX_NIENKHOA - 1] = '\0';
                lopTinChi->hocKy = hocKy;
                lopTinChi->nhom = nhom;
                lopTinChi->minSV = soSinhVienToiThieu;
                lopTinChi->maxSV = soSinhVienToiDa;
                lopTinChi->huyLop = false;
                danhSachLopTinChi.lopTC[danhSachLopTinChi.soLuong++] = lopTinChi;
                mucDangChon = danhSachLopTinChi.soLuong - 1;
                luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                thongBao = "Đã mở lớp tín chỉ mã " + to_string(lopTinChi->maLopTC) + " thành công!";
                coLoi = false;
                break;
            }
            delete[] f;
            canVeLaiToanBo = true;
        } else if ((suKien.type == SK_PHIM_ENTER || hanhDong == 1) && danhSachLopTinChi.soLuong > 0) {
            LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[mucDangChon];
            TruongNhap* f = new TruongNhap[8];
            f[0] = { "Mã Lớp TC (Cố định)", to_string(lopTinChi->maLopTC), 10, TRUONG_CHI_DOC, 0, 0, 0 };
            f[1] = { "Mã môn học", lopTinChi->maMH, 10, TRUONG_MA, 0, 0, 0 };
            f[2] = { "Niên khóa", lopTinChi->nienKhoa, 11, TRUONG_MA, 0, 0, 0 };
            f[3] = { "Học kỳ (1..3)", to_string(lopTinChi->hocKy), 1, TRUONG_SO, 0, 0, 0 };
            f[4] = { "Nhóm", to_string(lopTinChi->nhom), 2, TRUONG_SO, 0, 0, 0 };
            f[5] = { "Số SV tối thiểu (Min)", to_string(lopTinChi->minSV), 4, TRUONG_SO, 0, 0, 0 };
            f[6] = { "Số SV tối đa (Max)", to_string(lopTinChi->maxSV), 4, TRUONG_SO, 0, 0, 0 };
            f[7] = { "Hủy lớp (0:Mở, 1:Hủy)", lopTinChi->huyLop ? "1" : "0", 1, TRUONG_SO, 0, 0, 0 };

            string thongBaoLoi = "";
            while (true) {
                if (!chayBieuMau("HIỆU CHỈNH LỚP TÍN CHỈ #" + to_string(lopTinChi->maLopTC), f, 8, thongBaoLoi)) break;

                string maMH = GiaoDienConsole::vietHoa(f[1].value);
                string nienKhoa = GiaoDienConsole::catKhoangTrang(f[2].value);
                int hocKy = atoi(f[3].value.c_str());
                int nhom = atoi(f[4].value.c_str());
                int soSinhVienToiThieu = atoi(f[5].value.c_str());
                int soSinhVienToiDa = atoi(f[6].value.c_str());
                int huy = atoi(f[7].value.c_str());
                int soDangKy = demSVDangKyHieuLuc(lopTinChi->dssvdk);

                if (maMH.empty() || nienKhoa.empty()) {
                    thongBaoLoi = "Mã môn học và Niên khóa không được để trống!";
                    continue;
                }
                if (timMonHoc(cayMonHoc, maMH.c_str()) == nullptr) {
                    thongBaoLoi = "Mã môn học '" + maMH + "' không tồn tại!";
                    continue;
                }
                if (hocKy < 1 || hocKy > 3 || nhom < 1) {
                    thongBaoLoi = "Học kỳ (1..3) hoặc Nhóm (>=1) không hợp lệ!";
                    continue;
                }
                if (soSinhVienToiThieu <= 0 || soSinhVienToiDa < soSinhVienToiThieu || soSinhVienToiDa < soDangKy) {
                    thongBaoLoi = "Số SV Max phải >= Số SV Min và >= Số SV đã đăng ký (" + to_string(soDangKy) + ")!";
                    continue;
                }
                int viTriBiTrung = timLopTinChiTheoThamSo(danhSachLopTinChi, nienKhoa.c_str(), hocKy, maMH.c_str(), nhom);
                if (viTriBiTrung != -1 && viTriBiTrung != mucDangChon) {
                    thongBaoLoi = "Bộ (Mã MH, Niên khóa, Học kỳ, Nhóm) bị trùng với Lớp TC #" + to_string(danhSachLopTinChi.lopTC[viTriBiTrung]->maLopTC) + "!";
                    continue;
                }

                strncpy(lopTinChi->maMH, maMH.c_str(), MAX_MAMH - 1); lopTinChi->maMH[MAX_MAMH - 1] = '\0';
                strncpy(lopTinChi->nienKhoa, nienKhoa.c_str(), MAX_NIENKHOA - 1); lopTinChi->nienKhoa[MAX_NIENKHOA - 1] = '\0';
                lopTinChi->hocKy = hocKy;
                lopTinChi->nhom = nhom;
                lopTinChi->minSV = soSinhVienToiThieu;
                lopTinChi->maxSV = soSinhVienToiDa;
                lopTinChi->huyLop = (huy != 0);

                luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                thongBao = "Đã cập nhật Lớp tín chỉ #" + to_string(lopTinChi->maLopTC) + " thành công!";
                coLoi = false;
                break;
            }
            delete[] f;
            canVeLaiToanBo = true;
        } else if ((suKien.type == EVT_KEY_DELETE || hanhDong == 2) && danhSachLopTinChi.soLuong > 0) {
            LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[mucDangChon];
            int soDangKy = demTongDangKy(lopTinChi->dssvdk);
            if (soDangKy > 0) {
                thongBao = "Không thể xóa Lớp TC #" + to_string(lopTinChi->maLopTC) + " vì đã có " + to_string(soDangKy) + " sinh viên đăng ký!";
                coLoi = true;
                canVeLaiToanBo = true;
                continue;
            }
            if (GiaoDienConsole::hopXacNhan("XÁC NHẬN XÓA LỚP TÍN CHỈ",
                                         "Bạn có chắc chắn muốn xóa Lớp tín chỉ #" + to_string(lopTinChi->maLopTC) + " (" + string(lopTinChi->maMH) + ")?")) {
                giaiPhongDSDK(lopTinChi->dssvdk);
                delete lopTinChi;
                for (int i = mucDangChon; i < danhSachLopTinChi.soLuong - 1; ++i) {
                    danhSachLopTinChi.lopTC[i] = danhSachLopTinChi.lopTC[i + 1];
                }
                danhSachLopTinChi.lopTC[--danhSachLopTinChi.soLuong] = nullptr;
                luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                thongBao = "Đã xóa lớp tín chỉ thành công!";
                coLoi = false;
            }
            canVeLaiToanBo = true;
        }
    }
}

// ============================================================================
// CHỨC NĂNG (b): IN DSSV ĐÃ ĐĂNG KÝ LỚP TÍN CHỈ THEO THAM SỐ
// ============================================================================

// ============================================================================
void chucNangB_InDSSVDangKyLopTC(PTRMH cayMonHoc, const DanhSachLopTinChi& danhSachLopTinChi, const DanhSachLop& danhSachLop) {
    while (true) {
        int viTriLopTinChi = chonLopTinChiTheoThongTin(cayMonHoc, danhSachLopTinChi, "CHỨC NĂNG (b): IN DSSV ĐÃ ĐĂNG KÝ LỚP TÍN CHỈ");
        if (viTriLopTinChi == -1) break;

        LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChi];
        NodeMH* mh = timMonHoc(cayMonHoc, lopTinChi->maMH);
        string tenMonHoc = mh ? mh->mh.tenMH : lopTinChi->maMH;

        int tongSoDangKy = demTongDangKy(lopTinChi->dssvdk);
        struct DongDangKy {
            string maSV;
            string hoTen;
            string trangThai;
            bool daHuy;
        };
        DongDangKy* cacDong = (tongSoDangKy > 0) ? new DongDangKy[tongSoDangKy] : nullptr;
        int soDong = 0;
        for (PTRDK p = lopTinChi->dssvdk; p != nullptr; p = p->next) {
            NodeSV* sv = timSinhVienToanTruong(danhSachLop, p->dk.maSV);
            string hoTen = sv ? (string(sv->sv.ho) + " " + sv->sv.ten) : "(Không tìm thấy SV)";
            cacDong[soDong].maSV = p->dk.maSV;
            cacDong[soDong].hoTen = hoTen;
            cacDong[soDong].daHuy = p->dk.huyDangKy;
            cacDong[soDong].trangThai = p->dk.huyDangKy ? "Đã hủy đăng ký" : "Còn đăng ký";
            soDong++;
        }

        int trang = 0;
        int soDongMoiTrang = 13;
        int tongSoTrang = (soDong == 0) ? 1 : (soDong + soDongMoiTrang - 1) / soDongMoiTrang;
        int trangTruoc = -1;
        NutChuNhat nutQuayLai = { 46, 23, 24, 1, 0, "[ QUAY LẠI (ESC) ]" };

        while (true) {
            if (trang != trangTruoc) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH SINH VIÊN ĐĂNG KÝ LỚP TÍN CHỈ");

                GiaoDienConsole::datMau(VANG, DEN);
                GiaoDienConsole::diDenToaDo(4, 4);
                cout << "Môn học: " << tenMonHoc << " (" << lopTinChi->maMH << ")  |  Niên khóa: " << lopTinChi->nienKhoa
                     << "  |  Học kỳ: " << lopTinChi->hocKy << "  |  Nhóm: " << lopTinChi->nhom;

                GiaoDienConsole::datMau(VANG, LAM);
                GiaoDienConsole::diDenToaDo(4, 6);
                cout << GiaoDienConsole::canPhai(" STT", 10)
                     << GiaoDienConsole::canPhai("MÃ SINH VIÊN", 22)
                     << GiaoDienConsole::canPhai("HỌ VÀ TÊN SINH VIÊN", 48)
                     << GiaoDienConsole::canPhai("TRẠNG THÁI", 28);

                int viTriDau = trang * soDongMoiTrang;
                int viTriCuoi = (viTriDau + soDongMoiTrang < soDong) ? (viTriDau + soDongMoiTrang) : soDong;

                if (soDong == 0) {
                    GiaoDienConsole::datMau(DO_NHAT, DEN);
                    GiaoDienConsole::diDenToaDo(4, 10);
                    cout << GiaoDienConsole::canGiua("Chưa có sinh viên nào đăng ký lớp tín chỉ này!", 108);
                } else {
                    for (int r = 0; r < soDongMoiTrang; ++r) {
                        int i = viTriDau + r;
                        GiaoDienConsole::diDenToaDo(4, 8 + r);
                        if (i < viTriCuoi) {
                            GiaoDienConsole::datMau(cacDong[i].daHuy ? DO_NHAT : TRANG, DEN);
                            cout << GiaoDienConsole::canPhai(" " + to_string(i + 1), 10)
                                 << GiaoDienConsole::canPhai(cacDong[i].maSV, 22)
                                 << GiaoDienConsole::canPhai(cacDong[i].hoTen, 48)
                                 << GiaoDienConsole::canPhai(cacDong[i].trangThai, 28);
                        } else {
                            cout << string(108, ' ');
                        }
                    }
                }

                GiaoDienConsole::veNut(nutQuayLai, true, TRANG, XAM_DAM, DEN, VANG);
                GiaoDienConsole::veThanhTrangThai("Mũi tên Lên/Xuống hoặc Cuộn chuột để chuyển trang | Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | ESC/Enter: Quay lại");
                trangTruoc = trang;
            }

            SuKienNhap suKien = GiaoDienConsole::docSuKien();
            if (suKien.type == SK_PHIM_ESC || suKien.type == SK_PHIM_ENTER) break;
            if (suKien.type == SK_CHUOT_TRAI && nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) break;
            if (suKien.type == EVT_KEY_PGDN || suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
                if (trang + 1 < tongSoTrang) trang++;
            } else if (suKien.type == EVT_KEY_PGUP || suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
                if (trang > 0) trang--;
            }
        }

        if (cacDong != nullptr) delete[] cacDong;
    }
}

// ============================================================================
// CHỨC NĂNG (c): NHẬP SINH VIÊN CỦA 1 LỚP (Thêm / Xóa / Hiệu chỉnh)
// ============================================================================

// ============================================================================
void chucNangE_CapNhatMonHoc(PTRMH& cayMonHoc, const DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi) {
    int mucDangChon = 0;
    int soDongMoiTrang = 14;
    string thongBao = "";
    bool coLoi = false;
    bool canVeLaiToanBo = true;
    int mucChonTruoc = -1;
    int trangTruoc = -1;
    int nutDangTro = -1;
    int nutTruocDo = -1;

    int soNut = 4;
    NutChuNhat* cacNut = new NutChuNhat[soNut];
    cacNut[0] = { 6,  23, 24, 1, 0, "[ + THÊM MỚI (Ins) ]" };
    cacNut[1] = { 34, 23, 24, 1, 1, "[ HIỆU CHỈNH (Enter) ]" };
    cacNut[2] = { 62, 23, 22, 1, 2, "[ XÓA MÔN (Del) ]" };
    cacNut[3] = { 88, 23, 22, 1, 3, "[ QUAY LẠI (ESC) ]" };

    while (true) {
        int soMonHoc = demMonHoc(cayMonHoc);
        MonHoc* mangMonHoc = (soMonHoc > 0) ? new MonHoc[soMonHoc] : nullptr;
        int idx = 0;
        bstSangMang(cayMonHoc, mangMonHoc, idx);

        if (mucDangChon >= soMonHoc) mucDangChon = (soMonHoc > 0) ? soMonHoc - 1 : 0;
        int tongSoTrang = (soMonHoc == 0) ? 1 : (soMonHoc + soDongMoiTrang - 1) / soDongMoiTrang;
        int trangHienTai = mucDangChon / soDongMoiTrang;
        int viTriDau = trangHienTai * soDongMoiTrang;
        int viTriCuoi = (viTriDau + soDongMoiTrang < soMonHoc) ? (viTriDau + soDongMoiTrang) : soMonHoc;

        if (trangHienTai != trangTruoc) canVeLaiToanBo = true;

        auto veMotDong = [&](int i, bool dangDuocChon) {
            int r = i - viTriDau;
            if (r < 0 || r >= soDongMoiTrang) return;
            GiaoDienConsole::diDenToaDo(4, 7 + r);
            if (i >= 0 && i < soMonHoc) {
                GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
                cout << GiaoDienConsole::canPhai(" " + to_string(i + 1), 8)
                     << GiaoDienConsole::canPhai(mangMonHoc[i].maMH, 18)
                     << GiaoDienConsole::canPhai(mangMonHoc[i].tenMH, 48)
                     << GiaoDienConsole::canPhai("   " + to_string(mangMonHoc[i].soTCLT), 18)
                     << GiaoDienConsole::canPhai("   " + to_string(mangMonHoc[i].soTCTH), 16);
            } else {
                GiaoDienConsole::datMau(XAM_NHAT, DEN);
                cout << string(108, ' ');
            }
        };

        if (canVeLaiToanBo) {
            GiaoDienConsole::xoaManHinh();
            GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH MÔN HỌC");

            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 5);
            cout << GiaoDienConsole::canPhai(" STT", 8)
                 << GiaoDienConsole::canPhai("MÃ MÔN HỌC", 18)
                 << GiaoDienConsole::canPhai("TÊN MÔN HỌC", 48)
                 << GiaoDienConsole::canPhai("STC LÝ THUYẾT", 18)
                 << GiaoDienConsole::canPhai("STC THỰC HÀNH", 16);

            for (int r = 0; r < soDongMoiTrang; ++r) {
                int i = viTriDau + r;
                veMotDong(i, i == mucDangChon);
            }

            for (int i = 0; i < soNut; ++i) {
                GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
            }

            GiaoDienConsole::veThanhTrangThai("Ins: Thêm môn học | Enter: Hiệu chỉnh | Del: Xóa môn học | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang) + " | ESC: Quay lại", thongBao, coLoi);
            thongBao = "";
            canVeLaiToanBo = false;
            mucChonTruoc = mucDangChon;
            trangTruoc = trangHienTai;
            nutTruocDo = nutDangTro;
        } else {
            if (mucDangChon != mucChonTruoc) {
                veMotDong(mucChonTruoc, false);
                veMotDong(mucDangChon, true);
                mucChonTruoc = mucDangChon;
            }
            if (nutDangTro != nutTruocDo) {
                for (int i = 0; i < soNut; ++i) {
                    GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
                }
                nutTruocDo = nutDangTro;
            }
        }

        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        int hanhDong = -1;

        if (suKien.type == SK_CHUOT_DI_CHUYEN) {
            nutDangTro = -1;
            for (int i = 0; i < soNut; ++i) {
                if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                    nutDangTro = i;
                    break;
                }
            }
            if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                int viTriClick = viTriDau + (suKien.toaDoChuotY - 7);
                if (viTriClick < viTriCuoi) mucDangChon = viTriClick;
            }
            if (mangMonHoc) delete[] mangMonHoc;
            continue;
        } else if (suKien.type == SK_CHUOT_TRAI) {
            for (int i = 0; i < soNut; ++i) {
                if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                    hanhDong = cacNut[i].id;
                    break;
                }
            }
            if (hanhDong == -1 && suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                int viTriClick = viTriDau + (suKien.toaDoChuotY - 7);
                if (viTriClick < viTriCuoi) {
                    if (mucDangChon == viTriClick) hanhDong = 1;
                    else mucDangChon = viTriClick;
                }
            }
        }

        if (suKien.type == SK_PHIM_ESC || hanhDong == 3) {
            if (mangMonHoc) delete[] mangMonHoc;
            delete[] cacNut;
            luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
            break;
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
            if (soMonHoc > 0) mucDangChon = (mucDangChon - 1 + soMonHoc) % soMonHoc;
        } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
            if (soMonHoc > 0) mucDangChon = (mucDangChon + 1) % soMonHoc;
        } else if (suKien.type == EVT_KEY_INSERT || hanhDong == 0) {
            TruongNhap* f = new TruongNhap[4];
            f[0] = { "Mã môn học", "", 10, TRUONG_MA, 0, 0, 0 };
            f[1] = { "Tên môn học", "", 50, TRUONG_CHU, 0, 0, 0 };
            f[2] = { "Số tín chỉ lý thuyết", "3", 2, TRUONG_SO, 0, 0, 0 };
            f[3] = { "Số tín chỉ thực hành", "1", 2, TRUONG_SO, 0, 0, 0 };

            string thongBaoLoi = "";
            while (true) {
                if (!chayBieuMau("THÊM MÔN HỌC MỚI", f, 4, thongBaoLoi)) break;

                string ma = GiaoDienConsole::vietHoa(f[0].value);
                string ten = GiaoDienConsole::catKhoangTrang(f[1].value);
                int tinChiLyThuyet = atoi(f[2].value.c_str());
                int tinChiThucHanh = atoi(f[3].value.c_str());

                if (ma.empty() || ten.empty()) {
                    thongBaoLoi = "Mã môn học và Tên môn học không được để trống!";
                    continue;
                }
                if (timMonHoc(cayMonHoc, ma.c_str()) != nullptr) {
                    thongBaoLoi = "Mã môn học '" + ma + "' đã tồn tại!";
                    continue;
                }
                if (tinChiLyThuyet < 0 || tinChiThucHanh < 0 || (tinChiLyThuyet + tinChiThucHanh) <= 0) {
                    thongBaoLoi = "Tổng số tín chỉ phải lớn hơn 0!";
                    continue;
                }

                themMonHoc(cayMonHoc, taoMonHoc(ma.c_str(), ten.c_str(), tinChiLyThuyet, tinChiThucHanh));
                luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                thongBao = "Đã thêm môn học " + ma + " - " + ten + " thành công!";
                coLoi = false;
                break;
            }
            delete[] f;
            canVeLaiToanBo = true;
        } else if ((suKien.type == SK_PHIM_ENTER || hanhDong == 1) && soMonHoc > 0) {
            MonHoc monHocHienTai = mangMonHoc[mucDangChon];
            TruongNhap* f = new TruongNhap[4];
            f[0] = { "Mã môn học", monHocHienTai.maMH, 10, TRUONG_CHI_DOC, 0, 0, 0 };
            f[1] = { "Tên môn học", monHocHienTai.tenMH, 50, TRUONG_CHU, 0, 0, 0 };
            f[2] = { "Số tín chỉ lý thuyết", to_string(monHocHienTai.soTCLT), 2, TRUONG_SO, 0, 0, 0 };
            f[3] = { "Số tín chỉ thực hành", to_string(monHocHienTai.soTCTH), 2, TRUONG_SO, 0, 0, 0 };

            string thongBaoLoi = "";
            while (true) {
                if (!chayBieuMau("HIỆU CHỈNH MÔN HỌC: " + string(monHocHienTai.maMH), f, 4, thongBaoLoi)) break;

                string ten = GiaoDienConsole::catKhoangTrang(f[1].value);
                int tinChiLyThuyet = atoi(f[2].value.c_str());
                int tinChiThucHanh = atoi(f[3].value.c_str());

                if (ten.empty()) {
                    thongBaoLoi = "Tên môn học không được để trống!";
                    continue;
                }
                if (tinChiLyThuyet < 0 || tinChiThucHanh < 0 || (tinChiLyThuyet + tinChiThucHanh) <= 0) {
                    thongBaoLoi = "Tổng số tín chỉ (soTCLT + soTCTH) phải lớn hơn 0!";
                    continue;
                }

                NodeMH* nutMonHoc = timMonHoc(cayMonHoc, monHocHienTai.maMH);
                if (nutMonHoc != nullptr) {
                    strncpy(nutMonHoc->mh.tenMH, ten.c_str(), MAX_TENMH - 1);
                    nutMonHoc->mh.tenMH[MAX_TENMH - 1] = '\0';
                    nutMonHoc->mh.soTCLT = tinChiLyThuyet;
                    nutMonHoc->mh.soTCTH = tinChiThucHanh;
                    luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                    thongBao = "Đã hiệu chỉnh môn học " + string(monHocHienTai.maMH) + " thành công!";
                    coLoi = false;
                }
                break;
            }
            delete[] f;
            canVeLaiToanBo = true;
        } else if ((suKien.type == EVT_KEY_DELETE || hanhDong == 2) && soMonHoc > 0) {
            string ma = mangMonHoc[mucDangChon].maMH;
            if (kiemTraMonHocDaMoLop(danhSachLopTinChi, ma.c_str())) {
                thongBao = "Không thể xóa môn học '" + ma + "' vì đã được mở Lớp tín chỉ!";
                coLoi = true;
            } else {
                if (GiaoDienConsole::hopXacNhan("XÁC NHẬN XÓA MÔN HỌC",
                                             "Bạn có chắc chắn muốn xóa môn học " + ma + " (" + string(mangMonHoc[mucDangChon].tenMH) + ")?")) {
                    xoaMonHoc(cayMonHoc, ma.c_str());
                    luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                    thongBao = "Đã xóa môn học " + ma + " khỏi cây BST!";
                    coLoi = false;
                }
            }
            canVeLaiToanBo = true;
        }

        if (mangMonHoc) delete[] mangMonHoc;
    }
}

// ============================================================================
// CHỨC NĂNG (f): IN DANH SÁCH MÔN HỌC TĂNG DẦN THEO TÊN MÔN HỌC
// ============================================================================

// ============================================================================
void chucNangF_InDSMonHocTheoTen(PTRMH cayMonHoc) {
    int soMonHoc = demMonHoc(cayMonHoc);
    MonHoc* mangMonHoc = (soMonHoc > 0) ? new MonHoc[soMonHoc] : nullptr;
    int idx = 0;
    bstSangMang(cayMonHoc, mangMonHoc, idx);
    sapXepMonHocTheoTen(mangMonHoc, soMonHoc);

    int trang = 0;
    int soDongMoiTrang = 14;
    int tongSoTrang = (soMonHoc == 0) ? 1 : (soMonHoc + soDongMoiTrang - 1) / soDongMoiTrang;
    int trangTruoc = -1;
    NutChuNhat nutQuayLai = { 46, 23, 24, 1, 0, "[ QUAY LẠI (ESC) ]" };

    while (true) {
        if (trang != trangTruoc) {
            GiaoDienConsole::xoaManHinh();
            GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH MÔN HỌC");

            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 5);
            cout << GiaoDienConsole::canPhai(" STT", 8)
                 << GiaoDienConsole::canPhai("MÃ MÔN HỌC", 18)
                 << GiaoDienConsole::canPhai("TÊN MÔN HỌC", 48)
                 << GiaoDienConsole::canPhai("STC LÝ THUYẾT", 18)
                 << GiaoDienConsole::canPhai("STC THỰC HÀNH", 16);

            int viTriDau = trang * soDongMoiTrang;
            int viTriCuoi = (viTriDau + soDongMoiTrang < soMonHoc) ? (viTriDau + soDongMoiTrang) : soMonHoc;

            for (int r = 0; r < soDongMoiTrang; ++r) {
                int i = viTriDau + r;
                GiaoDienConsole::diDenToaDo(4, 7 + r);
                if (i < viTriCuoi) {
                    GiaoDienConsole::datMau(TRANG, DEN);
                    cout << GiaoDienConsole::canPhai(" " + to_string(i + 1), 8)
                         << GiaoDienConsole::canPhai(mangMonHoc[i].maMH, 18)
                         << GiaoDienConsole::canPhai(mangMonHoc[i].tenMH, 48)
                         << GiaoDienConsole::canPhai("   " + to_string(mangMonHoc[i].soTCLT), 18)
                         << GiaoDienConsole::canPhai("   " + to_string(mangMonHoc[i].soTCTH), 16);
                } else {
                    cout << string(108, ' ');
                }
            }

            GiaoDienConsole::veNut(nutQuayLai, true, TRANG, XAM_DAM, DEN, VANG);
            GiaoDienConsole::veThanhTrangThai("Mũi tên Lên/Xuống hoặc Cuộn chuột để chuyển trang | Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | ESC/Enter: Quay lại");
            trangTruoc = trang;
        }

        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        if (suKien.type == SK_PHIM_ESC || suKien.type == SK_PHIM_ENTER) break;
        if (suKien.type == SK_CHUOT_TRAI && nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) break;
        if (suKien.type == SK_PHIM_XUONG || suKien.type == EVT_KEY_PGDN || suKien.type == SK_CON_LAN_XUONG) {
            if (trang + 1 < tongSoTrang) trang++;
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == EVT_KEY_PGUP || suKien.type == SK_CON_LAN_LEN) {
            if (trang > 0) trang--;
        }
    }

    if (mangMonHoc) delete[] mangMonHoc;
}

// ============================================================================
// CHỨC NĂNG (g): ĐĂNG KÝ LỚP TÍN CHỈ
// ============================================================================

// ============================================================================
void chucNangG_DangKyLopTinChi(PTRMH cayMonHoc, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi) {
    TruongNhap* fSV = new TruongNhap[1];
    fSV[0] = { "Nhập Mã Sinh Viên", "N22DCCN001", 15, TRUONG_MA, 0, 0, 0 };
    string thongBaoLoiSinhVien = "";
    NodeSV* nutSinhVien = nullptr;
    int viTriLop = -1;

    GiaoDienConsole::xoaManHinh();
    while (true) {
        if (!chayBieuMau("TRA CỨU THÔNG TIN SINH VIÊN", fSV, 1, thongBaoLoiSinhVien)) {
            delete[] fSV;
            return;
        }
        string maSV = GiaoDienConsole::vietHoa(fSV[0].value);
        if (maSV.empty()) {
            thongBaoLoiSinhVien = "Mã sinh viên không được để trống!";
            continue;
        }
        nutSinhVien = timSinhVienToanTruong(danhSachLop, maSV, &viTriLop);
        if (nutSinhVien == nullptr) {
            thongBaoLoiSinhVien = "Không tìm thấy sinh viên có mã '" + maSV + "'!";
            continue;
        }
        break;
    }
    delete[] fSV;

    SinhVien& sv = nutSinhVien->sv;
    Lop& lopSinhVien = danhSachLop.lop[viTriLop];

    TruongNhap* fHK = new TruongNhap[2];
    fHK[0] = { "Niên khóa (VD: 2025-2026)", "2025-2026", 11, TRUONG_MA, 0, 0, 0 };
    fHK[1] = { "Học kỳ (1, 2, 3)", "1", 1, TRUONG_SO, 0, 0, 0 };
    string thongBaoLoiHocKy = "";

    while (true) {
        GiaoDienConsole::xoaManHinh();
        GiaoDienConsole::veKhung(12, 3, 92, 5, VANG, LAM, "THÔNG TIN SINH VIÊN ĐĂNG KÝ");
        GiaoDienConsole::datMau(TRANG, LAM);
        GiaoDienConsole::diDenToaDo(15, 5);
        cout << "Mã SV: " << sv.maSV << "  |  Họ tên: " << sv.ho << " " << sv.ten
             << "  |  Phái: " << sv.phai << "  |  Lớp: " << lopSinhVien.maLop;

        if (!chayBieuMau("NHẬP NIÊN KHÓA VÀ HỌC KỲ ĐỂ LỌC LỚP TÍN CHỈ", fHK, 2, thongBaoLoiHocKy)) {
            delete[] fHK;
            return;
        }
        string nienKhoa = GiaoDienConsole::catKhoangTrang(fHK[0].value);
        int hocKy = atoi(fHK[1].value.c_str());
        if (nienKhoa.empty() || hocKy < 1 || hocKy > 3) {
            thongBaoLoiHocKy = "Vui lòng nhập Niên khóa hợp lệ và Học kỳ (1, 2 hoặc 3)!";
            continue;
        }

        int* viTriLopTinChiDaLoc = new int[danhSachLopTinChi.soLuong + 1];
        int soMucPhuHop = 0;
        for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
            if (!danhSachLopTinChi.lopTC[i]->huyLop &&
                _stricmp(danhSachLopTinChi.lopTC[i]->nienKhoa, nienKhoa.c_str()) == 0 &&
                danhSachLopTinChi.lopTC[i]->hocKy == hocKy) {
                viTriLopTinChiDaLoc[soMucPhuHop++] = i;
            }
        }

        int mucDangChon = 0;
        int mucChonTruoc = -1;
        string thongBao = "";
        bool coLoi = false;
        bool canVeLaiToanBo = true;
        int nutDangTro = -1;
        int nutTruocDo = -1;

        NutChuNhat* cacNut = new NutChuNhat[2];
        cacNut[0] = { 22, 23, 34, 1, 0, "[ ĐĂNG KÝ / HỦY ĐK (Enter) ]" };
        cacNut[1] = { 62, 23, 28, 1, 1, "[ ĐỔI HỌC KỲ / THOÁT (ESC) ]" };

        auto veMotDong = [&](int r, bool dangDuocChon) {
            if (r < 0 || r >= soMucPhuHop || r >= 11) return;
            LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChiDaLoc[r]];
            NodeMH* mh = timMonHoc(cayMonHoc, lopTinChi->maMH);
            string tenMonHoc = mh ? mh->mh.tenMH : lopTinChi->maMH;
            int daDK = demSVDangKyHieuLuc(lopTinChi->dssvdk);
            int conTrong = lopTinChi->maxSV - daDK;
            if (conTrong < 0) conTrong = 0;

            NodeDK* nutDangKy = timDangKy(lopTinChi->dssvdk, sv.maSV);
            bool dangDK = (nutDangKy != nullptr && !nutDangKy->dk.huyDangKy);

            GiaoDienConsole::datMau(dangDuocChon ? DEN : (dangDK ? XANH_LA_NHAT : TRANG), dangDuocChon ? VANG : DEN);
            GiaoDienConsole::diDenToaDo(4, 11 + r);
            cout << GiaoDienConsole::canPhai(lopTinChi->maMH, 12)
                 << GiaoDienConsole::canPhai(tenMonHoc, 36)
                 << GiaoDienConsole::canPhai("  " + to_string(lopTinChi->nhom), 8)
                 << GiaoDienConsole::canPhai("    " + to_string(daDK), 16)
                 << GiaoDienConsole::canPhai("      " + to_string(conTrong), 16)
                 << GiaoDienConsole::canPhai(dangDK ? "[ĐÃ ĐĂNG KÝ]" : "Chưa đăng ký", 20);
        };

        while (true) {
            if (canVeLaiToanBo) {
                GiaoDienConsole::xoaManHinh();

                GiaoDienConsole::veKhung(1, 2, 114, 4, VANG, LAM, "THÔNG TIN SINH VIÊN");
                GiaoDienConsole::datMau(TRANG, LAM);
                GiaoDienConsole::diDenToaDo(4, 4);
                cout << "Mã SV: " << sv.maSV << "  |  Họ tên: " << sv.ho << " " << sv.ten
                     << "  |  Phái: " << sv.phai << "  |  Lớp: " << lopSinhVien.maLop << "  |  SĐT: " << sv.soDT;

                GiaoDienConsole::veKhung(1, 7, 114, 19, XANH_NGOC_NHAT, DEN, "DANH SÁCH LỚP TÍN CHỈ ĐÃ MỞ TRONG NIÊN KHÓA " + nienKhoa + " - HỌC KỲ " + to_string(hocKy));
                GiaoDienConsole::datMau(VANG, LAM);
                GiaoDienConsole::diDenToaDo(4, 9);
                cout << GiaoDienConsole::canPhai("MÃ MH", 12)
                     << GiaoDienConsole::canPhai("TÊN MÔN HỌC", 36)
                     << GiaoDienConsole::canPhai("NHÓM", 8)
                     << GiaoDienConsole::canPhai("SỐ SV ĐÃ ĐK", 16)
                     << GiaoDienConsole::canPhai("SLOT CÒN TRỐNG", 16)
                     << GiaoDienConsole::canPhai("TRẠNG THÁI CỦA BẠN", 20);

                if (soMucPhuHop == 0) {
                    GiaoDienConsole::datMau(DO_NHAT, DEN);
                    GiaoDienConsole::diDenToaDo(4, 14);
                    cout << GiaoDienConsole::canGiua("Không có lớp tín chỉ nào đang mở trong Niên khóa và Học kỳ này!", 108);
                } else {
                    for (int r = 0; r < soMucPhuHop && r < 11; ++r) {
                        veMotDong(r, r == mucDangChon);
                    }
                }

                for (int i = 0; i < 2; ++i) {
                    GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, i == 0 ? XANH_LA : XAM_DAM, DEN, VANG);
                }

                GiaoDienConsole::veThanhTrangThai("Click chuột vào lớp tín chỉ hoặc nhấn Enter để Đăng ký / Hủy đăng ký | ESC: Quay lại", thongBao, coLoi);
                thongBao = "";
                canVeLaiToanBo = false;
                mucChonTruoc = mucDangChon;
                nutTruocDo = nutDangTro;
            } else {
                if (mucDangChon != mucChonTruoc) {
                    veMotDong(mucChonTruoc, false);
                    veMotDong(mucDangChon, true);
                    mucChonTruoc = mucDangChon;
                }
                if (nutDangTro != nutTruocDo) {
                    for (int i = 0; i < 2; ++i) {
                        GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, i == 0 ? XANH_LA : XAM_DAM, DEN, VANG);
                    }
                    nutTruocDo = nutDangTro;
                }
            }

            SuKienNhap suKien = GiaoDienConsole::docSuKien();
            int hanhDong = -1;

            if (suKien.type == SK_CHUOT_DI_CHUYEN) {
                nutDangTro = -1;
                for (int i = 0; i < 2; ++i) {
                    if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                        nutDangTro = i;
                        break;
                    }
                }
                if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 11 && suKien.toaDoChuotY < 11 + soMucPhuHop && (suKien.toaDoChuotY - 11) < 11) {
                    mucDangChon = suKien.toaDoChuotY - 11;
                }
                continue;
            } else if (suKien.type == SK_CHUOT_TRAI) {
                if (cacNut[0].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) hanhDong = 0;
                else if (cacNut[1].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) hanhDong = 1;
                else if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 11 && suKien.toaDoChuotY < 11 + soMucPhuHop && (suKien.toaDoChuotY - 11) < 11) {
                    mucDangChon = suKien.toaDoChuotY - 11;
                    hanhDong = 0;
                }
            }

            if (suKien.type == SK_PHIM_ESC || hanhDong == 1) {
                luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                delete[] cacNut;
                break;
            } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
                if (soMucPhuHop > 0) mucDangChon = (mucDangChon - 1 + soMucPhuHop) % soMucPhuHop;
            } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
                if (soMucPhuHop > 0) mucDangChon = (mucDangChon + 1) % soMucPhuHop;
            } else if ((suKien.type == SK_PHIM_ENTER || hanhDong == 0) && soMucPhuHop > 0) {
                LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChiDaLoc[mucDangChon]];
                NodeDK* nutDangKy = timDangKy(lopTinChi->dssvdk, sv.maSV);
                bool dangDK = (nutDangKy != nullptr && !nutDangKy->dk.huyDangKy);

                if (dangDK) {
                    if (nutDangKy->dk.diem >= 0.0f) {
                        thongBao = "Không thể hủy đăng ký vì sinh viên đã có điểm môn học này!";
                        coLoi = true;
                    } else if (GiaoDienConsole::hopXacNhan("XÁC NHẬN HỦY ĐĂNG KÝ",
                                                        "Bạn muốn hủy đăng ký môn " + string(lopTinChi->maMH) + " - Nhóm " + to_string(lopTinChi->nhom) + "?")) {
                        nutDangKy->dk.huyDangKy = true;
                        luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                        thongBao = "Đã hủy đăng ký lớp tín chỉ môn " + string(lopTinChi->maMH) + "!";
                        coLoi = false;
                    }
                } else {
                    bool trungMon = false;
                    for (int k = 0; k < soMucPhuHop; ++k) {
                        if (k == mucDangChon) continue;
                        LopTinChi* other = danhSachLopTinChi.lopTC[viTriLopTinChiDaLoc[k]];
                        if (strcmp(other->maMH, lopTinChi->maMH) == 0) {
                            NodeDK* od = timDangKy(other->dssvdk, sv.maSV);
                            if (od != nullptr && !od->dk.huyDangKy) {
                                trungMon = true;
                                break;
                            }
                        }
                    }
                    int daDK = demSVDangKyHieuLuc(lopTinChi->dssvdk);
                    if (trungMon) {
                        thongBao = "Sinh viên đã đăng ký nhóm khác của môn " + string(lopTinChi->maMH) + " trong học kỳ này!";
                        coLoi = true;
                    } else if (daDK >= lopTinChi->maxSV) {
                        thongBao = "Lớp tín chỉ này đã hết slot trống!";
                        coLoi = true;
                    } else {
                        themDangKy(lopTinChi->dssvdk, taoDangKy(sv.maSV, -1.0f, false));
                        luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                        thongBao = "Đăng ký thành công môn " + string(lopTinChi->maMH) + " - Nhóm " + to_string(lopTinChi->nhom) + "!";
                        coLoi = false;
                    }
                }
                canVeLaiToanBo = true;
            }
        }
        delete[] viTriLopTinChiDaLoc;
    }
    delete[] fHK;
}

// ============================================================================
// CHỨC NĂNG (h): HỦY LỚP TÍN CHỈ (Số SV ĐK < Số SV Min)
// ============================================================================

// ============================================================================
void chucNangH_HuyLopTinChi(PTRMH cayMonHoc, const DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi) {
    TruongNhap* f = new TruongNhap[2];
    f[0] = { "Niên khóa", "2025-2026", 11, TRUONG_MA, 0, 0, 0 };
    f[1] = { "Học kỳ", "1", 1, TRUONG_SO, 0, 0, 0 };
    string thongBaoLoi = "";

    while (true) {
        GiaoDienConsole::xoaManHinh();
        if (!chayBieuMau("NHẬP NIÊN KHÓA VÀ HỌC KỲ CẦN RÀ SOÁT HỦY LỚP", f, 2, thongBaoLoi)) break;

        string nienKhoa = GiaoDienConsole::catKhoangTrang(f[0].value);
        int hocKy = atoi(f[1].value.c_str());
        if (nienKhoa.empty() || hocKy < 1 || hocKy > 3) {
            thongBaoLoi = "Vui lòng nhập Niên khóa hợp lệ và Học kỳ từ 1 đến 3!";
            continue;
        }

        int* danhSachLopBiHuy = new int[danhSachLopTinChi.soLuong + 1];
        int soLopTinChiHuy = 0;
        for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
            LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[i];
            if (!lopTinChi->huyLop &&
                _stricmp(lopTinChi->nienKhoa, nienKhoa.c_str()) == 0 &&
                lopTinChi->hocKy == hocKy) {
                int soDangKy = demSVDangKyHieuLuc(lopTinChi->dssvdk);
                if (soDangKy < lopTinChi->minSV) {
                    danhSachLopBiHuy[soLopTinChiHuy++] = i;
                }
            }
        }

        GiaoDienConsole::xoaManHinh();
        GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH CÁC LỚP TÍN CHỈ CÓ SỐ SV ĐĂNG KÝ < SỐ SV MIN");

        GiaoDienConsole::datMau(VANG, LAM);
        GiaoDienConsole::diDenToaDo(4, 5);
        cout << GiaoDienConsole::canPhai("MÃ LTC", 10)
             << GiaoDienConsole::canPhai("MÃ MH", 14)
             << GiaoDienConsole::canPhai("TÊN MÔN HỌC", 42)
             << GiaoDienConsole::canPhai("NHÓM", 10)
             << GiaoDienConsole::canPhai("SỐ SV ĐÃ ĐK", 16)
             << GiaoDienConsole::canPhai("SỐ SV MIN", 16);

        if (soLopTinChiHuy == 0) {
            GiaoDienConsole::datMau(XANH_LA_NHAT, DEN);
            GiaoDienConsole::diDenToaDo(4, 11);
            cout << GiaoDienConsole::canGiua("Tất cả các lớp tín chỉ trong Niên khóa " + nienKhoa + " - Học kỳ " + to_string(hocKy) + " đều đủ sĩ số tối thiểu!", 108);
            GiaoDienConsole::hopThongBao("THÔNG BÁO", "Không có lớp tín chỉ nào có Số SV đăng ký < Số SV Min!");
        } else {
            for (int r = 0; r < soLopTinChiHuy && r < 14; ++r) {
                LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[danhSachLopBiHuy[r]];
                NodeMH* mh = timMonHoc(cayMonHoc, lopTinChi->maMH);
                string tenMonHoc = mh ? mh->mh.tenMH : lopTinChi->maMH;
                int soDangKy = demSVDangKyHieuLuc(lopTinChi->dssvdk);
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                GiaoDienConsole::diDenToaDo(4, 7 + r);
                cout << GiaoDienConsole::canPhai(" " + to_string(lopTinChi->maLopTC), 10)
                     << GiaoDienConsole::canPhai(lopTinChi->maMH, 14)
                     << GiaoDienConsole::canPhai(tenMonHoc, 42)
                     << GiaoDienConsole::canPhai("  " + to_string(lopTinChi->nhom), 10)
                     << GiaoDienConsole::canPhai("    " + to_string(soDangKy), 16)
                     << GiaoDienConsole::canPhai("    " + to_string(lopTinChi->minSV), 16);
            }

            if (GiaoDienConsole::hopXacNhan("XÁC NHẬN HỦY LỚP TÍN CHỈ",
                                         "Tìm thấy " + to_string(soLopTinChiHuy) + " lớp tín chỉ có số SV ĐK < số SV Min.",
                                         "Bạn có xác nhận HỦY các lớp tín chỉ này không?")) {
                for (int r = 0; r < soLopTinChiHuy; ++r) {
                    danhSachLopTinChi.lopTC[danhSachLopBiHuy[r]]->huyLop = true;
                }
                luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                GiaoDienConsole::hopThongBao("THÀNH CÔNG", "Đã hủy " + to_string(soLopTinChiHuy) + " lớp tín chỉ và lưu dữ liệu thành công!");
            }
        }

        delete[] danhSachLopBiHuy;
    }
    delete[] f;
}

// ============================================================================
// CHỨC NĂNG (i): NHẬP ĐIỂM TRỰC TIẾP TRÊN KHUNG CỘT ĐIỂM (KHÔNG NHẤP NHÁY)
// ============================================================================


