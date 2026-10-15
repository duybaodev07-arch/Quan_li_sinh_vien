#include "LopTinChi.h"
#include "Input.h"
#include "HienThi.h"
#include "MonHoc.h"
#include <cerrno>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

const string DUONG_DAN_FILE_LOP_TIN_CHI = "data/LOPTC.txt";
static int maLopTCTiepTheo = 0;

namespace {
const int MAX_DO_DAI_TRUONG_FILE = 64;

bool tachTruongFile(
    const string& dong,
    char truong[][MAX_DO_DAI_TRUONG_FILE],
    int soTruongToiDa,
    int& soTruong
) {
    soTruong = 0;
    size_t batDau = 0;

    for (size_t i = 0; i <= dong.length(); ++i) {
        if (i != dong.length() && dong[i] != '|') {
            continue;
        }

        if (soTruong >= soTruongToiDa) {
            return false;
        }

        const size_t doDai = i - batDau;
        if (doDai >= MAX_DO_DAI_TRUONG_FILE) {
            return false;
        }

        memcpy(truong[soTruong], dong.c_str() + batDau, doDai);
        truong[soTruong][doDai] = '\0';
        ++soTruong;
        batDau = i + 1;
    }

    return true;
}

bool docSoNguyen(const char* noiDung, int& ketQua) {
    if (noiDung == nullptr || noiDung[0] == '\0') {
        return false;
    }

    errno = 0;
    char* phanDu = nullptr;
    const long giaTri = strtol(noiDung, &phanDu, 10);

    if (errno != 0 || phanDu == noiDung || *phanDu != '\0' ||
        giaTri < INT_MIN || giaTri > INT_MAX) {
        return false;
    }

    ketQua = static_cast<int>(giaTri);
    return true;
}

bool docSoThuc(const char* noiDung, float& ketQua) {
    if (noiDung == nullptr || noiDung[0] == '\0') {
        return false;
    }

    errno = 0;
    char* phanDu = nullptr;
    const float giaTri = strtof(noiDung, &phanDu);

    if (errno != 0 || phanDu == noiDung || *phanDu != '\0' ||
        !isfinite(giaTri)) {
        return false;
    }

    ketQua = giaTri;
    return true;
}

bool maChiChuaChuVaSo(const char* ma) {
    if (ma == nullptr || ma[0] == '\0') {
        return false;
    }

    for (int i = 0; ma[i] != '\0'; ++i) {
        if (!isalnum(static_cast<unsigned char>(ma[i]))) {
            return false;
        }
    }
    return true;
}

bool laNienKhoaHopLe(const char* nienKhoa) {
    if (nienKhoa == nullptr || strlen(nienKhoa) != 9 ||
        nienKhoa[4] != '-') {
        return false;
    }

    for (int i = 0; i < 9; ++i) {
        if (i == 4) {
            continue;
        }
        if (!isdigit(static_cast<unsigned char>(nienKhoa[i]))) {
            return false;
        }
    }

    char namTruocText[5]{};
    char namSauText[5]{};
    memcpy(namTruocText, nienKhoa, 4);
    memcpy(namSauText, nienKhoa + 5, 4);

    int namTruoc = 0;
    int namSau = 0;
    return docSoNguyen(namTruocText, namTruoc) &&
           docSoNguyen(namSauText, namSau) &&
           namSau >= namTruoc;
}

void giaiPhongLopDangDoc(PTRLTC& lopTC) {
    if (lopTC == nullptr) {
        return;
    }
    xoaTatCaDangKy(lopTC->dssvdk);
    delete lopTC;
    lopTC = nullptr;
}
}

PTRLTC taoLopTinChi() {
    return new LopTinChi;
}

int timLopTinChiTheoMa(
    const DanhSachLopTinChi& dsLopTC,
    int maLopTC
) {
    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        if (dsLopTC.lopTC[i] != nullptr &&
            dsLopTC.lopTC[i]->maLopTC == maLopTC) {
            return i;
        }
    }
    return -1;
}

