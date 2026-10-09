#include "MonHoc.h"
#include "Input.h"
#include<cstring>
#include<iostream>
#include<iomanip>
#include<limits>
#include<fstream>
using namespace std;

void khoiTaoDanhSachMonHoc(PTRMH& root) {
    root = nullptr;
}
bool nhapMonHoc(MonHoc& mh) {
    cout << "Nhap ma mon hoc (chuoi rong de thoat): ";

    if (!nhapMa(mh.maMH, MAX_MAMH)) {
        return false;   
    }

    cout << "Nhap ten mon hoc: ";
    nhapChuoi(mh.tenMH, MAX_TENMH);

    cout << "Nhap so tin chi ly thuyet: ";
    mh.soTCLT = nhapSoNguyenTrongKhoang(MIN_TCLT, MAX_TCLT);

    cout << "Nhap so tin chi thuc hanh: ";
    mh.soTCTH = nhapSoNguyenTrongKhoang(MIN_TCTH, MAX_TCTH);

    return true;
}
void nhapThongTinHieuChinh(MonHoc& mh) { 
// hieu chinh, ko sua maMH
    cout << "Nhap ten mon hoc moi: ";
    nhapChuoi(mh.tenMH, MAX_TENMH);
    cout << "Nhap so tin chi ly thuyet moi: ";
    mh.soTCLT = nhapSoNguyenTrongKhoang(MIN_TCLT, MAX_TCLT);

    cout << "Nhap so tin chi thuc hanh moi: ";
    mh.soTCTH = nhapSoNguyenTrongKhoang(MIN_TCTH, MAX_TCTH);

}
PTRMH taoNodeMonHoc(const MonHoc& mh) {
    PTRMH p = new NodeMH;
    p->mh = mh;
    return p;
}
PTRMH timMonHocTheoMa(PTRMH root, const char* maMH) {
    while (root != nullptr) {
        int ketQua = strcmp(maMH, root->mh.maMH);

        if (ketQua == 0)
            return root;

        if (ketQua < 0)
            root = root->left;
        else
            root = root->right;
    }

    return nullptr;
}
bool themMonHocVaoDSMH(PTRMH& root, const MonHoc& mh) {
    if (root == nullptr) { // lop rỗng, thêm thẳng
        root = taoNodeMonHoc(mh);
        return true;
    }

    PTRMH cur = root;

    while (true) {
        int cmp = strcmp(mh.maMH, cur->mh.maMH); 

        if (cmp == 0) // mã đã tồn tại 
            return false;

        if (cmp < 0) { // mã nhỏ hơn đi sang trái
            if (cur->left == nullptr) {
                cur->left = taoNodeMonHoc(mh);
                return true;
            }
            cur = cur->left;
        }
        else { // mã lớn hơn đi sang phải
            if (cur->right == nullptr) {
                cur->right = taoNodeMonHoc(mh);
                return true;
            }
            cur = cur->right;
        }
    }
}
void themMonHoc(PTRMH& root) {
    
    while (true) {
        MonHoc mh;

        if (!nhapMonHoc(mh)) {
            break;
        }

        if (themMonHocVaoDSMH(root, mh)) {
            cout << "Them mon hoc thanh cong!\n\n";
        }
        else {
            cout << "Ma mon hoc da ton tai! Vui long nhap ma khac!\n";
            cout << "Them that bai!\n\n";
        }
    }
}
bool hieuChinhMonHoc(PTRMH root, const char* maMH) {
    PTRMH find = timMonHocTheoMa(root, maMH);

    if (find == nullptr) 
        return false;

    nhapThongTinHieuChinh(find-> mh);
    return true;
}
PTRMH timNodeNhoNhatBenPhai(PTRMH root) {
    // Tim node nho nhat ben phai
    PTRMH p = root;

    while (p != nullptr && p->left != nullptr) {
        p = p->left;
    }

    return p;
}
bool xoaMonHoc(PTRMH& root, const char* maMH) {
    if (root == nullptr) { // ko tìm thấy, check rỗng ở menu nha
        return false;
    }

    int ketQua = strcmp(maMH, root->mh.maMH);

    if (ketQua < 0) 
        return xoaMonHoc(root->left, maMH);
    
    if (ketQua > 0) 
        return xoaMonHoc(root->right, maMH);
    // Tìm thấy node cần xóa

    // Trường hợp 1: không có con
    if (root->left == nullptr && root->right == nullptr) {
        delete root;
        root = nullptr;
        return true;
    }
    // Trường hợp 2: chỉ có con phải
    if (root->left == nullptr) {
        PTRMH temp = root;
        root = root->right;
        delete temp;
        return true;
    }
    // Trường hợp 3: chỉ có con trái
    if (root->right == nullptr) {
        PTRMH temp = root;
        root = root->left;
        delete temp;
        return true;
    }

    // Trường hợp 4: có cả hai con, lấy min right, copy sv lên node cần xóa
    PTRMH temp = timNodeNhoNhatBenPhai(root->right);
    root->mh = temp->mh;
    return xoaMonHoc(root->right, temp->mh.maMH); // đệ quy xóa node min right
}
void inMonHoc(const MonHoc& mh) {
    cout << "Ma mon hoc          : " << mh.maMH << endl;
    cout << "Ten mon hoc         : " << mh.tenMH << endl;
    cout << "So tin chi ly thuyet: " << mh.soTCLT << endl;
    cout << "So tin chi thuc hanh: " << mh.soTCTH << endl;
}
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
void inDuLieuMonHocTheoMa(PTRMH root) {
    if (root == nullptr) {
        return;
    }

    inDuLieuMonHocTheoMa(root->left);

    cout << "| "
         << left << setw(MAX_MAMH) << root->mh.maMH
         << " | "
         << left << setw(MAX_TENMH) << root->mh.tenMH
         << " | "
         << right << setw(5) << root->mh.soTCLT
         << " | "
         << right << setw(5) << root->mh.soTCTH
         << " |\n";

    inDuLieuMonHocTheoMa(root->right);
}
void inDanhSachMonHocTheoMa(PTRMH root) {
    if (root == nullptr) {
        cout << "Danh sach mon hoc rong!\n";
        return;
    }

    inTieuDeDanhSachMonHoc();
    inDuLieuMonHocTheoMa(root);

    cout << "+" << string(MAX_MAMH + 2, '-')
        << "+" << string(MAX_TENMH + 2, '-')
        << "+-------+-------+\n";
}
void demSoMonHoc(PTRMH root, int& n) {
    if (root == nullptr) {
        return;
    }
    n++;
    demSoMonHoc(root->left, n);
    demSoMonHoc(root->right, n);
}
void duaMonHocVaoMang(PTRMH root, MonHoc mh[], int& n) {
    if (root == nullptr) {
        return;
    }
    duaMonHocVaoMang(root->left, mh, n);
    mh[n] = root->mh;
    n++;
    duaMonHocVaoMang(root->right, mh, n);
}
bool monHocDungTruoc(const MonHoc& a, const MonHoc& b) {
    int ketQua = strcmp(a.tenMH, b.tenMH);
    if (ketQua < 0) 
        return true;
    
    if (ketQua > 0) 
        return false;

    return strcmp(a.maMH, b.maMH) < 0;
}
void inDanhSachMonHocTheoTen(PTRMH root) {
    if (root == nullptr) {
        cout << "Danh sach rong!\n";
        return;
    }
    int soLuong = 0;
    demSoMonHoc(root, soLuong);

    MonHoc* pMH = new MonHoc[soLuong]; // mang con tro monhoc

    int n = 0;
    duaMonHocVaoMang(root, pMH, n);

    for (int i = 0; i < soLuong - 1; i++) {
        int viTri = i;
        for (int j = i + 1; j < soLuong; j++) {
            if (monHocDungTruoc(pMH[j], pMH[viTri])) {
                viTri = j;
            }
        }
        if (viTri != i) {
            MonHoc temp = pMH[i];
            pMH[i] = pMH[viTri];
            pMH[viTri] = temp;
        }
    }
    inTieuDeDanhSachMonHoc();
    for (int i = 0; i < soLuong; i++) {
        cout << "| "
             << left << setw(MAX_MAMH) << pMH[i].maMH
             << " | "
             << left << setw(MAX_TENMH) << pMH[i].tenMH
             << " | "
             << right << setw(5) << pMH[i].soTCLT
             << " | "
             << right << setw(5) << pMH[i].soTCTH
             << " |\n";
    }
    cout << "+" << string(MAX_MAMH + 2, '-')
        << "+" << string(MAX_TENMH + 2, '-')
        << "+-------+-------+\n";

    delete[] pMH;
}
void ghiDuLieuMonHoc(PTRMH root, ofstream& file) {
    if (root == nullptr) {
        return;
    }

    ghiDuLieuMonHoc(root->left, file);

    file << root->mh.maMH << '|'
         << root->mh.tenMH << '|'
         << root->mh.soTCLT << '|'
         << root->mh.soTCTH << '\n';

    ghiDuLieuMonHoc(root->right, file);
}
bool ghiFileMonHoc(PTRMH root) {
    ofstream file("sv/MONHOC.txt");

    if (!file) {
        return false;
    }

    ghiDuLieuMonHoc(root, file);

    file.close();
    return true;
}
void giaiPhongMonHoc(PTRMH& root) {
    if(root == nullptr) 
        return;
    giaiPhongMonHoc(root->left);
    giaiPhongMonHoc(root->right);
    delete root;
    root = nullptr;
}
bool docFileMonHoc(PTRMH& root) {

    ifstream file("sv/MONHOC.txt");

    if (!file) {
        return false;
    }
    giaiPhongMonHoc(root);
    MonHoc mh;

    while (file.getline(mh.maMH, MAX_MAMH, '|')) {
        file.getline(mh.tenMH, MAX_TENMH, '|');
        file >> mh.soTCLT;
        file.ignore(1, '|');
        file >> mh.soTCTH;
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        themMonHocVaoDSMH(root, mh);
    }

    file.close();
    return true;
}
