#include "ui/GiaoDienConsole.h"
#include "features/QuanLyMonHocLopTinChi.h"
#include "model/LichSuThaoTac.h"
#include "features/TienIchChung.h"

#include "storage/DocGhiDuLieu.h"

#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>

#include <cstring>

using namespace std;

void chucNangA_MoLopTinChi(PTRMH& root, DanhSachLopTinChi& danhSachLopTinChi, DanhSachLop& danhSachLop, QuanLyLichSu& lichSu) {
    string tuKhoaNienKhoa, tuKhoaHocKy, tuKhoaMaMonHoc, tuKhoaTenMonHoc, tuKhoaNhom, tuKhoaTrangThai;
    int truongLocDangChon = 0;
    size_t viTriConTroOLoc[6] = {};
    int* cacLopTinChiLoc = new int[MAX_LOP_TC];
    int soLopTinChiLoc = 0;
    int mucDangChon = 0;
    int soDongMoiTrang = 13;
    string thongBao = "";
    bool coLoi = false;
    bool canVeLaiToanBo = true;
    bool canVeLaiCacOLoc = true;
    bool canVeLaiBang = true;
    int mucChonTruoc = -1;
    int trangTruoc = -1;
    int nutDangTro = -1;
    int nutTruocDo = -1;

    int soNut = 3;
    NutChuNhat* cacNut = new NutChuNhat[soNut];
    cacNut[0] = { 20, 23, 28, 1, 0, "[ + THÊM MỚI (Ins) ]" };
    cacNut[1] = { 50, 23, 17, 1, 4, "[ F3: HOÀN TÁC ]" };
    cacNut[2] = { 68, 23, 28, 1, 3, "[ QUAY LẠI (ESC) ]" };

    auto veMotDong = [&](int idx, int viTriDau, bool dangDuocChon) {
        int r = idx - viTriDau;
        if (r < 0 || r >= soDongMoiTrang) return;
        int toaDoDong = 8 + r;
        GiaoDienConsole::diDenToaDo(4, toaDoDong);
        if (idx >= 0 && idx < soLopTinChiLoc) {
            PTRLTC lopTinChi = danhSachLopTinChi.lopTC[cacLopTinChiLoc[idx]];
            PTRMH mh = timMonHoc(root, lopTinChi->maMH);
            string tenMonHoc = mh ? mh->mh.tenMH : "(Không rõ)";
            int soDangKy = demSVDangKyHieuLuc(lopTinChi->dssvdk);
            GiaoDienConsole::datMau(dangDuocChon ? DEN : (lopTinChi->huyLop ? DO_NHAT : TRANG), dangDuocChon ? VANG : DEN);
            cout << GiaoDienConsole::canTrai(" " + to_string(lopTinChi->maLopTC), 7)
                 << GiaoDienConsole::canTrai(lopTinChi->maMH, 10)
                 << GiaoDienConsole::canTrai(tenMonHoc, 32)
                 << GiaoDienConsole::canTrai(lopTinChi->nienKhoa, 12)
                 << GiaoDienConsole::canTrai(to_string(lopTinChi->hocKy), 5)
                 << GiaoDienConsole::canTrai(to_string(lopTinChi->nhom), 6)
                 << GiaoDienConsole::canTrai(to_string(lopTinChi->minSV), 8)
                 << GiaoDienConsole::canTrai(to_string(lopTinChi->maxSV), 8)
                 << GiaoDienConsole::canTrai(to_string(soDangKy), 8)
                 << GiaoDienConsole::canTrai(lopTinChi->huyLop ? "Đã hủy" : "Đang mở", 12);
        } else {
            GiaoDienConsole::datMau(XAM_NHAT, DEN);
            cout << string(108, ' ');
        }
    };

    auto veCacOLoc = [&]() {
        const int viTriOLoc[] = {4, 28, 34, 4, 52, 64};
        const int dongOLoc[] = {4, 4, 4, 5, 4, 5};
        const int gioiHanNhap[] = {11, 1, 10, 50, 2, 1};
        const string nhanOLoc[] = {"Niên khóa", "HK", "Mã MH", "Tên MH", "Nhóm", "TT (M/H)"};
        string* giaTriOLoc[] = {&tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom, &tuKhoaTrangThai};
        for (int i = 0; i < 6; ++i) {
            GiaoDienConsole::diDenToaDo(viTriOLoc[i], dongOLoc[i]);
            GiaoDienConsole::datMau(truongLocDangChon == i ? DEN : TRANG, truongLocDangChon == i ? VANG : XANH_NGOC);
            string nhan = nhanOLoc[i] + ": ";
            int doRongNhan = GiaoDienConsole::doDaiUtf8(nhan);
            int chieuRong = doRongNhan + gioiHanNhap[i];
            size_t dau = viTriConTroOLoc[i] > static_cast<size_t>(gioiHanNhap[i]) && gioiHanNhap[i] > 0
                ? viTriConTroOLoc[i] - static_cast<size_t>(gioiHanNhap[i]) : 0;
            string noiDung = nhan + giaTriOLoc[i]->substr(dau, static_cast<size_t>(gioiHanNhap[i]));
            int doDai = GiaoDienConsole::doDaiUtf8(noiDung);
            cout << noiDung << string(max(0, chieuRong - doDai), ' ');
            if (i == 5) {
                GiaoDienConsole::datMau(VANG, DEN);
                GiaoDienConsole::diDenToaDo(77, dongOLoc[i]);
                cout << "M=Mo, H=Huy";
            }
        }
    };

    auto datViTriConTroOLoc = [&]() {
        const int viTriOLoc[] = {4, 28, 34, 4, 52, 64};
        const int dongOLoc[] = {4, 4, 4, 5, 4, 5};
        const int gioiHanNhap[] = {11, 1, 10, 50, 2, 1};
        const string nhanOLoc[] = {"Niên khóa", "HK", "Mã MH", "Tên MH", "Nhóm", "TT (M/H)"};
        int doRongNhan = GiaoDienConsole::doDaiUtf8(nhanOLoc[truongLocDangChon] + ": ");
        int rongGiaTri = gioiHanNhap[truongLocDangChon];
        size_t dau = viTriConTroOLoc[truongLocDangChon] > static_cast<size_t>(rongGiaTri) && rongGiaTri > 0
            ? viTriConTroOLoc[truongLocDangChon] - static_cast<size_t>(rongGiaTri) : 0;
        int viTriConTro = viTriOLoc[truongLocDangChon] + doRongNhan
            + static_cast<int>(min(viTriConTroOLoc[truongLocDangChon] - dau, static_cast<size_t>(rongGiaTri)));
        GiaoDienConsole::diDenToaDo(viTriConTro, dongOLoc[truongLocDangChon]);
        GiaoDienConsole::hienConTro(true);
    };

    while (true) {
        soLopTinChiLoc = 0;
        string khoaNienKhoa = GiaoDienConsole::vietHoa(tuKhoaNienKhoa);
        string khoaMaMH = GiaoDienConsole::vietHoa(tuKhoaMaMonHoc);
        string khoaTenMH = GiaoDienConsole::vietHoa(tuKhoaTenMonHoc);
        string khoaTrangThai = GiaoDienConsole::vietHoa(tuKhoaTrangThai);
        khoaTrangThai.erase(remove_if(khoaTrangThai.begin(), khoaTrangThai.end(),
            [](unsigned char kyTu) { return kyTu == ' ' || kyTu == '-' || kyTu == '_'; }), khoaTrangThai.end());
        for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
            PTRLTC lop = danhSachLopTinChi.lopTC[i];
            if (!khoaNienKhoa.empty() && GiaoDienConsole::vietHoa(lop->nienKhoa).find(khoaNienKhoa) == string::npos) continue;
            if (!tuKhoaHocKy.empty() && to_string(lop->hocKy).find(tuKhoaHocKy) == string::npos) continue;
            if (!khoaMaMH.empty()) {
                if (GiaoDienConsole::vietHoa(lop->maMH).find(khoaMaMH) == string::npos) continue;
            }
            if (!khoaTenMH.empty()) {
                PTRMH mon = timMonHoc(root, lop->maMH);
                string tenMon = mon ? GiaoDienConsole::vietHoa(mon->mh.tenMH) : string();
                if (tenMon.find(khoaTenMH) == string::npos) continue;
            }
            if (!tuKhoaNhom.empty() && to_string(lop->nhom).find(tuKhoaNhom) == string::npos) continue;
            if (!khoaTrangThai.empty()) {
                bool locLopHuy = khoaTrangThai == "H";
                if ((khoaTrangThai != "M" && khoaTrangThai != "H") || lop->huyLop != locLopHuy) continue;
            }
            cacLopTinChiLoc[soLopTinChiLoc++] = i;
        }
        if (mucDangChon >= soLopTinChiLoc) mucDangChon = soLopTinChiLoc > 0 ? soLopTinChiLoc - 1 : 0;
        int tongSoTrang = (soLopTinChiLoc == 0) ? 1 : (soLopTinChiLoc + soDongMoiTrang - 1) / soDongMoiTrang;
        int trangHienTai = mucDangChon / soDongMoiTrang;
        int viTriDau = trangHienTai * soDongMoiTrang;
        int viTriCuoi = min(viTriDau + soDongMoiTrang, soLopTinChiLoc);

        if (trangHienTai != trangTruoc) canVeLaiToanBo = true;

        if (canVeLaiToanBo) {
            GiaoDienConsole::xoaManHinh();
            GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH LỚP TÍN CHỈ");

            veCacOLoc();

            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 6);
            cout << GiaoDienConsole::canTrai("MÃ LTC", 7)
                 << GiaoDienConsole::canTrai("MÃ MH", 10)
                 << GiaoDienConsole::canTrai("TÊN MÔN HỌC", 32)
                 << GiaoDienConsole::canTrai("NIÊN KHÓA", 12)
                 << GiaoDienConsole::canTrai("HK", 5)
                 << GiaoDienConsole::canTrai("NHÓM", 6)
                 << GiaoDienConsole::canTrai("SV MIN", 8)
                 << GiaoDienConsole::canTrai("SV MAX", 8)
                 << GiaoDienConsole::canTrai("ĐÃ ĐK", 8)
                 << GiaoDienConsole::canTrai("TRẠNG THÁI", 12);

            for (int r = 0; r < soDongMoiTrang; ++r) {
                int idx = viTriDau + r;
                veMotDong(idx, viTriDau, idx == mucDangChon);
            }
            if (soLopTinChiLoc == 0) {
                GiaoDienConsole::diDenToaDo(4, 10);
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                cout << GiaoDienConsole::canGiua("Không tìm thấy lớp tín chỉ phù hợp với điều kiện lọc.", 108);
            }

            for (int i = 0; i < soNut; ++i) {
                GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
            }

            string moTaUndo = LichSuThaoTac::moTaThaoTacGanNhat(lichSu);
            GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang) + " | F3: hoàn tác [" + moTaUndo + "] | Enter sửa | Ins mở | Esc về | Tìm không dấu", thongBao, coLoi);
            thongBao = "";
            canVeLaiToanBo = false;
            canVeLaiCacOLoc = false;
            canVeLaiBang = false;
            mucChonTruoc = mucDangChon;
            trangTruoc = trangHienTai;
            nutTruocDo = nutDangTro;
        } else {
            if (canVeLaiCacOLoc) {
                veCacOLoc();
                canVeLaiCacOLoc = false;
            }
            if (canVeLaiBang) {
                GiaoDienConsole::diDenToaDo(4, 7);
                GiaoDienConsole::datMau(XAM_NHAT, DEN);
                cout << string(108, ' ');
                for (int r = 0; r < soDongMoiTrang; ++r) {
                    int idx = viTriDau + r;
                    veMotDong(idx, viTriDau, idx == mucDangChon);
                }
                if (soLopTinChiLoc == 0) {
                    GiaoDienConsole::diDenToaDo(4, 10);
                    GiaoDienConsole::datMau(DO_NHAT, DEN);
                    cout << GiaoDienConsole::canGiua("Không tìm thấy lớp tín chỉ phù hợp với điều kiện lọc.", 108);
                }
                mucChonTruoc = mucDangChon;
                canVeLaiBang = false;
            }
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

        datViTriConTroOLoc();
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
            if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 8 && suKien.toaDoChuotY < 8 + soDongMoiTrang) {
                int idx = viTriDau + (suKien.toaDoChuotY - 8);
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
            if (suKien.toaDoChuotY == 4 || suKien.toaDoChuotY == 5) {
                const int dauOLoc[] = {4, 28, 34, 4, 52, 64};
                const int rongOLoc[] = {23, 5, 17, 58, 8, 11};
                const int dongOLoc[] = {4, 4, 4, 5, 4, 5};
                string* giaTriOLoc[] = {&tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom, &tuKhoaTrangThai};
                for (int i = 0; i < 6; ++i) {
                    if (suKien.toaDoChuotY == dongOLoc[i] && suKien.toaDoChuotX >= dauOLoc[i] && suKien.toaDoChuotX < dauOLoc[i] + rongOLoc[i]) {
                        truongLocDangChon = i;
                        viTriConTroOLoc[i] = giaTriOLoc[i]->size();
                        canVeLaiCacOLoc = true;
                    }
                }
            }
            if (hanhDong == -1 && suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 8 && suKien.toaDoChuotY < 8 + soDongMoiTrang) {
                int idx = viTriDau + (suKien.toaDoChuotY - 8);
                if (idx < viTriCuoi) {
                    if (mucDangChon == idx) hanhDong = 1;
                    else mucDangChon = idx;
                }
            }
        }

        if (suKien.type == EVT_KEY_F3 || hanhDong == 4) {
            const string moTaUndo = LichSuThaoTac::moTaThaoTacGanNhat(lichSu);
            string thongBaoUndo;
            bool daHoanTac = LichSuThaoTac::hoanTacThaoTac(
                lichSu, root, danhSachLop, danhSachLopTinChi, thongBaoUndo);
            thongBao = daHoanTac ? "Đã hoàn tác: " + moTaUndo : thongBaoUndo;
            coLoi = !daHoanTac;
            canVeLaiToanBo = true;
            continue;
        } else if (suKien.type == SK_PHIM_ESC || hanhDong == 3) {
            GiaoDienConsole::hienConTro(false);
            luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
            delete[] cacNut;
            delete[] cacLopTinChiLoc;
            break;
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
            if (soLopTinChiLoc > 0) mucDangChon = (mucDangChon - 1 + soLopTinChiLoc) % soLopTinChiLoc;
        } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
            if (soLopTinChiLoc > 0) mucDangChon = (mucDangChon + 1) % soLopTinChiLoc;
        } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_SHIFT_TAB) {
            int buoc = suKien.type == SK_PHIM_SHIFT_TAB ? -1 : 1;
            truongLocDangChon = (truongLocDangChon + buoc + 6) % 6;
            canVeLaiCacOLoc = true;
        } else if (suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI) {
            string* giaTriOLoc[] = {&tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom, &tuKhoaTrangThai};
            if (suKien.type == SK_PHIM_TRAI && viTriConTroOLoc[truongLocDangChon] > 0) --viTriConTroOLoc[truongLocDangChon];
            else if (suKien.type == SK_PHIM_PHAI && viTriConTroOLoc[truongLocDangChon] < giaTriOLoc[truongLocDangChon]->size()) ++viTriConTroOLoc[truongLocDangChon];
            canVeLaiCacOLoc = true;
        } else if (suKien.type == EVT_KEY_HOME) {
            viTriConTroOLoc[truongLocDangChon] = 0;
            canVeLaiCacOLoc = true;
        } else if (suKien.type == EVT_KEY_END) {
            string* giaTriOLoc[] = {&tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom, &tuKhoaTrangThai};
            viTriConTroOLoc[truongLocDangChon] = giaTriOLoc[truongLocDangChon]->size();
            canVeLaiCacOLoc = true;
        } else if (suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
            string* giaTriOLoc[] = {&tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom, &tuKhoaTrangThai};
            if (suKien.type == EVT_KEY_BACKSPACE && viTriConTroOLoc[truongLocDangChon] > 0) {
                giaTriOLoc[truongLocDangChon]->erase(viTriConTroOLoc[truongLocDangChon] - 1, 1);
                --viTriConTroOLoc[truongLocDangChon];
            } else if (suKien.type == EVT_KEY_DELETE && viTriConTroOLoc[truongLocDangChon] < giaTriOLoc[truongLocDangChon]->size()) {
                giaTriOLoc[truongLocDangChon]->erase(viTriConTroOLoc[truongLocDangChon], 1);
            }
            mucDangChon = 0;
            canVeLaiCacOLoc = true;
            canVeLaiBang = true;
        } else if (suKien.type == SK_KY_TU) {
            string* giaTriOLoc[] = {&tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom, &tuKhoaTrangThai};
            const int doDaiToiDa[] = {11, 1, 10, 50, 2, 1};
            char kyTu = suKien.ch;
            bool kyTuHopLe = false;
            if (truongLocDangChon == 1 || truongLocDangChon == 4) {
                kyTuHopLe = kyTu >= '0' && kyTu <= '9';
            } else if (truongLocDangChon == 5) {
                kyTu = static_cast<char>(toupper(static_cast<unsigned char>(kyTu)));
                kyTuHopLe = kyTu == 'M' || kyTu == 'H';
            } else {
                kyTuHopLe = (kyTu >= 'a' && kyTu <= 'z') || (kyTu >= 'A' && kyTu <= 'Z') ||
                    (kyTu >= '0' && kyTu <= '9') || kyTu == '-' || kyTu == '_';
                if (truongLocDangChon == 3 && kyTu == ' ') kyTuHopLe = true;
            }
            string& giaTri = *giaTriOLoc[truongLocDangChon];
            if (kyTuHopLe && static_cast<int>(giaTri.size()) < doDaiToiDa[truongLocDangChon]) {
                if (truongLocDangChon != 3 && kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
                giaTri.insert(viTriConTroOLoc[truongLocDangChon], 1, kyTu);
                ++viTriConTroOLoc[truongLocDangChon];
            }
            mucDangChon = 0;
            canVeLaiCacOLoc = true;
            canVeLaiBang = true;
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
                bool daNhanF1 = false;
                bool daLuu = chayBieuMau("MỞ LỚP TÍN CHỈ MỚI", f, 7, thongBaoLoi,
                    "[ LƯU (Enter) ]", "[ HỦY (ESC) ]", "", nullptr,
                    "[ F1: CHỌN MÔN ]", &daNhanF1);
                if (daNhanF1) {
                    string maMonHocDuocChon = chonMonHocNhanh(root);
                    if (!maMonHocDuocChon.empty()) {
                        f[1].value = maMonHocDuocChon;
                        thongBaoLoi.clear();
                    }
                    continue;
                }
                if (!daLuu) break;

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
                if (timMonHoc(root, maMH.c_str()) == nullptr) {
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

                PTRLTC lopTinChi = new LopTinChi();
                lopTinChi->maLopTC = layMaLopTCKeTiep(danhSachLopTinChi);
                strncpy(lopTinChi->maMH, maMH.c_str(), MAX_MAMH - 1); lopTinChi->maMH[MAX_MAMH - 1] = '\0';
                strncpy(lopTinChi->nienKhoa, nienKhoa.c_str(), MAX_NIENKHOA - 1); lopTinChi->nienKhoa[MAX_NIENKHOA - 1] = '\0';
                lopTinChi->hocKy = hocKy;
                lopTinChi->nhom = nhom;
                lopTinChi->minSV = soSinhVienToiThieu;
                lopTinChi->maxSV = soSinhVienToiDa;
                lopTinChi->huyLop = false;
                int viTriLopTinChiMoi = danhSachLopTinChi.soLuong;
                danhSachLopTinChi.lopTC[danhSachLopTinChi.soLuong++] = lopTinChi;
                LichSuThaoTac::ghiNhanThemLopTinChi(lichSu, *lopTinChi, viTriLopTinChiMoi);
                tuKhoaNienKhoa.clear();
                tuKhoaHocKy.clear();
                tuKhoaMaMonHoc.clear();
                tuKhoaNhom.clear();
                tuKhoaTrangThai.clear();
                mucDangChon = danhSachLopTinChi.soLuong - 1;
                luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                thongBao = "Đã mở lớp tín chỉ mã " + to_string(lopTinChi->maLopTC) + " thành công!";
                coLoi = false;
                break;
            }
            delete[] f;
            canVeLaiToanBo = true;
        } else if ((suKien.type == SK_PHIM_ENTER || hanhDong == 1) && soLopTinChiLoc > 0) {
            int viTriLopTinChiThat = cacLopTinChiLoc[mucDangChon];
            PTRLTC lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChiThat];
            TruongNhap* f = new TruongNhap[8];
            bool lopDaBiXoa = false;
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
                bool daChonXoa = false;
                bool daNhanF1 = false;
                bool daLuuBieuMau = chayBieuMau("HIỆU CHỈNH LỚP TÍN CHỈ #" + to_string(lopTinChi->maLopTC), f, 8, thongBaoLoi,
                    "[ LƯU (Enter) ]", "[ HỦY (ESC) ]", "[ XÓA LỚP ]", &daChonXoa,
                    "[ F1: CHỌN MÔN ]", &daNhanF1);
                if (daNhanF1) {
                    string maMonHocDuocChon = chonMonHocNhanh(root);
                    if (!maMonHocDuocChon.empty()) {
                        f[1].value = maMonHocDuocChon;
                        thongBaoLoi.clear();
                    }
                    continue;
                }
                if (daChonXoa) {
                    int soDangKy = demTongDangKy(lopTinChi->dssvdk);
                    if (soDangKy > 0) {
                        thongBaoLoi = "Không thể xóa lớp vì đã có " + to_string(soDangKy) + " sinh viên đăng ký!";
                        continue;
                    }
                    if (!GiaoDienConsole::hopXacNhan("XÁC NHẬN XÓA LỚP TÍN CHỈ",
                        "Bạn có chắc chắn muốn xóa Lớp tín chỉ #" + to_string(lopTinChi->maLopTC) + " (" + string(lopTinChi->maMH) + ")?")) {
                        continue;
                    }
                    int maLopTCDaXoa = lopTinChi->maLopTC;
                    LichSuThaoTac::ghiNhanXoaLopTinChi(lichSu, *lopTinChi, viTriLopTinChiThat);
                    giaiPhongDSDK(lopTinChi->dssvdk);
                    delete lopTinChi;
                    for (int i = viTriLopTinChiThat; i < danhSachLopTinChi.soLuong - 1; ++i) {
                        danhSachLopTinChi.lopTC[i] = danhSachLopTinChi.lopTC[i + 1];
                    }
                    danhSachLopTinChi.lopTC[--danhSachLopTinChi.soLuong] = nullptr;
                    if (mucDangChon >= soLopTinChiLoc - 1 && mucDangChon > 0) --mucDangChon;
                    luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                    thongBao = "Đã xóa lớp tín chỉ #" + to_string(maLopTCDaXoa) + " thành công!";
                    coLoi = false;
                    lopDaBiXoa = true;
                    break;
                }
                if (!daLuuBieuMau) break;

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
                if (timMonHoc(root, maMH.c_str()) == nullptr) {
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
                if (viTriBiTrung != -1 && viTriBiTrung != viTriLopTinChiThat) {
                    thongBaoLoi = "Bộ (Mã MH, Niên khóa, Học kỳ, Nhóm) bị trùng với Lớp TC #" + to_string(danhSachLopTinChi.lopTC[viTriBiTrung]->maLopTC) + "!";
                    continue;
                }

                ThongTinLopTinChiLichSu thongTinTruoc{};
                thongTinTruoc.maLopTC=lopTinChi->maLopTC;
                memcpy(thongTinTruoc.maMH,lopTinChi->maMH,sizeof(thongTinTruoc.maMH));
                memcpy(thongTinTruoc.nienKhoa,lopTinChi->nienKhoa,sizeof(thongTinTruoc.nienKhoa));
                thongTinTruoc.hocKy=lopTinChi->hocKy; thongTinTruoc.nhom=lopTinChi->nhom;
                thongTinTruoc.minSV=lopTinChi->minSV; thongTinTruoc.maxSV=lopTinChi->maxSV;
                thongTinTruoc.huyLop=lopTinChi->huyLop;
                strncpy(lopTinChi->maMH, maMH.c_str(), MAX_MAMH - 1); lopTinChi->maMH[MAX_MAMH - 1] = '\0';
                strncpy(lopTinChi->nienKhoa, nienKhoa.c_str(), MAX_NIENKHOA - 1); lopTinChi->nienKhoa[MAX_NIENKHOA - 1] = '\0';
                lopTinChi->hocKy = hocKy;
                lopTinChi->nhom = nhom;
                lopTinChi->minSV = soSinhVienToiThieu;
                lopTinChi->maxSV = soSinhVienToiDa;
                lopTinChi->huyLop = (huy != 0);
                LichSuThaoTac::ghiNhanSuaLopTinChi(lichSu, thongTinTruoc, *lopTinChi);

                luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                thongBao = "Đã cập nhật Lớp tín chỉ #" + to_string(lopTinChi->maLopTC) + " thành công!";
                coLoi = false;
                break;
            }
            delete[] f;
            canVeLaiToanBo = true;
            if (lopDaBiXoa) continue;
        }
    }
}

