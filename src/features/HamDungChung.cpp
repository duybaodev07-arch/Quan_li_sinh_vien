#include "ui/GiaoDienConsole.h"
#include "features/HamDungChung.h"
#include <iostream>
#include <algorithm>
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
// HỘP THOẠI CHỌN MÔN HỌC, TÌM NHÚNG THEO MÃ HOẶC TÊN
// ============================================================================
string chonMonHocNhanh(PTRMH cayMonHoc) {
    int soMonHoc = demMonHoc(cayMonHoc);
    if (soMonHoc <= 0) return "";

    MonHoc* cacMonHoc = new MonHoc[soMonHoc];
    int viTriGhi = 0;
    bstSangMang(cayMonHoc, cacMonHoc, viTriGhi);
    int* cacMonHocLoc = new int[soMonHoc];
    string tuKhoa;
    size_t viTriConTro = 0;
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

    auto datViTriConTro = [&]() {
        GiaoDienConsole::diDenToaDo(4 + GiaoDienConsole::doDaiUtf8("Mã/Tên môn: ") + static_cast<int>(viTriConTro), 4);
        GiaoDienConsole::hienConTro(true);
    };
    auto veMotDong = [&](int chiSo, int viTriDau, bool duocChon) {
        int r = chiSo - viTriDau;
        if (r < 0 || r >= soDongMoiTrang) return;
        GiaoDienConsole::diDenToaDo(4, 8 + r);
        if (chiSo >= 0 && chiSo < soMonHocLoc) {
            const MonHoc& monHoc = cacMonHoc[cacMonHocLoc[chiSo]];
            GiaoDienConsole::datMau(duocChon ? DEN : TRANG, duocChon ? VANG : DEN);
            cout << GiaoDienConsole::canPhai(" " + to_string(chiSo + 1), 8)
                 << GiaoDienConsole::canPhai(monHoc.maMH, 18)
                 << GiaoDienConsole::canPhai(monHoc.tenMH, 58)
                 << GiaoDienConsole::canPhai(to_string(monHoc.soTCLT + monHoc.soTCTH) + " tín chỉ", 24);
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
            string khoa = GiaoDienConsole::vietHoa(tuKhoa);
            if (!khoa.empty() && ma.find(khoa) == string::npos && ten.find(khoa) == string::npos) continue;
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
            GiaoDienConsole::diDenToaDo(4, 4);
            GiaoDienConsole::datMau(DEN, VANG);
            string hienThiTuKhoa = "Mã/Tên môn: " + tuKhoa;
            if (GiaoDienConsole::doDaiUtf8(hienThiTuKhoa) > 60) hienThiTuKhoa.resize(60);
            cout << hienThiTuKhoa << string(max(0, 60 - GiaoDienConsole::doDaiUtf8(hienThiTuKhoa)), ' ');
            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 6);
            cout << GiaoDienConsole::canPhai(" STT", 8)
                 << GiaoDienConsole::canPhai("MÃ MÔN HỌC", 18)
                 << GiaoDienConsole::canPhai("TÊN MÔN HỌC", 58)
                 << GiaoDienConsole::canPhai("SỐ TÍN CHỈ", 24);
            for (int r = 0; r < soDongMoiTrang; ++r) veMotDong(viTriDau + r, viTriDau, viTriDau + r == mucDangChon);
            if (soMonHocLoc == 0) {
                GiaoDienConsole::diDenToaDo(4, 11);
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                cout << GiaoDienConsole::canGiua("Không tìm thấy môn học phù hợp.", 108);
            }
            GiaoDienConsole::veNut(nutHuy, true, TRANG, XAM_DAM, DEN, VANG);
            GiaoDienConsole::veThanhTrangThai("Gõ để tìm theo mã/tên | ↑/↓: chọn môn | Enter/nhấn dòng: chọn | ESC: quay lại | Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang));
            canVeLaiToanBo = false;
            canVeLaiTuKhoa = false;
            canVeLaiDanhSach = false;
            canVeLaiThanhTrangThai = false;
            mucChonTruoc = mucDangChon;
            trangTruoc = trang;
        } else {
            if (canVeLaiTuKhoa) {
                GiaoDienConsole::diDenToaDo(4, 4);
                GiaoDienConsole::datMau(DEN, VANG);
                string hienThiTuKhoa = "Mã/Tên môn: " + tuKhoa;
                if (GiaoDienConsole::doDaiUtf8(hienThiTuKhoa) > 60) hienThiTuKhoa.resize(60);
                cout << hienThiTuKhoa << string(max(0, 60 - GiaoDienConsole::doDaiUtf8(hienThiTuKhoa)), ' ');
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
                GiaoDienConsole::veThanhTrangThai("Gõ để tìm theo mã/tên | ↑/↓: chọn môn | Enter/nhấn dòng: chọn | ESC: quay lại | Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang));
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
        if (suKien.type == SK_PHIM_LEN || suKien.type == SK_PHIM_XUONG) {
            if (soMonHocLoc > 0) {
                mucDangChon = (mucDangChon + (suKien.type == SK_PHIM_LEN ? soMonHocLoc - 1 : 1)) % soMonHocLoc;
                canVeLaiThanhTrangThai = true;
            }
        } else if (suKien.type == EVT_KEY_BACKSPACE) {
            if (viTriConTro > 0) {
                tuKhoa.erase(viTriConTro - 1, 1);
                --viTriConTro;
            }
            mucDangChon = 0;
            canVeLaiTuKhoa = true;
            canVeLaiDanhSach = true;
            canVeLaiThanhTrangThai = true;
        } else if (suKien.type == SK_KY_TU && tuKhoa.size() < 40 && suKien.ch >= 32 && suKien.ch <= 126) {
            char kyTu = suKien.ch;
            if (kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
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
string chonLopNhanh(DanhSachLop& danhSachLop, bool choPhepTaoMoi) {
    string tuKhoaMaLop, tuKhoaTenLop;
    int oLocDangChon = 0;
    size_t viTriConTroMaLop = 0;
    size_t viTriConTroTenLop = 0;
    int* cacLopLoc = new int[MAX_LOP];
    int* cacLopLocTruoc = new int[MAX_LOP];
    int soLopLoc = 0;
    int mucDangChon = 0;
    int soDongMoiTrang = 14;
    bool canVeLaiToanBo = true;
    bool canVeLaiBoLoc = true;
    bool canVeLaiDanhSach = true;
    bool canVeLaiThanhTrangThai = true;
    int mucChonTruoc = -1;
    int trangTruoc = -1;
    int nutDangTro = -1;
    int nutTruocDo = -1;

    int soNut = choPhepTaoMoi ? 3 : 2;
    NutChuNhat* cacNut = new NutChuNhat[soNut];
    cacNut[0] = { 6,  23, 22, 1, 0, "[ CHỌN LỚP (Enter) ]" };
    if (choPhepTaoMoi) {
        cacNut[1] = { 35, 23, 25, 1, 2, "[ + THÊM LỚP MỚI (Ins) ]" };
        cacNut[2] = { 89, 23, 20, 1, 3, "[ QUAY LẠI (ESC) ]" };
    } else {
        cacNut[1] = { 89, 23, 20, 1, 2, "[ QUAY LẠI (ESC) ]" };
    }

    auto veBoLoc = [&]() {
        GiaoDienConsole::diDenToaDo(4, 4);
        GiaoDienConsole::datMau(oLocDangChon == 0 ? DEN : TRANG, oLocDangChon == 0 ? VANG : XANH_NGOC);
        cout << GiaoDienConsole::canPhai("Mã lớp: " + tuKhoaMaLop, 30);
        GiaoDienConsole::datMau(oLocDangChon == 1 ? DEN : TRANG, oLocDangChon == 1 ? VANG : XANH_NGOC);
        GiaoDienConsole::diDenToaDo(36, 4);
        cout << GiaoDienConsole::canPhai("Tên lớp: " + tuKhoaTenLop, 48);
    };
    auto datViTriConTroBoLoc = [&]() {
        int x = oLocDangChon == 0
            ? 4 + GiaoDienConsole::doDaiUtf8("Mã lớp: ") + static_cast<int>(viTriConTroMaLop)
            : 36 + GiaoDienConsole::doDaiUtf8("Tên lớp: ") + static_cast<int>(viTriConTroTenLop);
        GiaoDienConsole::diDenToaDo(x, 4);
        GiaoDienConsole::hienConTro(true);
    };

    auto veMotDong = [&](int idx, int viTriDau, bool dangDuocChon) {
        int r = idx - viTriDau;
        if (r < 0 || r >= soDongMoiTrang) return;
        int toaDoDong = 7 + r;
        GiaoDienConsole::diDenToaDo(4, toaDoDong);
        if (idx >= 0 && idx < soLopLoc) {
            int viTriLopGoc = cacLopLoc[idx];
            GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
            int soSV = demSVTrongLop(danhSachLop.lop[viTriLopGoc].dssv);
            cout << GiaoDienConsole::canPhai(" " + to_string(idx + 1), 8)
                 << GiaoDienConsole::canPhai(danhSachLop.lop[viTriLopGoc].maLop, 20)
                 << GiaoDienConsole::canPhai(danhSachLop.lop[viTriLopGoc].tenLop, 58)
                 << GiaoDienConsole::canPhai(to_string(soSV) + " sinh viên", 22);
        } else {
            GiaoDienConsole::datMau(XAM_NHAT, DEN);
            cout << string(108, ' ');
        }
    };

    while (true) {
        int soLopLocCu = soLopLoc;
        int mucChonCu = mucChonTruoc;
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
        int tongSoTrang = (soLopLoc == 0) ? 1 : (soLopLoc + soDongMoiTrang - 1) / soDongMoiTrang;
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
            GiaoDienConsole::diDenToaDo(4, 5);
            cout << GiaoDienConsole::canPhai(" STT", 8)
                 << GiaoDienConsole::canPhai("MÃ LỚP", 20)
                 << GiaoDienConsole::canPhai("TÊN LỚP HỌC", 58)
                 << GiaoDienConsole::canPhai("SỈ SỐ LỚP", 22);

            for (int r = 0; r < soDongMoiTrang; ++r) {
                int idx = viTriDau + r;
                veMotDong(idx, viTriDau, idx == mucDangChon);
            }
            if (soLopLoc == 0) {
                GiaoDienConsole::diDenToaDo(4, 10);
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                cout << GiaoDienConsole::canGiua("Không tìm thấy lớp phù hợp với điều kiện tìm kiếm.", 108);
            }

            for (int i = 0; i < soNut; ++i) {
                GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
            }

            GiaoDienConsole::veThanhTrangThai("Gõ để tìm theo mã/tên | Tab: đổi ô | ←/→: di chuyển con trỏ | Enter/nhấn dòng: chọn lớp | Ins: thêm lớp | ESC: quay lại | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang));
            canVeLaiToanBo = false;
            canVeLaiBoLoc = false;
            canVeLaiDanhSach = false;
            canVeLaiThanhTrangThai = false;
            mucChonTruoc = mucDangChon;
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
                    bool canVeDong = doiTrang || lopCu != lopMoi || chonCu != chonMoi;
                    // Hàng thông báo không kết quả nằm tại dòng dữ liệu thứ tư.
                    if (r == 3 && ((soLopLocCu == 0) != (soLopLoc == 0))) canVeDong = true;
                    if (canVeDong) veMotDong(chiSoMoi, viTriDau, chonMoi);
                }
                if (soLopLoc == 0) {
                    if (soLopLocCu != 0 || doiTrang) {
                        GiaoDienConsole::diDenToaDo(4, 10);
                        GiaoDienConsole::datMau(DO_NHAT, DEN);
                        cout << GiaoDienConsole::canGiua("Không tìm thấy lớp phù hợp với điều kiện tìm kiếm.", 108);
                    }
                }
                mucChonTruoc = mucDangChon;
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
                GiaoDienConsole::veThanhTrangThai("Gõ để tìm theo mã/tên | Tab: đổi ô | ←/→: di chuyển con trỏ | Enter/nhấn dòng: chọn lớp | Ins: thêm lớp | ESC: quay lại | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang));
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
            if (suKien.toaDoChuotY == 4) {
                if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 34) {
                    oLocDangChon = 0;
                    viTriConTroMaLop = 0;
                    canVeLaiBoLoc = true;
                } else if (suKien.toaDoChuotX >= 36 && suKien.toaDoChuotX < 84) {
                    oLocDangChon = 1;
                    viTriConTroTenLop = 0;
                    canVeLaiBoLoc = true;
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
            GiaoDienConsole::hienConTro(false);
            delete[] cacNut;
            delete[] cacLopLoc;
            delete[] cacLopLocTruoc;
            return "";
        } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
                if (soLopLoc > 0) mucDangChon = (mucDangChon - 1 + soLopLoc) % soLopLoc;
        } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
                if (soLopLoc > 0) mucDangChon = (mucDangChon + 1) % soLopLoc;
        } else if (suKien.type == SK_PHIM_ENTER || nutDuocBam == 0) {
            if (soLopLoc > 0) {
                string res = danhSachLop.lop[cacLopLoc[mucDangChon]].maLop;
                GiaoDienConsole::hienConTro(false);
                delete[] cacNut;
                delete[] cacLopLoc;
                delete[] cacLopLocTruoc;
                return res;
            }
        } else if (suKien.type == SK_PHIM_TAB) {
            oLocDangChon = 1 - oLocDangChon;
            canVeLaiBoLoc = true;
        } else if (suKien.type == SK_PHIM_TRAI) {
            if (oLocDangChon == 0 && viTriConTroMaLop > 0) --viTriConTroMaLop;
            else if (oLocDangChon == 1 && viTriConTroTenLop > 0) --viTriConTroTenLop;
        } else if (suKien.type == SK_PHIM_PHAI) {
            if (oLocDangChon == 0 && viTriConTroMaLop < tuKhoaMaLop.size()) ++viTriConTroMaLop;
            else if (oLocDangChon == 1 && viTriConTroTenLop < tuKhoaTenLop.size()) ++viTriConTroTenLop;
        } else if (suKien.type == EVT_KEY_BACKSPACE) {
            string& tuKhoa = oLocDangChon == 0 ? tuKhoaMaLop : tuKhoaTenLop;
            size_t& viTriConTro = oLocDangChon == 0 ? viTriConTroMaLop : viTriConTroTenLop;
            if (viTriConTro > 0) {
                tuKhoa.erase(viTriConTro - 1, 1);
                --viTriConTro;
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
                danhSachLop.lop[danhSachLop.soLuong++] = taoLop(ma.c_str(), ten.c_str());
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
int chonLopTinChiTheoThongTin(PTRMH cayMonHoc, const DanhSachLopTinChi& danhSachLopTinChi, const string& tieuDeManHinh) {
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
            if (!nienKhoaCanTim.empty() && GiaoDienConsole::vietHoa(lopTinChi->nienKhoa).find(nienKhoaCanTim) == string::npos) continue;
            if (!hocKyCanTim.empty() && to_string(lopTinChi->hocKy).find(hocKyCanTim) == string::npos) continue;
            if (!maMonHocCanTim.empty()) {
                if (GiaoDienConsole::vietHoa(lopTinChi->maMH).find(maMonHocCanTim) == string::npos) continue;
            }
            if (!tenMonHocCanTim.empty()) {
                NodeMH* monHoc = timMonHoc(cayMonHoc, lopTinChi->maMH);
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
        GiaoDienConsole::diDenToaDo(x, 4);
        bool dangNhap = (truongLocDangChon == chiSo);
        GiaoDienConsole::datMau(dangNhap ? DEN : TRANG, dangNhap ? VANG : XANH_NGOC);
        string hienThi = " " + nhan + ": " + giaTri;
        int doDai = GiaoDienConsole::doDaiUtf8(hienThi);
        if (doDai > chieuRong) {
            int doRongNhan = GiaoDienConsole::doDaiUtf8(" " + nhan + ": ");
            hienThi = " " + nhan + ": " + giaTri.substr(0, max(0, chieuRong - doRongNhan));
            doDai = GiaoDienConsole::doDaiUtf8(hienThi);
        }
        cout << hienThi << string(max(0, chieuRong - doDai), ' ');
    };

    auto datViTriConTroOLoc = [&]() {
        const string cacNhan[] = { "Niên khóa", "HK", "Mã MH", "Tên MH", "Nhóm" };
        const int toaDoX[] = { 4, 28, 40, 61, 94 };
        const int chieuRong[] = { 22, 10, 19, 31, 18 };
        int doDaiNhan = GiaoDienConsole::doDaiUtf8(" " + cacNhan[truongLocDangChon] + ": ");
        int soKyTuHienThi = max(0, chieuRong[truongLocDangChon] - doDaiNhan);
        int viTriX = toaDoX[truongLocDangChon] + doDaiNhan
                   + static_cast<int>(min(viTriConTroOLoc[truongLocDangChon], static_cast<size_t>(soKyTuHienThi)));
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
            NodeMH* monHoc = timMonHoc(cayMonHoc, lopTinChi->maMH);
            string tenMonHoc = monHoc ? monHoc->mh.tenMH : "(Không rõ)";
            int soSinhVienDangKy = demSVDangKyHieuLuc(lopTinChi->dssvdk);
            GiaoDienConsole::datMau(dangDuocChon ? DEN : (lopTinChi->huyLop ? DO_NHAT : TRANG), dangDuocChon ? VANG : DEN);
            cout << GiaoDienConsole::canPhai(" " + to_string(lopTinChi->maLopTC), 8)
                 << GiaoDienConsole::canPhai(lopTinChi->maMH, 11)
                 << GiaoDienConsole::canPhai(tenMonHoc, 36)
                 << GiaoDienConsole::canPhai(lopTinChi->nienKhoa, 13)
                 << GiaoDienConsole::canPhai(to_string(lopTinChi->hocKy), 7)
                 << GiaoDienConsole::canPhai(to_string(lopTinChi->nhom), 7)
                 << GiaoDienConsole::canPhai(to_string(soSinhVienDangKy) + "/" + to_string(lopTinChi->maxSV), 12)
                 << GiaoDienConsole::canPhai(lopTinChi->huyLop ? "Đã hủy" : "Đang mở", 14);
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
            veTruongLoc(4, 22, "Niên khóa", tuKhoaNienKhoa, 0);
            veTruongLoc(28, 10, "HK", tuKhoaHocKy, 1);
            veTruongLoc(40, 19, "Mã MH", tuKhoaMaMonHoc, 2);
            veTruongLoc(61, 31, "Tên MH", tuKhoaTenMonHoc, 3);
            veTruongLoc(94, 18, "Nhóm", tuKhoaNhom, 4);

            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 6);
            cout << GiaoDienConsole::canPhai("MÃ LTC", 8)
                 << GiaoDienConsole::canPhai("MÃ MH", 11)
                 << GiaoDienConsole::canPhai("TÊN MÔN HỌC", 36)
                 << GiaoDienConsole::canPhai("NIÊN KHÓA", 13)
                 << GiaoDienConsole::canPhai("HK", 7)
                 << GiaoDienConsole::canPhai("NHÓM", 7)
                 << GiaoDienConsole::canPhai("SV ĐK/MAX", 12)
                 << GiaoDienConsole::canPhai("TRẠNG THÁI", 14);

            if (soKetQua == 0) {
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                GiaoDienConsole::diDenToaDo(4, 10);
                cout << GiaoDienConsole::canGiua("Không tìm thấy lớp tín chỉ phù hợp với điều kiện lọc.", 108);
                for (int r = 1; r < soDongMoiTrang; ++r) veMotDong(viTriDau + r, viTriDau, false);
            } else {
                for (int r = 0; r < soDongMoiTrang; ++r) {
                    int viTriLoc = viTriDau + r;
                    veMotDong(viTriLoc, viTriDau, viTriLoc == mucDangChon);
                }
            }

            GiaoDienConsole::veNut(nutQuayLai, nutDangTro == 0, TRANG, XAM_DAM, DEN, VANG);
            GiaoDienConsole::veThanhTrangThai("Gõ để lọc | Tab/←→: Đổi ô | Home/End: đầu/cuối ô | ↑/↓: Chọn lớp | Enter: In danh sách | Backspace: Xóa ký tự | ESC: Quay lại | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang));
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
                veTruongLoc(4, 22, "Niên khóa", tuKhoaNienKhoa, 0);
                veTruongLoc(28, 10, "HK", tuKhoaHocKy, 1);
                veTruongLoc(40, 19, "Mã MH", tuKhoaMaMonHoc, 2);
                veTruongLoc(61, 31, "Tên MH", tuKhoaTenMonHoc, 3);
                veTruongLoc(94, 18, "Nhóm", tuKhoaNhom, 4);
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
                    cout << GiaoDienConsole::canGiua("Không tìm thấy lớp tín chỉ phù hợp với điều kiện lọc.", 108);
                }
                mucChonTruoc = mucDangChon;
                canVeLaiBangKetQua = false;
            } else if (mucDangChon != mucChonTruoc) {
                veMotDong(mucChonTruoc, viTriDau, false);
                veMotDong(mucDangChon, viTriDau, true);
                mucChonTruoc = mucDangChon;
            }
            if (canVeLaiThanhTrangThai) {
                GiaoDienConsole::veThanhTrangThai("Gõ để lọc | Tab/←→: Đổi ô | Home/End: đầu/cuối ô | ↑/↓: Chọn lớp | Enter: In danh sách | Backspace: Xóa ký tự | ESC: Quay lại | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang));
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
                if (viTriClick < viTriCuoi && mucDangChon != viTriClick) mucDangChon = viTriClick;
            }
            continue;
        }
        if (suKien.type == SK_CHUOT_TRAI) {
            if (nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                GiaoDienConsole::hienConTro(false);
                delete[] cacViTriLopTinChi;
                return -1;
            }
            if (suKien.toaDoChuotY == 4) {
                int truongMoi = -1;
                const int dauOLoc[] = {4, 28, 40, 61, 94};
                const int rongOLoc[] = {22, 10, 19, 31, 18};
                for (int i = 0; i < 5; ++i) {
                    if (suKien.toaDoChuotX >= dauOLoc[i] && suKien.toaDoChuotX < dauOLoc[i] + rongOLoc[i]) {
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
        } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI) {
            int buoc = (suKien.type == SK_PHIM_TRAI) ? -1 : 1;
            truongLocDangChon = (truongLocDangChon + buoc + 5) % 5;
            canVeLaiCacOLoc = true;
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
        } else if (suKien.type == EVT_KEY_BACKSPACE) {
            string* cacTuKhoa[] = { &tuKhoaNienKhoa, &tuKhoaHocKy, &tuKhoaMaMonHoc, &tuKhoaTenMonHoc, &tuKhoaNhom };
            if (viTriConTroOLoc[truongLocDangChon] > 0) {
                cacTuKhoa[truongLocDangChon]->erase(viTriConTroOLoc[truongLocDangChon] - 1, 1);
                --viTriConTroOLoc[truongLocDangChon];
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
