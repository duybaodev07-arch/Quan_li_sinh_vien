#include "ui/GiaoDienConsole.h"
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <cmath>

using namespace std;

void GiaoDienConsole::datLaiMau() {
        datMau(XAM_NHAT, DEN);
    }

void GiaoDienConsole::toMauVung(int x, int y, int w, int h, int mauChu, int mauNen, char ch) {
        datMau(mauChu, mauNen);
        string dong(w, ch);
        for (int r = 0; r < h; ++r) {
            diDenToaDo(x, y + r);
            cout << dong;
        } 
    }

int GiaoDienConsole::doDaiUtf8(const string& str) {
        int doDai = 0;
        for (size_t i = 0; i < str.size(); ++i) {
            if ((str[i] & 0xC0) != 0x80) doDai++; // 0xC0: 11000000(dùng lọc), 0x80: 10000000(byte đuôi)
        }
        return doDai;
    }

string GiaoDienConsole::catChuoiUtf8(const string& str, int soKyTuToiDa) {
        int soKyTu = 0;
        size_t i = 0;
        while (i < str.size() && soKyTu < soKyTuToiDa) {
            if ((str[i] & 0xC0) != 0x80) soKyTu++;  
            i++;
            while (i < str.size() && (str[i] & 0xC0) == 0x80) i++;
        }
        return str.substr(0, i);
    }

string GiaoDienConsole::canTrai(const string& str, int chieuRong) { // chieu rong console
        if (chieuRong <= 0) return "";
        int doDai = doDaiUtf8(str);
        if (doDai > chieuRong) {
            if (chieuRong > 2) return catChuoiUtf8(str, chieuRong - 2) + "..";
            return catChuoiUtf8(str, chieuRong);
        }   
        return str + string(chieuRong - doDai, ' ');
    }

string GiaoDienConsole::canGiua(const string& str, int chieuRong) {
        if (chieuRong <= 0) return "";
        int doDai = doDaiUtf8(str); 
        if (doDai >= chieuRong) return canTrai(str, chieuRong);
        int trai = (chieuRong - doDai) / 2;
        int phai = chieuRong - doDai - trai;
        return string(trai, ' ') + str + string(phai, ' ');
    }

void GiaoDienConsole::veKhung(int x, int y, int w, int h, int mauChu, int mauNen, const string& tieuDe) {
        datMau(mauChu, mauNen);
        diDenToaDo(x, y);
        cout << "+" << string(w - 2, '-') << "+";
        string giua(w - 2, ' ');
        for (int i = 1; i < h - 1; ++i) {
            diDenToaDo(x, y + i);
            cout << "|" << giua << "|";
        }
        diDenToaDo(x, y + h - 1);
        cout << "+" << string(w - 2, '-') << "+";

        if (!tieuDe.empty()) {
            string t = " " + tieuDe + " ";
            int tLen = doDaiUtf8(t);
            if (tLen > w - 4) {
                t = " " + catChuoiUtf8(tieuDe, w - 6) + " ";
                tLen = doDaiUtf8(t);            
            }
            int tx = x + (w - tLen) / 2;
            if (tx > x) {
                diDenToaDo(tx, y);
                datMau(VANG, mauNen);
                cout << t;
            }
        }
    }

void GiaoDienConsole::veTieuDe(const string& tieuDePhu) {
        datMau(VANG, LAM);
        diDenToaDo(1, 0);
        cout << canGiua("QUẢN LÍ SINH VIÊN HỆ TÍN CHỈ", 114);
        if (!tieuDePhu.empty()) {
            datMau(TRANG, XANH_NGOC);
            diDenToaDo(1, 1);
            cout << canGiua(tieuDePhu, 114);
        }
        datLaiMau();
    }

void GiaoDienConsole::veThanhTrangThai(const string& goiY, const string& thongBao, bool coLoi) {
        diDenToaDo(1, 26);
        if (!thongBao.empty()) {
            datMau(TRANG, coLoi ? DO : XANH_LA);
            cout << canGiua(thongBao, 114);
        } else {
            datMau(XAM_NHAT, DEN);
            cout << string(114, ' ');
        }

        diDenToaDo(1, 27);
        datMau(DEN, XAM_NHAT);
        cout << canTrai(" " + goiY, 114);
        datLaiMau();
    }