// ============================================================================
// CHỨC NĂNG (b): IN DSSV ĐÃ ĐĂNG KÝ LỚP TÍN CHỈ THEO THAM SỐ
// ============================================================================

// ============================================================================
void chucNangB_InDSSVDangKyLopTC(PTRMH root, const DanhSachLopTinChi& danhSachLopTinChi, const DanhSachLop& danhSachLop) {
    while (true) {
        int viTriLopTinChi = chonLopTinChiTheoThongTin(root, danhSachLopTinChi, "CHỨC NĂNG (b): IN DSSV ĐÃ ĐĂNG KÝ LỚP TÍN CHỈ");
        if (viTriLopTinChi == -1) break;

        PTRLTC lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChi];
        PTRMH mh = timMonHoc(root, lopTinChi->maMH);
        string tenMonHoc = mh ? mh->mh.tenMH : lopTinChi->maMH;

        int tongSoDangKy = demTongDangKy(lopTinChi->dssvdk);
        struct DongDangKy {
            string maSV;
            string hoTen;
            string trangThaiSinhVien;
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
            cacDong[soDong].trangThaiSinhVien = p->dk.huyDangKy ? "Đã hủy" : "Đã đăng ký";
            soDong++;
        }

        int* cacDongLoc = tongSoDangKy > 0 ? new int[tongSoDangKy] : nullptr;
        string tuKhoaMaSV, tuKhoaHoTen, tuKhoaTrangThai;
        string* cacTuKhoa[] = {&tuKhoaMaSV, &tuKhoaHoTen, &tuKhoaTrangThai};
        size_t viTriConTro[3] = {};
        int oLocDangChon = 0;
        int soDongLoc = 0;
        int trang = 0;
        int soDongMoiTrang = 13;
        int trangTruoc = -1;
        int dongDangTro = -1;
        int cacDongGocDaVe[13];
        bool cacDongDaTroDaVe[13] = {};
        for (int i = 0; i < 13; ++i) cacDongGocDaVe[i] = -1;
        bool daVeThongBaoKhongKetQua = false;
        bool canVeLaiBang = true;
        bool canVeLaiBoLoc = true;
        NutChuNhat nutQuayLai = { 46, 23, 24, 1, 0, "[ QUAY LẠI (ESC) ]" };

        auto apDungBoLocSinhVien = [&]() {
            soDongLoc = 0;
            for (int i = 0; i < soDong; ++i) {
                string ma = GiaoDienConsole::vietHoa(cacDong[i].maSV);
                string hoTen = GiaoDienConsole::vietHoa(cacDong[i].hoTen);
                string trangThai = GiaoDienConsole::vietHoa(cacDong[i].trangThaiSinhVien);
                string tuKhoaTrangThaiKhongDau = GiaoDienConsole::vietHoa(tuKhoaTrangThai);
                string trangThaiKhongDau = cacDong[i].daHuy ? "HUY DA HUY HUY DANG KY" : "DANG KY DA DANG KY CON DANG KY DK";
                if ((!tuKhoaMaSV.empty() && ma.find(GiaoDienConsole::vietHoa(tuKhoaMaSV)) == string::npos) ||
                    (!tuKhoaHoTen.empty() && hoTen.find(GiaoDienConsole::vietHoa(tuKhoaHoTen)) == string::npos) ||
                    (!tuKhoaTrangThai.empty() && trangThai.find(tuKhoaTrangThaiKhongDau) == string::npos &&
                     trangThaiKhongDau.find(tuKhoaTrangThaiKhongDau) == string::npos)) continue;
                cacDongLoc[soDongLoc++] = i;
            }
            if (dongDangTro >= soDongLoc) dongDangTro = -1;
        };
        auto veCacOLocSinhVien = [&]() {
            const int toaDoX[] = {4, 32, 79};
            const int chieuRong[] = {25, 44, 33};
            const string nhan[] = {"Mã SV: ", "Họ/Tên: ", "Trạng thái: "};
            for (int i = 0; i < 3; ++i) {
                GiaoDienConsole::diDenToaDo(toaDoX[i], 6);
                GiaoDienConsole::datMau(oLocDangChon == i ? DEN : TRANG, oLocDangChon == i ? VANG : XANH_NGOC);
                int doRongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
                size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong[i] - doRongNhan));
                size_t dauHienThi = viTriConTro[i] >= rongGiaTri && rongGiaTri > 0 ? viTriConTro[i] - rongGiaTri + 1 : 0;
                string hienThi = nhan[i] + cacTuKhoa[i]->substr(dauHienThi, rongGiaTri);
                int doDai = GiaoDienConsole::doDaiUtf8(hienThi);
                cout << hienThi << string(max(0, chieuRong[i] - doDai), ' ');
            }
        };
        auto datViTriConTroTimSV = [&]() {
            const int toaDoX[] = {4, 32, 79};
            const int chieuRong[] = {25, 44, 33};
            const string nhan[] = {"Mã SV: ", "Họ/Tên: ", "Trạng thái: "};
            int doRongNhan = GiaoDienConsole::doDaiUtf8(nhan[oLocDangChon]);
            int soKyTuHienThi = max(0, chieuRong[oLocDangChon] - doRongNhan);
            size_t dauHienThi = viTriConTro[oLocDangChon] >= static_cast<size_t>(soKyTuHienThi) && soKyTuHienThi > 0
                ? viTriConTro[oLocDangChon] - static_cast<size_t>(soKyTuHienThi) + 1 : 0;
            int x = toaDoX[oLocDangChon] + doRongNhan
                + static_cast<int>(min(viTriConTro[oLocDangChon] - dauHienThi, static_cast<size_t>(soKyTuHienThi)));
            x = min(x, toaDoX[oLocDangChon] + chieuRong[oLocDangChon] - 1);
            GiaoDienConsole::diDenToaDo(x, 6);
            GiaoDienConsole::hienConTro(true);
        };

        auto veDongDangKy = [&](int chiSoDong, int trangHienTai, bool dangTro) {
            int viTriDau = trangHienTai * soDongMoiTrang;
            int viTriCuoi = min(viTriDau + soDongMoiTrang, soDongLoc);
            if (chiSoDong < viTriDau || chiSoDong >= viTriCuoi) return;
            GiaoDienConsole::diDenToaDo(4, 9 + chiSoDong - viTriDau);
            const DongDangKy& dong = cacDong[cacDongLoc[chiSoDong]];
            GiaoDienConsole::datMau(dangTro ? DEN : (dong.daHuy ? DO_NHAT : TRANG), dangTro ? VANG : DEN);
            cout << GiaoDienConsole::canTrai(" " + to_string(chiSoDong + 1), 7);
            GiaoDienConsole::datMau(dangTro ? DEN : XANH_NGOC, dangTro ? VANG : DEN); cout << "|";
            GiaoDienConsole::datMau(dangTro ? DEN : (dong.daHuy ? DO_NHAT : TRANG), dangTro ? VANG : DEN);
            cout << GiaoDienConsole::canTrai(dong.maSV, 20);
            GiaoDienConsole::datMau(dangTro ? DEN : XANH_NGOC, dangTro ? VANG : DEN); cout << "|";
            GiaoDienConsole::datMau(dangTro ? DEN : (dong.daHuy ? DO_NHAT : TRANG), dangTro ? VANG : DEN);
            cout << GiaoDienConsole::canTrai(dong.hoTen, 48);
            GiaoDienConsole::datMau(dangTro ? DEN : XANH_NGOC, dangTro ? VANG : DEN); cout << "|";
            GiaoDienConsole::datMau(dangTro ? DEN : (dong.daHuy ? DO_NHAT : TRANG), dangTro ? VANG : DEN);
            cout << GiaoDienConsole::canTrai(dong.trangThaiSinhVien, 30);
        };

        while (true) {
            apDungBoLocSinhVien();
            int tongSoTrang = soDongLoc == 0 ? 1 : (soDongLoc + soDongMoiTrang - 1) / soDongMoiTrang;
            if (trang >= tongSoTrang) trang = tongSoTrang - 1;
            if (trang != trangTruoc) canVeLaiBang = true;
            int viTriDau = trang * soDongMoiTrang;
            int viTriCuoi = min(viTriDau + soDongMoiTrang, soDongLoc);

            if (trangTruoc == -1) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN,
                    string("DANH SÁCH SINH VIÊN ĐĂNG KÝ LỚP TÍN CHỈ") + (lopTinChi->huyLop ? " - LỚP ĐÃ HỦY" : ""));

                GiaoDienConsole::datMau(VANG, DEN);
                GiaoDienConsole::diDenToaDo(4, 4);
                cout << "Môn học: " << tenMonHoc << " (" << lopTinChi->maMH << ")  |  Niên khóa: " << lopTinChi->nienKhoa
                     << "  |  Học kỳ: " << lopTinChi->hocKy << "  |  Nhóm: " << lopTinChi->nhom;

                GiaoDienConsole::diDenToaDo(4, 5);
                GiaoDienConsole::datMau(lopTinChi->huyLop ? DO_NHAT : XANH_LA_NHAT, DEN);
                cout << "Trạng thái lớp: " << (lopTinChi->huyLop ? "Đã hủy" : "Đang mở");

                veCacOLocSinhVien();

                GiaoDienConsole::diDenToaDo(4, 7);
                GiaoDienConsole::datMau(VANG, LAM);
                cout << GiaoDienConsole::canTrai(" STT", 7);
                GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
                GiaoDienConsole::datMau(VANG, LAM);
                cout << GiaoDienConsole::canTrai("MÃ SINH VIÊN", 20);
                GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
                GiaoDienConsole::datMau(VANG, LAM);
                cout << GiaoDienConsole::canTrai("HỌ VÀ TÊN SINH VIÊN", 48);
                GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
                GiaoDienConsole::datMau(VANG, LAM);
                cout << GiaoDienConsole::canTrai("TRẠNG THÁI SV", 30);

                GiaoDienConsole::diDenToaDo(4, 8);
                GiaoDienConsole::datMau(XANH_NGOC, DEN);
                cout << string(108, '-');

                if (soDongLoc == 0) {
                    GiaoDienConsole::datMau(DO_NHAT, DEN);
                    GiaoDienConsole::diDenToaDo(4, 10);
                    cout << GiaoDienConsole::canGiua(soDong == 0 ? "Chưa có sinh viên nào đăng ký lớp tín chỉ này!" : "Không tìm thấy sinh viên phù hợp với điều kiện tìm kiếm.", 108);
                } else {
                    for (int r = 0; r < soDongMoiTrang; ++r) {
                        int i = viTriDau + r;
                        GiaoDienConsole::diDenToaDo(4, 9 + r);
                        if (i < viTriCuoi) veDongDangKy(i, trang, i == dongDangTro);
                        else { GiaoDienConsole::datMau(XAM_NHAT, DEN); cout << string(108, ' '); }
                        cacDongGocDaVe[r] = i < viTriCuoi ? cacDongLoc[i] : -1;
                        cacDongDaTroDaVe[r] = i < viTriCuoi && i == dongDangTro;
                    }
                }
                daVeThongBaoKhongKetQua = soDongLoc == 0;

                GiaoDienConsole::veNut(nutQuayLai, true, TRANG, XAM_DAM, DEN, VANG);
                GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | Tìm mã/tên/trạng thái | ↑/↓ chọn | Enter chọn | Esc về | Tìm không dấu");
                trangTruoc = trang;
                canVeLaiBang = false;
                canVeLaiBoLoc = false;
            } else {
                if (canVeLaiBoLoc) {
                    veCacOLocSinhVien();
                    canVeLaiBoLoc = false;
                }
                if (canVeLaiBang) {
                    for (int r = 0; r < soDongMoiTrang; ++r) {
                        int i = viTriDau + r;
                        int dongGocMoi = i < viTriCuoi ? cacDongLoc[i] : -1;
                        bool dangTroMoi = i < viTriCuoi && i == dongDangTro;
                        if (dongGocMoi != cacDongGocDaVe[r] || dangTroMoi != cacDongDaTroDaVe[r] ||
                            (r == 1 && daVeThongBaoKhongKetQua != (soDongLoc == 0))) {
                            GiaoDienConsole::diDenToaDo(4, 9 + r);
                            GiaoDienConsole::datMau(XAM_NHAT, DEN);
                            cout << string(108, ' ');
                            if (i < viTriCuoi) veDongDangKy(i, trang, dangTroMoi);
                            cacDongGocDaVe[r] = dongGocMoi;
                            cacDongDaTroDaVe[r] = dangTroMoi;
                        }
                    }
                    if (soDongLoc == 0) {
                        GiaoDienConsole::diDenToaDo(4, 10);
                        GiaoDienConsole::datMau(DO_NHAT, DEN);
                        cout << GiaoDienConsole::canGiua(soDong == 0 ? "Chưa có sinh viên nào đăng ký lớp tín chỉ này!" : "Không tìm thấy sinh viên phù hợp với điều kiện tìm kiếm.", 108);
                    }
                    daVeThongBaoKhongKetQua = soDongLoc == 0;
                    GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | Tìm mã/tên/trạng thái | ↑/↓ chọn | Enter chọn | Esc về | Tìm không dấu");
                    trangTruoc = trang;
                    canVeLaiBang = false;
                }
            }

            datViTriConTroTimSV();
            SuKienNhap suKien = GiaoDienConsole::docSuKien();
            if (suKien.type == SK_CHUOT_DI_CHUYEN) {
                int dongMoi = -1;
                int viTriDau = trang * soDongMoiTrang;
                int viTriCuoi = min(viTriDau + soDongMoiTrang, soDongLoc);
                if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 9 && suKien.toaDoChuotY < 9 + soDongMoiTrang) {
                    int viTri = viTriDau + suKien.toaDoChuotY - 9;
                    if (viTri < viTriCuoi) dongMoi = viTri;
                }
                if (dongMoi != dongDangTro) {
                    if (dongDangTro >= 0) {
                        veDongDangKy(dongDangTro, trang, false);
                        cacDongDaTroDaVe[dongDangTro % soDongMoiTrang] = false;
                    }
                    if (dongMoi >= 0) {
                        veDongDangKy(dongMoi, trang, true);
                        cacDongDaTroDaVe[dongMoi % soDongMoiTrang] = true;
                    }
                    dongDangTro = dongMoi;
                }
                continue;
            }
            if (suKien.type == SK_PHIM_ESC || suKien.type == SK_PHIM_ENTER) break;
            if (suKien.type == SK_CHUOT_TRAI && nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) break;
            if (suKien.type == SK_CHUOT_TRAI && suKien.toaDoChuotY == 6) {
                if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 29) oLocDangChon = 0;
                else if (suKien.toaDoChuotX >= 32 && suKien.toaDoChuotX < 76) oLocDangChon = 1;
                else if (suKien.toaDoChuotX >= 79 && suKien.toaDoChuotX < 112) oLocDangChon = 2;
                canVeLaiBoLoc = true;
            } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_SHIFT_TAB) {
                int buoc = suKien.type == SK_PHIM_SHIFT_TAB ? -1 : 1;
                oLocDangChon = (oLocDangChon + buoc + 3) % 3;
                canVeLaiBoLoc = true;
            } else if (suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI) {
                if (suKien.type == SK_PHIM_TRAI && viTriConTro[oLocDangChon] > 0) --viTriConTro[oLocDangChon];
                else if (suKien.type == SK_PHIM_PHAI && viTriConTro[oLocDangChon] < cacTuKhoa[oLocDangChon]->size()) ++viTriConTro[oLocDangChon];
            } else if (suKien.type == EVT_KEY_HOME) {
                viTriConTro[oLocDangChon] = 0;
            } else if (suKien.type == EVT_KEY_END) {
                viTriConTro[oLocDangChon] = cacTuKhoa[oLocDangChon]->size();
            } else if (suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
                if (suKien.type == EVT_KEY_BACKSPACE && viTriConTro[oLocDangChon] > 0) {
                    cacTuKhoa[oLocDangChon]->erase(viTriConTro[oLocDangChon] - 1, 1);
                    --viTriConTro[oLocDangChon];
                } else if (suKien.type == EVT_KEY_DELETE && viTriConTro[oLocDangChon] < cacTuKhoa[oLocDangChon]->size()) {
                    cacTuKhoa[oLocDangChon]->erase(viTriConTro[oLocDangChon], 1);
                }
                trang = 0; dongDangTro = -1; canVeLaiBoLoc = true; canVeLaiBang = true;
            } else if (suKien.type == SK_KY_TU) {
                char kyTu = suKien.ch;
                if (kyTu >= 32 && kyTu <= 126 && cacTuKhoa[oLocDangChon]->size() < (oLocDangChon == 0 ? 15u : 50u)) {
                    if (oLocDangChon == 0 && kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
                    cacTuKhoa[oLocDangChon]->insert(viTriConTro[oLocDangChon], 1, kyTu);
                    ++viTriConTro[oLocDangChon];
                }
                trang = 0; dongDangTro = -1; canVeLaiBoLoc = true; canVeLaiBang = true;
            } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG || suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
                if (soDongLoc > 0) {
                    if (dongDangTro < 0) dongDangTro = 0;
                    else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) dongDangTro = (dongDangTro + 1) % soDongLoc;
                    else dongDangTro = (dongDangTro - 1 + soDongLoc) % soDongLoc;
                    trang = dongDangTro / soDongMoiTrang;
                    canVeLaiBang = true;
                }
            } else if (suKien.type == EVT_KEY_PGDN || suKien.type == EVT_KEY_PGUP) {
                if (soDongLoc > 0) {
                    int dongHienTai = dongDangTro >= 0 ? dongDangTro : trang * soDongMoiTrang;
                    int buoc = suKien.type == EVT_KEY_PGDN ? soDongMoiTrang : -soDongMoiTrang;
                    dongDangTro = max(0, min(soDongLoc - 1, dongHienTai + buoc));
                    trang = dongDangTro / soDongMoiTrang;
                    canVeLaiBang = true;
                }
            }
        }

        if (cacDongLoc != nullptr) delete[] cacDongLoc;
        if (cacDong != nullptr) delete[] cacDong;
    }
}

