#include "MonHoc.h"
#include "Input.h"

#include <cstring>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <system_error>
#include <utility>

using namespace std;    
// NAMESPACE DÙNG CHO CÁC HÀM IN GỌI TRONG IN DANH SÁCH MÔN HỌC
namespace {
const char* DUONG_DAN_FILE_MON_HOC = "data/MONHOC.txt";

void inTieuDeDanhSachMonHoc() {
    cout << "+" << string(MAX_MAMH + 2, '-')
         << "+" << string(MAX_TENMH + 2, '-')
         << "+-------+-------+\n";
    cout << "| " << left << setw(MAX_MAMH) << "Ma MH"
         << " | " << left << setw(MAX_TENMH) << "Ten mon hoc"
         << " | TCLT  | TCTH  |\n";
    cout << "+" << string(MAX_MAMH + 2, '-')
         << "+" << string(MAX_TENMH + 2, '-')
         << "+-------+-------+\n";
}

void inKhungDuoiDanhSachMonHoc() {
    cout << "+" << string(MAX_MAMH + 2, '-')
         << "+" << string(MAX_TENMH + 2, '-')
         << "+-------+-------+\n";
}

void inDongMonHoc(const MonHoc& mh) {
    cout << "| " << left << setw(MAX_MAMH) << mh.maMH
         << " | " << left << setw(MAX_TENMH) << mh.tenMH
         << " | " << right << setw(5) << mh.soTCLT
         << " | " << right << setw(5) << mh.soTCTH << " |\n";
}
// ĐỌC TỪNG TRƯỜNG DỮ LIỆU
bool docTruong(ifstream& file, string& value, char delimiter) {
    return static_cast<bool>(getline(file, value, delimiter));
}
// DÙNG ĐỌC FILE, CHUYỂN STRING THÀNH SỐ NGUYÊN 
bool chuyenSoNguyen(const string& value, int& result) {
    try {
        size_t consumed = 0;
        result = stoi(value, &consumed);
        return consumed == value.size();
    } catch (...) {
        return false;
    }
}
}

void khoiTaoDanhSachMonHoc(PTRMH& root) { root = nullptr; }

bool nhapMonHoc(MonHoc& mh) {
    cout << "Nhap ma mon hoc (chuoi rong de thoat): ";
    if (!nhapMa(mh.maMH, MAX_MAMH)) return false;

    cout << "Nhap ten mon hoc: ";
    nhapTen(mh.tenMH, MAX_TENMH);
    cout << "Nhap so tin chi ly thuyet: ";
    mh.soTCLT = nhapSoNguyenTrongKhoang(MIN_TCLT, MAX_TCLT);
    cout << "Nhap so tin chi thuc hanh: ";
    mh.soTCTH = nhapSoNguyenTrongKhoang(MIN_TCTH, MAX_TCTH);
    return true;
}

void nhapThongTinHieuChinh(MonHoc& mh) {
    cout << "Nhap ten mon hoc moi: ";
    nhapTen(mh.tenMH, MAX_TENMH);
    cout << "Nhap so tin chi ly thuyet moi: ";
    mh.soTCLT = nhapSoNguyenTrongKhoang(MIN_TCLT, MAX_TCLT);
    cout << "Nhap so tin chi thuc hanh moi: ";
    mh.soTCTH = nhapSoNguyenTrongKhoang(MIN_TCTH, MAX_TCTH);
}

PTRMH taoNodeMonHoc(const MonHoc& mh) {
    PTRMH node = new NodeMH{};
    node->mh = mh;
    return node;
}