int timLopTinChiTheoMonHoc(
    const DanhSachLopTinChi& dsLopTC,
    const char* maMH,
    const char* nienKhoa,
    int hocKy,
    int nhom
) {
    if (maMH == nullptr || nienKhoa == nullptr) {
        return -1;
    }

    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        LopTinChi* lopTC = dsLopTC.lopTC[i];
        if (lopTC == nullptr) {
            continue;
        }

        if (strcmp(lopTC->maMH, maMH) == 0 &&
            strcmp(lopTC->nienKhoa, nienKhoa) == 0 &&
            lopTC->hocKy == hocKy &&
            lopTC->nhom == nhom) {
            return i;
        }
    }
    return -1;
}

void nhapLopTinChi(LopTinChi& lopTC) {
    cout << "Ma mon hoc: ";
    nhapMaBatBuoc(lopTC.maMH, MAX_MAMH);

    nhapNienKhoa(lopTC.nienKhoa, MAX_NIENKHOA);

    cout << "Hoc ky: ";
    lopTC.hocKy = nhapSoNguyenKhongAm();

    cout << "Nhom: ";
    lopTC.nhom = nhapSoNguyenKhongAm();

    cout << "So sinh vien toi thieu: ";
    lopTC.minSV = nhapSoNguyenKhongAm();

    do {
        cout << "So sinh vien toi da: ";
        lopTC.maxSV = nhapSoNguyenKhongAm();

        if (lopTC.maxSV < lopTC.minSV)
            cout << "So sinh vien toi da phai >= so sinh vien toi thieu!\n";

    } while (lopTC.maxSV < lopTC.minSV);
}

void nhapThongTinTimLopTinChi(char* maMH, char* nienKhoa, int& hocKy, int& nhom) {
    nhapNienKhoa(nienKhoa, MAX_NIENKHOA);

    cout << "Hoc ky: ";
    hocKy = nhapSoNguyenKhongAm();

    cout << "Ma mon hoc: ";
    nhapMaBatBuoc(maMH, MAX_MAMH);

    cout << "Nhom: ";
    nhom = nhapSoNguyenKhongAm();
}
bool themLopTinChiVaoDanhSach(
    DanhSachLopTinChi& dsLopTC,
    PTRLTC lopTC
) {
    if (lopTC == nullptr || dsLopTC.soLuong < 0) {
        return false;
    }

    if (dsLopTC.soLuong >= MAX_LOP_TC) {
        return false;
    }

    if (lopTC->maxSV < lopTC->minSV || lopTC->minSV < 0) {
        return false;
    }

    const int viTriTrung = timLopTinChiTheoMonHoc(
        dsLopTC,
        lopTC->maMH,
        lopTC->nienKhoa,
        lopTC->hocKy,
        lopTC->nhom
    );
    if (viTriTrung != -1) {
        return false;
    }

    lopTC->maLopTC = maLopTCTiepTheo++;
    dsLopTC.lopTC[dsLopTC.soLuong] = lopTC;
    ++dsLopTC.soLuong;

    return true;
}

void themLopTinChi(DanhSachLopTinChi& dsLopTC) {
    PTRLTC lopTC = taoLopTinChi();

    nhapLopTinChi(*lopTC);

    if (!themLopTinChiVaoDanhSach(dsLopTC, lopTC)) {
        delete lopTC;
        cout << "Them lop tin chi that bai. Kiem tra trung lop hoac suc chua.\n";
        return;
    }

    cout << "Them lop tin chi thanh cong!\n";
    cout << "Ma lop tin chi: " << lopTC->maLopTC << endl;
}