void chucNangE_CapNhatMonHoc(PTRMH& root, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi, QuanLyLichSu& lichSu) {
    const int soDongMoiTrang = 14;
    const int toaDoX[] = {4, 43};
    const int chieuRong[] = {36, 69};
    const string nhan[] = {"Mã môn: ", "Tên môn: "};
    const size_t doDaiToiDa[] = {20, 60};
    string tuKhoa[2];
    size_t viTriConTro[2] = {};
    int oDangChon = 0;
    int mucDangChon = 0;
    int nutDangTro = -1;
    int nutTruocDo = -1;
    bool canVeKhung = true;
    bool canVeOLoc = true;
    bool canVeBang = true;
    bool canVeTrangThai = true;
    int mucChonTruoc = -1;
    string thongBao;
    bool coLoi = false;
    const NutChuNhat nutThem = { 6, 23, 24, 1, 0, "[ + THÊM MỚI (Ins) ]" };
    const NutChuNhat nutUndo = { 43, 23, 30, 1, 2, "[ F3: HOÀN TÁC ]" };
    const NutChuNhat nutQuayLai = { 88, 23, 22, 1, 1, "[ QUAY LẠI (ESC) ]" };

    auto veOLoc = [&](int i) {
        GiaoDienConsole::diDenToaDo(toaDoX[i], 4);
        GiaoDienConsole::datMau(oDangChon == i ? DEN : TRANG, oDangChon == i ? VANG : XANH_NGOC);
        int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
        size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong[i] - rongNhan));
        size_t dau = viTriConTro[i] >= rongGiaTri && rongGiaTri > 0 ? viTriConTro[i] - rongGiaTri + 1 : 0;
        string hienThi = nhan[i] + tuKhoa[i].substr(dau, rongGiaTri);
        int doDai = GiaoDienConsole::doDaiUtf8(hienThi);
        cout << hienThi << string(max(0, chieuRong[i] - doDai), ' ');
    };

    auto datViTriConTro = [&]() {
        int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[oDangChon]);
        size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong[oDangChon] - rongNhan));
        size_t dau = viTriConTro[oDangChon] >= rongGiaTri && rongGiaTri > 0 ? viTriConTro[oDangChon] - rongGiaTri + 1 : 0;
        int x = toaDoX[oDangChon] + rongNhan + static_cast<int>(min(viTriConTro[oDangChon] - dau, rongGiaTri));
        int cotCuoi = toaDoX[oDangChon] + chieuRong[oDangChon] - 1;
        if (x > cotCuoi) x = cotCuoi;
        GiaoDienConsole::diDenToaDo(x, 4);
        GiaoDienConsole::hienConTro(true);
    };

    while (true) {
        const int soMonHoc = demMonHoc(root);
        MonHoc* mangMonHoc = soMonHoc > 0 ? new MonHoc[soMonHoc] : nullptr;
        int viTriMonHoc = 0;
        bstSangMang(root, mangMonHoc, viTriMonHoc);
        int* monHocLoc = soMonHoc > 0 ? new int[soMonHoc] : nullptr;
        int soMonHocLoc = 0;
        string khoaMa = GiaoDienConsole::vietHoa(tuKhoa[0]);
        string khoaTen = GiaoDienConsole::vietHoa(tuKhoa[1]);
        for (int i = 0; i < soMonHoc; ++i) {
            string ma = GiaoDienConsole::vietHoa(mangMonHoc[i].maMH);
            string ten = GiaoDienConsole::vietHoa(mangMonHoc[i].tenMH);
            if ((!khoaMa.empty() && ma.find(khoaMa) == string::npos) ||
                (!khoaTen.empty() && ten.find(khoaTen) == string::npos)) continue;
            monHocLoc[soMonHocLoc++] = i;
        }

        if (mucDangChon >= soMonHocLoc) mucDangChon = max(0, soMonHocLoc - 1);
        const int tongSoTrang = max(1, (soMonHocLoc + soDongMoiTrang - 1) / soDongMoiTrang);
        const int trang = soMonHocLoc > 0 ? mucDangChon / soDongMoiTrang : 0;
        const int viTriDau = trang * soDongMoiTrang;
        const int viTriCuoi = min(viTriDau + soDongMoiTrang, soMonHocLoc);
        auto veMotDong = [&](int chiSoLoc, bool dangDuocChon) {
            int r = chiSoLoc - viTriDau;
            if (r < 0 || r >= soDongMoiTrang) return;
            GiaoDienConsole::diDenToaDo(4, 8 + r);
            if (chiSoLoc >= 0 && chiSoLoc < soMonHocLoc) {
                const MonHoc& monHoc = mangMonHoc[monHocLoc[chiSoLoc]];
                GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
                cout << GiaoDienConsole::canTrai(" " + to_string(chiSoLoc + 1), 8)
                     << GiaoDienConsole::canTrai(monHoc.maMH, 18)
                     << GiaoDienConsole::canTrai(monHoc.tenMH, 48)
                     << GiaoDienConsole::canTrai("   " + to_string(monHoc.soTCLT), 18)
                     << GiaoDienConsole::canTrai("   " + to_string(monHoc.soTCTH), 16);
            } else {
                GiaoDienConsole::datMau(XAM_NHAT, DEN);
                cout << string(108, ' ');
            }
        };

        if (canVeKhung) {
            GiaoDienConsole::xoaManHinh();
            GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH MÔN HỌC");
            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 6);
            cout << GiaoDienConsole::canTrai(" STT", 8)
                 << GiaoDienConsole::canTrai("MÃ MÔN HỌC", 18)
                 << GiaoDienConsole::canTrai("TÊN MÔN HỌC", 48)
                 << GiaoDienConsole::canTrai("STC LÝ THUYẾT", 18)
                 << GiaoDienConsole::canTrai("STC THỰC HÀNH", 16);
            GiaoDienConsole::diDenToaDo(4, 7);
            GiaoDienConsole::datMau(XANH_NGOC_NHAT, DEN);
            cout << string(108, '-');
            canVeKhung = false;
            canVeBang = canVeOLoc = canVeTrangThai = true;
        }
        if (canVeOLoc) {
            veOLoc(0);
            veOLoc(1);
            canVeOLoc = false;
        }
        if (canVeBang) {
            for (int r = 0; r < soDongMoiTrang; ++r) veMotDong(viTriDau + r, viTriDau + r == mucDangChon);
            if (soMonHocLoc == 0) {
                GiaoDienConsole::diDenToaDo(4, 13);
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                cout << GiaoDienConsole::canGiua("Không tìm thấy môn học phù hợp với điều kiện tìm kiếm.", 108);
            }
            canVeBang = false;
            canVeTrangThai = true;
            mucChonTruoc = mucDangChon;
        }
        if (canVeTrangThai) {
            GiaoDienConsole::veNut(nutThem, nutDangTro == 0, TRANG, XAM_DAM, DEN, VANG);
            GiaoDienConsole::veNut(nutUndo, nutDangTro == 2, TRANG, NAU, DEN, VANG);
            GiaoDienConsole::veNut(nutQuayLai, nutDangTro == 1, TRANG, XAM_DAM, DEN, VANG);
            string moTaUndo = LichSuThaoTac::moTaThaoTacGanNhat(lichSu);
            GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | F3: hoàn tác [" + moTaUndo + "] | Enter sửa | Ins thêm | Esc về | Tìm không dấu", thongBao, coLoi);
            thongBao.clear();
            coLoi = false;
            canVeTrangThai = false;
            nutTruocDo = nutDangTro;
        }

        datViTriConTro();
        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        int hanhDong = -1;
        bool canLocLai = false;
        if (suKien.type == SK_CHUOT_DI_CHUYEN) {
            nutDangTro = nutThem.contains(suKien.toaDoChuotX, suKien.toaDoChuotY) ? 0 :
                         (nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY) ? 1 :
                         (nutUndo.contains(suKien.toaDoChuotX, suKien.toaDoChuotY) ? 2 : -1));
            if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 8 && suKien.toaDoChuotY < 8 + soDongMoiTrang) {
                int viTri = viTriDau + suKien.toaDoChuotY - 8;
                if (viTri < viTriCuoi) mucDangChon = viTri;
            }
            if (nutDangTro != nutTruocDo) canVeTrangThai = true;
            if (mucDangChon != mucChonTruoc) {
                int mucCu = mucChonTruoc;
                if (mucCu >= viTriDau && mucCu < viTriCuoi) veMotDong(mucCu, false);
                veMotDong(mucDangChon, true);
                mucChonTruoc = mucDangChon;
            }
        } else if (suKien.type == SK_CHUOT_TRAI) {
            if (nutThem.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) hanhDong = 0;
            else if (nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) hanhDong = 3;
            else if (nutUndo.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) hanhDong = 4;
            else if (suKien.toaDoChuotY == 4) {
                const int soField = 2;
                for (int i = 0; i < soField; ++i) {
                    if (suKien.toaDoChuotX >= toaDoX[i] && suKien.toaDoChuotX < toaDoX[i] + chieuRong[i]) {
                        oDangChon = i;
                        int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
                        size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong[i] - rongNhan));
                        size_t dau = viTriConTro[i] >= rongGiaTri && rongGiaTri > 0 ? viTriConTro[i] - rongGiaTri + 1 : 0;
                        viTriConTro[i] = min(dau + static_cast<size_t>(max(0, suKien.toaDoChuotX - toaDoX[i] - rongNhan)), tuKhoa[i].size());
                        canVeOLoc = true;
                        break;
                    }
                }
            } else if (suKien.toaDoChuotY >= 8 && suKien.toaDoChuotY < 8 + soDongMoiTrang) {
                int viTri = viTriDau + suKien.toaDoChuotY - 8;
                if (viTri < viTriCuoi) {
                    if (mucDangChon == viTri) hanhDong = 1;
                    else { mucDangChon = viTri; canVeBang = canVeTrangThai = true; }
                }
            }
        }

        if (suKien.type == EVT_KEY_F3 || hanhDong == 4) {
            const string moTaUndo = LichSuThaoTac::moTaThaoTacGanNhat(lichSu);
            string thongBaoUndo;
            bool daHoanTac = LichSuThaoTac::hoanTacThaoTac(
                lichSu, root, danhSachLop, danhSachLopTinChi, thongBaoUndo);
            thongBao = daHoanTac ? "Đã hoàn tác: " + moTaUndo : thongBaoUndo;
            coLoi = !daHoanTac;
            canVeKhung = true;
            canVeBang = canVeOLoc = canVeTrangThai = true;
        } else if (suKien.type == SK_PHIM_ESC || hanhDong == 3) {
            GiaoDienConsole::hienConTro(false);
            delete[] monHocLoc;
            delete[] mangMonHoc;
            luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
            return;
        } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_SHIFT_TAB) {
            int buoc = suKien.type == SK_PHIM_SHIFT_TAB ? -1 : 1;
            oDangChon = (oDangChon + buoc + 2) % 2;
            canVeOLoc = true;
        } else if (suKien.type == SK_PHIM_TRAI) {
            if (viTriConTro[oDangChon] > 0) --viTriConTro[oDangChon];
        } else if (suKien.type == SK_PHIM_PHAI) {
            if (viTriConTro[oDangChon] < tuKhoa[oDangChon].size()) ++viTriConTro[oDangChon];
        } else if (suKien.type == EVT_KEY_HOME) {
            viTriConTro[oDangChon] = 0;
        } else if (suKien.type == EVT_KEY_END) {
            viTriConTro[oDangChon] = tuKhoa[oDangChon].size();
        } else if (suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
            if (suKien.type == EVT_KEY_BACKSPACE && viTriConTro[oDangChon] > 0) {
                tuKhoa[oDangChon].erase(viTriConTro[oDangChon] - 1, 1);
                --viTriConTro[oDangChon];
                canLocLai = true;
            } else if (suKien.type == EVT_KEY_DELETE && viTriConTro[oDangChon] < tuKhoa[oDangChon].size()) {
                tuKhoa[oDangChon].erase(viTriConTro[oDangChon], 1);
                canLocLai = true;
            }
        } else if (suKien.type == SK_KY_TU) {
            char kyTu = suKien.ch;
            if (kyTu >= 32 && kyTu <= 126 && tuKhoa[oDangChon].size() < doDaiToiDa[oDangChon]) {
                if (oDangChon == 0 && kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
                tuKhoa[oDangChon].insert(viTriConTro[oDangChon], 1, kyTu);
                ++viTriConTro[oDangChon];
                canLocLai = true;
            }
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_PHIM_XUONG) {
            if (soMonHocLoc > 0) {
                int buoc = suKien.type == SK_PHIM_LEN ? -1 : 1;
                mucDangChon = (mucDangChon + buoc + soMonHocLoc) % soMonHocLoc;
                canVeBang = canVeTrangThai = true;
            }
        } else if (suKien.type == EVT_KEY_PGUP || suKien.type == EVT_KEY_PGDN) {
            if (soMonHocLoc > 0) {
                int buoc = suKien.type == EVT_KEY_PGUP ? -soDongMoiTrang : soDongMoiTrang;
                mucDangChon = max(0, min(soMonHocLoc - 1, mucDangChon + buoc));
                canVeBang = canVeTrangThai = true;
            }
        } else if (suKien.type == SK_CON_LAN_LEN || suKien.type == SK_CON_LAN_XUONG) {
            if (soMonHocLoc > 0) {
                int buoc = suKien.type == SK_CON_LAN_LEN ? -1 : 1;
                mucDangChon = (mucDangChon + buoc + soMonHocLoc) % soMonHocLoc;
                canVeBang = canVeTrangThai = true;
            }
        } else if (suKien.type == EVT_KEY_INSERT || hanhDong == 0) {
            TruongNhap* f = new TruongNhap[4];
            f[0] = { "Mã môn học", "", 10, TRUONG_MA, 0, 0, 0 };
            f[1] = { "Tên môn học", "", 50, TRUONG_CHU, 0, 0, 0 };
            f[2] = { "Số tín chỉ lý thuyết", "3", 2, TRUONG_SO, 0, 0, 0 };
            f[3] = { "Số tín chỉ thực hành", "1", 2, TRUONG_SO, 0, 0, 0 };
            string loi;
            while (true) {
                if (!chayBieuMau("THÊM MÔN HỌC MỚI", f, 4, loi)) break;
                string ma = GiaoDienConsole::vietHoa(f[0].value);
                string ten = GiaoDienConsole::catKhoangTrang(f[1].value);
                int tinChiLyThuyet = atoi(f[2].value.c_str());
                int tinChiThucHanh = atoi(f[3].value.c_str());
                if (ma.empty() || ten.empty()) { loi = "Mã môn học và Tên môn học không được để trống!"; continue; }
                if (timMonHoc(root, ma.c_str()) != nullptr) { loi = "Mã môn học '" + ma + "' đã tồn tại!"; continue; }
                if (tinChiLyThuyet < 0 || tinChiThucHanh < 0 || tinChiLyThuyet + tinChiThucHanh <= 0) { loi = "Tổng số tín chỉ phải lớn hơn 0!"; continue; }
                MonHoc monHocMoi = taoMonHoc(ma.c_str(), ten.c_str(), tinChiLyThuyet, tinChiThucHanh);
                if (!themMonHoc(root, monHocMoi)) { loi = "Không thể thêm môn học."; continue; }
                LichSuThaoTac::ghiNhanThemMonHoc(lichSu, monHocMoi);
                luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                thongBao = "Đã thêm môn học " + ma + " thành công!";
                break;
            }
            delete[] f;
            canVeKhung = true;
            canVeBang = canVeTrangThai = true;
        } else if ((suKien.type == SK_PHIM_ENTER || hanhDong == 1) && soMonHocLoc > 0) {
            MonHoc monHocHienTai = mangMonHoc[monHocLoc[mucDangChon]];
            TruongNhap* f = new TruongNhap[4];
            f[0] = { "Mã môn học (Cố định)", monHocHienTai.maMH, 10, TRUONG_CHI_DOC, 0, 0, 0 };
            f[1] = { "Tên môn học", monHocHienTai.tenMH, 50, TRUONG_CHU, 0, 0, 0 };
            f[2] = { "Số tín chỉ lý thuyết", to_string(monHocHienTai.soTCLT), 2, TRUONG_SO, 0, 0, 0 };
            f[3] = { "Số tín chỉ thực hành", to_string(monHocHienTai.soTCTH), 2, TRUONG_SO, 0, 0, 0 };
            string loi;
            bool daChonXoa = false;
            while (true) {
                bool daLuu = chayBieuMau("THÔNG TIN MÔN HỌC: " + string(monHocHienTai.maMH), f, 4, loi,
                    "[ LƯU (Enter) ]", "[ HỦY (ESC) ]", "[ XÓA MÔN ]", &daChonXoa);
                if (!daLuu) {
                    if (!daChonXoa) break;
                    if (kiemTraMonHocDaMoLop(danhSachLopTinChi, monHocHienTai.maMH)) {
                        loi = "Không thể xóa môn học vì đã được mở lớp tín chỉ!";
                        daChonXoa = false;
                        continue;
                    }
                    if (GiaoDienConsole::hopXacNhan("XÁC NHẬN XÓA MÔN HỌC", "Xóa môn học " + string(monHocHienTai.maMH) + "?")) {
                        LichSuThaoTac::ghiNhanXoaMonHoc(lichSu, monHocHienTai);
                        xoaMonHoc(root, monHocHienTai.maMH);
                        luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                        thongBao = "Đã xóa môn học " + string(monHocHienTai.maMH) + ".";
                    }
                    break;
                }

                string ten = GiaoDienConsole::catKhoangTrang(f[1].value);
                int tinChiLyThuyet = atoi(f[2].value.c_str());
                int tinChiThucHanh = atoi(f[3].value.c_str());
                if (ten.empty()) { loi = "Tên môn học không được để trống!"; continue; }
                if (tinChiLyThuyet < 0 || tinChiThucHanh < 0 || tinChiLyThuyet + tinChiThucHanh <= 0) { loi = "Tổng số tín chỉ phải lớn hơn 0!"; continue; }
                PTRMH nutMonHoc = timMonHoc(root, monHocHienTai.maMH);
                if (nutMonHoc != nullptr) {
                    strncpy(nutMonHoc->mh.tenMH, ten.c_str(), MAX_TENMH - 1);
                    nutMonHoc->mh.tenMH[MAX_TENMH - 1] = '\0';
                    nutMonHoc->mh.soTCLT = tinChiLyThuyet;
                    nutMonHoc->mh.soTCTH = tinChiThucHanh;
                    LichSuThaoTac::ghiNhanSuaMonHoc(lichSu, monHocHienTai, nutMonHoc->mh);
                    luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                    thongBao = "Đã cập nhật môn học " + string(monHocHienTai.maMH) + ".";
                }
                break;
            }
            delete[] f;
            canVeKhung = true;
            canVeBang = canVeTrangThai = true;
        }

        if (canLocLai) {
            mucDangChon = 0;
            canVeBang = canVeTrangThai = canVeOLoc = true;
        }
        delete[] monHocLoc;
        delete[] mangMonHoc;
    }
}

