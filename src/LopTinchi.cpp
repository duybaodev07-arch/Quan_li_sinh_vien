#include "LopTinChi.h"
#include "Input.h"
#include "HienThi.h"
#include "MonHoc.h"
#include<cstring>
#include<limits>
#include<fstream>
#include<iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;
static int maLopTCTiepTheo = 0;
PTRLTC taoLopTinChi() {
    return new LopTinChi;
}
int timLopTinChi(const DanhSachLopTinChi& dsLopTc, int maLopTC) {
    for(int i = 0; i < dsLopTc.soLuong; i++) {
        if(dsLopTc.lopTC[i]->maLopTC == maLopTC) 
            return i;
    }
    return -1;
}
int timLopTinChiTheoMonHoc(const DanhSachLopTinChi& dsLopTC, const char* maMH, const char* nienKhoa, int hocKy, int nhom) {
    for (int i = 0; i < dsLopTC.soLuong; i++) {
        LopTinChi* lopTC = dsLopTC.lopTC[i];

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
    dsLopTC.lopTC[dsLopTC.soLuong] = lopTC;
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

    nhapLopTinChi(*dsLopTC.lopTC[findIdx]); // ko sua dssv dki

    return true;
}
bool xoaLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC) {
    int viTri = timLopTinChi(dsLopTC, maLopTC);

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
    for(int i = 0; i < dsLopTC.soLuong; i++) {
        xoaTatCaDangKy(dsLopTC.lopTC[i]->dssvdk);
        delete dsLopTC.lopTC[i];
        dsLopTC.lopTC[i] = nullptr;
    }
    dsLopTC.soLuong = 0;
}
bool huyLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC) {
    int viTri = timLopTinChi(dsLopTC, maLopTC);

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
        tuDongHuyLopKhongDuSinhVien(*dsLopTC.lopTC[i]);
    }
}
void inLopTinChi(const LopTinChi& lopTC) {
    cout << "+----------+------------+------------+-----+------+--------+--------+----------+\n";
    cout << "| Ma lop TC| Ma mon hoc | Nien khoa  | HK  | Nhom | Min SV | Max SV | Status  |\n";
    cout << "+----------+------------+------------+-----+------+--------+--------+----------+\n";
    cout << "| " << left << setw(8) << lopTC.maLopTC << " | "
         << setw(10) << lopTC.maMH << " | " << setw(10) << lopTC.nienKhoa << " | "
         << right << setw(3) << lopTC.hocKy << " | " << setw(4) << lopTC.nhom << " | "
         << setw(6) << lopTC.minSV << " | " << setw(6) << lopTC.maxSV << " | "
         << left << setw(8) << (lopTC.huyLop ? "Da huy" : "Dang mo") << " |\n";
    cout << "+----------+------------+------------+-----+------+--------+--------+----------+\n";
}
void inDanhSachLopTinChi(const DanhSachLopTinChi& dsLopTC) {
    cout << "+----------+------------+------------+-----+------+--------+--------+----------+\n";
    cout << "| Ma lop TC| Ma mon hoc | Nien khoa  | HK  | Nhom | Min SV | Max SV | Status  |\n";
    cout << "+----------+------------+------------+-----+------+--------+--------+----------+\n";
    for (int i = 0; i < dsLopTC.soLuong; i++) {
        const LopTinChi& lopTC = *dsLopTC.lopTC[i];
        cout << "| " << left << setw(8) << lopTC.maLopTC << " | "
             << setw(10) << lopTC.maMH << " | " << setw(10) << lopTC.nienKhoa << " | "
             << right << setw(3) << lopTC.hocKy << " | " << setw(4) << lopTC.nhom << " | "
             << setw(6) << lopTC.minSV << " | " << setw(6) << lopTC.maxSV << " | "
             << left << setw(8) << (lopTC.huyLop ? "Da huy" : "Dang mo") << " |\n";
    }
    cout << "+----------+------------+------------+-----+------+--------+--------+----------+\n";
}
void inDanhSachSinhVienDangKy(
    DanhSachLop& dsLop,
    const LopTinChi& lopTC
) {
    cout << "\nDANH SACH SINH VIEN DANG KY - LOP TC " << lopTC.maLopTC << "\n";

    if (lopTC.dssvdk == nullptr) {
        cout << "Chua co sinh vien dang ky!\n";
        return;
    }
    cout << "+-----+---------------+-------------------------+-----------------+------------+\n";
    cout << "| STT | Ma sinh vien  | Ho                      | Ten             | Trang thai |\n";
    cout << "+-----+---------------+-------------------------+-----------------+------------+\n";

    int stt = 1;
    PTRDK dk = lopTC.dssvdk;

    while (dk != nullptr) {
        PTRSV sv = timSinhVienTrongDanhSachLop(dsLop, dk->dk.maSV);
        if (sv != nullptr) {
            cout << "| " << right << setw(3) << stt << " | "
                 << left << setw(13) << sv->sv.maSV << " | "
                 << setw(23) << sv->sv.ho << " | "
                 << setw(15) << sv->sv.ten << " | "
                 << setw(10) << (dk->dk.huyDangKy ? "Da huy" : "Dang ky") << " |\n";
            stt++;
        }
        dk = dk->next;
    }
    cout << "+-----+---------------+-------------------------+-----------------+------------+\n";
}
bool ghiFileLopTinChi(const DanhSachLopTinChi& dsLopTC) {
    ofstream file("data/LOPTC.txt");

    if (!file)
        return false;

    for (int i = 0; i < dsLopTC.soLuong; i++) {
        const LopTinChi* lopTC = dsLopTC.lopTC[i];
        if (lopTC == nullptr) return false;
        file << "L|" << lopTC->maLopTC << '|' << lopTC->maMH << '|'
             << lopTC->nienKhoa << '|' << lopTC->hocKy << '|' << lopTC->nhom
             << '|' << lopTC->minSV << '|' << lopTC->maxSV << '|' << lopTC->huyLop << '\n';
        for (PTRDK dk = lopTC->dssvdk; dk != nullptr; dk = dk->next)
            file << "D|" << dk->dk.maSV << '|' << dk->dk.diem << '|'
                 << dk->dk.huyDangKy << '\n';
        file << "E\n";
    }

    file.close();
    return true;
}
bool docFileLopTinChi(DanhSachLopTinChi& dsLopTC) {
    ifstream file("data/LOPTC.txt");

    if (!file)
        return false;

    DanhSachLopTinChi loaded{};
    string line;
    PTRLTC current = nullptr;
    auto fields = [](const string& value) {
        vector<string> result;
        string part;
        istringstream stream(value);
        while (getline(stream, part, '|')) result.push_back(part);
        return result;
    };
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> f = fields(line);
        if (f[0] == "L" && f.size() == 9 && current == nullptr) {
            if (loaded.soLuong >= MAX_LOP_TC) { xoaTatCaLopTinChi(loaded); return false; }
            current = taoLopTinChi();
            try {
                current->maLopTC = stoi(f[1]);
                if (f[2].size() >= MAX_MAMH || f[3].size() >= MAX_NIENKHOA) throw runtime_error("field too long");
                strcpy(current->maMH, f[2].c_str()); strcpy(current->nienKhoa, f[3].c_str());
                current->hocKy = stoi(f[4]); current->nhom = stoi(f[5]);
                current->minSV = stoi(f[6]); current->maxSV = stoi(f[7]); current->huyLop = stoi(f[8]) != 0;
                if (current->maLopTC < 0 || current->hocKy < 0 || current->nhom < 0 || current->minSV < 0 || current->maxSV < current->minSV) throw runtime_error("invalid values");
            } catch (...) { delete current; current = nullptr; xoaTatCaLopTinChi(loaded); return false; }
        } else if (f[0] == "D" && f.size() == 4 && current != nullptr) {
            DangKy dk{};
            try {
                if (f[1].empty() || f[1].size() >= MAX_MASV) throw runtime_error("invalid student id");
                strcpy(dk.maSV, f[1].c_str()); dk.diem = stof(f[2]); dk.huyDangKy = stoi(f[3]) != 0;
                if (!isfinite(dk.diem) || dk.diem < -1 || dk.diem > 10) throw runtime_error("invalid grade");
                if (!themDangKy(current->dssvdk, dk)) throw runtime_error("duplicate registration");
            } catch (...) { delete current; current = nullptr; xoaTatCaLopTinChi(loaded); return false; }
        } else if (f[0] == "E" && f.size() == 1 && current != nullptr) {
            loaded.lopTC[loaded.soLuong++] = current;
            if (current->maLopTC >= maLopTCTiepTheo) maLopTCTiepTheo = current->maLopTC + 1;
            current = nullptr;
        } else { if (current) { xoaTatCaDangKy(current->dssvdk); delete current; } xoaTatCaLopTinChi(loaded); return false; }
    }
    if (current != nullptr) { xoaTatCaDangKy(current->dssvdk); delete current; xoaTatCaLopTinChi(loaded); return false; }
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
    nhapChuoi(nienKhoa, MAX_NIENKHOA);
    cout << "Nhap hoc ky: ";
    const int hocKy = nhapSoNguyenKhongAm();

    int soLopHienThi = 0;
    cout << "+----------+------------+------+----------------------+--------------+----------------+\n";
    cout << "| Ma lop TC| Ma MH      | Nhom | Ten mon hoc          | Da dang ky   | Slot con trong |\n";
    cout << "+----------+------------+------+----------------------+--------------+----------------+\n";
    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        const LopTinChi& lopTC = *dsLopTC.lopTC[i];
        if (lopTC.huyLop || strcmp(lopTC.nienKhoa, nienKhoa) != 0 || lopTC.hocKy != hocKy)
            continue;
        PTRMH monHoc = timMonHocTheoMa(rootMonHoc, lopTC.maMH);
        const char* tenMH = monHoc == nullptr ? "(khong tim thay)" : monHoc->mh.tenMH;
        const int daDangKy = demSoSVDangKy(lopTC.dssvdk);
        const int slotCon = lopTC.maxSV > daDangKy ? lopTC.maxSV - daDangKy : 0;
        cout << "| " << right << setw(8) << lopTC.maLopTC << " | "
             << left << setw(10) << lopTC.maMH << " | "
             << right << setw(4) << lopTC.nhom << " | "
             << left << setw(20) << tenMH << " | "
             << right << setw(12) << daDangKy << " | "
             << setw(14) << slotCon << " |\n";
        ++soLopHienThi;
    }
    cout << "+----------+------------+------+----------------------+--------------+----------------+\n";
    if (soLopHienThi == 0) {
        cout << "Khong co lop tin chi dang mo trong nien khoa, hoc ky nay.\n";
        return false;
    }

    cout << "Nhap ma lop tin chi muon dang ky: ";
    const int maLopTC = nhapSoNguyenKhongAm();
    const int viTriLopTC = timLopTinChi(dsLopTC, maLopTC);
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
