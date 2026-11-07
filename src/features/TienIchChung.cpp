#include "ui/GiaoDienConsole.h"
#include "features/TienIchChung.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>

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
// HỘP THOẠI CHỌN MÔN HỌC, TÌM NHÚNG THEO MÃ HOẶC TÊN
// ============================================================================
string chonMonHocNhanh(PTRMH root) {
    int soMonHoc = demMonHoc(root);
    if (soMonHoc <= 0) return "";

    MonHoc* cacMonHoc = new MonHoc[soMonHoc];
    int viTriGhi = 0;
    bstSangMang(root, cacMonHoc, viTriGhi);
    int* cacMonHocLoc = new int[soMonHoc];
    string tuKhoaMa, tuKhoaTen;
    size_t viTriConTroMa = 0, viTriConTroTen = 0;
    int oDangChon = 0;
    int mucDangChon = 0;
    int soDongMoiTrang = 14;
    int soMonHocLoc = 0;
    int mucChonTruoc = -1;
    int trangTruoc = -1;
    bool canVeLaiToanBo = true;
    bool canVeLaiTuKhoa = true;
    bool canVeLaiDanhSach = true;
    bool canVeLaiThanhTrangThai = true;
    NutChuNhat nutHuy = { 89, 23, 20, 1, 0, "[ HỦY (ESC) ]" };

    auto veBoLoc = [&]() {
        const int toaDoX[] = {4, 36};
        const int chieuRong[] = {28, 59};
        const string nhan[] = {"Mã môn: ", "Tên môn: "};
        const string* tuKhoa[] = {&tuKhoaMa, &tuKhoaTen};
        const size_t viTriConTro[] = {viTriConTroMa, viTriConTroTen};
        for (int i = 0; i < 2; ++i) {
            GiaoDienConsole::diDenToaDo(toaDoX[i], 4);
            GiaoDienConsole::datMau(oDangChon == i ? DEN : TRANG, oDangChon == i ? VANG : XANH_NGOC);
            int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
            int rongGiaTri = chieuRong[i] - rongNhan;
            size_t dau = viTriConTro[i] > static_cast<size_t>(rongGiaTri) && rongGiaTri > 0 ? viTriConTro[i] - rongGiaTri : 0;
            string hienThi = nhan[i] + tuKhoa[i]->substr(dau, static_cast<size_t>(rongGiaTri));
            int doDai = GiaoDienConsole::doDaiUtf8(hienThi);
            cout << hienThi << string(max(0, chieuRong[i] - doDai), ' ');
        }
    };
    auto datViTriConTro = [&]() {
        const int toaDoX[] = {4, 36};
        const int chieuRong[] = {28, 59};
        const string nhan[] = {"Mã môn: ", "Tên môn: "};
        const size_t viTriConTro[] = {viTriConTroMa, viTriConTroTen};
        int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[oDangChon]);
        int rongGiaTri = chieuRong[oDangChon] - rongNhan;
        size_t dau = viTriConTro[oDangChon] > static_cast<size_t>(rongGiaTri) && rongGiaTri > 0 ? viTriConTro[oDangChon] - rongGiaTri : 0;
        int x = toaDoX[oDangChon] + rongNhan + static_cast<int>(min(viTriConTro[oDangChon] - dau, static_cast<size_t>(rongGiaTri)));
        GiaoDienConsole::diDenToaDo(x, 4);
        GiaoDienConsole::hienConTro(true);
    };
    auto veMotDong = [&](int chiSo, int viTriDau, bool duocChon) {
        int r = chiSo - viTriDau;
        if (r < 0 || r >= soDongMoiTrang) return;
        GiaoDienConsole::diDenToaDo(4, 8 + r);
        if (chiSo >= 0 && chiSo < soMonHocLoc) {
            const MonHoc& monHoc = cacMonHoc[cacMonHocLoc[chiSo]];
            GiaoDienConsole::datMau(duocChon ? DEN : TRANG, duocChon ? VANG : DEN);
            cout << GiaoDienConsole::canTrai(" " + to_string(chiSo + 1), 8)
                 << GiaoDienConsole::canTrai(monHoc.maMH, 18)
                 << GiaoDienConsole::canTrai(monHoc.tenMH, 58)
                 << GiaoDienConsole::canTrai(to_string(monHoc.soTCLT + monHoc.soTCTH) + " tín chỉ", 24);
        } else {
            GiaoDienConsole::datMau(XAM_NHAT, DEN);
            cout << string(108, ' ');
        }
    };

    while (true) {
        soMonHocLoc = 0;
        for (int i = 0; i < soMonHoc; ++i) {
            string ma = GiaoDienConsole::vietHoa(cacMonHoc[i].maMH);
            string ten = GiaoDienConsole::vietHoa(cacMonHoc[i].tenMH);
            string khoaMa = GiaoDienConsole::vietHoa(tuKhoaMa);
            string khoaTen = GiaoDienConsole::vietHoa(tuKhoaTen);
            if ((!khoaMa.empty() && ma.find(khoaMa) == string::npos) ||
                (!khoaTen.empty() && ten.find(khoaTen) == string::npos)) continue;
            cacMonHocLoc[soMonHocLoc++] = i;
        }
        if (mucDangChon >= soMonHocLoc) mucDangChon = soMonHocLoc > 0 ? soMonHocLoc - 1 : 0;
        int tongSoTrang = soMonHocLoc == 0 ? 1 : (soMonHocLoc + soDongMoiTrang - 1) / soDongMoiTrang;
        int trang = mucDangChon / soDongMoiTrang;
        int viTriDau = trang * soDongMoiTrang;
        int viTriCuoi = min(viTriDau + soDongMoiTrang, soMonHocLoc);
        if (trang != trangTruoc) canVeLaiDanhSach = true;

        if (canVeLaiToanBo) {
            GiaoDienConsole::xoaManHinh();
            GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "CHỌN MÔN HỌC");
            veBoLoc();
            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 6);
            cout << GiaoDienConsole::canTrai(" STT", 8)
                 << GiaoDienConsole::canTrai("MÃ MÔN HỌC", 18)
                 << GiaoDienConsole::canTrai("TÊN MÔN HỌC", 58)
                 << GiaoDienConsole::canTrai("SỐ TÍN CHỈ", 24);
            for (int r = 0; r < soDongMoiTrang; ++r) veMotDong(viTriDau + r, viTriDau, viTriDau + r == mucDangChon);
            if (soMonHocLoc == 0) {
                GiaoDienConsole::diDenToaDo(4, 11);
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                cout << GiaoDienConsole::canGiua("Không tìm thấy môn học phù hợp.", 108);
            }
            GiaoDienConsole::veNut(nutHuy, true, TRANG, XAM_DAM, DEN, VANG);
            GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | Tìm môn | ↑/↓ chọn | PgUp/PgDn trang | Enter chọn | Esc về | Tìm không dấu");
            canVeLaiToanBo = false;
            canVeLaiTuKhoa = false;
            canVeLaiDanhSach = false;
            canVeLaiThanhTrangThai = false;
            mucChonTruoc = mucDangChon;
            trangTruoc = trang;
        } else {
            if (canVeLaiTuKhoa) {
                veBoLoc();
                canVeLaiTuKhoa = false;
            }
            if (canVeLaiDanhSach) {
                for (int r = 0; r < soDongMoiTrang; ++r) veMotDong(viTriDau + r, viTriDau, viTriDau + r == mucDangChon);
                if (soMonHocLoc == 0) {
                    GiaoDienConsole::diDenToaDo(4, 11);
                    GiaoDienConsole::datMau(DO_NHAT, DEN);
                    cout << GiaoDienConsole::canGiua("Không tìm thấy môn học phù hợp.", 108);
                }
                mucChonTruoc = mucDangChon;
                canVeLaiDanhSach = false;
            }
            if (mucDangChon != mucChonTruoc) {
                veMotDong(mucChonTruoc, viTriDau, false);
                veMotDong(mucDangChon, viTriDau, true);
                mucChonTruoc = mucDangChon;
            }
            if (canVeLaiThanhTrangThai) {
                GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | Tìm môn | ↑/↓ chọn | PgUp/PgDn trang | Enter chọn | Esc về | Tìm không dấu");
                canVeLaiThanhTrangThai = false;
            }
            trangTruoc = trang;
        }

        datViTriConTro();
        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        if (suKien.type == SK_CHUOT_DI_CHUYEN) {
            if (suKien.toaDoChuotY >= 8 && suKien.toaDoChuotY < 8 + soDongMoiTrang) {
                int idx = viTriDau + suKien.toaDoChuotY - 8;
                if (idx < viTriCuoi) mucDangChon = idx;
            }
            continue;
        }
        if (suKien.type == SK_CHUOT_TRAI) {
            if (nutHuy.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) break;
            if (suKien.toaDoChuotY == 4) {
                if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 32) oDangChon = 0;
                else if (suKien.toaDoChuotX >= 36 && suKien.toaDoChuotX < 95) oDangChon = 1;
                canVeLaiTuKhoa = true;
            }
            if (suKien.toaDoChuotY >= 8 && suKien.toaDoChuotY < 8 + soDongMoiTrang) {
                int idx = viTriDau + suKien.toaDoChuotY - 8;
                if (idx < viTriCuoi) {
                    string maMon = cacMonHoc[cacMonHocLoc[idx]].maMH;
                    GiaoDienConsole::hienConTro(false);
                    delete[] cacMonHocLoc;
                    delete[] cacMonHoc;
                    return maMon;
                }
            }
        }
        if (suKien.type == SK_PHIM_ESC) break;
        if (suKien.type == SK_PHIM_ENTER && soMonHocLoc > 0) {
            string maMon = cacMonHoc[cacMonHocLoc[mucDangChon]].maMH;
            GiaoDienConsole::hienConTro(false);
            delete[] cacMonHocLoc;
            delete[] cacMonHoc;
            return maMon;
        }
        if (suKien.type == SK_PHIM_LEN || suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_LEN || suKien.type == SK_CON_LAN_XUONG) {
            if (soMonHocLoc > 0) {
                int buoc = (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) ? -1 : 1;
                mucDangChon = (mucDangChon + buoc + soMonHocLoc) % soMonHocLoc;
                canVeLaiThanhTrangThai = true;
            }
        } else if (suKien.type == EVT_KEY_PGUP || suKien.type == EVT_KEY_PGDN) {
            if (soMonHocLoc > 0) {
                int buoc = suKien.type == EVT_KEY_PGUP ? -soDongMoiTrang : soDongMoiTrang;
                mucDangChon = max(0, min(soMonHocLoc - 1, mucDangChon + buoc));
                canVeLaiDanhSach = canVeLaiThanhTrangThai = true;
            }
        } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_SHIFT_TAB) {
            oDangChon = 1 - oDangChon;
            canVeLaiTuKhoa = true;
        } else if (suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI) {
            string& tuKhoa = oDangChon == 0 ? tuKhoaMa : tuKhoaTen;
            size_t& viTriConTro = oDangChon == 0 ? viTriConTroMa : viTriConTroTen;
            if (suKien.type == SK_PHIM_TRAI && viTriConTro > 0) --viTriConTro;
            else if (suKien.type == SK_PHIM_PHAI && viTriConTro < tuKhoa.size()) ++viTriConTro;
            canVeLaiTuKhoa = true;
        } else if (suKien.type == EVT_KEY_HOME || suKien.type == EVT_KEY_END) {
            const string& tuKhoa = oDangChon == 0 ? tuKhoaMa : tuKhoaTen;
            size_t& viTriConTro = oDangChon == 0 ? viTriConTroMa : viTriConTroTen;
            viTriConTro = suKien.type == EVT_KEY_HOME ? 0 : tuKhoa.size();
            canVeLaiTuKhoa = true;
        } else if (suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
            string& tuKhoa = oDangChon == 0 ? tuKhoaMa : tuKhoaTen;
            size_t& viTriConTro = oDangChon == 0 ? viTriConTroMa : viTriConTroTen;
            if (suKien.type == EVT_KEY_BACKSPACE && viTriConTro > 0) {
                tuKhoa.erase(viTriConTro - 1, 1);
                --viTriConTro;
            } else if (suKien.type == EVT_KEY_DELETE && viTriConTro < tuKhoa.size()) {
                tuKhoa.erase(viTriConTro, 1);
            }
            mucDangChon = 0;
            canVeLaiTuKhoa = true;
            canVeLaiDanhSach = true;
            canVeLaiThanhTrangThai = true;
        } else if (suKien.type == SK_KY_TU && suKien.ch >= 32 && suKien.ch <= 126) {
            string& tuKhoa = oDangChon == 0 ? tuKhoaMa : tuKhoaTen;
            size_t& viTriConTro = oDangChon == 0 ? viTriConTroMa : viTriConTroTen;
            const size_t doDaiToiDa = oDangChon == 0 ? 20 : 50;
            if (tuKhoa.size() >= doDaiToiDa) continue;
            char kyTu = suKien.ch;
            if (oDangChon == 0 && kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
            tuKhoa.insert(viTriConTro, 1, kyTu);
            ++viTriConTro;
            mucDangChon = 0;
            canVeLaiTuKhoa = true;
            canVeLaiDanhSach = true;
            canVeLaiThanhTrangThai = true;
        }
    }

    GiaoDienConsole::hienConTro(false);
    delete[] cacMonHocLoc;
    delete[] cacMonHoc;
    return "";
}