// ============================================================================
// CHỨC NĂNG (f): IN DANH SÁCH MÔN HỌC TĂNG DẦN THEO TÊN MÔN HỌC
// ============================================================================

// ============================================================================
void chucNangF_InDSMonHocTheoTen(PTRMH root) {
    const int soDongMoiTrang = 14;
    const int toaDoX[] = {4, 31, 75, 94};
    const int chieuRong[] = {26, 43, 18, 18};
    const string nhan[] = {"Mã MH: ", "Tên MH: ", "STC LT: ", "STC TH: "};
    const size_t doDaiToiDa[] = {20, 60, 2, 2};
    string tuKhoa[4];
    size_t viTriConTro[4] = {};
    int oDangChon = 0;
    int mucDangChon = 0;
    int mucChonTruoc = -1;
    int nutDangTro = -1;
    int nutTruocDo = -1;
    bool canVeBang = true;
    bool canVeOLoc = true;
    bool canVeTrangThai = true;
    string thongBao;
    const NutChuNhat nutQuayLai = { 88, 23, 22, 1, 1, "[ QUAY LẠI (ESC) ]" };

    auto veOLoc = [&](int i) {
        GiaoDienConsole::diDenToaDo(toaDoX[i], 4);
        GiaoDienConsole::datMau(oDangChon == i ? DEN : TRANG, oDangChon == i ? VANG : XANH_NGOC);
        int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
        size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong[i] - rongNhan));
        size_t dau = viTriConTro[i] >= rongGiaTri && rongGiaTri > 0 ? viTriConTro[i] - rongGiaTri + 1 : 0;
        string hienThi = nhan[i] + tuKhoa[i].substr(dau, rongGiaTri);
        int doDai = GiaoDienConsole::doDaiUtf8(hienThi);
        cout << hienThi << string(max(0, chieuRong[i] - doDai), ' ');
    };

    auto veBang = [&](const MonHoc* dsMonHoc, const int* monHocLoc, int soMonHocLoc,
                      int viTriDau, int viTriCuoi) {
        for (int r = 0; r < soDongMoiTrang; ++r) {
            int chiSoLoc = viTriDau + r;
            GiaoDienConsole::diDenToaDo(4, 8 + r);
            if (chiSoLoc < viTriCuoi) {
                const MonHoc& monHoc = dsMonHoc[monHocLoc[chiSoLoc]];
                bool dangChon = chiSoLoc == mucDangChon;
                GiaoDienConsole::datMau(dangChon ? DEN : TRANG, dangChon ? VANG : DEN);
                cout << GiaoDienConsole::canTrai(" " + to_string(chiSoLoc + 1), 8)
                     << GiaoDienConsole::canTrai(monHoc.maMH, 18)
                     << GiaoDienConsole::canTrai(monHoc.tenMH, 48)
                     << GiaoDienConsole::canTrai("   " + to_string(monHoc.soTCLT), 18)
                     << GiaoDienConsole::canTrai("   " + to_string(monHoc.soTCTH), 16);
            } else {
                GiaoDienConsole::datMau(TRANG, DEN);
                cout << string(108, ' ');
            }
        }
        if (soMonHocLoc == 0) {
            GiaoDienConsole::diDenToaDo(4, 13);
            GiaoDienConsole::datMau(DO_NHAT, DEN);
            cout << GiaoDienConsole::canGiua("Không tìm thấy môn học phù hợp với điều kiện tìm kiếm.", 108);
        }
    };

    GiaoDienConsole::xoaManHinh();
    GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH MÔN HỌC");
    GiaoDienConsole::datMau(VANG, LAM);
    GiaoDienConsole::diDenToaDo(4, 6);
    cout << GiaoDienConsole::canTrai(" STT", 8)
         << GiaoDienConsole::canTrai("MÃ MÔN HỌC", 18)
         << GiaoDienConsole::canTrai("TÊN MÔN HỌC", 48)
         << GiaoDienConsole::canTrai("STC LÝ THUYẾT", 18)
         << GiaoDienConsole::canTrai("STC THỰC HÀNH", 16);
    GiaoDienConsole::diDenToaDo(4, 7);
    GiaoDienConsole::datMau(XANH_NGOC_NHAT, DEN);
    cout << string(108, '-');

    while (true) {
        int soMonHoc = demMonHoc(root);
        MonHoc* mangMonHoc = soMonHoc > 0 ? new MonHoc[soMonHoc] : nullptr;
        int viTriMonHoc = 0;
        bstSangMang(root, mangMonHoc, viTriMonHoc);
        sapXepMonHocTheoTen(mangMonHoc, soMonHoc);

        int* monHocLoc = soMonHoc > 0 ? new int[soMonHoc] : nullptr;
        int soMonHocLoc = 0;
        string khoaMa = GiaoDienConsole::vietHoa(tuKhoa[0]);
        string khoaTen = GiaoDienConsole::vietHoa(tuKhoa[1]);
        string khoaTCLT = tuKhoa[2];
        string khoaTCTH = tuKhoa[3];
        for (int i = 0; i < soMonHoc; ++i) {
            string ma = GiaoDienConsole::vietHoa(mangMonHoc[i].maMH);
            string ten = GiaoDienConsole::vietHoa(mangMonHoc[i].tenMH);
            if ((!khoaMa.empty() && ma.find(khoaMa) == string::npos) ||
                (!khoaTen.empty() && ten.find(khoaTen) == string::npos) ||
                (!khoaTCLT.empty() && to_string(mangMonHoc[i].soTCLT).find(khoaTCLT) == string::npos) ||
                (!khoaTCTH.empty() && to_string(mangMonHoc[i].soTCTH).find(khoaTCTH) == string::npos)) continue;
            monHocLoc[soMonHocLoc++] = i;
        }

        if (mucDangChon >= soMonHocLoc) mucDangChon = max(0, soMonHocLoc - 1);
        int tongSoTrang = max(1, (soMonHocLoc + soDongMoiTrang - 1) / soDongMoiTrang);
        int trang = soMonHocLoc > 0 ? mucDangChon / soDongMoiTrang : 0;
        int viTriDau = trang * soDongMoiTrang;
        int viTriCuoi = min(viTriDau + soDongMoiTrang, soMonHocLoc);

        if (canVeOLoc) {
            for (int i = 0; i < 4; ++i) veOLoc(i);
            canVeOLoc = false;
        }
        if (canVeBang) {
            veBang(mangMonHoc, monHocLoc, soMonHocLoc, viTriDau, viTriCuoi);
            canVeBang = false;
            canVeTrangThai = true;
            mucChonTruoc = mucDangChon;
        }
        if (canVeTrangThai) {
            GiaoDienConsole::veNut(nutQuayLai, nutDangTro == 0, TRANG, XAM_DAM, DEN, VANG);
            GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | Tìm môn | ↑/↓ chọn | PgUp/PgDn trang | Enter xem | Esc về | Tìm không dấu", thongBao);
            thongBao.clear();
            canVeTrangThai = false;
            nutTruocDo = nutDangTro;
        }

        int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[oDangChon]);
        size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong[oDangChon] - rongNhan));
        size_t dau = viTriConTro[oDangChon] >= rongGiaTri && rongGiaTri > 0 ? viTriConTro[oDangChon] - rongGiaTri + 1 : 0;
        int toaDoConTro = toaDoX[oDangChon] + rongNhan + static_cast<int>(min(viTriConTro[oDangChon] - dau, rongGiaTri));
        GiaoDienConsole::diDenToaDo(toaDoConTro, 4);
        GiaoDienConsole::hienConTro(true);

        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        bool canLocLai = false;
        if (suKien.type == SK_CHUOT_DI_CHUYEN) {
            nutDangTro = nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY) ? 0 : -1;
            if (suKien.toaDoChuotY >= 8 && suKien.toaDoChuotY < 8 + soDongMoiTrang) {
                int chiSoLoc = viTriDau + suKien.toaDoChuotY - 8;
                if (chiSoLoc < viTriCuoi) mucDangChon = chiSoLoc;
            }
            if (nutDangTro != nutTruocDo) canVeTrangThai = true;
            if (mucDangChon != mucChonTruoc) {
                int mucCu = mucChonTruoc;
                if (mucCu >= viTriDau && mucCu < viTriCuoi) {
                    const MonHoc& monHocCu = mangMonHoc[monHocLoc[mucCu]];
                    GiaoDienConsole::diDenToaDo(4, 8 + mucCu - viTriDau);
                    GiaoDienConsole::datMau(TRANG, DEN);
                    cout << GiaoDienConsole::canTrai(" " + to_string(mucCu + 1), 8)
                         << GiaoDienConsole::canTrai(monHocCu.maMH, 18)
                         << GiaoDienConsole::canTrai(monHocCu.tenMH, 48)
                         << GiaoDienConsole::canTrai("   " + to_string(monHocCu.soTCLT), 18)
                         << GiaoDienConsole::canTrai("   " + to_string(monHocCu.soTCTH), 16);
                }
                if (mucDangChon >= viTriDau && mucDangChon < viTriCuoi) {
                    const MonHoc& monHocMoi = mangMonHoc[monHocLoc[mucDangChon]];
                    GiaoDienConsole::diDenToaDo(4, 8 + mucDangChon - viTriDau);
                    GiaoDienConsole::datMau(DEN, VANG);
                    cout << GiaoDienConsole::canTrai(" " + to_string(mucDangChon + 1), 8)
                         << GiaoDienConsole::canTrai(monHocMoi.maMH, 18)
                         << GiaoDienConsole::canTrai(monHocMoi.tenMH, 48)
                         << GiaoDienConsole::canTrai("   " + to_string(monHocMoi.soTCLT), 18)
                         << GiaoDienConsole::canTrai("   " + to_string(monHocMoi.soTCTH), 16);
                }
                mucChonTruoc = mucDangChon;
                canVeTrangThai = true;
            }
        } else if (suKien.type == SK_CHUOT_TRAI) {
            if (nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                GiaoDienConsole::hienConTro(false);
                delete[] monHocLoc;
                delete[] mangMonHoc;
                return;
            }
            if (suKien.toaDoChuotY == 4) {
                for (int i = 0; i < 4; ++i) {
                    if (suKien.toaDoChuotX >= toaDoX[i] && suKien.toaDoChuotX < toaDoX[i] + chieuRong[i]) {
                        oDangChon = i;
                        int rongNhanField = GiaoDienConsole::doDaiUtf8(nhan[i]);
                        size_t rongGiaTriField = static_cast<size_t>(max(0, chieuRong[i] - rongNhanField));
                        size_t dauField = viTriConTro[i] >= rongGiaTriField && rongGiaTriField > 0 ? viTriConTro[i] - rongGiaTriField + 1 : 0;
                        viTriConTro[i] = min(dauField + static_cast<size_t>(max(0, suKien.toaDoChuotX - toaDoX[i] - rongNhanField)), tuKhoa[i].size());
                        canVeOLoc = canVeTrangThai = true;
                        break;
                    }
                }
            } else if (suKien.toaDoChuotY >= 8 && suKien.toaDoChuotY < 8 + soDongMoiTrang) {
                int chiSoLoc = viTriDau + suKien.toaDoChuotY - 8;
                if (chiSoLoc < viTriCuoi) {
                    mucDangChon = chiSoLoc;
                    canVeBang = canVeTrangThai = true;
                }
            }
        }

        if (suKien.type == SK_PHIM_ESC || suKien.type == SK_PHIM_ENTER) {
            GiaoDienConsole::hienConTro(false);
            delete[] monHocLoc;
            delete[] mangMonHoc;
            return;
        } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_SHIFT_TAB) {
            int buoc = suKien.type == SK_PHIM_SHIFT_TAB ? -1 : 1;
            oDangChon = (oDangChon + buoc + 4) % 4;
            canVeOLoc = true;
        } else if (suKien.type == SK_PHIM_TRAI) {
            if (viTriConTro[oDangChon] > 0) --viTriConTro[oDangChon];
        } else if (suKien.type == SK_PHIM_PHAI) {
            if (viTriConTro[oDangChon] < tuKhoa[oDangChon].size()) ++viTriConTro[oDangChon];
        } else if (suKien.type == EVT_KEY_HOME) {
            viTriConTro[oDangChon] = 0;
        } else if (suKien.type == EVT_KEY_END) {
            viTriConTro[oDangChon] = tuKhoa[oDangChon].size();
        } else if (suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
            if (suKien.type == EVT_KEY_BACKSPACE && viTriConTro[oDangChon] > 0) {
                tuKhoa[oDangChon].erase(viTriConTro[oDangChon] - 1, 1);
                --viTriConTro[oDangChon];
                canLocLai = true;
            } else if (suKien.type == EVT_KEY_DELETE && viTriConTro[oDangChon] < tuKhoa[oDangChon].size()) {
                tuKhoa[oDangChon].erase(viTriConTro[oDangChon], 1);
                canLocLai = true;
            }
        } else if (suKien.type == SK_KY_TU) {
            char kyTu = suKien.ch;
            bool hopLe = oDangChon < 2 ? (kyTu >= 32 && kyTu <= 126) : (kyTu >= '0' && kyTu <= '9');
            if (hopLe && tuKhoa[oDangChon].size() < doDaiToiDa[oDangChon]) {
                if (oDangChon == 0 && kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
                tuKhoa[oDangChon].insert(viTriConTro[oDangChon], 1, kyTu);
                ++viTriConTro[oDangChon];
                canLocLai = true;
            }
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_LEN || suKien.type == SK_CON_LAN_XUONG || suKien.type == EVT_KEY_PGUP || suKien.type == EVT_KEY_PGDN) {
            if (soMonHocLoc > 0) {
                if (suKien.type == EVT_KEY_PGUP || suKien.type == EVT_KEY_PGDN) {
                    int buocTrang = suKien.type == EVT_KEY_PGUP ? -soDongMoiTrang : soDongMoiTrang;
                    mucDangChon = max(0, min(soMonHocLoc - 1, mucDangChon + buocTrang));
                } else {
                    int buoc = (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) ? -1 : 1;
                    mucDangChon = (mucDangChon + buoc + soMonHocLoc) % soMonHocLoc;
                }
                canVeBang = canVeTrangThai = true;
            }
        }

        if (canLocLai) {
            mucDangChon = 0;
            canVeOLoc = canVeBang = canVeTrangThai = true;
        }
        delete[] monHocLoc;
        delete[] mangMonHoc;
    }
}

