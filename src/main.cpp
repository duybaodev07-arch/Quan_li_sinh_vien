#include <ui/GiaoDienConsole.h>
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"
#include "model/LichSuThaoTac.h"
#include "storage/DocGhiDuLieu.h"
#include "ui/Menu.h"
#include <string>

int main() {
    GiaoDienConsole::khoiTaoConsole("QUAN LI SINH VIEN HE TIN CHI");

    PTRMH root = nullptr;
    static DanhSachLop danhSachLop;
    static DanhSachLopTinChi danhSachLopTinChi;

    std::string thongBaoLoi;
    while (!docTatCaDuLieu(root, danhSachLop, danhSachLopTinChi, thongBaoLoi)) {
        bool thuDocLai = GiaoDienConsole::hopXacNhan(
            "KHÔNG ĐỌC ĐƯỢC DỮ LIỆU",
            thongBaoLoi,
            "Đồng ý: thử lại | Hủy bỏ: vào menu rỗng");
        if (!thuDocLai) break;
    }
    QuanLyLichSu lichSu;
    chayMenu(root, danhSachLop, danhSachLopTinChi, lichSu);
    LichSuThaoTac::giaiPhongLichSu(lichSu);

    giaiPhongBST(root);
    giaiPhongDanhSachLop(danhSachLop);
    giaiPhongDanhSachLopTinChi(danhSachLopTinChi);
    return 0;
}

