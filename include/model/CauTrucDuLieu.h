#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include "ui/GiaoDienConsole.h"

using namespace std;

const int MAX_LOP = 10000;
const int MAX_LOPTC = 10000;

// ============================================================================
// 1. DANH SÁCH MÔN HỌC: CÂY NHỊ PHÂN TÌM KIẾM (BST theo MAMH)
// ============================================================================
struct MonHoc {
    char MAMH[11];   // Tối đa 10 ký tự (C10)
    char TENMH[51];  // Tối đa 50 ký tự (C50)
    int STCLT;       // Số tín chỉ lý thuyết
    int STCTH;       // Số tín chỉ thực hành

    MonHoc() {
        MAMH[0] = '\0';
        TENMH[0] = '\0';
        STCLT = 0;
        STCTH = 0;
    }

    MonHoc(const char* ma, const char* ten, int tinChiLyThuyet, int tinChiThucHanh) {
        strncpy(MAMH, ma, 10); MAMH[10] = '\0';
        strncpy(TENMH, ten, 50); TENMH[50] = '\0';
        STCLT = tinChiLyThuyet;
        STCTH = tinChiThucHanh;
    }
};

struct NodeMH {
    MonHoc duLieu;
    NodeMH* nutTrai;
    NodeMH* nutPhai;

    NodeMH(const MonHoc& mh) : duLieu(mh), nutTrai(nullptr), nutPhai(nullptr) {}
};

typedef NodeMH* PTRMH;

// Tìm kiếm môn học theo MAMH trên cây BST
inline NodeMH* timMonHoc(PTRMH cay, const char* maMH) {
    NodeMH* p = cay;
    while (p != nullptr) {
        int cmp = strcmp(maMH, p->duLieu.MAMH);
        if (cmp == 0) return p;
        else if (cmp < 0) p = p->nutTrai;
        else p = p->nutPhai;
    }
    return nullptr;
}

// Thêm môn học vào cây BST
inline bool themMonHoc(PTRMH& cay, const MonHoc& mh) {
    if (cay == nullptr) {
        cay = new NodeMH(mh);
        return true;
    }
    int cmp = strcmp(mh.MAMH, cay->duLieu.MAMH);
    if (cmp == 0) return false; // Trùng mã môn học
    else if (cmp < 0) return themMonHoc(cay->nutTrai, mh);
    else return themMonHoc(cay->nutPhai, mh);
}

// Nút thế mạng khi xóa nút có 2 cây con trên BST (cực trái của cây con phải)
inline void timNutTheMang(PTRMH& p, PTRMH& q) {
    if (q->nutTrai != nullptr) {
        timNutTheMang(p, q->nutTrai);
    } else {
        p->duLieu = q->duLieu;
        p = q;
        q = q->nutPhai;
    }
}

// Xóa môn học khỏi cây BST theo MAMH
inline bool xoaMonHoc(PTRMH& cay, const char* maMH) {
    if (cay == nullptr) return false;
    int cmp = strcmp(maMH, cay->duLieu.MAMH);
    if (cmp < 0) return xoaMonHoc(cay->nutTrai, maMH);
    else if (cmp > 0) return xoaMonHoc(cay->nutPhai, maMH);
    else {
        NodeMH* p = cay;
        if (cay->nutTrai == nullptr) {
            cay = cay->nutPhai;
        } else if (cay->nutPhai == nullptr) {
            cay = cay->nutTrai;
        } else {
            timNutTheMang(p, cay->nutPhai);
        }
        delete p;
        return true;
    }
}

// Đếm số lượng môn học trong cây BST
inline int demMonHoc(PTRMH cay) {
    if (cay == nullptr) return 0;
    return 1 + demMonHoc(cay->nutTrai) + demMonHoc(cay->nutPhai);
}

// Đổ dữ liệu từ BST ra mảng động (duyệt LNR)
inline void bstSangMang(PTRMH cay, MonHoc* arr, int& idx) {
    if (cay == nullptr) return;
    bstSangMang(cay->nutTrai, arr, idx);
    arr[idx++] = cay->duLieu;
    bstSangMang(cay->nutPhai, arr, idx);
}