// ============================================================================
// CHỨC NĂNG (g): ĐĂNG KÝ LỚP TÍN CHỈ
// ============================================================================

// ============================================================================
void chucNangG_DangKyLopTinChi(PTRMH root, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi) {
    auto canTrai = [](const string& vanBan, int chieuRong) {
        string ketQua = vanBan;
        while (!ketQua.empty() && GiaoDienConsole::doDaiUtf8(ketQua) > chieuRong) {
            size_t dauKyTuCuoi = ketQua.size() - 1;
            while (dauKyTuCuoi > 0 && (static_cast<unsigned char>(ketQua[dauKyTuCuoi]) & 0xC0) == 0x80) --dauKyTuCuoi;
            ketQua.erase(dauKyTuCuoi);
        }
        int doDai = GiaoDienConsole::doDaiUtf8(ketQua);
        ketQua += string(chieuRong - doDai, ' ');
        return ketQua;
    };
    string maSV = "";
    size_t viTriConTroMaSV = 0;
    NodeSV* nutSinhVien = nullptr;
    int viTriLop = -1;
    GiaoDienConsole::xoaManHinh();
    GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "ĐĂNG KÝ LỚP TÍN CHỈ - TÌM SINH VIÊN");
    string thongBaoTimSV;
    bool canVeOThongTinSV = true;
    bool canVeKetQuaSV = true;
    bool canVeTrangThaiSV = true;
    while (true) {
        if (canVeOThongTinSV) {
            GiaoDienConsole::diDenToaDo(4, 5);
            GiaoDienConsole::datMau(DEN, VANG);
            cout << canTrai("Mã SV: " + maSV, 23);
            canVeOThongTinSV = false;
        }
        if (canVeKetQuaSV) {
            GiaoDienConsole::diDenToaDo(4, 8);
            GiaoDienConsole::datMau(nutSinhVien != nullptr ? XANH_LA_NHAT : TRANG, DEN);
            cout << canTrai(nutSinhVien != nullptr ? "Tìm thấy: " + string(nutSinhVien->sv.maSV) + " - " + nutSinhVien->sv.ho + " " + nutSinhVien->sv.ten : "", 108);
            GiaoDienConsole::diDenToaDo(4, 10);
            GiaoDienConsole::datMau(nutSinhVien != nullptr ? XANH_NGOC_NHAT : DEN, DEN);
            cout << canTrai(nutSinhVien != nullptr ? "Nhấn Enter để tiếp tục đăng ký cho sinh viên này." : "", 108);
            canVeKetQuaSV = false;
        }
        if (canVeTrangThaiSV) {
            GiaoDienConsole::veThanhTrangThai("Nhập mã SV (không dấu) | ←/→: di chuyển con trỏ | Enter: xác nhận | Esc: quay lại", thongBaoTimSV, !thongBaoTimSV.empty());
            canVeTrangThaiSV = false;
        }
        int doDaiMa = GiaoDienConsole::doDaiUtf8("Mã SV: ");
        GiaoDienConsole::diDenToaDo(4 + doDaiMa + static_cast<int>(viTriConTroMaSV), 5);
        GiaoDienConsole::hienConTro(true);
        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        if (suKien.type == SK_PHIM_ESC) {
            GiaoDienConsole::hienConTro(false);
            return;
        }
        if (suKien.type == SK_PHIM_TRAI && viTriConTroMaSV > 0) --viTriConTroMaSV;
        else if (suKien.type == SK_PHIM_PHAI && viTriConTroMaSV < maSV.size()) ++viTriConTroMaSV;
        else if (suKien.type == EVT_KEY_HOME) viTriConTroMaSV = 0;
        else if (suKien.type == EVT_KEY_END) viTriConTroMaSV = maSV.size();
        else if (suKien.type == EVT_KEY_BACKSPACE && viTriConTroMaSV > 0) {
            maSV.erase(--viTriConTroMaSV, 1);
        } else if (suKien.type == EVT_KEY_DELETE && viTriConTroMaSV < maSV.size()) {
            maSV.erase(viTriConTroMaSV, 1);
        } else if (suKien.type == SK_KY_TU && maSV.size() < 15 && suKien.ch >= 32 && suKien.ch <= 126) {
            char kyTu = suKien.ch;
            if (kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
            maSV.insert(viTriConTroMaSV++, 1, kyTu);
        }
        if (suKien.type == SK_KY_TU || suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
            nutSinhVien = maSV.empty() ? nullptr : timSinhVienToanTruong(danhSachLop, maSV.c_str(), &viTriLop);
            canVeOThongTinSV = canVeKetQuaSV = canVeTrangThaiSV = true;
        }
        if (suKien.type == SK_PHIM_ENTER) {
            if (nutSinhVien != nullptr) break;
            thongBaoTimSV = maSV.empty() ? "Nhập mã sinh viên trước khi xác nhận." : "Không tìm thấy sinh viên có mã '" + maSV + "'.";
            canVeTrangThaiSV = true;
        } else if (suKien.type == SK_KY_TU || suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
            thongBaoTimSV.clear();
        }
    }
    GiaoDienConsole::hienConTro(false);

    SinhVien& sv = nutSinhVien->sv;
    Lop& lopSinhVien = danhSachLop.lop[viTriLop];
    string tuKhoa[5] = { "2025-2026", "1", "", "", "" };
    size_t viTriConTro[5] = {tuKhoa[0].size(), tuKhoa[1].size(), 0, 0, 0};
    int oDangChon = 0;
    const int toaDoX[] = {4, 30, 40, 4, 67};
    const int toaDoY[] = {5, 5, 5, 6, 6};
    const int chieuRong[] = {23, 6, 18, 60, 9};
    const string nhan[] = {"Niên khóa: ", "HK: ", "Mã MH: ", "Tên môn: ", "Nhóm: "};
    const size_t doDaiToiDa[] = {11, 1, 10, 50, 2};
    int* viTriLopTinChiDaLoc = new int[max(1, danhSachLopTinChi.soLuong)];
    int soMucPhuHop = 0;
        int mucDangChon = 0;
        int mucChonTruoc = -1;
        string thongBao = "";
        bool coLoi = false;
    bool canVeKhung = false;
    bool canVeOLoc = true;
    bool canVeBang = true;
    bool canVeTrangThai = true;
        int nutDangTro = -1;
        int nutTruocDo = -1;
    int viTriDauBang = 0;
    int viTriCuoiBang = 0;
        NutChuNhat* cacNut = new NutChuNhat[2];
    cacNut[0] = { 22, 23, 34, 1, 0, "[ ĐĂNG KÝ / HỦY ĐK (Enter) ]" };
    cacNut[1] = { 62, 23, 28, 1, 1, "[ QUAY LẠI (ESC) ]" };

    auto veOLoc = [&](int i) {
        GiaoDienConsole::diDenToaDo(toaDoX[i], toaDoY[i]);
        int mauNen = oDangChon == i ? VANG : XANH_NGOC;
        GiaoDienConsole::datMau(oDangChon == i ? DEN : TRANG, mauNen);
        int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
        size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong[i] - rongNhan));
        size_t dau = viTriConTro[i] >= rongGiaTri && rongGiaTri > 0 ? viTriConTro[i] - rongGiaTri + 1 : 0;
        string noiDung = nhan[i] + tuKhoa[i].substr(dau, rongGiaTri);
        cout << noiDung << string(max(0, chieuRong[i] - GiaoDienConsole::doDaiUtf8(noiDung)), ' ');
    };

    auto napDanhSachLoc = [&]() {
        soMucPhuHop = 0;
        string khoaNienKhoa = GiaoDienConsole::vietHoa(tuKhoa[0]);
        string khoaHocKy = tuKhoa[1];
        string khoaMaMH = GiaoDienConsole::vietHoa(tuKhoa[2]);
        string khoaTenMH = GiaoDienConsole::vietHoa(tuKhoa[3]);
        string khoaNhom = tuKhoa[4];
        for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
            PTRLTC lopTinChi = danhSachLopTinChi.lopTC[i];
            if (lopTinChi == nullptr || lopTinChi->huyLop) continue;
            PTRMH monHoc = timMonHoc(root, lopTinChi->maMH);
            string maMH = GiaoDienConsole::vietHoa(lopTinChi->maMH);
            string tenMH = monHoc ? GiaoDienConsole::vietHoa(monHoc->mh.tenMH) : maMH;
            if ((!khoaNienKhoa.empty() && GiaoDienConsole::vietHoa(lopTinChi->nienKhoa).find(khoaNienKhoa) == string::npos) ||
                (!khoaHocKy.empty() && to_string(lopTinChi->hocKy).find(khoaHocKy) == string::npos) ||
                (!khoaMaMH.empty() && maMH.find(khoaMaMH) == string::npos) ||
                (!khoaTenMH.empty() && tenMH.find(khoaTenMH) == string::npos) ||
                (!khoaNhom.empty() && to_string(lopTinChi->nhom).find(khoaNhom) == string::npos)) continue;
            viTriLopTinChiDaLoc[soMucPhuHop++] = i;
        }
        if (mucDangChon >= soMucPhuHop) mucDangChon = max(0, soMucPhuHop - 1);
    };

    auto veMotDong = [&](int chiSo, bool dangDuocChon) {
        if (chiSo < viTriDauBang || chiSo >= viTriCuoiBang || chiSo < 0 || chiSo >= soMucPhuHop) return;
        PTRLTC lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChiDaLoc[chiSo]];
        PTRMH monHoc = timMonHoc(root, lopTinChi->maMH);
        string tenMonHoc = monHoc ? monHoc->mh.tenMH : lopTinChi->maMH;
        int daDK = demSVDangKyHieuLuc(lopTinChi->dssvdk);
        int conTrong = max(0, lopTinChi->maxSV - daDK);
        NodeDK* nutDangKy = timDangKy(lopTinChi->dssvdk, sv.maSV);
        bool dangDK = nutDangKy != nullptr && !nutDangKy->dk.huyDangKy;
        GiaoDienConsole::datMau(dangDuocChon ? DEN : (dangDK ? XANH_LA_NHAT : TRANG), dangDuocChon ? VANG : DEN);
        GiaoDienConsole::diDenToaDo(4, 10 + chiSo - viTriDauBang);
        cout << GiaoDienConsole::canTrai(lopTinChi->maMH, 12)
             << GiaoDienConsole::canTrai(tenMonHoc, 36)
             << GiaoDienConsole::canTrai("  " + to_string(lopTinChi->nhom), 8)
             << GiaoDienConsole::canTrai("    " + to_string(daDK), 16)
             << GiaoDienConsole::canTrai("      " + to_string(conTrong), 16)
             << GiaoDienConsole::canTrai(dangDK ? "[ĐÃ ĐĂNG KÝ]" : "Chưa đăng ký", 20);
    };

    auto veKhungChinh = [&]() {
        GiaoDienConsole::xoaManHinh();
        GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "ĐĂNG KÝ LỚP TÍN CHỈ");
        GiaoDienConsole::datMau(VANG, LAM);
        GiaoDienConsole::diDenToaDo(4, 3);
        cout << canTrai("Sinh viên: " + string(sv.maSV) + " - " + sv.ho + " " + sv.ten + " | Lớp: " + lopSinhVien.maLop, 108);
        GiaoDienConsole::diDenToaDo(4, 8);
        cout << GiaoDienConsole::canTrai("MÃ MH", 12)
             << GiaoDienConsole::canTrai("TÊN MÔN HỌC", 36)
             << GiaoDienConsole::canTrai("NHÓM", 8)
             << GiaoDienConsole::canTrai("SỐ SV ĐÃ ĐK", 16)
             << GiaoDienConsole::canTrai("SLOT CÒN TRỐNG", 16)
             << GiaoDienConsole::canTrai("TRẠNG THÁI CỦA BẠN", 20);
        GiaoDienConsole::diDenToaDo(4, 9);
        GiaoDienConsole::datMau(XANH_NGOC_NHAT, DEN);
        cout << string(108, '-');
    };
    veKhungChinh();

    while (true) {
        if (canVeKhung) {
            veKhungChinh();
            canVeKhung = false;
            canVeOLoc = canVeBang = canVeTrangThai = true;
        }
        napDanhSachLoc();
        int tongSoTrang = max(1, (soMucPhuHop + 10) / 11);
        int trang = soMucPhuHop > 0 ? mucDangChon / 11 : 0;
        int viTriDau = trang * 11;
        int viTriCuoi = min(viTriDau + 11, soMucPhuHop);
        viTriDauBang = viTriDau;
        viTriCuoiBang = viTriCuoi;
        if (canVeOLoc) {
            for (int i = 0; i < 5; ++i) veOLoc(i);
            canVeOLoc = false;
        }
        if (canVeBang) {
            for (int r = 0; r < 11; ++r) {
                int chiSo = viTriDau + r;
                GiaoDienConsole::diDenToaDo(4, 10 + r);
                if (chiSo < viTriCuoi) veMotDong(chiSo, chiSo == mucDangChon);
                else { GiaoDienConsole::datMau(TRANG, DEN); cout << string(108, ' '); }
            }
            if (soMucPhuHop == 0) {
                GiaoDienConsole::diDenToaDo(4, 14);
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                cout << GiaoDienConsole::canGiua("Không tìm thấy lớp tín chỉ phù hợp với các điều kiện lọc.", 108);
            }
            canVeBang = false;
            canVeTrangThai = true;
            mucChonTruoc = mucDangChon;
        }
        if (canVeTrangThai) {
            GiaoDienConsole::veNut(cacNut[0], nutDangTro == 0, TRANG, XANH_LA, DEN, VANG);
            GiaoDienConsole::veNut(cacNut[1], nutDangTro == 1, TRANG, XAM_DAM, DEN, VANG);
            GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | Lọc lớp TC | ↑/↓ chọn | PgUp/PgDn trang | Enter đăng ký/hủy | Esc thoát | Tìm không dấu", thongBao, coLoi);
            thongBao.clear();
            coLoi = false;
            canVeTrangThai = false;
            nutTruocDo = nutDangTro;
        }

        int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[oDangChon]);
        size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong[oDangChon] - rongNhan));
        size_t dau = viTriConTro[oDangChon] >= rongGiaTri && rongGiaTri > 0 ? viTriConTro[oDangChon] - rongGiaTri + 1 : 0;
        size_t lechConTro = rongGiaTri > 0 ? min(viTriConTro[oDangChon] - dau, rongGiaTri - 1) : 0;
        int toaDoConTro = toaDoX[oDangChon] + rongNhan + static_cast<int>(lechConTro);
        GiaoDienConsole::diDenToaDo(toaDoConTro, toaDoY[oDangChon]);
        GiaoDienConsole::hienConTro(true);

        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        int hanhDong = -1;
        bool canLocLai = false;
        if (suKien.type == SK_CHUOT_DI_CHUYEN) {
            nutDangTro = cacNut[0].contains(suKien.toaDoChuotX, suKien.toaDoChuotY) ? 0 :
                         (cacNut[1].contains(suKien.toaDoChuotX, suKien.toaDoChuotY) ? 1 : -1);
            if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 10 && suKien.toaDoChuotY < 21) {
                int chiSo = viTriDau + suKien.toaDoChuotY - 10;
                if (chiSo < viTriCuoi) mucDangChon = chiSo;
            }
            if (nutDangTro != nutTruocDo) canVeTrangThai = true;
            if (mucDangChon != mucChonTruoc) {
                if (mucChonTruoc >= viTriDau && mucChonTruoc < viTriCuoi) veMotDong(mucChonTruoc, false);
                if (mucDangChon >= viTriDau && mucDangChon < viTriCuoi) veMotDong(mucDangChon, true);
                mucChonTruoc = mucDangChon;
                if (mucDangChon / 11 != trang) canVeTrangThai = true;
            }
        } else if (suKien.type == SK_CHUOT_TRAI) {
            if (cacNut[0].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) hanhDong = 0;
            else if (cacNut[1].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) hanhDong = 1;
            else if (suKien.toaDoChuotY == 5 || suKien.toaDoChuotY == 6) {
                for (int i = 0; i < 5; ++i) {
                    if (suKien.toaDoChuotY == toaDoY[i] && suKien.toaDoChuotX >= toaDoX[i] && suKien.toaDoChuotX < toaDoX[i] + chieuRong[i]) {
                        oDangChon = i;
                        int nhanRong = GiaoDienConsole::doDaiUtf8(nhan[i]);
                        size_t giaTriRong = static_cast<size_t>(max(0, chieuRong[i] - nhanRong));
                        size_t dauHien = viTriConTro[i] >= giaTriRong && giaTriRong > 0 ? viTriConTro[i] - giaTriRong + 1 : 0;
                        viTriConTro[i] = min(dauHien + static_cast<size_t>(max(0, suKien.toaDoChuotX - toaDoX[i] - nhanRong)), tuKhoa[i].size());
                        canVeOLoc = true;
                        break;
                    }
                }
            } else if (suKien.toaDoChuotY >= 10 && suKien.toaDoChuotY < 21) {
                int chiSo = viTriDau + suKien.toaDoChuotY - 10;
                if (chiSo < viTriCuoi) { mucDangChon = chiSo; canVeBang = true; }
            }
        }

            if (suKien.type == SK_PHIM_ESC || hanhDong == 1) {
                luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                delete[] cacNut;
                delete[] viTriLopTinChiDaLoc;
                GiaoDienConsole::hienConTro(false);
                return;
            } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
                if (soMucPhuHop > 0) { mucDangChon = max(0, mucDangChon - 1); canVeBang = true; }
            } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
                if (soMucPhuHop > 0) { mucDangChon = min(soMucPhuHop - 1, mucDangChon + 1); canVeBang = true; }
            } else if (suKien.type == EVT_KEY_PGUP || suKien.type == EVT_KEY_PGDN) {
                if (soMucPhuHop > 0) {
                    int buoc = suKien.type == EVT_KEY_PGUP ? -11 : 11;
                    mucDangChon = max(0, min(soMucPhuHop - 1, mucDangChon + buoc));
                    canVeBang = true;
                }
            } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_SHIFT_TAB) {
                int buoc = suKien.type == SK_PHIM_SHIFT_TAB ? -1 : 1;
                oDangChon = (oDangChon + buoc + 5) % 5;
                canVeOLoc = true;
            } else if (suKien.type == SK_PHIM_TRAI) {
                if (viTriConTro[oDangChon] > 0) --viTriConTro[oDangChon];
            } else if (suKien.type == SK_PHIM_PHAI) {
                if (viTriConTro[oDangChon] < tuKhoa[oDangChon].size()) ++viTriConTro[oDangChon];
            } else if (suKien.type == EVT_KEY_HOME) {
                viTriConTro[oDangChon] = 0;
            } else if (suKien.type == EVT_KEY_END) {
                viTriConTro[oDangChon] = tuKhoa[oDangChon].size();
            } else if (suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
                if (suKien.type == EVT_KEY_BACKSPACE && viTriConTro[oDangChon] > 0) {
                    tuKhoa[oDangChon].erase(viTriConTro[oDangChon] - 1, 1);
                    --viTriConTro[oDangChon];
                    canLocLai = true;
                } else if (suKien.type == EVT_KEY_DELETE && viTriConTro[oDangChon] < tuKhoa[oDangChon].size()) {
                    tuKhoa[oDangChon].erase(viTriConTro[oDangChon], 1);
                    canLocLai = true;
                }
            } else if (suKien.type == SK_KY_TU) {
                char kyTu = suKien.ch;
                bool hopLe = oDangChon == 0 ? (isdigit(static_cast<unsigned char>(kyTu)) || kyTu == '-') :
                             (oDangChon == 1 || oDangChon == 4) ? isdigit(static_cast<unsigned char>(kyTu)) :
                             (kyTu >= 32 && kyTu <= 126);
                if (hopLe && tuKhoa[oDangChon].size() < doDaiToiDa[oDangChon]) {
                    if ((oDangChon == 2) && kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
                    tuKhoa[oDangChon].insert(viTriConTro[oDangChon]++, 1, kyTu);
                    canLocLai = true;
                }
            } else if ((suKien.type == SK_PHIM_ENTER || hanhDong == 0) && soMucPhuHop > 0) {
                PTRLTC lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChiDaLoc[mucDangChon]];
                NodeDK* nutDangKy = timDangKy(lopTinChi->dssvdk, sv.maSV);
                bool dangDK = (nutDangKy != nullptr && !nutDangKy->dk.huyDangKy);

                if (dangDK) {
                    if (nutDangKy->dk.diem >= 0.0f) {
                        thongBao = "Không thể hủy đăng ký vì sinh viên đã có điểm môn học này!";
                        coLoi = true;
                    } else {
                        bool xacNhanHuy = GiaoDienConsole::hopXacNhan("XÁC NHẬN HỦY ĐĂNG KÝ",
                            "Bạn muốn hủy đăng ký môn " + string(lopTinChi->maMH) + " - Nhóm " + to_string(lopTinChi->nhom) + "?");
                        canVeKhung = true;
                        if (xacNhanHuy) {
                            nutDangKy->dk.huyDangKy = true;
                            luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                            thongBao = "Đã hủy đăng ký lớp tín chỉ môn " + string(lopTinChi->maMH) + "!";
                            coLoi = false;
                        }
                    }
                } else {
                    bool trungMon = false;
                    for (int k = 0; k < soMucPhuHop; ++k) {
                        if (k == mucDangChon) continue;
                        PTRLTC other = danhSachLopTinChi.lopTC[viTriLopTinChiDaLoc[k]];
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
                        luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                        thongBao = "Đăng ký thành công môn " + string(lopTinChi->maMH) + " - Nhóm " + to_string(lopTinChi->nhom) + "!";
                        coLoi = false;
                    }
                }
                canVeBang = canVeTrangThai = true;
            }
        if (canLocLai) {
            napDanhSachLoc();
            mucDangChon = 0;
            canVeOLoc = canVeBang = canVeTrangThai = true;
        }
        if (canVeBang && soMucPhuHop == 0) canVeTrangThai = true;
    }
}

