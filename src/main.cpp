#include "Menu.h"

#include <exception>
#include <iostream>
#include <filesystem>
#include <system_error>

int main() {
    PTRMH root = nullptr;
    static DanhSachLop dsLop{};
    static DanhSachLopTinChi dsLopTC{};
    bool choPhepGhiMonHoc = true;
    bool choPhepGhiLopTinChi = true;
    bool chayMenu = true;

    try {
        switch (docFileMonHoc(root)) {
        case KetQuaDocMonHoc::ThanhCong:
            break;
        case KetQuaDocMonHoc::KhongTimThayFile:
            std::cerr << "Chua co data/MONHOC.txt; bat dau voi danh sach mon hoc rong.\n";
            break;
        case KetQuaDocMonHoc::KhongMoDuocFile:
            std::cerr << "Khong mo duoc data/MONHOC.txt; bo qua menu de bao toan file.\n";
            choPhepGhiMonHoc = false;
            chayMenu = false;
            break;
        case KetQuaDocMonHoc::SaiDinhDang:
            std::cerr << "data/MONHOC.txt sai dinh dang; bo qua menu va giu nguyen file.\n";
            choPhepGhiMonHoc = false;
            chayMenu = false;
            break;
        case KetQuaDocMonHoc::MaMonHocTrung:
            std::cerr << "data/MONHOC.txt co ma mon hoc bi trung; bo qua menu va giu nguyen file.\n";
            choPhepGhiMonHoc = false;
            chayMenu = false;
            break;
        case KetQuaDocMonHoc::TinChiNgoaiKhoang:
            std::cerr << "Tin chi trong data/MONHOC.txt nam ngoai gioi han; bo qua menu va giu nguyen file.\n";
            choPhepGhiMonHoc = false;
            chayMenu = false;
            break;
        case KetQuaDocMonHoc::LoiDocFile:
            std::cerr << "Loi khi doc data/MONHOC.txt; bo qua menu va giu nguyen file.\n";
            choPhepGhiMonHoc = false;
            chayMenu = false;
            break;
        }
        if (!docFileLop(dsLop)) std::cerr << "Khong doc duoc data/LOP.txt; bat dau voi danh sach lop rong.\n";
        std::error_code loiKiemTraFileLopTC;
        const bool coFileLopTC = std::filesystem::exists("data/LOPTC.txt", loiKiemTraFileLopTC);
        if (loiKiemTraFileLopTC) {
            std::cerr << "Khong kiem tra duoc data/LOPTC.txt; bo qua menu va giu nguyen file.\n";
            choPhepGhiLopTinChi = false;
            chayMenu = false;
        } else if (!coFileLopTC) {
            std::cerr << "Chua co data/LOPTC.txt; bat dau voi danh sach lop tin chi rong.\n";
        } else if (!docFileLopTinChi(dsLopTC, root)) {
            std::cerr << "data/LOPTC.txt loi hoac sai dinh dang; bo qua menu va giu nguyen file.\n";
            choPhepGhiLopTinChi = false;
            chayMenu = false;
        }
        if (chayMenu) menuChinh(root, dsLop, dsLopTC);
    } catch (const std::exception& e) {
        // Cac ham nhap nem exception khi cin gap EOF (vi du Ctrl+Z, Enter).
        std::cerr << "\nLoi: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "\nLoi khong xac dinh trong chuong trinh.\n";
    }

    try {
        bool saved = true;
        if (choPhepGhiMonHoc) {
            if (!ghiFileMonHoc(root)) saved = false;
        } else {
            std::cerr << "Bo qua ghi MONHOC.txt de khong ghi de file loi.\n";
        }
        if (!ghiFileLop(dsLop)) saved = false;
        if (choPhepGhiLopTinChi) {
            if (!ghiFileLopTinChi(dsLopTC)) saved = false;
        } else {
            std::cerr << "Bo qua ghi LOPTC de khong ghi de file loi.\n";
        }
        if (!saved) std::cerr << "Canh bao: khong the luu day du du lieu vao thu muc data.\n";
    } catch (const std::exception& e) {
        std::cerr << "Canh bao: loi khi luu du lieu: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Canh bao: loi khong xac dinh khi luu du lieu.\n";
    }
    giaiPhongMonHoc(root);
    xoaTatCaLop(dsLop);
    xoaTatCaLopTinChi(dsLopTC);

    return 0;
}


