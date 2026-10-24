#include "ui/GiaoDienConsole.h"
#include "features/HamDungChung.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"
#include "storage/DocGhiDuLieu.h"
using namespace std;

string dinhDangDiem(float diem, bool deTrongNeuChuaCo) {
    if (diem < 0.0f) return deTrongNeuChuaCo ? "" : "Chưa chấm";
    stringstream ss;
    ss << fixed << setprecision(2) << diem;
    string s = ss.str();
    while (s.size() > 1 && s.back() == '0' && s.find('.') != string::npos) {
        s.pop_back();
    }
    if (!s.empty() && s.back() == '.') s.pop_back();
    return s;
}

// ============================================================================
// HỘP THOẠI CHỌN NHANH MÃ LỚP (Không nhấp nháy, chỉ vẽ lại dòng thay đổi)
// ============================================================================
string chonLopNhanh(DanhSachLop& danhSachLop, bool choPhepTaoMoi) {
    int mucDangChon = 0;
    int soDongMoiTrang = 14;
    bool canVeLaiToanBo = true;
    int mucChonTruoc = -1;
    int trangTruoc = -1;
    int nutDangTro = -1;
    int nutTruocDo = -1;

    int soNut = choPhepTaoMoi ? 4 : 3;
    NutChuNhat* cacNut = new NutChuNhat[soNut];
    cacNut[0] = { 6,  23, 22, 1, 0, "[ CHỌN LỚP (Enter) ]" };
    cacNut[1] = { 32, 23, 24, 1, 1, "[ NHẬP MÃ LỚP (F1) ]" };
    if (choPhepTaoMoi) {
        cacNut[2] = { 60, 23, 25, 1, 2, "[ + THÊM LỚP MỚI (Ins) ]" };
        cacNut[3] = { 89, 23, 20, 1, 3, "[ QUAY LẠI (ESC) ]" };
    } else {
        cacNut[2] = { 60, 23, 20, 1, 2, "[ QUAY LẠI (ESC) ]" };
    }

    auto veMotDong = [&](int idx, int viTriDau, bool dangDuocChon) {
        int r = idx - viTriDau;
        if (r < 0 || r >= soDongMoiTrang) return;
        int toaDoDong = 7 + r;
        GiaoDienConsole::diDenToaDo(4, toaDoDong);
        if (idx >= 0 && idx < danhSachLop.soLuong) {
            GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
            int soSV = demSVTrongLop(danhSachLop.lop[idx].dssv);
            cout << GiaoDienConsole::canPhai(" " + to_string(idx + 1), 8)
                 << GiaoDienConsole::canPhai(danhSachLop.lop[idx].maLop, 20)
                 << GiaoDienConsole::canPhai(danhSachLop.lop[idx].tenLop, 58)
                 << GiaoDienConsole::canPhai(to_string(soSV) + " sinh viên", 22);
        } else {
            GiaoDienConsole::datMau(XAM_NHAT, DEN);
            cout << string(108, ' ');
        }
    };

    while (true) {
        if (mucDangChon >= danhSachLop.soLuong) mucDangChon = (danhSachLop.soLuong > 0) ? danhSachLop.soLuong - 1 : 0;
        int tongSoTrang = (danhSachLop.soLuong == 0) ? 1 : (danhSachLop.soLuong + soDongMoiTrang - 1) / soDongMoiTrang;
        int trangHienTai = mucDangChon / soDongMoiTrang;
        int viTriDau = trangHienTai * soDongMoiTrang;
        int viTriCuoi = (viTriDau + soDongMoiTrang < danhSachLop.soLuong) ? (viTriDau + soDongMoiTrang) : danhSachLop.soLuong;

        if (trangHienTai != trangTruoc) canVeLaiToanBo = true;

        if (canVeLaiToanBo) {
            GiaoDienConsole::xoaManHinh();
            GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH CÁC LỚP HỌC HIỆN CÓ");

            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 5);
            cout << GiaoDienConsole::canPhai(" STT", 8)
                 << GiaoDienConsole::canPhai("MÃ LỚP", 20)
                 << GiaoDienConsole::canPhai("TÊN LỚP HỌC", 58)
                 << GiaoDienConsole::canPhai("SỈ SỐ LỚP", 22);

            for (int r = 0; r < soDongMoiTrang; ++r) {
                int idx = viTriDau + r;
                veMotDong(idx, viTriDau, idx == mucDangChon);
            }

            for (int i = 0; i < soNut; ++i) {
                GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
            }

            GiaoDienConsole::veThanhTrangThai("Mũi tên Lên/Xuống hoặc Click chuột vào dòng để chọn Lớp | F1: Nhập Mã Lớp | ESC: Thoát | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang));
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
        int nutDuocBam = -1;

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
                    nutDuocBam = cacNut[i].id;
                    break;
                }
            }
            if (nutDuocBam == -1 && suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                int idx = viTriDau + (suKien.toaDoChuotY - 7);
                if (idx < viTriCuoi) {
                    mucDangChon = idx;
                    nutDuocBam = 0; // Click vào dòng lớp -> chọn lớp đó ngay
                }
            }
        }

        if (suKien.type == SK_PHIM_ESC || nutDuocBam == 3 || (!choPhepTaoMoi && nutDuocBam == 2)) {
            delete[] cacNut;
            return "";
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
            if (danhSachLop.soLuong > 0) mucDangChon = (mucDangChon - 1 + danhSachLop.soLuong) % danhSachLop.soLuong;
        } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
            if (danhSachLop.soLuong > 0) mucDangChon = (mucDangChon + 1) % danhSachLop.soLuong;
        } else if (suKien.type == SK_PHIM_ENTER || nutDuocBam == 0) {
            if (danhSachLop.soLuong > 0) {
                string res = danhSachLop.lop[mucDangChon].maLop;
                delete[] cacNut;
                return res;
            }
        } else if (suKien.type == EVT_KEY_F1 || nutDuocBam == 1) {
            TruongNhap* f = new TruongNhap[1];
            f[0] = { "Nhập Mã Lớp", "", 15, TRUONG_MA, 0, 0, 0 };
            string thongBaoLoi = "";
            while (true) {
                if (!chayBieuMau("NHẬP MÃ LỚP CẦN TRA CỨU", f, 1, thongBaoLoi)) break;
                string ma = GiaoDienConsole::vietHoa(f[0].value);
                if (ma.empty()) {
                    thongBaoLoi = "Mã lớp không được để trống!";
                    continue;
                }
                int idx = timLop(danhSachLop, ma);
                if (idx == -1) {
                    thongBaoLoi = "Không tìm thấy lớp '" + ma + "' trong danh sách!";
                    continue;
                }
                delete[] f;
                delete[] cacNut;
                return ma;
            }
            delete[] f;
            canVeLaiToanBo = true;
        } else if (choPhepTaoMoi && (suKien.type == EVT_KEY_INSERT || nutDuocBam == 2)) {
            if (danhSachLop.soLuong >= MAX_LOP) {
                GiaoDienConsole::hopThongBao("LỖI", "Danh sách lớp đã đầy (tối đa 10000 lớp)!", true);
                canVeLaiToanBo = true;
                continue;
            }
            TruongNhap* f = new TruongNhap[2];
            f[0] = { "Mã Lớp (tối đa 15 KT)", "", 15, TRUONG_MA, 0, 0, 0 };
            f[1] = { "Tên Lớp", "", 50, TRUONG_CHU, 0, 0, 0 };
            string thongBaoLoi = "";
            while (true) {
                if (!chayBieuMau("THÊM LỚP HỌC MỚI", f, 2, thongBaoLoi)) break;
                string ma = GiaoDienConsole::vietHoa(f[0].value);
                string ten = GiaoDienConsole::vietHoaDauTu(f[1].value);
                if (ma.empty() || ten.empty()) {
                    thongBaoLoi = "Vui lòng nhập đầy đủ Mã Lớp và Tên Lớp!";
                    continue;
                }
                if (timLop(danhSachLop, ma) != -1) {
                    thongBaoLoi = "Mã lớp '" + ma + "' đã tồn tại!";
                    continue;
                }
                danhSachLop.lop[danhSachLop.soLuong++] = taoLop(ma, ten);
                mucDangChon = danhSachLop.soLuong - 1;
                break;
            }
            delete[] f;
            canVeLaiToanBo = true;
        }
    }
}

