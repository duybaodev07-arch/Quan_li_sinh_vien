#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <csignal>
#include <cstdlib>
#include <strings.h>

inline int _stricmp(const char* a, const char* b) {
    return strcasecmp(a, b);
}
#endif

#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <cmath>

using namespace std;

// Kích thước chuẩn an toàn tuyệt đối cho mọi cửa sổ Windows Terminal / CMD (mặc định 120x30)
// Không bao giờ vẽ chạm cột 119 hay dòng 29 để tránh tự động xuống dòng (wrap) và cuộn màn hình (scroll)
const int CHIEU_RONG_GIAO_DIEN = 116;
const int CHIEU_CAO_GIAO_DIEN = 28;

enum MauConsole {
    DEN = 0,
    LAM = 1,
    XANH_LA = 2,
    XANH_NGOC = 3,
    DO = 4,
    TIM = 5,
    NAU = 6,
    XAM_NHAT = 7,
    XAM_DAM = 8,
    LAM_NHAT = 9,
    XANH_LA_NHAT = 10,
    XANH_NGOC_NHAT = 11,
    DO_NHAT = 12,
    TIM_NHAT = 13,
    VANG = 14,
    TRANG = 15
};

enum LoaiSuKienNhap {
    SK_KHONG = 0,
    SK_PHIM_LEN,
    SK_PHIM_XUONG,
    SK_PHIM_TRAI,
    SK_PHIM_PHAI,
    SK_PHIM_ENTER,
    SK_PHIM_ESC,
    SK_PHIM_TAB,
    EVT_KEY_BACKSPACE,
    EVT_KEY_DELETE,
    EVT_KEY_INSERT,
    EVT_KEY_HOME,
    EVT_KEY_END,
    EVT_KEY_PGUP,
    EVT_KEY_PGDN,
    EVT_KEY_F1,
    EVT_KEY_F2,
    EVT_KEY_F3,
    EVT_KEY_F4,
    EVT_KEY_F5,
    SK_KY_TU,
    SK_CHUOT_DI_CHUYEN,
    SK_CHUOT_TRAI,
    SK_CHUOT_PHAI,
    SK_CON_LAN_LEN,
    SK_CON_LAN_XUONG
};

struct SuKienNhap {
    LoaiSuKienNhap type;
    char ch;
    int toaDoChuotX;
    int toaDoChuotY;
};

struct NutChuNhat {
    int x, y, w, h;
    int id;
    string label;

    bool contains(int toaDoX, int toaDoY) const {
        return (toaDoX >= x && toaDoX < x + w && toaDoY >= y && toaDoY < y + h);
    }
};

class GiaoDienConsole {
public:
#ifdef _WIN32
    static HANDLE hOut;
    static HANDLE hIn;
#else
    static struct termios origTermios;
    static bool termInitialized;
    static int pendingByte;
#endif
    static int lastMouseX;
    static int lastMouseY;

#ifndef _WIN32
    static void khoiPhucConsole();

    static void xuLyTinHieu(int);

    static bool docByteCoThoiHan(unsigned char& c, int thoiGianChoMs);
#endif

    static void khoiTaoConsole(const char* tieuDe = "QUAN LY diem SINH VIEN THEO HE TIN CHI");

    static void batNhapChuot();

    static void hienConTro(bool dangHien);

    static void diDenToaDo(int x, int y);

    static void datMau(int mauChu, int mauNen = DEN);

    static void datLaiMau();

    static void xoaManHinh(int mauNen = DEN);

    static void toMauVung(int x, int y, int w, int h, int mauChu = XAM_NHAT, int mauNen = DEN, char ch = ' ');

    static int doDaiUtf8(const string& str);

    static string catChuoiUtf8(const string& str, int soKyTuToiDa);

    static string canPhai(const string& str, int chieuRong);

    static string canGiua(const string& str, int chieuRong);

    static void veKhung(int x, int y, int w, int h, int mauChu = XANH_NGOC_NHAT, int mauNen = DEN, const string& tieuDe = "");

    // Header gọn gàng (dòng 0, rộng 114 cột tại x=1)
    static void veTieuDe(const string& tieuDePhu = "");

    // Thanh trạng thái cố định ở dòng 26 & 27 (không bao giờ vượt quá dòng 27)
    static void veThanhTrangThai(const string& goiY, const string& thongBao = "", bool coLoi = false);

    static void veNut(const NutChuNhat& btn, bool dangHoatDong, int mauChuThuong = TRANG, int mauNenThuong = XAM_DAM, int mauChuChon = DEN, int mauNenChon = VANG);

    // Đọc sự kiện Bàn phím & Chuột (chỉ phát sinh EVT_MOUSE_MOVE khi tọa độ ô chuột thực sự thay đổi!)
    static SuKienNhap docSuKien();

    // Hộp thoại xác nhận Yes/No: Chỉ vẽ khung 1 lần, chỉ vẽ lại nút khi đổi lựa chọn
    static bool hopXacNhan(const string& tieuDe, const string& cauHoi, const string& chiTiet = "");

    static void hopThongBao(const string& tieuDe, const string& msg, bool coLoi = false);

    static string catKhoangTrang(const string& s);

    static string vietHoa(const string& s);

    static string vietHoaDauTu(const string& s);
};

enum KieuTruongNhap {
    TRUONG_MA,
    TRUONG_CHU,
    TRUONG_SO,
    TRUONG_DIEN_THOAI,
    TRUONG_SO_THUC,
    TRUONG_CHI_DOC
};

struct TruongNhap {
    string label;
    string value;
    int maxLen;
    KieuTruongNhap mode;
    int x, y, w;
};

// Form nhập liệu KHÔNG NHẤP NHÁY: Chỉ vẽ khung 1 lần, chỉ vẽ lại đúng ô đang gõ hoặc khi chuyển ô
bool chayBieuMau(const string& tieuDe, TruongNhap* fields, int soTruong, string& thongBaoLoi, const string& nhanNutLuu = "[ LƯU (Enter) ]", const string& nhanNutHuy = "[ HỦY (ESC) ]");


#endif // CONSOLE_UI_H