void GiaoDienConsole::veNut(const NutChuNhat& btn, bool dangHoatDong, int mauChuThuong, int mauNenThuong, int mauChuChon, int mauNenChon) {
        datMau(dangHoatDong ? mauChuChon : mauChuThuong, dangHoatDong ? mauNenChon : mauNenThuong);
        for (int r = 0; r < btn.h; ++r) {
            diDenToaDo(btn.x, btn.y + r);
            if (r == btn.h / 2) {
                cout << canGiua(btn.label, btn.w);
            } else {
                cout << string(btn.w, ' ');
            }
        }
        datLaiMau();
    }

bool GiaoDienConsole::hopXacNhan(const string& tieuDe, const string& cauHoi, const string& chiTiet) {
        hienConTro(false);
        int w = 68, h = 9;
        int x = (CHIEU_RONG_GIAO_DIEN - w) / 2;
        int y = (CHIEU_CAO_GIAO_DIEN - h) / 2;

        veKhung(x, y, w, h, VANG, LAM, tieuDe);
        datMau(TRANG, LAM);
        diDenToaDo(x + 2, y + 2);
        cout << canGiua(cauHoi, w - 4);
        if (!chiTiet.empty()) {
            datMau(XANH_NGOC_NHAT, LAM);
            diDenToaDo(x + 2, y + 4);
            cout << canGiua(chiTiet, w - 4);
        }

        int soNut = 2;
        NutChuNhat* cacNut = new NutChuNhat[soNut];
        cacNut[0] = { x + 13, y + 6, 16, 1, 1, "[ ĐỒNG Ý ]" };
        cacNut[1] = { x + 39, y + 6, 16, 1, 0, "[ HỦY BỎ ]" };

        int mucDangChon = 0;
        int mucChonTruoc = -1;
        bool ketQua = false;
        bool dangChay = true;

        while (dangChay) {
            if (mucDangChon != mucChonTruoc) {
                for (int i = 0; i < soNut; ++i) {
                    veNut(cacNut[i], i == mucDangChon, TRANG, XAM_DAM, DEN, VANG);
                }
                mucChonTruoc = mucDangChon;
            }

            SuKienNhap suKien = docSuKien();
            if (suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI || suKien.type == SK_PHIM_TAB) {
                mucDangChon = 1 - mucDangChon;
            } else if (suKien.type == SK_PHIM_ENTER) {
                ketQua = (mucDangChon == 0);
                dangChay = false;
            } else if (suKien.type == SK_PHIM_ESC) {
                ketQua = false;
                dangChay = false;
            } else if (suKien.type == SK_CHUOT_DI_CHUYEN) {
                for (int i = 0; i < soNut; ++i) {
                    if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                        mucDangChon = i;
                    }
                }
            } else if (suKien.type == SK_CHUOT_TRAI) {
                for (int i = 0; i < soNut; ++i) {
                    if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                        mucDangChon = i;
                        ketQua = (mucDangChon == 0);
                        dangChay = false;
                        break;
                    }
                }
            }
        }

        delete[] cacNut;
        return ketQua;
    }

void GiaoDienConsole::hopThongBao(const string& tieuDe, const string& msg, bool coLoi) {
        hienConTro(false);
        int w = 68, h = 8;
        int x = (CHIEU_RONG_GIAO_DIEN - w) / 2;
        int y = (CHIEU_CAO_GIAO_DIEN - h) / 2;
        int mauNen = coLoi ? DO : LAM;

        veKhung(x, y, w, h, VANG, mauNen, tieuDe);
        datMau(TRANG, mauNen);
        diDenToaDo(x + 2, y + 2);
        cout << canGiua(msg, w - 4);

        NutChuNhat okBtn = { x + (w - 18) / 2, y + 5, 18, 1, 1, "[ ĐÓNG (Enter) ]" };
        veNut(okBtn, true, TRANG, XAM_DAM, DEN, VANG);

        while (true) {
            SuKienNhap suKien = docSuKien();
            if (suKien.type == SK_PHIM_ENTER || suKien.type == SK_PHIM_ESC) break;
            if (suKien.type == SK_CHUOT_TRAI && okBtn.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) break;
        }
    }

