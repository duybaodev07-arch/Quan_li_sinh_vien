#pragma once

#include <string>
#include "model/SinhVien.h"
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"

enum class LoaiDuLieuLichSu { MonHoc, SinhVien, LopTinChi, Lop };
enum class LoaiThaoTacLichSu { Them, Xoa, Sua };

// Gia tri lop tin chi khong chua con tro danh sach dang ky.
struct ThongTinLopTinChiLichSu {
    int maLopTC = 0;
    char maMH[MAX_MAMH] = {};
    char nienKhoa[MAX_NIENKHOA] = {};
    int hocKy = 0;
    int nhom = 0;
    int minSV = 0;
    int maxSV = 0;
    bool huyLop = false;
};

struct ThongTinLopLichSu {
    char maLop[MAX_MALOP] = {};
    char tenLop[MAX_TENLOP] = {};
};

struct ThaoTacLichSu {
    LoaiDuLieuLichSu loaiDuLieu = LoaiDuLieuLichSu::MonHoc;
    LoaiThaoTacLichSu loaiThaoTac = LoaiThaoTacLichSu::Them;
    char maLop[MAX_MALOP] = {};
    int viTriLopTinChi = -1;
    MonHoc monHocCu = {};
    MonHoc monHocMoi = {};
    SinhVien sinhVienCu = {};
    SinhVien sinhVienMoi = {};
    ThongTinLopTinChiLichSu lopTinChiCu = {};
    ThongTinLopTinChiLichSu lopTinChiMoi = {};
    int viTriDanhSachLop = -1;
    ThongTinLopLichSu lopCu = {};
    ThongTinLopLichSu lopMoi = {};
};

struct NodeLichSu {
    ThaoTacLichSu thaoTac;
    NodeLichSu* next = nullptr;
};

struct StackLichSu {
    NodeLichSu* top = nullptr;
};

struct QuanLyLichSu {
    StackLichSu undo;
    StackLichSu undoSinhVienLop;
};

class LichSuThaoTac {
public:
    static void dayLichSu(QuanLyLichSu& lichSu, const ThaoTacLichSu& thaoTac);
    static void ghiNhanThemLopTinChi(QuanLyLichSu& lichSu, const LopTinChi& lopTinChi, int viTri);
    static void ghiNhanXoaLopTinChi(QuanLyLichSu& lichSu, const LopTinChi& lopTinChi, int viTri);
    static void ghiNhanSuaLopTinChi(QuanLyLichSu& lichSu, const ThongTinLopTinChiLichSu& truoc, const LopTinChi& sau);
    static void ghiNhanThemSinhVien(QuanLyLichSu& lichSu, const char* maLop, const SinhVien& sinhVien);
    static void ghiNhanXoaSinhVien(QuanLyLichSu& lichSu, const char* maLop, const SinhVien& sinhVien);
    static void ghiNhanSuaSinhVien(QuanLyLichSu& lichSu, const char* maLop, const SinhVien& truoc, const SinhVien& sau);
    static void ghiNhanThemLop(QuanLyLichSu& lichSu, const Lop& lop, int viTri);
    static void ghiNhanXoaLop(QuanLyLichSu& lichSu, const Lop& lop, int viTri);
    static void ghiNhanSuaLop(QuanLyLichSu& lichSu, const Lop& truoc, const Lop& sau);
    static void ghiNhanThemMonHoc(QuanLyLichSu& lichSu, const MonHoc& monHoc);
    static void ghiNhanXoaMonHoc(QuanLyLichSu& lichSu, const MonHoc& monHoc);
    static void ghiNhanSuaMonHoc(QuanLyLichSu& lichSu, const MonHoc& truoc, const MonHoc& sau);
    static bool hoanTacThaoTac(QuanLyLichSu& lichSu, PTRMH& root, DanhSachLop& danhSachLop,
                               DanhSachLopTinChi& danhSachLopTinChi, std::string& thongBao);
    static bool hoanTacThaoTacC(QuanLyLichSu& lichSu, PTRMH& root, DanhSachLop& danhSachLop,
                                DanhSachLopTinChi& danhSachLopTinChi, std::string& thongBao);
    static std::string moTaThaoTacGanNhat(const QuanLyLichSu& lichSu);
    static std::string moTaThaoTacCGanNhat(const QuanLyLichSu& lichSu);
    static void giaiPhongLichSu(QuanLyLichSu& lichSu);
};