// ============================================================================
// HỘP THOẠI CHỌN LỚP TÍN CHỈ THEO 4 THAM SỐ HOẶC CHỌN TRỰC TIẾP TRÊN BẢNG
// ============================================================================
int chonLopTinChiTheoThongTin(PTRMH cayMonHoc, const DanhSachLopTinChi& danhSachLopTinChi, const string& tieuDeManHinh) {
    TruongNhap* f = new TruongNhap[4];
    f[0] = { "Niên khóa (VD: 2025-2026)", "2025-2026", 11, TRUONG_MA, 0, 0, 0 };
    f[1] = { "Học kỳ (1, 2, 3)", "1", 1, TRUONG_SO, 0, 0, 0 };
    f[2] = { "Mã môn học (VD: INT1306)", "INT1306", 10, TRUONG_MA, 0, 0, 0 };
    f[3] = { "Nhóm (1, 2, ...)", "1", 2, TRUONG_SO, 0, 0, 0 };

    int mucDangChon = 0;
    int soDongMoiTrang = 14;
    bool canVeLaiToanBo = true;
    int mucChonTruoc = -1;
    int trangTruoc = -1;
    int nutDangTro = -1;
    int nutTruocDo = -1;

    NutChuNhat* cacNut = new NutChuNhat[3];
    cacNut[0] = { 10, 23, 26, 1, 0, "[ CHỌN LỚP NÀY (Enter) ]" };
    cacNut[1] = { 42, 23, 34, 1, 1, "[ NHẬP NIÊN KHÓA/HK/MH/NHÓM (F1) ]" };
    cacNut[2] = { 82, 23, 22, 1, 2, "[ QUAY LẠI (ESC) ]" };

    auto veMotDong = [&](int idx, int viTriDau, bool dangDuocChon) {
        int r = idx - viTriDau;
        if (r < 0 || r >= soDongMoiTrang) return;
        int toaDoDong = 7 + r;
        GiaoDienConsole::diDenToaDo(4, toaDoDong);
        if (idx >= 0 && idx < danhSachLopTinChi.soLuong) {
            LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[idx];
            NodeMH* mh = timMonHoc(cayMonHoc, lopTinChi->maMH);
            string tenMonHoc = (mh != nullptr) ? mh->mh.tenMH : "(Không rõ)";
            int svDK = demSVDangKyHieuLuc(lopTinChi->dssvdk);
            GiaoDienConsole::datMau(dangDuocChon ? DEN : (lopTinChi->huyLop ? DO_NHAT : TRANG), dangDuocChon ? VANG : DEN);
            cout << GiaoDienConsole::canPhai(" " + to_string(lopTinChi->maLopTC), 8)
                 << GiaoDienConsole::canPhai(lopTinChi->maMH, 11)
                 << GiaoDienConsole::canPhai(tenMonHoc, 36)
                 << GiaoDienConsole::canPhai(lopTinChi->nienKhoa, 13)
                 << GiaoDienConsole::canPhai(to_string(lopTinChi->hocKy), 7)
                 << GiaoDienConsole::canPhai(to_string(lopTinChi->nhom), 7)
                 << GiaoDienConsole::canPhai(to_string(svDK) + "/" + to_string(lopTinChi->maxSV), 12)
                 << GiaoDienConsole::canPhai(lopTinChi->huyLop ? "Đã hủy" : "Đang mở", 14);
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
            (void)tieuDeManHinh;
            GiaoDienConsole::xoaManHinh();
            GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH LỚP TÍN CHỈ");

            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 5);
            cout << GiaoDienConsole::canPhai("MÃ LTC", 8)
                 << GiaoDienConsole::canPhai("MÃ MH", 11)
                 << GiaoDienConsole::canPhai("TÊN MÔN HỌC", 36)
                 << GiaoDienConsole::canPhai("NIÊN KHÓA", 13)
                 << GiaoDienConsole::canPhai("HK", 7)
                 << GiaoDienConsole::canPhai("NHÓM", 7)
                 << GiaoDienConsole::canPhai("SV ĐK/MAX", 12)
                 << GiaoDienConsole::canPhai("TRẠNG THÁI", 14);

            for (int r = 0; r < soDongMoiTrang; ++r) {
                int idx = viTriDau + r;
                veMotDong(idx, viTriDau, idx == mucDangChon);
            }

            for (int i = 0; i < 3; ++i) {
                GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
            }
            GiaoDienConsole::veThanhTrangThai("Click chuột hoặc Lên/Xuống + Enter để chọn | F1: Nhập 4 tham số | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang) + " | ESC: Quay lại");
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
                for (int i = 0; i < 3; ++i) {
                    GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
                }
                nutTruocDo = nutDangTro;
            }
        }

        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        int nutDuocBam = -1;

        if (suKien.type == SK_CHUOT_DI_CHUYEN) {
            nutDangTro = -1;
            for (int i = 0; i < 3; ++i) {
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
            for (int i = 0; i < 3; ++i) {
                if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                    nutDuocBam = cacNut[i].id;
                    break;
                }
            }
            if (nutDuocBam == -1 && suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                int idx = viTriDau + (suKien.toaDoChuotY - 7);
                if (idx < viTriCuoi) {
                    mucDangChon = idx;
                    nutDuocBam = 0;
                }
            }
        }

        if (suKien.type == SK_PHIM_ESC || nutDuocBam == 2) {
            delete[] cacNut;
            delete[] f;
            return -1;
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
            if (danhSachLopTinChi.soLuong > 0) mucDangChon = (mucDangChon - 1 + danhSachLopTinChi.soLuong) % danhSachLopTinChi.soLuong;
        } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
            if (danhSachLopTinChi.soLuong > 0) mucDangChon = (mucDangChon + 1) % danhSachLopTinChi.soLuong;
        } else if (suKien.type == SK_PHIM_ENTER || nutDuocBam == 0) {
            if (danhSachLopTinChi.soLuong > 0) {
                delete[] cacNut;
                delete[] f;
                return mucDangChon;
            }
        } else if (suKien.type == EVT_KEY_F1 || nutDuocBam == 1) {
            if (danhSachLopTinChi.soLuong > 0 && mucDangChon >= 0 && mucDangChon < danhSachLopTinChi.soLuong) {
                f[0].value = danhSachLopTinChi.lopTC[mucDangChon]->nienKhoa;
                f[1].value = to_string(danhSachLopTinChi.lopTC[mucDangChon]->hocKy);
                f[2].value = danhSachLopTinChi.lopTC[mucDangChon]->maMH;
                f[3].value = to_string(danhSachLopTinChi.lopTC[mucDangChon]->nhom);
            }
            string thongBaoLoi = "";
            while (true) {
                if (!chayBieuMau("NHẬP THÔNG TIN LỚP TÍN CHỈ", f, 4, thongBaoLoi, "[ TÌM LỚP (Enter) ]", "[ HỦY (ESC) ]")) {
                    break;
                }
                string nienKhoa = GiaoDienConsole::catKhoangTrang(f[0].value);
                int hocKy = atoi(f[1].value.c_str());
                string maMH = GiaoDienConsole::vietHoa(f[2].value);
                int nhom = atoi(f[3].value.c_str());

                if (nienKhoa.empty() || maMH.empty() || hocKy <= 0 || nhom <= 0) {
                    thongBaoLoi = "Vui lòng nhập đầy đủ Niên khóa, Học kỳ, Mã MH, Nhóm!";
                    continue;
                }
                int idx = timLopTinChiTheoThamSo(danhSachLopTinChi, nienKhoa.c_str(), hocKy, maMH.c_str(), nhom);
                if (idx == -1) {
                    thongBaoLoi = "Không tìm thấy Lớp tín chỉ phù hợp với 4 tham số đã nhập!";
                    continue;
                }
                delete[] cacNut;
                delete[] f;
                return idx;
            }
            canVeLaiToanBo = true;
        }
    }
}


