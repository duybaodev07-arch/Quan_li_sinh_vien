#include "model/MonHoc.h"

#include <cstring>

MonHoc taoMonHoc(const std::string& maMH, const std::string& tenMH, int soTCLT, int soTCTH) {
    MonHoc monHoc{};
    std::strncpy(monHoc.maMH, maMH.c_str(), MAX_MAMH - 1);
    std::strncpy(monHoc.tenMH, tenMH.c_str(), MAX_TENMH - 1);
    monHoc.soTCLT = soTCLT;
    monHoc.soTCTH = soTCTH;
    return monHoc;
}

NodeMH* timMonHoc(PTRMH root, const char* maMH) {
    NodeMH* nut = root;
    while (nut != nullptr) {
        int ketQua = std::strcmp(maMH, nut->mh.maMH);
        if (ketQua == 0) return nut;
        nut = ketQua < 0 ? nut->left : nut->right;
    }
    return nullptr;
}

bool themMonHoc(PTRMH& root, const MonHoc& monHoc) {
    if (root == nullptr) {
        root = new NodeMH{monHoc, nullptr, nullptr};
        return true;
    }
    int ketQua = std::strcmp(monHoc.maMH, root->mh.maMH);
    if (ketQua == 0) return false;
    return ketQua < 0 ? themMonHoc(root->left, monHoc) : themMonHoc(root->right, monHoc);
}

void timNutTheMang(PTRMH& nutCanXoa, PTRMH& nutThayThe) {
    if (nutThayThe->left != nullptr) {
        timNutTheMang(nutCanXoa, nutThayThe->left);
    } else {
        nutCanXoa->mh = nutThayThe->mh;
        nutCanXoa = nutThayThe;
        nutThayThe = nutThayThe->right;
    }
}

bool xoaMonHoc(PTRMH& root, const char* maMH) {
    if (root == nullptr) return false;
    int ketQua = std::strcmp(maMH, root->mh.maMH);
    if (ketQua < 0) return xoaMonHoc(root->left, maMH);
    if (ketQua > 0) return xoaMonHoc(root->right, maMH);
    NodeMH* nutXoa = root;
    if (root->left == nullptr) root = root->right;
    else if (root->right == nullptr) root = root->left;
    else timNutTheMang(nutXoa, root->right);
    delete nutXoa;
    return true;
}

int demMonHoc(PTRMH root) {
    if (root == nullptr) return 0;
    return 1 + demMonHoc(root->left) + demMonHoc(root->right);
}

void bstSangMang(PTRMH root, MonHoc* mangMonHoc, int& viTri) {
    if (root == nullptr) return;
    bstSangMang(root->left, mangMonHoc, viTri);
    mangMonHoc[viTri++] = root->mh;
    bstSangMang(root->right, mangMonHoc, viTri);
}

void sapXepMonHocTheoTen(MonHoc* mangMonHoc, int soLuong) {
    for (int i = 0; i < soLuong - 1; ++i) {
        for (int j = i + 1; j < soLuong; ++j) {
            int ketQua = _stricmp(mangMonHoc[i].tenMH, mangMonHoc[j].tenMH);
            if (ketQua > 0 || (ketQua == 0 && std::strcmp(mangMonHoc[i].maMH, mangMonHoc[j].maMH) > 0)) {
                MonHoc monHocTam = mangMonHoc[i];
                mangMonHoc[i] = mangMonHoc[j];
                mangMonHoc[j] = monHocTam;
            }
        }
    }
}

void giaiPhongBST(PTRMH& root) {
    if (root == nullptr) return;
    giaiPhongBST(root->left);
    giaiPhongBST(root->right);
    delete root;
    root = nullptr;
}