PTRMH timMonHocTheoMa(PTRMH root, const char* maMH) {
    while (root != nullptr) {
        const int cmp = strcmp(maMH, root->mh.maMH);
        if (cmp == 0) return root;
        root = cmp < 0 ? root->left : root->right;
    }
    return nullptr;
}
bool themMonHocVaoDSMH(PTRMH& root, const MonHoc& mh) {
    PTRMH* link = &root;
    while (*link != nullptr) {
        const int cmp = strcmp(mh.maMH, (*link)->mh.maMH);
        if (cmp == 0) return false;
        link = cmp < 0 ? &(*link)->left : &(*link)->right;
    }
    *link = taoNodeMonHoc(mh);
    return true;
}
void themMonHoc(PTRMH& root) {
    while (true) {
        MonHoc mh{};
        if (!nhapMonHoc(mh)) break;
        if (themMonHocVaoDSMH(root, mh)) {
            cout << "Them mon hoc thanh cong!\n\n";
        } else {
            cout << "Ma mon hoc da ton tai! Vui long nhap ma khac!\n"
                 << "Them that bai!\n\n";
        }
    }
}

bool hieuChinhMonHoc(PTRMH root, const char* maMH) {
    PTRMH node = timMonHocTheoMa(root, maMH);
    if (node == nullptr) return false;
    nhapThongTinHieuChinh(node->mh);
    return true;
}
PTRMH timNodeNhoNhatBenPhai(PTRMH root) {
    if (root == nullptr) return nullptr;
    while (root->left != nullptr) root = root->left;
    return root;
}
bool xoaMonHoc(PTRMH& root, const char* maMH) {
    if (root == nullptr) return false;
    const int cmp = strcmp(maMH, root->mh.maMH);
    if (cmp < 0) return xoaMonHoc(root->left, maMH);
    if (cmp > 0) return xoaMonHoc(root->right, maMH);

    if (root->left == nullptr || root->right == nullptr) {
        PTRMH old = root;
        root = root->left != nullptr ? root->left : root->right;
        delete old;
        return true;
    }

    PTRMH successor = timNodeNhoNhatBenPhai(root->right);
    root->mh = successor->mh;
    return xoaMonHoc(root->right, successor->mh.maMH);
}

void inMonHoc(const MonHoc& mh) {
    cout << "Ma mon hoc          : " << mh.maMH << '\n'
         << "Ten mon hoc         : " << mh.tenMH << '\n'
         << "So tin chi ly thuyet: " << mh.soTCLT << '\n'
         << "So tin chi thuc hanh: " << mh.soTCTH << '\n';
}
void inDuLieuMonHocTheoMa(PTRMH root) {
    if (root == nullptr) return;
    inDuLieuMonHocTheoMa(root->left);
    inDongMonHoc(root->mh);
    inDuLieuMonHocTheoMa(root->right);
}
void inDanhSachMonHocTheoMa(PTRMH root) {
    if (root == nullptr) {
        cout << "Danh sach mon hoc rong!\n";
        return;
    }
    inTieuDeDanhSachMonHoc();
    inDuLieuMonHocTheoMa(root);
    inKhungDuoiDanhSachMonHoc();
}
void demSoMonHoc(PTRMH root, int& n) {
    if (root == nullptr) return;
    ++n;
    demSoMonHoc(root->left, n);
    demSoMonHoc(root->right, n);
}

void duaMonHocVaoMang(PTRMH root, MonHoc mh[], int& n) {
    if (root == nullptr) return;
    duaMonHocVaoMang(root->left, mh, n);
    mh[n++] = root->mh;
    duaMonHocVaoMang(root->right, mh, n);
}
bool monHocDungTruoc(const MonHoc& a, const MonHoc& b) {
    const int cmpTen = strcmp(a.tenMH, b.tenMH);
    return cmpTen < 0 || (cmpTen == 0 && strcmp(a.maMH, b.maMH) < 0);
}
void inDanhSachMonHocTheoTen(PTRMH root) {
    int soLuong = 0;
    demSoMonHoc(root, soLuong);
    if (soLuong == 0) {
        cout << "Danh sach mon hoc rong!\n";
        return;
    }

    MonHoc* monHocs = new MonHoc[soLuong];
    int n = 0;
    duaMonHocVaoMang(root, monHocs, n);
    for (int i = 0; i < soLuong - 1; ++i) {
        int viTri = i;
        for (int j = i + 1; j < soLuong; ++j)
            if (monHocDungTruoc(monHocs[j], monHocs[viTri])) viTri = j;
        if (viTri != i) swap(monHocs[i], monHocs[viTri]);
    }

    inTieuDeDanhSachMonHoc();
    for (int i = 0; i < soLuong; ++i) inDongMonHoc(monHocs[i]);
    inKhungDuoiDanhSachMonHoc();
    delete[] monHocs;
}

