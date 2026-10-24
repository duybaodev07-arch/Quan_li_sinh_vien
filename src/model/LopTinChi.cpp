#include "model/LopTinChi.h"

#include <cstring>

int timLopTinChiTheoThamSo(const DanhSachLopTinChi& danhSachLopTinChi, const char* nienKhoa, int hocKy, const char* maMH, int nhom) {
    for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
        const LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[i];
        if (_stricmp(lopTinChi->nienKhoa, nienKhoa) == 0 && lopTinChi->hocKy == hocKy &&
            _stricmp(lopTinChi->maMH, maMH) == 0 && lopTinChi->nhom == nhom) return i;
    }
    return -1;
}

int timLopTinChiTheoMa(const DanhSachLopTinChi& danhSachLopTinChi, int maLopTC) {
    for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
        if (danhSachLopTinChi.lopTC[i]->maLopTC == maLopTC) return i;
    }
    return -1;
}

bool kiemTraMonHocDaMoLop(const DanhSachLopTinChi& danhSachLopTinChi, const char* maMH) {
    for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
        if (std::strcmp(danhSachLopTinChi.lopTC[i]->maMH, maMH) == 0) return true;
    }
    return false;
}

bool kiemTraSVDaDangKyLTC(const DanhSachLopTinChi& danhSachLopTinChi, const std::string& maSV) {
    for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
        if (timDangKy(danhSachLopTinChi.lopTC[i]->dssvdk, maSV) != nullptr) return true;
    }
    return false;
}

int layMaLopTCKeTiep(const DanhSachLopTinChi& danhSachLopTinChi) {
    int maLopTiepTheo = 1;
    for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
        if (danhSachLopTinChi.lopTC[i] != nullptr && danhSachLopTinChi.lopTC[i]->maLopTC >= maLopTiepTheo) {
            maLopTiepTheo = danhSachLopTinChi.lopTC[i]->maLopTC + 1;
        }
    }
    return maLopTiepTheo;
}

void giaiPhongDanhSachLopTinChi(DanhSachLopTinChi& danhSachLopTinChi) {
    for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
        if (danhSachLopTinChi.lopTC[i] == nullptr) continue;
        giaiPhongDSDK(danhSachLopTinChi.lopTC[i]->dssvdk);
        delete danhSachLopTinChi.lopTC[i];
        danhSachLopTinChi.lopTC[i] = nullptr;
    }
    danhSachLopTinChi.soLuong = 0;
}