bool hieuChinhLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC) {
    const int findIdx = timLopTinChiTheoMa(dsLopTC, maLopTC);

    if (findIdx == -1)
        return false;

    LopTinChi thongTinMoi = *dsLopTC.lopTC[findIdx];
    nhapLopTinChi(thongTinMoi);

    const int viTriTrung = timLopTinChiTheoMonHoc(
        dsLopTC,
        thongTinMoi.maMH,
        thongTinMoi.nienKhoa,
        thongTinMoi.hocKy,
        thongTinMoi.nhom
    );
    if (viTriTrung != -1 && viTriTrung != findIdx) {
        cout << "Thong tin lop tin chi bi trung voi lop khac!\n";
        return false;
    }

    thongTinMoi.maLopTC = dsLopTC.lopTC[findIdx]->maLopTC;
    thongTinMoi.dssvdk = dsLopTC.lopTC[findIdx]->dssvdk;
    *dsLopTC.lopTC[findIdx] = thongTinMoi;

    return true;
}
bool xoaLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC) {
    const int viTri = timLopTinChiTheoMa(dsLopTC, maLopTC);

    if (viTri == -1)
        return false;

    xoaTatCaDangKy(dsLopTC.lopTC[viTri]->dssvdk);
    delete dsLopTC.lopTC[viTri];

    for (int i = viTri; i < dsLopTC.soLuong - 1; i++)
        dsLopTC.lopTC[i] = dsLopTC.lopTC[i + 1];

    dsLopTC.soLuong--;
    dsLopTC.lopTC[dsLopTC.soLuong] = nullptr;

    return true;
}
void xoaTatCaLopTinChi(DanhSachLopTinChi& dsLopTC) {
    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        if (dsLopTC.lopTC[i] == nullptr) {
            continue;
        }
        xoaTatCaDangKy(dsLopTC.lopTC[i]->dssvdk);
        delete dsLopTC.lopTC[i];
        dsLopTC.lopTC[i] = nullptr;
    }
    dsLopTC.soLuong = 0;
}
bool huyLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC) {
    const int viTri = timLopTinChiTheoMa(dsLopTC, maLopTC);

    if (viTri == -1)
        return false;

    dsLopTC.lopTC[viTri]->huyLop = true;

    return true;
}
bool tuDongHuyLopKhongDuSinhVien(LopTinChi& lopTC) {
    if (lopTC.huyLop)
        return false;

    int soSVDangKy = demSoSVDangKy(lopTC.dssvdk);

    if (soSVDangKy >= lopTC.minSV)
        return false;

    cout << "Lop tin chi khong du so sinh vien!\n";
    cout << "Ma lop TC: " << lopTC.maLopTC << endl;
    cout << "So SV dang ky: " << soSVDangKy << endl;
    cout << "So SV toi thieu: " << lopTC.minSV << endl;

    cout << "Ban co muon huy lop nay? (Y/N): ";
    const char xacNhan = nhapXacNhan();

    if (xacNhan == 'Y') {
        lopTC.huyLop = true;
        cout << "Da huy lop tin chi!\n";
        return true;
    }

    return false;
}
void tuDongHuyDanhSachLopKhongDuSinhVien(DanhSachLopTinChi& dsLopTC) {
    if (dsLopTC.soLuong <= 0) {
        cout << "Danh sach lop tin chi rong!\n";
        return;
    }

    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        tuDongHuyLopKhongDuSinhVien(*dsLopTC.lopTC[i]);
    }
}
namespace {
const int DO_RONG_MA_LOP_TC = 10;
const int DO_RONG_HOC_KY = 4;
const int DO_RONG_NHOM = 5;
const int DO_RONG_SI_SO = 6;
const int DO_RONG_TRANG_THAI = 8;

void inDuongKeLopTinChi() {
    cout << "+" << string(DO_RONG_MA_LOP_TC + 2, '-')
         << "+" << string(MAX_MAMH - 1 + 2, '-')
         << "+" << string(MAX_NIENKHOA - 1 + 2, '-')
         << "+" << string(DO_RONG_HOC_KY + 2, '-')
         << "+" << string(DO_RONG_NHOM + 2, '-')
         << "+" << string(DO_RONG_SI_SO + 2, '-')
         << "+" << string(DO_RONG_SI_SO + 2, '-')
         << "+" << string(DO_RONG_TRANG_THAI + 2, '-')
         << "+\n";
}

void inTieuDeLopTinChi() {
    inDuongKeLopTinChi();
    cout << "| " << left << setw(DO_RONG_MA_LOP_TC) << "Ma lop TC"
         << " | " << setw(MAX_MAMH - 1) << "Ma mon hoc"
         << " | " << setw(MAX_NIENKHOA - 1) << "Nien khoa"
         << " | " << right << setw(DO_RONG_HOC_KY) << "HK"
         << " | " << setw(DO_RONG_NHOM) << "Nhom"
         << " | " << setw(DO_RONG_SI_SO) << "Min SV"
         << " | " << setw(DO_RONG_SI_SO) << "Max SV"
         << " | " << left << setw(DO_RONG_TRANG_THAI) << "Trang thai"
         << " |\n";
    inDuongKeLopTinChi();
}

void inDongLopTinChi(const LopTinChi& lopTC) {
    cout << "| " << left << setw(DO_RONG_MA_LOP_TC) << lopTC.maLopTC
         << " | " << setw(MAX_MAMH - 1) << lopTC.maMH
         << " | " << setw(MAX_NIENKHOA - 1) << lopTC.nienKhoa
         << " | " << right << setw(DO_RONG_HOC_KY) << lopTC.hocKy
         << " | " << setw(DO_RONG_NHOM) << lopTC.nhom
         << " | " << setw(DO_RONG_SI_SO) << lopTC.minSV
         << " | " << setw(DO_RONG_SI_SO) << lopTC.maxSV
         << " | " << left << setw(DO_RONG_TRANG_THAI)
         << (lopTC.huyLop ? "Da huy" : "Dang mo") << " |\n";
}
}