void giaiPhongMonHoc(PTRMH& root) {
    if (root == nullptr) return;
    giaiPhongMonHoc(root->left);
    giaiPhongMonHoc(root->right);
    delete root;
    root = nullptr;
}
void ghiDuLieuMonHoc(PTRMH root, ofstream& file) {
    if (root == nullptr || !file) return;
    ghiDuLieuMonHoc(root->left, file);
    file << root->mh.maMH << '|'
         << root->mh.tenMH << '|'
         << root->mh.soTCLT << '|'
         << root->mh.soTCTH << '\n';
    ghiDuLieuMonHoc(root->right, file);
}
bool ghiFileMonHoc(PTRMH root) {
    ofstream file(DUONG_DAN_FILE_MON_HOC);
    if (!file) return false;
    ghiDuLieuMonHoc(root, file);
    file.close();
    return file.good();
}

KetQuaDocMonHoc docFileMonHoc(PTRMH& root) {
    namespace fs = std::filesystem;
    std::error_code errorCode;
    const bool fileTonTai = fs::exists(DUONG_DAN_FILE_MON_HOC, errorCode);
    if (errorCode) return KetQuaDocMonHoc::KhongMoDuocFile;
    if (!fileTonTai) return KetQuaDocMonHoc::KhongTimThayFile;

    ifstream file(DUONG_DAN_FILE_MON_HOC);
    if (!file) return KetQuaDocMonHoc::KhongMoDuocFile;

    PTRMH rootTam = nullptr;
    auto thatBai = [&](KetQuaDocMonHoc ketQua) {
        giaiPhongMonHoc(rootTam);
        return ketQua;
    };

    string ma, ten, tcltText, tcthText;
    while (docTruong(file, ma, '|')) {
        if (!docTruong(file, ten, '|') || !docTruong(file, tcltText, '|') ||
            !static_cast<bool>(getline(file, tcthText))) {
            return thatBai(KetQuaDocMonHoc::SaiDinhDang);
        }

        int tclt = 0;
        int tcth = 0;
        if (ma.size() >= MAX_MAMH || ten.size() >= MAX_TENMH ||
            !laMaHopLe(ma) || !laTenHopLe(ten) ||
            !chuyenSoNguyen(tcltText, tclt) || !chuyenSoNguyen(tcthText, tcth)) {
            return thatBai(KetQuaDocMonHoc::SaiDinhDang);
        }
        if (tclt < MIN_TCLT || tclt > MAX_TCLT ||
            tcth < MIN_TCTH || tcth > MAX_TCTH) {
            return thatBai(KetQuaDocMonHoc::TinChiNgoaiKhoang);
        }

        MonHoc mh{};
        memcpy(mh.maMH, ma.c_str(), ma.size() + 1);
        chuanHoaMa(mh.maMH);
        memcpy(mh.tenMH, ten.c_str(), ten.size() + 1);
        mh.soTCLT = tclt;
        mh.soTCTH = tcth;
        if (!themMonHocVaoDSMH(rootTam, mh)) {
            return thatBai(KetQuaDocMonHoc::MaMonHocTrung);
        }
    }

    if (file.bad() || !file.eof()) {
        return thatBai(KetQuaDocMonHoc::LoiDocFile);
    }

    giaiPhongMonHoc(root);
    root = rootTam;
    return KetQuaDocMonHoc::ThanhCong;
}