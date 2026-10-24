#include "ui/GiaoDienConsole.h"
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"
#include "storage/DocGhiDuLieu.h"
#include "ui/Menu.h"

int main() {
    GiaoDienConsole::khoiTaoConsole("QUAN LI SINH VIEN HE TIN CHI");

    PTRMH cayMonHoc = nullptr;
    static DanhSachLop danhSachLop;
    static DanhSachLopTinChi danhSachLopTinChi;

    docTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
    chayMenu(cayMonHoc, danhSachLop, danhSachLopTinChi);

    giaiPhongBST(cayMonHoc);
    giaiPhongDanhSachLop(danhSachLop);
    giaiPhongDanhSachLopTinChi(danhSachLopTinChi);
    return 0;
}