void inLopTinChi(const LopTinChi& lopTC) {
    inTieuDeLopTinChi();
    inDongLopTinChi(lopTC);
    inDuongKeLopTinChi();
}

void inDanhSachLopTinChi(const DanhSachLopTinChi& dsLopTC) {
    if (dsLopTC.soLuong == 0) {
        cout << "Danh sach lop tin chi rong!\n";
        return;
    }

    inTieuDeLopTinChi();
    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        if (dsLopTC.lopTC[i] != nullptr) {
            inDongLopTinChi(*dsLopTC.lopTC[i]);
        }
    }
    inDuongKeLopTinChi();
}
void inDanhSachSinhVienDangKy(DanhSachLop& dsLop, const LopTinChi& lopTC) {
    cout << "\nDANH SACH SINH VIEN DANG KY - LOP TC " << lopTC.maLopTC << "\n";

    if (lopTC.dssvdk == nullptr) {
        cout << "Chua co sinh vien dang ky!\n";
        return;
    }
    const int doRongSTT = 3;
    const int doRongMaSV = MAX_MASV - 1;
    const int doRongHo = MAX_HO - 1;
    const int doRongTen = MAX_TEN - 1;
    const int doRongTrangThai = 10;

    const auto inDuongKe = [&]() {
        cout << "+" << string(doRongSTT + 2, '-')
             << "+" << string(doRongMaSV + 2, '-')
             << "+" << string(doRongHo + 2, '-')
             << "+" << string(doRongTen + 2, '-')
             << "+" << string(doRongTrangThai + 2, '-')
             << "+\n";
    };

    inDuongKe();
    cout << "| " << right << setw(doRongSTT) << "STT"
         << " | " << left << setw(doRongMaSV) << "Ma sinh vien"
         << " | " << setw(doRongHo) << "Ho"
         << " | " << setw(doRongTen) << "Ten"
         << " | " << setw(doRongTrangThai) << "Trang thai"
         << " |\n";
    inDuongKe();

    int stt = 1;
    PTRDK dk = lopTC.dssvdk;

    while (dk != nullptr) {
        PTRSV sv = timSinhVienTrongDanhSachLop(dsLop, dk->dk.maSV);
        if (sv != nullptr) {
            cout << "| " << right << setw(doRongSTT) << stt << " | "
                 << left << setw(doRongMaSV) << sv->sv.maSV << " | "
                 << setw(doRongHo) << sv->sv.ho << " | "
                 << setw(doRongTen) << sv->sv.ten << " | "
                 << setw(doRongTrangThai)
                 << (dk->dk.huyDangKy ? "Da huy" : "Dang ky")
                 << " |\n";
            stt++;
        }
        dk = dk->next;
    }
    inDuongKe();
}
bool ghiFileLopTinChi(const DanhSachLopTinChi& dsLopTC) {
    ofstream file(DUONG_DAN_FILE_LOP_TIN_CHI);
    if (!file) {
        return false;
    }

    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        const LopTinChi* lopTC = dsLopTC.lopTC[i];
        if (lopTC == nullptr) {
            return false;
        }

        file << "L|"
             << lopTC->maLopTC << '|'
             << lopTC->maMH << '|'
             << lopTC->nienKhoa << '|'
             << lopTC->hocKy << '|'
             << lopTC->nhom << '|'
             << lopTC->minSV << '|'
             << lopTC->maxSV << '|'
             << lopTC->huyLop << '\n';

        for (PTRDK dk = lopTC->dssvdk; dk != nullptr; dk = dk->next) {
            file << "D|"
                 << dk->dk.maSV << '|'
                 << dk->dk.diem << '|'
                 << dk->dk.huyDangKy << '\n';
        }
        file << "E\n";
    }

    file.close();
    return file.good();
}

