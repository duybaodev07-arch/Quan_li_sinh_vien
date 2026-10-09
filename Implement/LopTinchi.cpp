#include "LopTinChi.h"
#include "Input.h"
#include "HienThi.h"
#include<cstring>
#include<limits>
#include<fstream>
#include<iostream>
using namespace std;
static int maLopTCTiepTheo = 0;
PTRLTC taoLopTinChi() {
    return new LopTinChi;
}
int timLopTinChi(const DanhSachLopTinChi& dsLopTc, int maLopTC) {
    for(int i = 0; i < dsLopTc.soLuong; i++) {
        if(dsLopTc.lop[i]->maLopTC == maLopTC) 
            return i;
    }
    return -1;
}
int timLopTinChiTheoMonHoc(const DanhSachLopTinChi& dsLopTC, const char* maMH, const char* nienKhoa, int hocKy, int nhom) {
    for (int i = 0; i < dsLopTC.soLuong; i++) {
        LopTinChi* lopTC = dsLopTC.lop[i];

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
    nhapChuoi(lopTC.maMH, MAX_MAMH);

    cout << "Nien khoa: ";
    nhapChuoi(lopTC.nienKhoa, MAX_NIENKHOA);

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
    cout << "Nien khoa: ";
    nhapChuoi(nienKhoa, MAX_NIENKHOA);

    cout << "Hoc ky: ";
    hocKy = nhapSoNguyenKhongAm();

    cout << "Ma mon hoc: ";
    nhapChuoi(maMH, MAX_MAMH);

    cout << "Nhom: ";
    nhom = nhapSoNguyenKhongAm();
}
bool themLopTinChi(DanhSachLopTinChi& dsLopTC, PTRLTC lopTC) {
    if (lopTC == nullptr)
        return false;

    if (dsLopTC.soLuong >= MAX_LOP_TC)
        return false;

    lopTC->maLopTC = maLopTCTiepTheo++;
    dsLopTC.lop[dsLopTC.soLuong] = lopTC;
    dsLopTC.soLuong++;

    return true;
}
void themLopTinChiTuBanPhim(DanhSachLopTinChi& dsLopTC) {
    PTRLTC lopTC = taoLopTinChi();

    nhapLopTinChi(*lopTC);

    if (!themLopTinChi(dsLopTC, lopTC)) {
        delete lopTC;
        cout << "Them lop tin chi that bai!\n";
        return;
    }

    cout << "Them lop tin chi thanh cong!\n";
    cout << "Ma lop tin chi: " << lopTC->maLopTC << endl;
}
bool hieuChinhLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC) {
    int findIdx = timLopTinChi(dsLopTC, maLopTC);

    if (findIdx == -1)
        return false;

    nhapLopTinChi(*dsLopTC.lop[findIdx]); // ko sua dssv dki

    return true;
}
bool xoaLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC) {
    int viTri = timLopTinChi(dsLopTC, maLopTC);

    if (viTri == -1)
        return false;

    xoaTatCaDangKy(dsLopTC.lop[viTri]->dssvdk);
    delete dsLopTC.lop[viTri];

    for (int i = viTri; i < dsLopTC.soLuong - 1; i++)
        dsLopTC.lop[i] = dsLopTC.lop[i + 1];

    dsLopTC.soLuong--;
    dsLopTC.lop[dsLopTC.soLuong] = nullptr;

    return true;
}
void xoaTatCaLopTinChi(DanhSachLopTinChi& dsLopTC) {
    for(int i = 0; i < dsLopTC.soLuong; i++) {
        xoaTatCaDangKy(dsLopTC.lop[i]->dssvdk);
        delete dsLopTC.lop[i];
        dsLopTC.lop[i] = nullptr;
    }
    dsLopTC.soLuong = 0;
}
bool huyLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC) {
    int viTri = timLopTinChi(dsLopTC, maLopTC);

    if (viTri == -1)
        return false;

    dsLopTC.lop[viTri]->huyLop = true;

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

    char xacNhan;

    do {
        cout << "Ban co muon huy lop nay? (Y/N): ";
        cin >> xacNhan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (xacNhan != 'Y' && xacNhan != 'y' &&
            xacNhan != 'N' && xacNhan != 'n') {
            cout << "Vui long chi nhap Y hoac N!\n";
        }

    } while (xacNhan != 'Y' && xacNhan != 'y' &&
             xacNhan != 'N' && xacNhan != 'n');

    if (xacNhan == 'Y' || xacNhan == 'y') {
        lopTC.huyLop = true;
        cout << "Da huy lop tin chi!\n";
        return true;
    }

    return false;
}
void tuDongHuyDanhSachLopKhongDuSinhVien(DanhSachLopTinChi& dsLopTC) {
    for (int i = 0; i < dsLopTC.soLuong; i++) {
        tuDongHuyLopKhongDuSinhVien(*dsLopTC.lop[i]);
    }
}
void inLopTinChi(const LopTinChi& lopTC) {
    cout << "Ma lop TC : " << lopTC.maLopTC << endl;
    cout << "Ma mon hoc: " << lopTC.maMH << endl;
    cout << "Nien khoa : " << lopTC.nienKhoa << endl;
    cout << "Hoc ky    : " << lopTC.hocKy << endl;
    cout << "Nhom      : " << lopTC.nhom << endl;
    cout << "Min SV    : " << lopTC.minSV << endl;
    cout << "Max SV    : " << lopTC.maxSV << endl;
    cout << "Trang thai: " << (lopTC.huyLop ? "Da huy" : "Dang mo") << endl;
}
void inDanhSachLopTinChi(const DanhSachLopTinChi& dsLopTC) {
    for (int i = 0; i < dsLopTC.soLuong; i++) {
        inLopTinChi(*dsLopTC.lop[i]);
        cout << "-----------------------------" << endl;
    }
}
void inDanhSachSinhVienDangKy(
    DanhSachLop& dsLop,
    const LopTinChi& lopTC
) {
    cout << "\n";
    cout << "DANH SACH SINH VIEN DANG KY\n";
    inLopTinChi(lopTC);

    if (lopTC.dssvdk == nullptr) {
        cout << "Chua co sinh vien dang ky!\n";
        return;
    }
    cout << "---------------------------------------------\n";

    int stt = 1;
    PTRDK dk = lopTC.dssvdk;

    while (dk != nullptr) {
        PTRSV sv = timSinhVienTrongDanhSachLop(dsLop, dk->dk.maSV);
        if(sv != nullptr) {
            cout << "STT : " << stt << endl;
            inSinhVien(sv->sv);
            cout << "Diem  : " << dk->dk.diem << endl;
            cout << "---------------------------------------------\n";
            stt++;
        }
        dk = dk->next;
    }
}
bool ghiFileLopTinChi(const DanhSachLopTinChi& dsLopTC) {
    ofstream file("LopTinChi.txt");

    if (!file)
        return false;

    file << dsLopTC.soLuong << '\n';
    file << "--------------------\n";

    for (int i = 0; i < dsLopTC.soLuong; i++) {
        LopTinChi* lopTC = dsLopTC.lop[i];

        file << lopTC->maLopTC << '\n';
        file << lopTC->maMH << '\n';
        file << lopTC->nienKhoa << '\n';
        file << lopTC->hocKy << '\n';
        file << lopTC->nhom << '\n';
        file << lopTC->minSV << '\n';
        file << lopTC->maxSV << '\n';
        file << lopTC->huyLop << '\n';
        ghiFileLop(lopTC->dssvdk);
        if (i < dsLopTC.soLuong - 1)
            file << "--------------------\n";
    }

    file.close();
    return true;
}
bool docFileLopTinChi(DanhSachLopTinChi& dsLopTC) {
    ifstream file("LopTinChi.txt");

    if (!file)
        return false;

    xoaTatCaLopTinChi(dsLopTC);

    int soLuong;
    file >> soLuong;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    // Bỏ dòng --------------------
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < soLuong; i++) {
        PTRLTC lopTC = taoLopTinChi();

        file >> lopTC->maLopTC;
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        file.getline(lopTC->maMH, MAX_MAMH);
        file.getline(lopTC->nienKhoa, MAX_NIENKHOA);

        file >> lopTC->hocKy;
        file >> lopTC->nhom;
        file >> lopTC->minSV;
        file >> lopTC->maxSV;
        file >> lopTC->huyLop;

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        dsLopTC.lop[dsLopTC.soLuong] = lopTC;
        dsLopTC.soLuong++;

        // Cập nhật mã lớp tiếp theo
        if (lopTC->maLopTC >= maLopTCTiepTheo)
            maLopTCTiepTheo = lopTC->maLopTC + 1;

        // Bỏ dòng -------------------- giữa các lớp
        if (i < soLuong - 1)
            file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    file.close();
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

    LopTinChi* lopTC = dsLopTC.lop[viTri];

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

    LopTinChi* lopTC = dsLopTC.lop[viTri];

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
    DanhSachLop& dsLop
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

    LopTinChi* lopTC = dsLopTC.lop[viTriLopTC];

    if (lopTC->huyLop) {
        cout << "Lop tin chi da bi huy!\n";
        return false;
    }

    if (demSoSVDangKy(lopTC->dssvdk) >= lopTC->maxSV) {
        cout << "Lop tin chi da du so sinh vien!\n";
        return false;
    }

    cout << "Nhap ma sinh vien: ";
    nhapChuoi(maSV, MAX_MASV);

    PTRSV sv = timSinhVienTrongDanhSachLop(dsLop, maSV);

    if (sv == nullptr) {
        cout << "Khong tim thay sinh vien!\n";
        return false;
    }

    if (daDangKy(lopTC->dssvdk, maSV)) {
        cout << "Sinh vien da dang ky lop tin chi nay!\n";
        return false;
    }

    DangKy dk;
    strcpy(dk.maSV, maSV);
    dk.diem = 0;
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

    LopTinChi* lopTC = dsLopTC.lop[viTriLopTC];

    if (lopTC->huyLop) {
        cout << "Lop tin chi da bi huy!\n";
        return false;
    }

    cout << "Nhap ma sinh vien: ";
    nhapChuoi(maSV, MAX_MASV);

    if (!daDangKy(lopTC->dssvdk, maSV)) {
        cout << "Sinh vien chua dang ky lop tin chi nay!\n";
        return false;
    }

    if (!xoaDangKy(lopTC->dssvdk, maSV)) {
        cout << "Huy dang ky that bai!\n";
        return false;
    }

    cout << "Huy dang ky thanh cong!\n";
    return true;
}