string GiaoDienConsole::catKhoangTrang(const string& s) {
        size_t start = 0;
        while (start < s.size() && s[start] == ' ') start++;
        if (start == s.size()) return "";
        size_t end = s.size() - 1;
        while (end > start && s[end] == ' ') end--;
        string res = "";
        bool khoangTrangTruoc = false;
        for (size_t i = start; i <= end; ++i) {
            if (s[i] == ' ') {
                if (!khoangTrangTruoc) {
                    res += ' ';
                    khoangTrangTruoc = true;
                }
            } else {
                res += s[i];
                khoangTrangTruoc = false;
            }
        }
        return res;
    }

string GiaoDienConsole::vietHoa(const string& s) {
        string res = catKhoangTrang(s);
        for (size_t i = 0; i < res.size(); ++i) {
            if (res[i] >= 'a' && res[i] <= 'z') {
                res[i] = res[i] - 'a' + 'A';
            }
        }
        return res;
    }

string GiaoDienConsole::vietHoaDauTu(const string& s) {
        string res = catKhoangTrang(s);
        bool dauTuMoi = true;
        for (size_t i = 0; i < res.size(); ++i) {
            if (res[i] == ' ') {
                dauTuMoi = true;
            } else if (dauTuMoi) {
                if (res[i] >= 'a' && res[i] <= 'z') res[i] = res[i] - 'a' + 'A';
                dauTuMoi = false;
            }
        }
        return res;
    }

