#include "ui/GiaoDienConsole.h"
#include "model/CauTrucDuLieu.h"
#include "storage/DocGhiDuLieu.h"
#include "ui/Menu.h"

int main() {
    GiaoDienConsole::khoiTaoConsole("QUAN LI SINH VIEN HE TIN CHI");

    PTRMH cayMonHoc = nullptr;
    DSLop danhSachLop;
    DSLopTinChi danhSachLopTinChi;

    docTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
    chayMenu(cayMonHoc, danhSachLop, danhSachLopTinChi);

    giaiPhongBST(cayMonHoc);
    return 0;
}