// ============================================================================
// HỘP THOẠI CHỌN NHANH MÃ LỚP (Không nhấp nháy, chỉ vẽ lại dòng thay đổi)
// ============================================================================
string chonLopNhanh(DanhSachLop& danhSachLop, bool choPhepTaoMoi, bool cachBoLocVoiTieuDe,
                    PTRMH root, DanhSachLopTinChi* danhSachLopTinChi, QuanLyLichSu* lichSuC) {
    string tuKhoaMaLop, tuKhoaTenLop;
    int oLocDangChon = 0;
    size_t viTriConTroMaLop = 0;
    size_t viTriConTroTenLop = 0;
    int* cacLopLoc = new int[MAX_LOP];
    int* cacLopLocTruoc = new int[MAX_LOP];
    int soLopLoc = 0;
    int mucDangChon = 0;       // Hàng đã chọn, dùng cho phím và các nút thao tác.
    int mucDangHover = -1;     // Hàng rê chuột qua, chỉ dùng làm xem trước.
    int mucHoverTruoc = -1;
    int soDongMoiTrang = 14;
    bool canVeLaiToanBo = true;
    bool canVeLaiBoLoc = true;
    bool canVeLaiDanhSach = true;
    bool canVeLaiThanhTrangThai = true;
    int mucChonTruoc = -1;
    int trangTruoc = -1;
    int nutDangTro = -1;
    int nutTruocDo = -1;

    const bool quanLyLop = choPhepTaoMoi && lichSuC != nullptr && danhSachLopTinChi != nullptr;
    // Riêng câu d: bấm vào hàng hoặc Enter là mở lớp, không cần nút chọn lớp.
    const bool moLopKhiBamHang = !choPhepTaoMoi && !cachBoLocVoiTieuDe;
    const int idNutQuayLai = quanLyLop ? 6 : (choPhepTaoMoi ? 3 : (moLopKhiBamHang ? 1 : 2));
    int soNut = quanLyLop ? 6 : (choPhepTaoMoi ? 3 : (moLopKhiBamHang ? 1 : 2));
    NutChuNhat* cacNut = new NutChuNhat[soNut];
    if (quanLyLop) {
        cacNut[0] = { 4,  23, 17, 1, 0, "[ VÀO LỚP ]" };
        cacNut[1] = { 22, 23, 19, 1, 2, "[ + THÊM (Ins) ]" };
        cacNut[2] = { 42, 23, 17, 1, 3, "[ SỬA (F2) ]" };
        cacNut[3] = { 60, 23, 17, 1, 4, "[ XÓA (Del) ]" };
        cacNut[4] = { 78, 23, 20, 1, 5, "[ F3: HOÀN TÁC ]" };
        cacNut[5] = { 99, 23, 14, 1, 6, "[ QUAY LẠI ]" };
    } else if (choPhepTaoMoi) {
        cacNut[0] = { 6,  23, 22, 1, 0, "[ CHỌN LỚP (Enter) ]" };
        cacNut[1] = { 35, 23, 25, 1, 2, "[ + THÊM LỚP MỚI (Ins) ]" };
        cacNut[2] = { 89, 23, 20, 1, 3, "[ QUAY LẠI (ESC) ]" };
    } else if (moLopKhiBamHang) {
        cacNut[0] = { 89, 23, 20, 1, 1, "[ QUAY LẠI (ESC) ]" };
    } else {
        cacNut[0] = { 6,  23, 22, 1, 0, "[ CHỌN LỚP (Enter) ]" };
        cacNut[1] = { 89, 23, 20, 1, 2, "[ QUAY LẠI (ESC) ]" };
    }

    auto veBoLoc = [&]() {
        const int toaDoX[] = {4, 32};
        const int chieuRong[] = {23, 59};
        const string nhan[] = {"Mã lớp: ", "Tên lớp: "};
        const string* tuKhoa[] = {&tuKhoaMaLop, &tuKhoaTenLop};
        const size_t* conTro[] = {&viTriConTroMaLop, &viTriConTroTenLop};
        for (int i = 0; i < 2; ++i) {
            GiaoDienConsole::diDenToaDo(toaDoX[i], 4);
            GiaoDienConsole::datMau(oLocDangChon == i ? DEN : TRANG, oLocDangChon == i ? VANG : XANH_NGOC);
            int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
            size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong[i] - rongNhan));
            size_t dau = *conTro[i] > rongGiaTri && rongGiaTri > 0 ? *conTro[i] - rongGiaTri : 0;
            string noiDung = nhan[i] + tuKhoa[i]->substr(dau, rongGiaTri);
            cout << noiDung << string(max(0, chieuRong[i] - GiaoDienConsole::doDaiUtf8(noiDung)), ' ');
        }
    };
    auto datViTriConTroBoLoc = [&]() {
        const int toaDoX[] = {4, 32};
        const int chieuRong[] = {23, 59};
        const string nhan[] = {"Mã lớp: ", "Tên lớp: "};
        const size_t* conTro[] = {&viTriConTroMaLop, &viTriConTroTenLop};
        int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[oLocDangChon]);
        size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong[oLocDangChon] - rongNhan));
        size_t dau = *conTro[oLocDangChon] > rongGiaTri && rongGiaTri > 0 ? *conTro[oLocDangChon] - rongGiaTri : 0;
        int x = toaDoX[oLocDangChon] + rongNhan + static_cast<int>(min(*conTro[oLocDangChon] - dau, rongGiaTri > 0 ? rongGiaTri - 1 : 0));
        GiaoDienConsole::diDenToaDo(x, 4);
        GiaoDienConsole::hienConTro(true);
    };

    auto veMotDong = [&](int idx, int viTriDau, bool dangDuocChon) {
        int r = idx - viTriDau;
        if (r < 0 || r >= soDongMoiTrang) return;
        int toaDoDong = (cachBoLocVoiTieuDe ? 8 : 7) + r;
        GiaoDienConsole::diDenToaDo(4, toaDoDong);
        if (idx >= 0 && idx < soLopLoc) {
            int viTriLopGoc = cacLopLoc[idx];
            bool dangHover = quanLyLop && idx == mucDangHover;
            GiaoDienConsole::datMau(dangDuocChon || dangHover ? DEN : TRANG,
                dangDuocChon ? VANG : (dangHover ? XANH_NGOC : DEN));
            int soSV = demSVTrongLop(danhSachLop.lop[viTriLopGoc].dssv);
            cout << GiaoDienConsole::canTrai(" " + to_string(idx + 1), 8)
                 << GiaoDienConsole::canTrai(danhSachLop.lop[viTriLopGoc].maLop, 20)
                 << GiaoDienConsole::canTrai(danhSachLop.lop[viTriLopGoc].tenLop, 58)
                 << GiaoDienConsole::canTrai(to_string(soSV) + " sinh viên", 22);
        } else {
            GiaoDienConsole::datMau(XAM_NHAT, DEN);
            cout << string(108, ' ');
        }
    };

    auto veTrangThai = [&]() {
        string huongDan = "Trang " + to_string(mucDangChon / soDongMoiTrang + 1) + "/" +
            to_string(max(1, (soLopLoc + soDongMoiTrang - 1) / soDongMoiTrang));
        if (quanLyLop) {
            huongDan += " | F3: Hoàn tác " + LichSuThaoTac::moTaThaoTacCGanNhat(*lichSuC);
        } else if (moLopKhiBamHang) {
            huongDan += " | ↑/↓ chọn | Bấm vào lớp/Enter: mở lớp | Esc: về | Tìm không dấu";
        } else {
            huongDan += " | Tìm lớp | ↑/↓ chọn | PgUp/PgDn trang | Enter chọn | Esc về | Tìm không dấu";
        }
        GiaoDienConsole::veThanhTrangThai(huongDan);
    };

    while (true) {
        int soLopLocCu = soLopLoc;
        int mucChonCu = mucChonTruoc;
        int hoverCu = mucHoverTruoc;
        int trangCu = trangTruoc;
        for (int i = 0; i < soLopLocCu; ++i) cacLopLocTruoc[i] = cacLopLoc[i];
        soLopLoc = 0;
        for (int i = 0; i < danhSachLop.soLuong; ++i) {
            string ma = GiaoDienConsole::vietHoa(danhSachLop.lop[i].maLop);
            string ten = GiaoDienConsole::vietHoa(danhSachLop.lop[i].tenLop);
            if ((!tuKhoaMaLop.empty() && ma.find(GiaoDienConsole::vietHoa(tuKhoaMaLop)) == string::npos) ||
                (!tuKhoaTenLop.empty() && ten.find(GiaoDienConsole::vietHoa(tuKhoaTenLop)) == string::npos)) continue;
            cacLopLoc[soLopLoc++] = i;
        }
        if (mucDangChon >= soLopLoc) mucDangChon = (soLopLoc > 0) ? soLopLoc - 1 : 0;
        int trangHienTai = mucDangChon / soDongMoiTrang;
        int viTriDau = trangHienTai * soDongMoiTrang;
        int viTriCuoi = min(viTriDau + soDongMoiTrang, soLopLoc);

        if (trangHienTai != trangTruoc) {
            canVeLaiDanhSach = true;
            canVeLaiThanhTrangThai = true;
            trangTruoc = trangHienTai;
        }

        if (canVeLaiToanBo) {
            GiaoDienConsole::xoaManHinh();
            GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH CÁC LỚP HỌC HIỆN CÓ");

            veBoLoc();

            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, cachBoLocVoiTieuDe ? 6 : 5);
            cout << GiaoDienConsole::canTrai(" STT", 8)
                 << GiaoDienConsole::canTrai("MÃ LỚP", 20)
                 << GiaoDienConsole::canTrai("TÊN LỚP HỌC", 58)
                 << GiaoDienConsole::canTrai("SỈ SỐ LỚP", 22);

            for (int r = 0; r < soDongMoiTrang; ++r) {
                int idx = viTriDau + r;
                veMotDong(idx, viTriDau, idx == mucDangChon);
            }
            if (soLopLoc == 0) {
                GiaoDienConsole::diDenToaDo(4, cachBoLocVoiTieuDe ? 11 : 10);
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                cout << GiaoDienConsole::canGiua("Không tìm thấy lớp phù hợp với điều kiện tìm kiếm.", 108);
            }

            for (int i = 0; i < soNut; ++i) {
                GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
            }

            veTrangThai();
            canVeLaiToanBo = false;
            canVeLaiBoLoc = false;
            canVeLaiDanhSach = false;
            canVeLaiThanhTrangThai = false;
            mucChonTruoc = mucDangChon;
            mucHoverTruoc = mucDangHover;
            trangTruoc = trangHienTai;
            nutTruocDo = nutDangTro;
        } else {
            if (canVeLaiBoLoc) {
                veBoLoc();
                canVeLaiBoLoc = false;
            }
            if (canVeLaiDanhSach) {
                bool doiTrang = trangCu != trangHienTai;
                for (int r = 0; r < soDongMoiTrang; ++r) {
                    int chiSoMoi = viTriDau + r;
                    int chiSoCu = trangCu >= 0 ? trangCu * soDongMoiTrang + r : -1;
                    int lopCu = chiSoCu >= 0 && chiSoCu < soLopLocCu ? cacLopLocTruoc[chiSoCu] : -1;
                    int lopMoi = chiSoMoi < soLopLoc ? cacLopLoc[chiSoMoi] : -1;
                    bool chonCu = chiSoCu >= 0 && chiSoCu == mucChonCu;
                    bool chonMoi = chiSoMoi < soLopLoc && chiSoMoi == mucDangChon;
                    bool hoverDongCu = chiSoCu >= 0 && chiSoCu == hoverCu;
                    bool hoverDongMoi = chiSoMoi < soLopLoc && chiSoMoi == mucDangHover;
                    bool canVeDong = doiTrang || lopCu != lopMoi || chonCu != chonMoi || hoverDongCu != hoverDongMoi;
                    // Hàng thông báo không kết quả nằm tại dòng dữ liệu thứ tư.
                    if (r == 3 && ((soLopLocCu == 0) != (soLopLoc == 0))) canVeDong = true;
                    if (canVeDong) veMotDong(chiSoMoi, viTriDau, chonMoi);
                }
                if (soLopLoc == 0) {
                    if (soLopLocCu != 0 || doiTrang) {
                        GiaoDienConsole::diDenToaDo(4, cachBoLocVoiTieuDe ? 11 : 10);
                        GiaoDienConsole::datMau(DO_NHAT, DEN);
                        cout << GiaoDienConsole::canGiua("Không tìm thấy lớp phù hợp với điều kiện tìm kiếm.", 108);
                    }
                }
                mucChonTruoc = mucDangChon;
                mucHoverTruoc = mucDangHover;
                canVeLaiDanhSach = false;
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
            if (canVeLaiThanhTrangThai) {
            veTrangThai();
                canVeLaiThanhTrangThai = false;
            }
        }

        datViTriConTroBoLoc();
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
            int dongBatDau = cachBoLocVoiTieuDe ? 8 : 7;
            int hoverMoi = -1;
            if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= dongBatDau && suKien.toaDoChuotY < dongBatDau + soDongMoiTrang) {
                int idx = viTriDau + (suKien.toaDoChuotY - dongBatDau);
                if (idx < viTriCuoi) hoverMoi = idx;
            }
            if (quanLyLop && hoverMoi != mucDangHover) {
                mucDangHover = hoverMoi;
                canVeLaiDanhSach = true;
            }
            // Ở câu d, rê chuột di chuyển hàng đang chọn như trước; chỉ bỏ màu xanh xem trước.
            if (moLopKhiBamHang && hoverMoi >= 0 && hoverMoi != mucDangChon) {
                mucDangChon = hoverMoi;
                canVeLaiDanhSach = true;
                canVeLaiThanhTrangThai = true;
            }
            continue;
        } else if (suKien.type == SK_CHUOT_TRAI) {
            for (int i = 0; i < soNut; ++i) {
                if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                    nutDuocBam = cacNut[i].id;
                    break;
                }
            }
            if (suKien.toaDoChuotY == 4) {
                if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 27) {
                    oLocDangChon = 0;
                    viTriConTroMaLop = 0;
                    canVeLaiBoLoc = true;
                } else if (suKien.toaDoChuotX >= 32 && suKien.toaDoChuotX < 91) {
                    oLocDangChon = 1;
                    viTriConTroTenLop = 0;
                    canVeLaiBoLoc = true;
                }
            }
            int dongBatDau = cachBoLocVoiTieuDe ? 8 : 7;
            if (nutDuocBam == -1 && suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= dongBatDau && suKien.toaDoChuotY < dongBatDau + soDongMoiTrang) {
                int idx = viTriDau + (suKien.toaDoChuotY - dongBatDau);
                if (idx < viTriCuoi) {
                    mucDangChon = idx; // Click hàng để khóa lựa chọn cho thao tác tiếp theo.
                    mucDangHover = idx;
                    canVeLaiDanhSach = true;
                    if (!quanLyLop) nutDuocBam = 0; // Các màn chọn lớp cũ vẫn chọn ngay khi bấm hàng.
                }
            }
        }

        if (quanLyLop && (suKien.type == EVT_KEY_F3 || nutDuocBam == 5)) {
            string moTa = LichSuThaoTac::moTaThaoTacCGanNhat(*lichSuC);
            string thongBao;
            bool daHoanTac = LichSuThaoTac::hoanTacThaoTacC(*lichSuC, root, danhSachLop,
                *danhSachLopTinChi, thongBao);
            GiaoDienConsole::hopThongBao(daHoanTac ? "HOÀN TÁC" : "KHÔNG THỂ HOÀN TÁC",
                daHoanTac ? "Đã hoàn tác: " + moTa : thongBao, !daHoanTac);
            canVeLaiToanBo = canVeLaiThanhTrangThai = true;
        } else if (suKien.type == SK_PHIM_ESC || nutDuocBam == idNutQuayLai) {
            GiaoDienConsole::hienConTro(false);
            delete[] cacNut;
            delete[] cacLopLoc;
            delete[] cacLopLocTruoc;
            return "";
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
                if (soLopLoc > 0) mucDangChon = (mucDangChon - 1 + soLopLoc) % soLopLoc;
        } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
                if (soLopLoc > 0) mucDangChon = (mucDangChon + 1) % soLopLoc;
        } else if (suKien.type == EVT_KEY_PGUP || suKien.type == EVT_KEY_PGDN) {
            if (soLopLoc > 0) {
                int buoc = suKien.type == EVT_KEY_PGUP ? -soDongMoiTrang : soDongMoiTrang;
                mucDangChon = max(0, min(soLopLoc - 1, mucDangChon + buoc));
                canVeLaiDanhSach = canVeLaiThanhTrangThai = true;
            }
        } else if (suKien.type == SK_PHIM_ENTER || nutDuocBam == 0) {
            if (soLopLoc > 0) {
                string res = danhSachLop.lop[cacLopLoc[mucDangChon]].maLop;
                GiaoDienConsole::hienConTro(false);
                delete[] cacNut;
                delete[] cacLopLoc;
                delete[] cacLopLocTruoc;
                return res;
            }
        } else if (quanLyLop && (suKien.type == EVT_KEY_F2 || nutDuocBam == 3)) {
            if (soLopLoc == 0) continue;
            int viTri = cacLopLoc[mucDangChon];
            Lop lopCu = danhSachLop.lop[viTri];
            TruongNhap truong[2] = {};
            truong[0] = {"Mã lớp (cố định)", lopCu.maLop, 15, TRUONG_CHI_DOC, 0, 0, 0};
            truong[1] = {"Tên lớp", lopCu.tenLop, 50, TRUONG_CHU, 0, 0, 0};
            string loi;
            if (chayBieuMau("HIỆU CHỈNH LỚP", truong, 2, loi)) {
                string tenMoi = GiaoDienConsole::vietHoaDauTu(truong[1].value);
                if (tenMoi.empty()) {
                    GiaoDienConsole::hopThongBao("LỖI", "Tên lớp không được để trống.", true);
                } else if (tenMoi != lopCu.tenLop) {
                    Lop lopMoi = lopCu;
                    strncpy(lopMoi.tenLop, tenMoi.c_str(), MAX_TENLOP - 1);
                    lopMoi.tenLop[MAX_TENLOP - 1] = '\0';
                    LichSuThaoTac::ghiNhanSuaLop(*lichSuC, lopCu, lopMoi);
                    danhSachLop.lop[viTri] = lopMoi;
                    luuTatCaDuLieu(root, danhSachLop, *danhSachLopTinChi);
                }
            }
            canVeLaiToanBo = true;
        } else if (quanLyLop && (suKien.type == EVT_KEY_DELETE || nutDuocBam == 4)) {
            if (soLopLoc == 0) continue;
            int viTri = cacLopLoc[mucDangChon];
            Lop lopCu = danhSachLop.lop[viTri];
            int siSo = demSVTrongLop(lopCu.dssv);
            if (siSo > 0) {
                GiaoDienConsole::hopThongBao("KHÔNG THỂ XÓA LỚP",
                    "Lớp " + string(lopCu.maLop) + " còn " + to_string(siSo) + " sinh viên. Hãy xử lý sinh viên trước.", true);
            } else if (GiaoDienConsole::hopXacNhan("XÁC NHẬN XÓA LỚP",
                       "Xóa lớp " + string(lopCu.maLop) + " - " + lopCu.tenLop + "?")) {
                LichSuThaoTac::ghiNhanXoaLop(*lichSuC, lopCu, viTri);
                for (int i = viTri; i < danhSachLop.soLuong - 1; ++i) danhSachLop.lop[i] = danhSachLop.lop[i + 1];
                danhSachLop.lop[--danhSachLop.soLuong] = {};
                luuTatCaDuLieu(root, danhSachLop, *danhSachLopTinChi);
                mucDangChon = max(0, min(mucDangChon, soLopLoc - 2));
            }
            canVeLaiToanBo = true;
        } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_SHIFT_TAB) {
            oLocDangChon = (oLocDangChon + (suKien.type == SK_PHIM_SHIFT_TAB ? -1 : 1) + 2) % 2;
            canVeLaiBoLoc = true;
        } else if (suKien.type == SK_PHIM_TRAI) {
            if (oLocDangChon == 0 && viTriConTroMaLop > 0) --viTriConTroMaLop;
            else if (oLocDangChon == 1 && viTriConTroTenLop > 0) --viTriConTroTenLop;
        } else if (suKien.type == SK_PHIM_PHAI) {
            if (oLocDangChon == 0 && viTriConTroMaLop < tuKhoaMaLop.size()) ++viTriConTroMaLop;
            else if (oLocDangChon == 1 && viTriConTroTenLop < tuKhoaTenLop.size()) ++viTriConTroTenLop;
        } else if (suKien.type == EVT_KEY_HOME || suKien.type == EVT_KEY_END) {
            size_t& viTriConTro = oLocDangChon == 0 ? viTriConTroMaLop : viTriConTroTenLop;
            const string& tuKhoa = oLocDangChon == 0 ? tuKhoaMaLop : tuKhoaTenLop;
            viTriConTro = suKien.type == EVT_KEY_HOME ? 0 : tuKhoa.size();
            canVeLaiBoLoc = true;
        } else if (suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
            string& tuKhoa = oLocDangChon == 0 ? tuKhoaMaLop : tuKhoaTenLop;
            size_t& viTriConTro = oLocDangChon == 0 ? viTriConTroMaLop : viTriConTroTenLop;
            if (suKien.type == EVT_KEY_BACKSPACE && viTriConTro > 0) {
                tuKhoa.erase(viTriConTro - 1, 1);
                --viTriConTro;
            } else if (suKien.type == EVT_KEY_DELETE && viTriConTro < tuKhoa.size()) {
                tuKhoa.erase(viTriConTro, 1);
            }
            mucDangChon = 0;
            canVeLaiBoLoc = true;
            canVeLaiDanhSach = true;
            canVeLaiThanhTrangThai = true;
        } else if (suKien.type == SK_KY_TU) {
            string& tuKhoa = oLocDangChon == 0 ? tuKhoaMaLop : tuKhoaTenLop;
            size_t& viTriConTro = oLocDangChon == 0 ? viTriConTroMaLop : viTriConTroTenLop;
            if (tuKhoa.size() < (oLocDangChon == 0 ? 15u : 50u) && suKien.ch >= 32 && suKien.ch <= 126) {
                char kyTu = suKien.ch;
                if (oLocDangChon == 0 && kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
                tuKhoa.insert(viTriConTro, 1, kyTu);
                ++viTriConTro;
            }
            mucDangChon = 0;
            canVeLaiBoLoc = true;
            canVeLaiDanhSach = true;
            canVeLaiThanhTrangThai = true;
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
                if (timLop(danhSachLop, ma.c_str()) != -1) {
                    thongBaoLoi = "Mã lớp '" + ma + "' đã tồn tại!";
                    continue;
                }
                int viTriMoi = danhSachLop.soLuong;
                danhSachLop.lop[danhSachLop.soLuong++] = taoLop(ma.c_str(), ten.c_str());
                if (quanLyLop) {
                    LichSuThaoTac::ghiNhanThemLop(*lichSuC, danhSachLop.lop[viTriMoi], viTriMoi);
                    luuTatCaDuLieu(root, danhSachLop, *danhSachLopTinChi);
                }
                mucDangChon = danhSachLop.soLuong - 1;
                break;
            }
            delete[] f;
            canVeLaiToanBo = true;
        }
    }
}