bool chayBieuMau(const string& tieuDe, TruongNhap* fields, int soTruong, string& thongBaoLoi, const string& nhanNutLuu, const string& nhanNutHuy, const string& nhanNutXoa, bool* daChonXoa, const string& nhanNutF1, bool* daNhanF1) {
    GiaoDienConsole::hienConTro(false);
    bool coNutXoa = !nhanNutXoa.empty() && daChonXoa != nullptr;
    bool coNutF1 = !nhanNutF1.empty() && daNhanF1 != nullptr;
    if (daChonXoa != nullptr) *daChonXoa = false;
    if (daNhanF1 != nullptr) *daNhanF1 = false;
    int soNutHanhDong = 2 + (coNutXoa ? 1 : 0) + (coNutF1 ? 1 : 0);
    int viTriNutLuu = soTruong;
    int viTriNutXoa = coNutXoa ? soTruong + 1 : -1;
    int viTriNutF1 = coNutF1 ? soTruong + 1 + (coNutXoa ? 1 : 0) : -1;
    int viTriNutHuy = soTruong + soNutHanhDong - 1;
    int soMucDieuHuong = soTruong + soNutHanhDong;
    int chieuRongBieuMau = 76;
    int chieuCaoBieuMau = soTruong * 2 + 7;
    int toaDoXBieuMau = (CHIEU_RONG_GIAO_DIEN - chieuRongBieuMau) / 2;
    int toaDoYBieuMau = (CHIEU_CAO_GIAO_DIEN - chieuCaoBieuMau) / 2;
    if (toaDoYBieuMau < 3) toaDoYBieuMau = 3;

    GiaoDienConsole::veKhung(toaDoXBieuMau, toaDoYBieuMau, chieuRongBieuMau, chieuCaoBieuMau, XANH_NGOC_NHAT, DEN, tieuDe);

    for (int i = 0; i < soTruong; ++i) {
        fields[i].x = toaDoXBieuMau + 27;
        fields[i].y = toaDoYBieuMau + 2 + i * 2;
        fields[i].w = chieuRongBieuMau - 30;
        GiaoDienConsole::datMau(TRANG, DEN);
        GiaoDienConsole::diDenToaDo(toaDoXBieuMau + 3, fields[i].y);
        cout << GiaoDienConsole::canTrai(fields[i].label + ":", 23);
    }

    int toaDoNutY = toaDoYBieuMau + chieuCaoBieuMau - 2;
    bool baNutHanhDong = soNutHanhDong == 3;
    bool bonNutHanhDong = soNutHanhDong == 4;
    int nutRong = bonNutHanhDong ? 17 : (baNutHanhDong ? 20 : 18);
    int xNutLuu = bonNutHanhDong ? 1 : (baNutHanhDong ? 2 : 16);
    int xNutPhu = bonNutHanhDong ? 20 : 28;
    int xNutF1 = bonNutHanhDong ? 39 : 28;
    int xNutHuy = bonNutHanhDong ? 58 : (baNutHanhDong ? 54 : 42);
    NutChuNhat nutLuu = { toaDoXBieuMau + xNutLuu, toaDoNutY, nutRong, 1, 1, nhanNutLuu };
    NutChuNhat nutXoa = { toaDoXBieuMau + xNutPhu, toaDoNutY, nutRong, 1, 2, nhanNutXoa };
    NutChuNhat nutF1 = { toaDoXBieuMau + xNutF1, toaDoNutY, nutRong, 1, 3, nhanNutF1 };
    NutChuNhat nutHuy = { toaDoXBieuMau + xNutHuy, toaDoNutY, nutRong, 1, 0, nhanNutHuy };

    auto veMotTruong = [&](int i, bool dangHoatDong) {
        if (i < 0 || i >= soTruong) return;
        if (fields[i].mode == TRUONG_CHI_DOC) {
            GiaoDienConsole::datMau(VANG, XAM_DAM);
        } else if (dangHoatDong) {
            GiaoDienConsole::datMau(TRANG, LAM);
        } else {
            GiaoDienConsole::datMau(XAM_NHAT, XAM_DAM);
        }
        GiaoDienConsole::diDenToaDo(fields[i].x, fields[i].y);
        cout << " " << GiaoDienConsole::canTrai(fields[i].value, fields[i].w - 2) << " ";
    };

    auto veDongThongBao = [&]() {
        GiaoDienConsole::diDenToaDo(toaDoXBieuMau + 2, toaDoYBieuMau + chieuCaoBieuMau - 4);
        if (!thongBaoLoi.empty()) {
            GiaoDienConsole::datMau(DO_NHAT, DEN);
            cout << GiaoDienConsole::canGiua(thongBaoLoi, chieuRongBieuMau - 4);
        } else {
            GiaoDienConsole::datMau(XAM_DAM, DEN);
            string huongDan = coNutF1
                ? "F1 hoặc nút Chọn môn: mở danh sách môn học | Tab/Lên/Xuống: di chuyển"
                : "Click chuột vào ô hoặc dùng phím Lên/Xuống/Tab để di chuyển";
            cout << GiaoDienConsole::canGiua(huongDan, chieuRongBieuMau - 4);
        }
    };

    int viTriTruongDangChon = 0;
    while (viTriTruongDangChon < soTruong && fields[viTriTruongDangChon].mode == TRUONG_CHI_DOC) {
        viTriTruongDangChon++;
    }
    if (viTriTruongDangChon >= soTruong) viTriTruongDangChon = 0;

    // Vẽ toàn bộ các ô và nút lần đầu tiên
    for (int i = 0; i < soTruong; ++i) {
        veMotTruong(i, i == viTriTruongDangChon);
    }
    GiaoDienConsole::veNut(nutLuu, viTriTruongDangChon == viTriNutLuu, TRANG, XANH_LA, DEN, VANG);
    if (coNutXoa) GiaoDienConsole::veNut(nutXoa, viTriTruongDangChon == viTriNutXoa, TRANG, DO_NHAT, DEN, VANG);
    if (coNutF1) GiaoDienConsole::veNut(nutF1, viTriTruongDangChon == viTriNutF1, TRANG, XANH_NGOC, DEN, VANG);
    GiaoDienConsole::veNut(nutHuy, viTriTruongDangChon == viTriNutHuy, TRANG, DO, DEN, VANG);
    veDongThongBao();

    int viTriTruongTruoc = viTriTruongDangChon;
    string thongBaoTruoc = thongBaoLoi;
    bool giaTriDaSua = false;

    while (true) {
        // Chỉ cập nhật đúng phần thay đổi, tuyệt đối không vẽ lại cả Form
        if (thongBaoLoi != thongBaoTruoc) {
            veDongThongBao();
            thongBaoTruoc = thongBaoLoi;
        }
        if (viTriTruongDangChon != viTriTruongTruoc) {
            GiaoDienConsole::hienConTro(false);
            if (viTriTruongTruoc < soTruong) veMotTruong(viTriTruongTruoc, false);
            else if (viTriTruongTruoc == viTriNutLuu) GiaoDienConsole::veNut(nutLuu, false, TRANG, XANH_LA, DEN, VANG);
            else if (coNutXoa && viTriTruongTruoc == viTriNutXoa) GiaoDienConsole::veNut(nutXoa, false, TRANG, DO_NHAT, DEN, VANG);
            else if (coNutF1 && viTriTruongTruoc == viTriNutF1) GiaoDienConsole::veNut(nutF1, false, TRANG, XANH_NGOC, DEN, VANG);
            else if (viTriTruongTruoc == viTriNutHuy) GiaoDienConsole::veNut(nutHuy, false, TRANG, DO, DEN, VANG);

            if (viTriTruongDangChon < soTruong) veMotTruong(viTriTruongDangChon, true);
            else if (viTriTruongDangChon == viTriNutLuu) GiaoDienConsole::veNut(nutLuu, true, TRANG, XANH_LA, DEN, VANG);
            else if (coNutXoa && viTriTruongDangChon == viTriNutXoa) GiaoDienConsole::veNut(nutXoa, true, TRANG, DO_NHAT, DEN, VANG);
            else if (coNutF1 && viTriTruongDangChon == viTriNutF1) GiaoDienConsole::veNut(nutF1, true, TRANG, XANH_NGOC, DEN, VANG);
            else if (viTriTruongDangChon == viTriNutHuy) GiaoDienConsole::veNut(nutHuy, true, TRANG, DO, DEN, VANG);

            viTriTruongTruoc = viTriTruongDangChon;
            giaTriDaSua = false;
        } else if (giaTriDaSua && viTriTruongDangChon < soTruong) {
            GiaoDienConsole::hienConTro(false);
            veMotTruong(viTriTruongDangChon, true);
            giaTriDaSua = false;
        }

        // Đặt con trỏ nhấp nháy tại vị trí đang gõ
        if (viTriTruongDangChon >= 0 && viTriTruongDangChon < soTruong && fields[viTriTruongDangChon].mode != TRUONG_CHI_DOC) {
            int doDai = GiaoDienConsole::doDaiUtf8(fields[viTriTruongDangChon].value);
            if (doDai > fields[viTriTruongDangChon].w - 2) doDai = fields[viTriTruongDangChon].w - 2;
            GiaoDienConsole::diDenToaDo(fields[viTriTruongDangChon].x + 1 + doDai, fields[viTriTruongDangChon].y);
            GiaoDienConsole::hienConTro(true);
        } else {
            GiaoDienConsole::hienConTro(false);
        }

        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        if (suKien.type == SK_PHIM_ESC) {
            return false;
        } else if (suKien.type == SK_PHIM_LEN) {
            int viTriTiepTheo = viTriTruongDangChon;
            do {
                viTriTiepTheo = (viTriTiepTheo - 1 + soMucDieuHuong) % soMucDieuHuong;
            } while (viTriTiepTheo < soTruong && fields[viTriTiepTheo].mode == TRUONG_CHI_DOC);
            viTriTruongDangChon = viTriTiepTheo;
        } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_PHIM_TAB) {
            int viTriTiepTheo = viTriTruongDangChon;
            do {
                viTriTiepTheo = (viTriTiepTheo + 1) % soMucDieuHuong;
            } while (viTriTiepTheo < soTruong && fields[viTriTiepTheo].mode == TRUONG_CHI_DOC);
            viTriTruongDangChon = viTriTiepTheo;
        } else if ((suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI) && viTriTruongDangChon >= soTruong) {
            int buoc = (suKien.type == SK_PHIM_TRAI) ? -1 : 1;
            int viTriNut = viTriTruongDangChon - soTruong;
            viTriNut = (viTriNut + buoc + soNutHanhDong) % soNutHanhDong;
            viTriTruongDangChon = soTruong + viTriNut;
        } else if (coNutF1 && suKien.type == EVT_KEY_F1) {
            *daNhanF1 = true;
            return false;
        } else if (suKien.type == EVT_KEY_F2) {
            return true;
        } else if (suKien.type == SK_PHIM_ENTER) {
            if (viTriTruongDangChon == viTriNutHuy) {
                return false;
            } else if (coNutXoa && viTriTruongDangChon == viTriNutXoa) {
                *daChonXoa = true;
                return false;
            } else if (coNutF1 && viTriTruongDangChon == viTriNutF1) {
                *daNhanF1 = true;
                return false;
            } else if (viTriTruongDangChon == viTriNutLuu) {
                return true;
            } else {
                int nextIdx = viTriTruongDangChon + 1;
                while (nextIdx < soTruong && fields[nextIdx].mode == TRUONG_CHI_DOC) nextIdx++;
                if (nextIdx < soTruong) {
                    viTriTruongDangChon = nextIdx;
                } else {
                    return true;
                }
            }
        } else if (suKien.type == EVT_KEY_BACKSPACE) {
            if (viTriTruongDangChon >= 0 && viTriTruongDangChon < soTruong && fields[viTriTruongDangChon].mode != TRUONG_CHI_DOC) {
                if (!fields[viTriTruongDangChon].value.empty()) {
                    fields[viTriTruongDangChon].value.pop_back();
                    giaTriDaSua = true;
                    thongBaoLoi = "";
                }
            }
        } else if (suKien.type == SK_KY_TU) {
            if (viTriTruongDangChon >= 0 && viTriTruongDangChon < soTruong && fields[viTriTruongDangChon].mode != TRUONG_CHI_DOC) {
                TruongNhap& f = fields[viTriTruongDangChon];
                if ((int)f.value.size() < f.maxLen) {
                    char c = suKien.ch;
                    if (f.mode == TRUONG_MA) {
                        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_') {
                            if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
                            f.value += c;
                            giaTriDaSua = true;
                            thongBaoLoi = "";
                        }
                    } else if (f.mode == TRUONG_SO || f.mode == TRUONG_DIEN_THOAI) {
                        if (c >= '0' && c <= '9') {
                            f.value += c;
                            giaTriDaSua = true;
                            thongBaoLoi = "";
                        }
                    } else if (f.mode == TRUONG_SO_THUC) {
                        if ((c >= '0' && c <= '9') || (c == '.' && f.value.find('.') == string::npos)) {
                            f.value += c;
                            giaTriDaSua = true;
                            thongBaoLoi = "";
                        }
                    } else if (f.mode == TRUONG_CHU) {
                        if (c >= 32 && c <= 126) {
                            f.value += c;
                            giaTriDaSua = true;
                            thongBaoLoi = "";
                        }
                    }
                }
            }
        } else if (suKien.type == SK_CHUOT_TRAI) {
            for (int i = 0; i < soTruong; ++i) {
                if (fields[i].mode != TRUONG_CHI_DOC &&
                    suKien.toaDoChuotX >= fields[i].x && suKien.toaDoChuotX < fields[i].x + fields[i].w &&
                    suKien.toaDoChuotY == fields[i].y) {
                    viTriTruongDangChon = i;
                    break;
                }
            }
            if (nutLuu.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                GiaoDienConsole::hienConTro(false);
                return true;
            }
            if (coNutXoa && nutXoa.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                *daChonXoa = true;
                GiaoDienConsole::hienConTro(false);
                return false;
            }
            if (coNutF1 && nutF1.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                *daNhanF1 = true;
                GiaoDienConsole::hienConTro(false);
                return false;
            }
            if (nutHuy.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                GiaoDienConsole::hienConTro(false);
                return false;
            }
        } else if (suKien.type == SK_CHUOT_DI_CHUYEN) {
            if (nutLuu.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) viTriTruongDangChon = viTriNutLuu;
            else if (coNutXoa && nutXoa.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) viTriTruongDangChon = viTriNutXoa;
            else if (coNutF1 && nutF1.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) viTriTruongDangChon = viTriNutF1;
            else if (nutHuy.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) viTriTruongDangChon = viTriNutHuy;
        }
    }
}
