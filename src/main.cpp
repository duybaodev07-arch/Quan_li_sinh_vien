#include "Menu.h"

#include <exception>
#include <iostream>

int main() {
    PTRMH root = nullptr;
    static DanhSachLop dsLop{};
    static DanhSachLopTinChi dsLopTC{};

    try {
        if (!docFileMonHoc(root)) std::cerr << "Khong doc duoc data/MONHOC.txt; bat dau voi danh sach mon hoc rong.\n";
        if (!docFileLop(dsLop)) std::cerr << "Khong doc duoc data/LOP.txt; bat dau voi danh sach lop rong.\n";
        if (!docFileLopTinChi(dsLopTC)) std::cerr << "Khong doc duoc data/LOPTC.txt; bat dau voi danh sach lop tin chi rong.\n";
        menuChinh(root, dsLop, dsLopTC);
    } catch (const std::exception& e) {
        // Cac ham nhap nem exception khi cin gap EOF (vi du Ctrl+Z, Enter).
        std::cerr << "\n" << e.what() << '\n';
    }

    const bool saved = ghiFileMonHoc(root) && ghiFileLop(dsLop) && ghiFileLopTinChi(dsLopTC);
    if (!saved) std::cerr << "Canh bao: khong the luu day du du lieu vao thu muc data.\n";
    giaiPhongMonHoc(root);
    xoaTatCaLop(dsLop);
    xoaTatCaLopTinChi(dsLopTC);

    return 0;
}