// ============================================================================
// TRA CỨU VÀ CHỌN LỚP TÍN CHỈ TRỰC TIẾP TRÊN BẢNG
// ============================================================================
int chonLopTinChiTheoThongTin(PTRMH root, const DanhSachLopTinChi& danhSachLopTinChi, const string& tieuDeManHinh, bool chiHienLopDuoiSiSoToiThieu) {
    string tuKhoaNienKhoa;
    string tuKhoaHocKy;
    string tuKhoaMaMonHoc;
    string tuKhoaTenMonHoc;
    string tuKhoaNhom;
    size_t viTriConTroOLoc[5] = {};
    int truongLocDangChon = 0;
    int mucDangChon = 0;
    const int soDongMoiTrang = 14;
    bool canVeLaiToanBo = true;
    bool canVeLaiCacOLoc = true;
    bool canVeLaiBangKetQua = true;
    bool canVeLaiThanhTrangThai = true;
    int mucChonTruoc = -1;
    int trangTruoc = -1;
    int truongLocTruoc = -1;
    int nutDangTro = -1;
    int nutTruocDo = -1;
    int* cacViTriLopTinChi = danhSachLopTinChi.soLuong > 0 ? new int[danhSachLopTinChi.soLuong] : nullptr;
    int soViTriLopTinChi = 0;

    NutChuNhat nutQuayLai = { 46, 23, 24, 1, 0, "[ QUAY LẠI (ESC) ]" };

    auto apDungBoLoc = [&]() {
        soViTriLopTinChi = 0;
        string nienKhoaCanTim = GiaoDienConsole::vietHoa(tuKhoaNienKhoa);
        string hocKyCanTim = tuKhoaHocKy;
        string maMonHocCanTim = GiaoDienConsole::vietHoa(tuKhoaMaMonHoc);
        string tenMonHocCanTim = GiaoDienConsole::vietHoa(tuKhoaTenMonHoc);
        string nhomCanTim = tuKhoaNhom;

        for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
            const LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[i];
            if (lopTinChi == nullptr) continue;
            if (chiHienLopDuoiSiSoToiThieu && (lopTinChi->huyLop || demSVDangKyHieuLuc(lopTinChi->dssvdk) >= lopTinChi->minSV)) continue;
            if (!nienKhoaCanTim.empty() && GiaoDienConsole::vietHoa(lopTinChi->nienKhoa).find(nienKhoaCanTim) == string::npos) continue;
            if (!hocKyCanTim.empty() && to_string(lopTinChi->hocKy).find(hocKyCanTim) == string::npos) continue;
            if (!maMonHocCanTim.empty()) {
                if (GiaoDienConsole::vietHoa(lopTinChi->maMH).find(maMonHocCanTim) == string::npos) continue;
            }
            if (!tenMonHocCanTim.empty()) {
                NodeMH* monHoc = timMonHoc(root, lopTinChi->maMH);
                string tenMonHoc = monHoc ? GiaoDienConsole::vietHoa(monHoc->mh.tenMH) : string();
                if (tenMonHoc.find(tenMonHocCanTim) == string::npos) continue;
            }
            if (!nhomCanTim.empty() && to_string(lopTinChi->nhom).find(nhomCanTim) == string::npos) continue;
            cacViTriLopTinChi[soViTriLopTinChi++] = i;
        }
        if (mucDangChon >= soViTriLopTinChi) {
            mucDangChon = soViTriLopTinChi == 0 ? 0 : soViTriLopTinChi - 1;
        }
    };

    auto veTruongLoc = [&](int x, int chieuRong, const string& nhan, const string& giaTri, int chiSo) {
        GiaoDienConsole::diDenToaDo(x, chiSo == 3 ? 5 : 4);
        bool dangNhap = (truongLocDangChon == chiSo);
        GiaoDienConsole::datMau(dangNhap ? DEN : TRANG, dangNhap ? VANG : XANH_NGOC);
        string nhanTruong = " " + nhan + ": ";
        int doRongNhan = GiaoDienConsole::doDaiUtf8(nhanTruong);
        size_t rongGiaTri = static_cast<size_t>(max(0, chieuRong - doRongNhan));
        size_t dau = viTriConTroOLoc[chiSo] >= rongGiaTri && rongGiaTri > 0 ? viTriConTroOLoc[chiSo] - rongGiaTri + 1 : 0;
        string hienThi = nhanTruong + giaTri.substr(dau, rongGiaTri);
        int doDai = GiaoDienConsole::doDaiUtf8(hienThi);
        cout << hienThi << string(max(0, chieuRong - doDai), ' ');
    };

    auto datViTriConTroOLoc = [&]() {
        const string cacNhan[] = { "Niên khóa", "HK", "Mã MH", "Tên MH", "Nhóm" };
        const int toaDoX[] = { 4, 29, 35, 4, 54 };
        const int chieuRong[] = { 24, 5, 18, 59, 9 };
        int doDaiNhan = GiaoDienConsole::doDaiUtf8(" " + cacNhan[truongLocDangChon] + ": ");
        int soKyTuHienThi = max(0, chieuRong[truongLocDangChon] - doDaiNhan);
        size_t dauHienThi = viTriConTroOLoc[truongLocDangChon] >= static_cast<size_t>(soKyTuHienThi) && soKyTuHienThi > 0
            ? viTriConTroOLoc[truongLocDangChon] - static_cast<size_t>(soKyTuHienThi) + 1 : 0;
        int viTriX = toaDoX[truongLocDangChon] + doDaiNhan
                   + static_cast<int>(min(viTriConTroOLoc[truongLocDangChon] - dauHienThi, static_cast<size_t>(soKyTuHienThi)));
        int cotCuoi = toaDoX[truongLocDangChon] + chieuRong[truongLocDangChon] - 1;
        if (viTriX > cotCuoi) viTriX = cotCuoi;
        GiaoDienConsole::diDenToaDo(viTriX, 4);
        GiaoDienConsole::hienConTro(true);
    };

    auto veMotDong = [&](int viTriLoc, int viTriDau, bool dangDuocChon) {
        int r = viTriLoc - viTriDau;
        if (r < 0 || r >= soDongMoiTrang) return;
        GiaoDienConsole::diDenToaDo(4, 7 + r);
        if (viTriLoc >= 0 && viTriLoc < soViTriLopTinChi) {
            int viTriThat = cacViTriLopTinChi[viTriLoc];
            LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[viTriThat];
            NodeMH* monHoc = timMonHoc(root, lopTinChi->maMH);
            string tenMonHoc = monHoc ? monHoc->mh.tenMH : "(Không rõ)";
            int soSinhVienDangKy = demSVDangKyHieuLuc(lopTinChi->dssvdk);
            GiaoDienConsole::datMau(dangDuocChon ? DEN : (lopTinChi->huyLop ? DO_NHAT : TRANG), dangDuocChon ? VANG : DEN);
            cout << GiaoDienConsole::canTrai(" " + to_string(lopTinChi->maLopTC), 8)
                 << GiaoDienConsole::canTrai(lopTinChi->maMH, 11)
                 << GiaoDienConsole::canTrai(tenMonHoc, 36)
                 << GiaoDienConsole::canTrai(lopTinChi->nienKhoa, 13)
                 << GiaoDienConsole::canTrai(to_string(lopTinChi->hocKy), 7)
                 << GiaoDienConsole::canTrai(to_string(lopTinChi->nhom), 7)
                 << GiaoDienConsole::canTrai(to_string(soSinhVienDangKy) + "/" + to_string(chiHienLopDuoiSiSoToiThieu ? lopTinChi->minSV : lopTinChi->maxSV), 12)
                 << GiaoDienConsole::canTrai(lopTinChi->huyLop ? "Đã hủy" : (chiHienLopDuoiSiSoToiThieu ? "Chưa đủ sĩ số" : "Đang mở"), 14);
        } else {
            GiaoDienConsole::datMau(XAM_NHAT, DEN);
            cout << string(108, ' ');
        }
    };

    while (true) {
        apDungBoLoc();
        int soKetQua = soViTriLopTinChi;
        int tongSoTrang = (soKetQua == 0) ? 1 : (soKetQua + soDongMoiTrang - 1) / soDongMoiTrang;
        int trangHienTai = mucDangChon / soDongMoiTrang;
        int viTriDau = trangHienTai * soDongMoiTrang;
        int viTriCuoi = min(viTriDau + soDongMoiTrang, soKetQua);
        if (trangHienTai != trangTruoc) {
            canVeLaiBangKetQua = true;
            canVeLaiThanhTrangThai = true;
        }

        if (canVeLaiToanBo) {
            GiaoDienConsole::xoaManHinh();
            GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, tieuDeManHinh);
        veTruongLoc(4, 24, "Niên khóa", tuKhoaNienKhoa, 0);
        veTruongLoc(29, 5, "HK", tuKhoaHocKy, 1);
        veTruongLoc(35, 18, "Mã MH", tuKhoaMaMonHoc, 2);
        veTruongLoc(4, 59, "Tên MH", tuKhoaTenMonHoc, 3);
        veTruongLoc(54, 9, "Nhóm", tuKhoaNhom, 4);

            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 6);
            cout << GiaoDienConsole::canTrai("MÃ LTC", 8)
                 << GiaoDienConsole::canTrai("MÃ MH", 11)
                 << GiaoDienConsole::canTrai("TÊN MÔN HỌC", 36)
                 << GiaoDienConsole::canTrai("NIÊN KHÓA", 13)
                 << GiaoDienConsole::canTrai("HK", 7)
                 << GiaoDienConsole::canTrai("NHÓM", 7)
                 << GiaoDienConsole::canTrai(chiHienLopDuoiSiSoToiThieu ? "SV ĐK/MIN" : "SV ĐK/MAX", 12)
                 << GiaoDienConsole::canTrai("TRẠNG THÁI", 14);

            if (soKetQua == 0) {
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                GiaoDienConsole::diDenToaDo(4, 10);
                cout << GiaoDienConsole::canGiua(chiHienLopDuoiSiSoToiThieu ? "Không có lớp nào dưới sĩ số tối thiểu khớp điều kiện tìm." : "Không tìm thấy lớp tín chỉ phù hợp với điều kiện lọc.", 108);
                for (int r = 1; r < soDongMoiTrang; ++r) veMotDong(viTriDau + r, viTriDau, false);
            } else {
                for (int r = 0; r < soDongMoiTrang; ++r) {
                    int viTriLoc = viTriDau + r;
                    veMotDong(viTriLoc, viTriDau, viTriLoc == mucDangChon);
                }
            }

            GiaoDienConsole::veNut(nutQuayLai, nutDangTro == 0, TRANG, XAM_DAM, DEN, VANG);
            GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang) + " | Lọc lớp | ←/→ con trỏ | Tab đổi ô | ↑/↓ chọn | PgUp/PgDn trang | Enter chọn | Tìm không dấu");
            canVeLaiToanBo = false;
            canVeLaiCacOLoc = false;
            canVeLaiBangKetQua = false;
            canVeLaiThanhTrangThai = false;
            mucChonTruoc = mucDangChon;
            trangTruoc = trangHienTai;
            truongLocTruoc = truongLocDangChon;
            nutTruocDo = nutDangTro;
        } else {
            if (canVeLaiCacOLoc || truongLocDangChon != truongLocTruoc) {
                veTruongLoc(4, 24, "Niên khóa", tuKhoaNienKhoa, 0);
                veTruongLoc(29, 5, "HK", tuKhoaHocKy, 1);
                veTruongLoc(35, 18, "Mã MH", tuKhoaMaMonHoc, 2);
                veTruongLoc(4, 59, "Tên MH", tuKhoaTenMonHoc, 3);
                veTruongLoc(54, 9, "Nhóm", tuKhoaNhom, 4);
                truongLocTruoc = truongLocDangChon;
                canVeLaiCacOLoc = false;
            }
            if (canVeLaiBangKetQua) {
                for (int r = 0; r < soDongMoiTrang; ++r) {
                    int viTriLoc = viTriDau + r;
                    veMotDong(viTriLoc, viTriDau, viTriLoc == mucDangChon);
                }
                if (soKetQua == 0) {
                    GiaoDienConsole::datMau(DO_NHAT, DEN);
                    GiaoDienConsole::diDenToaDo(4, 10);
                    cout << GiaoDienConsole::canGiua(chiHienLopDuoiSiSoToiThieu ? "Không có lớp nào dưới sĩ số tối thiểu khớp điều kiện tìm." : "Không tìm thấy lớp tín chỉ phù hợp với điều kiện lọc.", 108);
                }
                mucChonTruoc = mucDangChon;
                trangTruoc = trangHienTai;
                canVeLaiBangKetQua = false;
            } else if (mucDangChon != mucChonTruoc) {
                veMotDong(mucChonTruoc, viTriDau, false);
                veMotDong(mucDangChon, viTriDau, true);
                mucChonTruoc = mucDangChon;
            }
            if (canVeLaiThanhTrangThai) {
                GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang) + " | Lọc lớp | ←/→ con trỏ | Tab đổi ô | ↑/↓ chọn | PgUp/PgDn trang | Enter chọn | Tìm không dấu");
                canVeLaiThanhTrangThai = false;
            }
            if (nutDangTro != nutTruocDo) {
                GiaoDienConsole::veNut(nutQuayLai, nutDangTro == 0, TRANG, XAM_DAM, DEN, VANG);
                nutTruocDo = nutDangTro;
            }
        }

        datViTriConTroOLoc();
        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        if (suKien.type == SK_CHUOT_DI_CHUYEN) {
            if (nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) nutDangTro = 0;
            else nutDangTro = -1;
            if (suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                int viTriClick = viTriDau + (suKien.toaDoChuotY - 7);
                if (viTriClick >= viTriDau && viTriClick < viTriCuoi && mucDangChon != viTriClick) mucDangChon = viTriClick;
            }
            continue;
        }
        if (suKien.type == SK_CHUOT_TRAI) {
            if (nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                GiaoDienConsole::hienConTro(false);
                delete[] cacViTriLopTinChi;
                return -1;
            }
            if (suKien.toaDoChuotY == 4 || suKien.toaDoChuotY == 5) {
                int truongMoi = -1;
                const int dauOLoc[] = {4, 29, 35, 4, 54};
                const int rongOLoc[] = {24, 5, 18, 59, 9};
                const int dongOLoc[] = {4, 4, 4, 5, 4};
                for (int i = 0; i < 5; ++i) {
                    if (suKien.toaDoChuotY == dongOLoc[i] && suKien.toaDoChuotX >= dauOLoc[i] && suKien.toaDoChuotX < dauOLoc[i] + rongOLoc[i]) {
                        truongMoi = i;
                        break;
                    }
                }
                if (truongMoi != -1) {
                    if (truongMoi != truongLocDangChon) {
                        truongLocDangChon = truongMoi;
                        canVeLaiCacOLoc = true;
                    }
                    string* cacTuKhoa[] = { &tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom };
                    viTriConTroOLoc[truongMoi] = cacTuKhoa[truongMoi]->size();
                }
            } else if (suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                int viTriClick = viTriDau + (suKien.toaDoChuotY - 7);
                if (viTriClick < viTriCuoi) {
                    if (mucDangChon == viTriClick) {
                        GiaoDienConsole::hienConTro(false);
                        int viTriChon = cacViTriLopTinChi[viTriClick];
                        delete[] cacViTriLopTinChi;
                        return viTriChon;
                    }
                    mucDangChon = viTriClick;
                }
            }
        }

        if (suKien.type == SK_PHIM_ESC) {
            GiaoDienConsole::hienConTro(false);
            delete[] cacViTriLopTinChi;
            return -1;
        }
        if (suKien.type == SK_PHIM_ENTER) {
            if (soKetQua > 0) {
                GiaoDienConsole::hienConTro(false);
                int viTriChon = cacViTriLopTinChi[mucDangChon];
                delete[] cacViTriLopTinChi;
                return viTriChon;
            }
        } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_SHIFT_TAB) {
            int buoc = suKien.type == SK_PHIM_SHIFT_TAB ? -1 : 1;
            truongLocDangChon = (truongLocDangChon + buoc + 5) % 5;
            canVeLaiCacOLoc = true;
        } else if (suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI) {
            string* cacTuKhoa[] = { &tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom };
            if (suKien.type == SK_PHIM_TRAI && viTriConTroOLoc[truongLocDangChon] > 0) --viTriConTroOLoc[truongLocDangChon];
            else if (suKien.type == SK_PHIM_PHAI && viTriConTroOLoc[truongLocDangChon] < cacTuKhoa[truongLocDangChon]->size()) ++viTriConTroOLoc[truongLocDangChon];
        } else if (suKien.type == EVT_KEY_HOME) {
            viTriConTroOLoc[truongLocDangChon] = 0;
        } else if (suKien.type == EVT_KEY_END) {
            string* cacTuKhoa[] = { &tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom };
            viTriConTroOLoc[truongLocDangChon] = cacTuKhoa[truongLocDangChon]->size();
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
            if (soKetQua > 0) mucDangChon = (mucDangChon - 1 + soKetQua) % soKetQua;
        } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
            if (soKetQua > 0) mucDangChon = (mucDangChon + 1) % soKetQua;
        } else if (suKien.type == EVT_KEY_PGUP) {
            if (mucDangChon >= soDongMoiTrang) mucDangChon -= soDongMoiTrang;
            else mucDangChon = 0;
        } else if (suKien.type == EVT_KEY_PGDN) {
            if (mucDangChon + soDongMoiTrang < soKetQua) mucDangChon += soDongMoiTrang;
            else if (soKetQua > 0) mucDangChon = soKetQua - 1;
        } else if (suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
            string* cacTuKhoa[] = { &tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom };
            if (suKien.type == EVT_KEY_BACKSPACE && viTriConTroOLoc[truongLocDangChon] > 0) {
                cacTuKhoa[truongLocDangChon]->erase(viTriConTroOLoc[truongLocDangChon] - 1, 1);
                --viTriConTroOLoc[truongLocDangChon];
            } else if (suKien.type == EVT_KEY_DELETE && viTriConTroOLoc[truongLocDangChon] < cacTuKhoa[truongLocDangChon]->size()) {
                cacTuKhoa[truongLocDangChon]->erase(viTriConTroOLoc[truongLocDangChon], 1);
            }
            mucDangChon = 0;
            canVeLaiCacOLoc = true;
            canVeLaiBangKetQua = true;
            canVeLaiThanhTrangThai = true;
        } else if (suKien.type == SK_KY_TU) {
            string* cacTuKhoa[] = { &tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom };
            const int doDaiToiDa[] = { 11, 1, 10, 50, 2 };
            string& tuKhoa = *cacTuKhoa[truongLocDangChon];
            char kyTu = suKien.ch;
            bool kyTuHopLe = false;
            if (truongLocDangChon == 0) kyTuHopLe = (kyTu >= 'a' && kyTu <= 'z') || (kyTu >= 'A' && kyTu <= 'Z') || (kyTu >= '0' && kyTu <= '9') || kyTu == '-';
            else if (truongLocDangChon == 1 || truongLocDangChon == 4) kyTuHopLe = kyTu >= '0' && kyTu <= '9';
            else kyTuHopLe = (kyTu >= 'a' && kyTu <= 'z') || (kyTu >= 'A' && kyTu <= 'Z') || (kyTu >= '0' && kyTu <= '9') || kyTu == '-' || kyTu == '_';
            if (kyTuHopLe && static_cast<int>(tuKhoa.size()) < doDaiToiDa[truongLocDangChon]) {
                if (truongLocDangChon != 3 && kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
                tuKhoa.insert(viTriConTroOLoc[truongLocDangChon], 1, kyTu);
                ++viTriConTroOLoc[truongLocDangChon];
                mucDangChon = 0;
                canVeLaiCacOLoc = true;
                canVeLaiBangKetQua = true;
                canVeLaiThanhTrangThai = true;
            }
        }
    }
}