bool docFileLopTinChi(DanhSachLopTinChi& dsLopTC) {
    ifstream file(DUONG_DAN_FILE_LOP_TIN_CHI);
    if (!file) {
        return false;
    }

    DanhSachLopTinChi loaded{};
    string line;
    PTRLTC current = nullptr;

    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }

        char truong[9][MAX_DO_DAI_TRUONG_FILE]{};
        int soTruong = 0;
        if (!tachTruongFile(line, truong, 9, soTruong)) {
            giaiPhongLopDangDoc(current);
            xoaTatCaLopTinChi(loaded);
            return false;
        }

        if (strcmp(truong[0], "L") == 0 &&
            soTruong == 9 && current == nullptr) {
            if (loaded.soLuong >= MAX_LOP_TC) {
                xoaTatCaLopTinChi(loaded);
                return false;
            }

            current = taoLopTinChi();
            int huyLop = 0;
            const bool hopLe =
                docSoNguyen(truong[1], current->maLopTC) &&
                strlen(truong[2]) < MAX_MAMH &&
                strlen(truong[3]) < MAX_NIENKHOA &&
                docSoNguyen(truong[4], current->hocKy) &&
                docSoNguyen(truong[5], current->nhom) &&
                docSoNguyen(truong[6], current->minSV) &&
                docSoNguyen(truong[7], current->maxSV) &&
                docSoNguyen(truong[8], huyLop) &&
                (huyLop == 0 || huyLop == 1);

            if (!hopLe) {
                giaiPhongLopDangDoc(current);
                xoaTatCaLopTinChi(loaded);
                return false;
            }

            strcpy(current->maMH, truong[2]);
            strcpy(current->nienKhoa, truong[3]);
            chuanHoaMa(current->maMH);
            current->huyLop = huyLop != 0;

            const bool giaTriHopLe =
                maChiChuaChuVaSo(current->maMH) &&
                laNienKhoaHopLe(current->nienKhoa) &&
                current->maLopTC >= 0 &&
                current->hocKy >= 0 &&
                current->nhom >= 0 &&
                current->minSV >= 0 &&
                current->maxSV >= current->minSV;
            const bool maKhongTrung =
                timLopTinChiTheoMa(loaded, current->maLopTC) == -1;
            const bool thongTinKhongTrung =
                timLopTinChiTheoMonHoc(
                    loaded,
                    current->maMH,
                    current->nienKhoa,
                    current->hocKy,
                    current->nhom
                ) == -1;

            if (!giaTriHopLe || !maKhongTrung || !thongTinKhongTrung) {
                giaiPhongLopDangDoc(current);
                xoaTatCaLopTinChi(loaded);
                return false;
            }
        } else if (strcmp(truong[0], "D") == 0 &&
                   soTruong == 4 && current != nullptr) {
            DangKy dk{};
            int huyDangKy = 0;
            const bool hopLe =
                strlen(truong[1]) < MAX_MASV &&
                docSoThuc(truong[2], dk.diem) &&
                docSoNguyen(truong[3], huyDangKy) &&
                (huyDangKy == 0 || huyDangKy == 1);

            if (!hopLe) {
                giaiPhongLopDangDoc(current);
                xoaTatCaLopTinChi(loaded);
                return false;
            }

            strcpy(dk.maSV, truong[1]);
            chuanHoaMa(dk.maSV);
            dk.huyDangKy = huyDangKy != 0;

            const bool duLieuDangKyHopLe =
                maChiChuaChuVaSo(dk.maSV) &&
                dk.diem >= -1.0f &&
                dk.diem <= 10.0f &&
                timDangKyTheoMaSV(current->dssvdk, dk.maSV) == nullptr;

            if (!duLieuDangKyHopLe ||
                !themDangKy(current->dssvdk, dk)) {
                giaiPhongLopDangDoc(current);
                xoaTatCaLopTinChi(loaded);
                return false;
            }
        } else if (strcmp(truong[0], "E") == 0 &&
                   soTruong == 1 && current != nullptr) {
            loaded.lopTC[loaded.soLuong] = current;
            ++loaded.soLuong;

            if (current->maLopTC >= maLopTCTiepTheo) {
                maLopTCTiepTheo = current->maLopTC + 1;
            }
            current = nullptr;
        } else {
            giaiPhongLopDangDoc(current);
            xoaTatCaLopTinChi(loaded);
            return false;
        }
    }

    if (file.bad() || current != nullptr) {
        giaiPhongLopDangDoc(current);
        xoaTatCaLopTinChi(loaded);
        return false;
    }

    xoaTatCaLopTinChi(dsLopTC);
    dsLopTC = loaded;
    return true;
}
bool nhapDiem(DanhSachLopTinChi& dsLopTC, DanhSachLop& dsLop) {
    char maMH[MAX_MAMH];
    char nienKhoa[MAX_NIENKHOA];
    int hocKy;
    int nhom;
    nhapThongTinTimLopTinChi(maMH, nienKhoa, hocKy, nhom);
    int viTri = timLopTinChiTheoMonHoc(
        dsLopTC,
        maMH,
        nienKhoa,
        hocKy,
        nhom
    );

    if (viTri == -1) {
        cout << "Khong tim thay lop tin chi!\n";
        return false;
    }

    LopTinChi* lopTC = dsLopTC.lopTC[viTri];

    if (lopTC->huyLop) {
        cout << "Lop tin chi da bi huy!\n";
        return false;
    }

    if (lopTC->dssvdk == nullptr) {
        cout << "Lop tin chi chua co sinh vien dang ky!\n";
        return false;
    }
    nhapDiemTrenBang(dsLop, lopTC->dssvdk);

    return true;
}
bool xemBangDiemLopTinChi(DanhSachLopTinChi& dsLopTC, DanhSachLop& dsLop) {
    char maMH[MAX_MAMH];
    char nienKhoa[MAX_NIENKHOA];
    int hocKy;
    int nhom;

    nhapThongTinTimLopTinChi(maMH, nienKhoa, hocKy, nhom);
    int viTri = timLopTinChiTheoMonHoc(
        dsLopTC,
        maMH,
        nienKhoa,
        hocKy,
        nhom
    );

    if (viTri == -1) {
        cout << "Khong tim thay lop tin chi!\n";
        return false;
    }

    LopTinChi* lopTC = dsLopTC.lopTC[viTri];

    if (lopTC->huyLop) {
        cout << "Lop tin chi da bi huy!\n";
        return false;
    }

    if (lopTC->dssvdk == nullptr) {
        cout << "Lop tin chi chua co sinh vien dang ky!\n";
        return false;
    }

    inBangDiemLopTinChi(dsLop, *lopTC);

    return true;
}
bool dangKyLopTinChi(
    DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH rootMonHoc
) {
    char maSV[MAX_MASV];
    char nienKhoa[MAX_NIENKHOA];
    cout << "Nhap ma sinh vien: ";
    if (!nhapMa(maSV, MAX_MASV)) return false;
    PTRSV sv = timSinhVienTrongDanhSachLop(dsLop, maSV);
    if (sv == nullptr) {
        cout << "Khong tim thay sinh vien!\n";
        return false;
    }
    cout << "Thong tin sinh vien:\n";
    inSinhVien(sv->sv);

    cout << "Nhap nien khoa: ";
    nhapNienKhoa(nienKhoa, MAX_NIENKHOA);
    cout << "Nhap hoc ky: ";
    const int hocKy = nhapSoNguyenKhongAm();

    int soLopHienThi = 0;
    const int doRongMaLopTC = 10;
    const int doRongMaMH = MAX_MAMH - 1;
    const int doRongNhom = 6;
    const int doRongTenMH = MAX_TENMH - 1;
    const int doRongDaDangKy = 14;
    const int doRongSlotCon = 16;
    const string dongKe =
        "+" + string(doRongMaLopTC, '-') +
        "+" + string(doRongMaMH, '-') +
        "+" + string(doRongNhom, '-') +
        "+" + string(doRongTenMH, '-') +
        "+" + string(doRongDaDangKy, '-') +
        "+" + string(doRongSlotCon, '-') + "+\n";

    cout << dongKe;
    cout << "|" << left << setw(doRongMaLopTC) << "Ma lop TC"
         << "|" << setw(doRongMaMH) << "Ma MH"
         << "|" << setw(doRongNhom) << "Nhom"
         << "|" << setw(doRongTenMH) << "Ten mon hoc"
         << "|" << setw(doRongDaDangKy) << "Da dang ky"
         << "|" << setw(doRongSlotCon) << "Slot con trong" << "|\n";
    cout << dongKe;
    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        const LopTinChi& lopTC = *dsLopTC.lopTC[i];
        if (lopTC.huyLop || strcmp(lopTC.nienKhoa, nienKhoa) != 0 || lopTC.hocKy != hocKy)
            continue;
        PTRMH monHoc = timMonHocTheoMa(rootMonHoc, lopTC.maMH);
        const char* tenMH = monHoc == nullptr ? "(khong tim thay)" : monHoc->mh.tenMH;
        const int daDangKy = demSoSVDangKy(lopTC.dssvdk);
        const int slotCon = lopTC.maxSV > daDangKy ? lopTC.maxSV - daDangKy : 0;
        cout << "|" << right << setw(doRongMaLopTC) << lopTC.maLopTC
             << "|" << left << setw(doRongMaMH) << lopTC.maMH
             << "|" << right << setw(doRongNhom) << lopTC.nhom
             << "|" << left << setw(doRongTenMH) << tenMH
             << "|" << right << setw(doRongDaDangKy) << daDangKy
             << "|" << setw(doRongSlotCon) << slotCon << "|\n";
        ++soLopHienThi;
    }
    cout << dongKe;
    if (soLopHienThi == 0) {
        cout << "Khong co lop tin chi dang mo trong nien khoa, hoc ky nay.\n";
        return false;
    }

    cout << "Nhap ma lop tin chi muon dang ky: ";
    const int maLopTC = nhapSoNguyenKhongAm();
    const int viTriLopTC = timLopTinChiTheoMa(dsLopTC, maLopTC);
    if (viTriLopTC < 0) {
        cout << "Ma lop tin chi khong hop le!\n";
        return false;
    }
    LopTinChi* lopTC = dsLopTC.lopTC[viTriLopTC];
    if (lopTC->huyLop || strcmp(lopTC->nienKhoa, nienKhoa) != 0 || lopTC->hocKy != hocKy) {
        cout << "Lop khong nam trong danh sach dang mo da hien thi!\n";
        return false;
    }
    if (demSoSVDangKy(lopTC->dssvdk) >= lopTC->maxSV) {
        cout << "Lop tin chi da du so sinh vien!\n";
        return false;
    }
    if (daDangKy(lopTC->dssvdk, maSV)) {
        cout << "Sinh vien da dang ky lop tin chi nay!\n";
        return false;
    }

    DangKy dk{};
    strcpy(dk.maSV, maSV);
    dk.diem = -1.0f; // chua nhap diem; diem 0 van la diem hop le
    dk.huyDangKy = false;

    if (!themDangKy(lopTC->dssvdk, dk)) {
        cout << "Dang ky that bai!\n";
        return false;
    }

    cout << "Dang ky lop tin chi thanh cong!\n";
    return true;
}
bool huyDangKyLopTinChi(
    DanhSachLopTinChi& dsLopTC
) {
    char maMH[MAX_MAMH];
    char nienKhoa[MAX_NIENKHOA];
    int hocKy;
    int nhom;
    char maSV[MAX_MASV];

    nhapThongTinTimLopTinChi(maMH, nienKhoa, hocKy, nhom);

    int viTriLopTC = timLopTinChiTheoMonHoc(
        dsLopTC,
        maMH,
        nienKhoa,
        hocKy,
        nhom
    );

    if (viTriLopTC == -1) {
        cout << "Khong tim thay lop tin chi!\n";
        return false;
    }

    LopTinChi* lopTC = dsLopTC.lopTC[viTriLopTC];

    if (lopTC->huyLop) {
        cout << "Lop tin chi da bi huy!\n";
        return false;
    }

    cout << "Nhap ma sinh vien: ";
    if (!nhapMa(maSV, MAX_MASV)) return false;

    if (!daDangKy(lopTC->dssvdk, maSV)) {
        cout << "Sinh vien chua dang ky lop tin chi nay!\n";
        return false;
    }

    if (!huyDangKyTheoMaSV(lopTC->dssvdk, maSV)) {
        cout << "Huy dang ky that bai!\n";
        return false;
    }

    cout << "Huy dang ky thanh cong!\n";
    return true;
}