// Sắp xếp mảng động môn học tăng dần theo TENMH (nếu trùng tên thì tăng dần theo MAMH)
inline void sapXepMonHocTheoTen(MonHoc* arr, int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int cmp = _stricmp(arr[i].TENMH, arr[j].TENMH);
            if (cmp > 0 || (cmp == 0 && strcmp(arr[i].MAMH, arr[j].MAMH) > 0)) {
                MonHoc tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}

// Giải phóng bộ nhớ cây BST
inline void giaiPhongBST(PTRMH& cay) {
    if (cay == nullptr) return;
    giaiPhongBST(cay->nutTrai);
    giaiPhongBST(cay->nutPhai);
    delete cay;
    cay = nullptr;
}


// ============================================================================
// 2. DANH SÁCH SINH VIÊN: DANH SÁCH LIÊN KẾT ĐƠN (Tăng dần theo Tên + Họ)
// ============================================================================
struct SinhVien {
    string MASV;
    string HO;
    string TEN;
    string PHAI;
    string SODT;

    SinhVien() {}
    SinhVien(const string& ma, const string& ho, const string& ten, const string& phai, const string& sdt)
        : MASV(ma), HO(ho), TEN(ten), PHAI(phai), SODT(sdt) {}
};

struct NodeSV {
    SinhVien duLieu;
    NodeSV* tiepTheo;

    NodeSV(const SinhVien& sv) : duLieu(sv), tiepTheo(nullptr) {}
};

typedef NodeSV* PTRSV;

// So sánh thứ tự ưu tiên Tên -> Họ -> Mã SV để giữ DSSV luôn tăng dần theo Tên
inline int soSanhTenSV(const SinhVien& a, const SinhVien& b) {
    int soSanhTen = _stricmp(a.TEN.c_str(), b.TEN.c_str());
    if (soSanhTen != 0) return soSanhTen;
    int soSanhHo = _stricmp(a.HO.c_str(), b.HO.c_str());
    if (soSanhHo != 0) return soSanhHo;
    return _stricmp(a.MASV.c_str(), b.MASV.c_str());
}

// Chèn sinh viên vào DSLK đơn sao cho luôn giữ thứ tự tăng dần theo Tên
inline void themSVCoThuTu(PTRSV& dauDanhSach, const SinhVien& sv) {
    PTRSV p = new NodeSV(sv);
    if (dauDanhSach == nullptr || soSanhTenSV(sv, dauDanhSach->duLieu) < 0) {
        p->tiepTheo = dauDanhSach;
        dauDanhSach = p;
        return;
    }
    PTRSV nutHienTai = dauDanhSach;
    while (nutHienTai->tiepTheo != nullptr && soSanhTenSV(nutHienTai->tiepTheo->duLieu, sv) <= 0) {
        nutHienTai = nutHienTai->tiepTheo;
    }
    p->tiepTheo = nutHienTai->tiepTheo;
    nutHienTai->tiepTheo = p;
}

// Xóa sinh viên theo MASV trong 1 lớp
inline bool xoaSVTheoMa(PTRSV& dauDanhSach, const string& maSV) {
    if (dauDanhSach == nullptr) return false;
    if (dauDanhSach->duLieu.MASV == maSV) {
        PTRSV p = dauDanhSach;
        dauDanhSach = dauDanhSach->tiepTheo;
        delete p;
        return true;
    }
    PTRSV nutHienTai = dauDanhSach;
    while (nutHienTai->tiepTheo != nullptr && nutHienTai->tiepTheo->duLieu.MASV != maSV) {
        nutHienTai = nutHienTai->tiepTheo;
    }
    if (nutHienTai->tiepTheo == nullptr) return false;
    PTRSV p = nutHienTai->tiepTheo;
    nutHienTai->tiepTheo = p->tiepTheo;
    delete p;
    return true;
}

// Đếm số sinh viên trong 1 lớp
inline int demSVTrongLop(PTRSV dauDanhSach) {
    int cnt = 0;
    for (PTRSV p = dauDanhSach; p != nullptr; p = p->tiepTheo) cnt++;
    return cnt;
}

// Giải phóng DSLK sinh viên
inline void giaiPhongDSSV(PTRSV& dauDanhSach) {
    while (dauDanhSach != nullptr) {
        PTRSV p = dauDanhSach;
        dauDanhSach = dauDanhSach->tiepTheo;
        delete p;
    }
}


// ============================================================================
// 3. DANH SÁCH LỚP: DANH SÁCH TUYẾN TÍNH (Tối đa 10000 lớp)
// ============================================================================
struct Lop {
    string MALOP;
    string TENLOP;
    PTRSV dssv;

    Lop() : dssv(nullptr) {}
    Lop(const string& ma, const string& ten) : MALOP(ma), TENLOP(ten), dssv(nullptr) {}
};

struct DSLop {
    Lop* cacNutDuLieu; // Mảng động cấp phát MAX_LOP phần tử
    int n;

    DSLop() {
        cacNutDuLieu = new Lop[MAX_LOP];
        n = 0;
    }

    ~DSLop() {
        for (int i = 0; i < n; ++i) {
            giaiPhongDSSV(cacNutDuLieu[i].dssv);
        }
        delete[] cacNutDuLieu;
    }
};

// Tìm chỉ số lớp theo MALOP
inline int timLop(const DSLop& danhSachLop, const string& maLop) {
    for (int i = 0; i < danhSachLop.n; ++i) {
        if (danhSachLop.cacNutDuLieu[i].MALOP == maLop) return i;
    }
    return -1;
}

// Tìm sinh viên trên toàn bộ các lớp theo MASV (trả về con trỏ NodeSV và chỉ số lớp nếu cần)
inline NodeSV* timSinhVienToanTruong(const DSLop& danhSachLop, const string& maSV, int* viTriLopTraVe = nullptr) {
    for (int i = 0; i < danhSachLop.n; ++i) {
        for (PTRSV p = danhSachLop.cacNutDuLieu[i].dssv; p != nullptr; p = p->tiepTheo) {
            if (p->duLieu.MASV == maSV) {
                if (viTriLopTraVe) *viTriLopTraVe = i;
                return p;
            }
        }
    }
    if (viTriLopTraVe) *viTriLopTraVe = -1;
    return nullptr;
}


// ============================================================================
// 4. DANH SÁCH ĐĂNG KÝ: DANH SÁCH LIÊN KẾT ĐƠN
// ============================================================================
struct DangKy {
    string MASV;
    float DIEM;       // -1.0f nếu chưa có điểm, 0.0 -> 10.0 nếu đã nhập điểm
    bool huyDangKy;   // false: đang đăng ký, true: đã hủy đăng ký

    DangKy() : MASV(""), DIEM(-1.0f), huyDangKy(false) {}
    DangKy(const string& ma, float d = -1.0f, bool huy = false)
        : MASV(ma), DIEM(d), huyDangKy(huy) {}
};

struct NodeDK {
    DangKy duLieu;
    NodeDK* tiepTheo;

    NodeDK(const DangKy& dk) : duLieu(dk), tiepTheo(nullptr) {}
};

typedef NodeDK* PTRDK;

// Thêm hoặc kích hoạt lại đăng ký vào cuối DSLK đơn
inline bool themDangKy(PTRDK& dauDanhSach, const DangKy& dk) {
    if (dauDanhSach == nullptr) {
        dauDanhSach = new NodeDK(dk);
        return true;
    }
    PTRDK p = dauDanhSach;
    while (true) {
        if (p->duLieu.MASV == dk.MASV) {
            if (p->duLieu.huyDangKy) {
                p->duLieu.huyDangKy = false;
                return true;
            }
            return false; // Đã đăng ký rồi
        }
        if (p->tiepTheo == nullptr) break;
        p = p->tiepTheo;
    }
    p->tiepTheo = new NodeDK(dk);
    return true;
}

// Tìm node đăng ký theo MASV trong 1 lớp tín chỉ
inline NodeDK* timDangKy(PTRDK dauDanhSach, const string& maSV) {
    for (PTRDK p = dauDanhSach; p != nullptr; p = p->tiepTheo) {
        if (p->duLieu.MASV == maSV) return p;
    }
    return nullptr;
}

// Đếm số SV đăng ký còn hiệu lực (huyDangKy == false)
inline int demSVDangKyHieuLuc(PTRDK dauDanhSach) {
    int cnt = 0;
    for (PTRDK p = dauDanhSach; p != nullptr; p = p->tiepTheo) {
        if (!p->duLieu.huyDangKy) cnt++;
    }
    return cnt;
}

// Đếm tổng số bản ghi đăng ký (bao gồm cả đã hủy)
inline int demTongDangKy(PTRDK dauDanhSach) {
    int cnt = 0;
    for (PTRDK p = dauDanhSach; p != nullptr; p = p->tiepTheo) cnt++;
    return cnt;
}

inline void giaiPhongDSDK(PTRDK& dauDanhSach) {
    while (dauDanhSach != nullptr) {
        PTRDK p = dauDanhSach;
        dauDanhSach = dauDanhSach->tiepTheo;
        delete p;
    }
}


// ============================================================================
// 5. DANH SÁCH LỚP TÍN CHỈ: MẢNG CON TRỎ TUYẾN TÍNH (Tối đa 10000 lớp)
// ============================================================================
struct LopTinChi {
    int MALOPTC;         // Số nguyên tự động tăng
    char MAMH[11];       // Mã môn học (C10)
    char nienKhoa[12];   // Niên khóa (VD: 2025-2026)
    int hocKy;           // Học kỳ (1, 2, 3)
    int nhom;            // Nhóm (1, 2, ...)
    int soSvMin;         // Số SV tối thiểu
    int soSvMax;         // Số SV tối đa
    bool huyLop;         // true nếu lớp đã bị hủy
    PTRDK dsSinhVienDangKy;        // Con trỏ tới danh sách sinh viên đăng ký

    LopTinChi() {
        MALOPTC = 0;
        MAMH[0] = '\0';
        nienKhoa[0] = '\0';
        hocKy = 1;
        nhom = 1;
        soSvMin = 15;
        soSvMax = 60;
        huyLop = false;
        dsSinhVienDangKy = nullptr;
    }
};

struct DSLopTinChi {
    LopTinChi* cacNutDuLieu[MAX_LOPTC];
    int n;
    int nextMaLopTC;

    DSLopTinChi() {
        n = 0;
        nextMaLopTC = 1;
        for (int i = 0; i < MAX_LOPTC; ++i) {
            cacNutDuLieu[i] = nullptr;
        }
    }

    ~DSLopTinChi() {
        for (int i = 0; i < n; ++i) {
            if (cacNutDuLieu[i] != nullptr) {
                giaiPhongDSDK(cacNutDuLieu[i]->dsSinhVienDangKy);
                delete cacNutDuLieu[i];
                cacNutDuLieu[i] = nullptr;
            }
        }
    }
};

// Tìm lớp tín chỉ theo bộ 4 khóa (Niên khóa, Học kỳ, Mã MH, Nhóm)
inline int timLopTinChiTheoThamSo(const DSLopTinChi& danhSachLopTinChi, const char* nienKhoa, int hocKy, const char* maMH, int nhom) {
    for (int i = 0; i < danhSachLopTinChi.n; ++i) {
        LopTinChi* lopTinChi = danhSachLopTinChi.cacNutDuLieu[i];
        if (_stricmp(lopTinChi->nienKhoa, nienKhoa) == 0 &&
            lopTinChi->hocKy == hocKy &&
            _stricmp(lopTinChi->MAMH, maMH) == 0 &&
            lopTinChi->nhom == nhom) {
            return i;
        }
    }
    return -1;
}

// Tìm lớp tín chỉ theo MALOPTC
inline int timLopTinChiTheoMa(const DSLopTinChi& danhSachLopTinChi, int maLopTC) {
    for (int i = 0; i < danhSachLopTinChi.n; ++i) {
        if (danhSachLopTinChi.cacNutDuLieu[i]->MALOPTC == maLopTC) return i;
    }
    return -1;
}

// Kiểm tra môn học đã được mở trong bất kỳ lớp tín chỉ nào chưa (để ràng buộc xóa môn học)
inline bool kiemTraMonHocDaMoLop(const DSLopTinChi& danhSachLopTinChi, const char* maMH) {
    for (int i = 0; i < danhSachLopTinChi.n; ++i) {
        if (strcmp(danhSachLopTinChi.cacNutDuLieu[i]->MAMH, maMH) == 0) return true;
    }
    return false;
}

// Kiểm tra sinh viên đã đăng ký lớp tín chỉ nào chưa (để ràng buộc xóa sinh viên)
inline bool kiemTraSVDaDangKyLTC(const DSLopTinChi& danhSachLopTinChi, const string& maSV) {
    for (int i = 0; i < danhSachLopTinChi.n; ++i) {
        if (timDangKy(danhSachLopTinChi.cacNutDuLieu[i]->dsSinhVienDangKy, maSV) != nullptr) return true;
    }
    return false;
}