// ============================================================================
// CHỨC NĂNG (h): HỦY LỚP TÍN CHỈ (Số SV ĐK < Số SV Min)
// ============================================================================

// ============================================================================
void chucNangH_HuyLopTinChi(PTRMH root, const DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi) {
    auto xemChiTietVaHuy = [&](int viTriLopTinChi) {
        PTRLTC lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChi];
        int soBanGhi = demTongDangKy(lopTinChi->dssvdk);
        int trang = 0;
        const int soDongMoiTrang = 12;
        bool veLai = true;
        NutChuNhat nutHuy = { 28, 23, 27, 1, 1, "[ HỦY LỚP (Enter) ]" };
        NutChuNhat nutQuayLai = { 66, 23, 24, 1, 0, "[ QUAY LẠI (Esc) ]" };
        int nutDangTro = -1;
        int nutTruocDo = -2;

        while (true) {
            if (veLai) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "CHI TIẾT LỚP TÍN CHỈ - XÁC NHẬN HỦY");
                PTRMH monHoc = timMonHoc(root, lopTinChi->maMH);
                GiaoDienConsole::datMau(VANG, DEN);
                GiaoDienConsole::diDenToaDo(4, 4);
                cout << "Môn học: " << (monHoc ? monHoc->mh.tenMH : lopTinChi->maMH) << " (" << lopTinChi->maMH << ")";
                GiaoDienConsole::datMau(VANG, DEN);
                GiaoDienConsole::diDenToaDo(4, 5);
                cout << "Niên khóa: " << lopTinChi->nienKhoa << " | Học kỳ: " << lopTinChi->hocKy << " | Nhóm: " << lopTinChi->nhom;
                int soDangKyHieuLuc = demSVDangKyHieuLuc(lopTinChi->dssvdk);
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                GiaoDienConsole::diDenToaDo(4, 6);
                cout << "Trạng thái: Đang mở | Sĩ số đăng ký: " << soDangKyHieuLuc << "/" << lopTinChi->minSV << " tối thiểu";
                GiaoDienConsole::datMau(VANG, LAM);
                GiaoDienConsole::diDenToaDo(4, 8);
                cout << GiaoDienConsole::canTrai("STT", 8)
                     << GiaoDienConsole::canTrai("MÃ SINH VIÊN", 18)
                     << GiaoDienConsole::canTrai("HỌ VÀ TÊN SINH VIÊN", 52)
                     << GiaoDienConsole::canTrai("TRẠNG THÁI ĐĂNG KÝ", 30);
                GiaoDienConsole::datMau(XANH_NGOC_NHAT, DEN);
                GiaoDienConsole::diDenToaDo(4, 9);
                cout << string(108, '-');

                int viTriDau = trang * soDongMoiTrang;
                NodeDK* dangKy = lopTinChi->dssvdk;
                for (int i = 0; i < viTriDau && dangKy != nullptr; ++i) dangKy = dangKy->next;
                for (int dong = 0; dong < soDongMoiTrang; ++dong) {
                    GiaoDienConsole::diDenToaDo(4, 10 + dong);
                    if (dangKy == nullptr) {
                        GiaoDienConsole::datMau(TRANG, DEN);
                        cout << string(108, ' ');
                        continue;
                    }
                    NodeSV* sinhVien = timSinhVienToanTruong(danhSachLop, dangKy->dk.maSV);
                    string hoTen = sinhVien ? string(sinhVien->sv.ho) + " " + sinhVien->sv.ten : "(Không tìm thấy sinh viên)";
                    GiaoDienConsole::datMau(dangKy->dk.huyDangKy ? XAM_NHAT : TRANG, DEN);
                    cout << GiaoDienConsole::canTrai(to_string(viTriDau + dong + 1), 8)
                         << GiaoDienConsole::canTrai(dangKy->dk.maSV, 18)
                         << GiaoDienConsole::canTrai(hoTen, 52)
                         << GiaoDienConsole::canTrai(dangKy->dk.huyDangKy ? "Đã hủy đăng ký" : "Đã đăng ký", 30);
                    dangKy = dangKy->next;
                }
                if (soBanGhi == 0) {
                    GiaoDienConsole::datMau(XANH_LA_NHAT, DEN);
                    GiaoDienConsole::diDenToaDo(4, 13);
                    cout << GiaoDienConsole::canGiua("Lớp chưa có sinh viên đăng ký.", 108);
                }
                GiaoDienConsole::veNut(nutHuy, nutDangTro == 0, TRANG, DO, DEN, VANG);
                GiaoDienConsole::veNut(nutQuayLai, nutDangTro == 1, TRANG, XAM_DAM, DEN, VANG);
                int tongSoTrang = max(1, (soBanGhi + soDongMoiTrang - 1) / soDongMoiTrang);
                GiaoDienConsole::veThanhTrangThai("Enter: xác nhận hủy lớp | Esc: quay lại | Lên/Xuống hoặc cuộn chuột: xem danh sách sinh viên | Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang));
                veLai = false;
                nutTruocDo = nutDangTro;
            }

            SuKienNhap suKien = GiaoDienConsole::docSuKien();
            if (suKien.type == SK_CHUOT_DI_CHUYEN) {
                nutDangTro = nutHuy.contains(suKien.toaDoChuotX, suKien.toaDoChuotY) ? 0 :
                             (nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY) ? 1 : -1);
                if (nutDangTro != nutTruocDo) {
                    GiaoDienConsole::veNut(nutHuy, nutDangTro == 0, TRANG, DO, DEN, VANG);
                    GiaoDienConsole::veNut(nutQuayLai, nutDangTro == 1, TRANG, XAM_DAM, DEN, VANG);
                    nutTruocDo = nutDangTro;
                }
                continue;
            }
            if (suKien.type == SK_PHIM_ESC || (suKien.type == SK_CHUOT_TRAI && nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY))) return false;
            if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
                if (trang > 0) { --trang; veLai = true; }
                continue;
            }
            if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
                int tongSoTrang = max(1, (soBanGhi + soDongMoiTrang - 1) / soDongMoiTrang);
                if (trang + 1 < tongSoTrang) { ++trang; veLai = true; }
                continue;
            }
            if (suKien.type == SK_PHIM_ENTER || (suKien.type == SK_CHUOT_TRAI && nutHuy.contains(suKien.toaDoChuotX, suKien.toaDoChuotY))) {
                int soDangKy = demSVDangKyHieuLuc(lopTinChi->dssvdk);
                if (soDangKy >= lopTinChi->minSV || lopTinChi->huyLop) {
                    GiaoDienConsole::hopThongBao("KHÔNG THỂ HỦY", "Lớp không còn thuộc danh sách chờ hủy.", true);
                    return false;
                }
                string thongTin = "Đang có " + to_string(soDangKy) + " sinh viên đăng ký; mức tối thiểu là " + to_string(lopTinChi->minSV) + ".";
                if (GiaoDienConsole::hopXacNhan("XÁC NHẬN HỦY LỚP TÍN CHỈ", "Hủy lớp " + string(lopTinChi->maMH) + " - Nhóm " + to_string(lopTinChi->nhom) + "?", thongTin)) {
                    lopTinChi->huyLop = true;
                    luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                    GiaoDienConsole::hopThongBao("THÀNH CÔNG", "Đã hủy lớp tín chỉ. Lịch sử đăng ký của sinh viên được giữ lại.");
                    return true;
                }
                veLai = true;
            }
        }
    };

    while (true) {
        int viTriLopTinChi = chonLopTinChiTheoThongTin(root, danhSachLopTinChi,
            "CHỨC NĂNG (h): TÌM VÀ HỦY LỚP TÍN CHỈ", true);
        if (viTriLopTinChi < 0) return;
        PTRLTC lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChi];
        int soDangKy = demSVDangKyHieuLuc(lopTinChi->dssvdk);
        if (!lopTinChi->huyLop && soDangKy < lopTinChi->minSV) {
            xemChiTietVaHuy(viTriLopTinChi);
        }
    }
}

// ============================================================================
// CHỨC NĂNG (i): NHẬP ĐIỂM TRỰC TIẾP TRÊN KHUNG CỘT ĐIỂM (KHÔNG NHẤP NHÁY)
// ============================================================================


