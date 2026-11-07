#include "ui/Menu.h"
#include "ui/GiaoDienConsole.h"
#include "storage/DocGhiDuLieu.h"
#include "features/QuanLyLopSinhVien.h"
#include "features/QuanLyMonHocLopTinChi.h"
#include "features/QuanLyDiemBaoCao.h"

#include <iostream>
#include <string>

using namespace std;

void chayMenu(PTRMH& cayMonHoc, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi, QuanLyLichSu& lichSu) {
    // Khởi tạo Menu chính bằng MẢNG ĐỘNG
    const int SO_MUC_MENU = 13;
    string* tieuDeMucMenu = new string[SO_MUC_MENU];

    tieuDeMucMenu[0]  = "a. Mở lớp tín chỉ (Thêm / Xóa / Hiệu chỉnh)";
    tieuDeMucMenu[1]  = "b. In DSSV đã đăng ký lớp tín chỉ";
    tieuDeMucMenu[2]  = "c. Nhập sinh viên của 1 lớp (Thêm / Xóa / Hiệu chỉnh)";
    tieuDeMucMenu[3]  = "d. In DSSV của 1 lớp (Tăng dần theo Mã SV)";
    tieuDeMucMenu[4]  = "e. Nhập môn học (Thêm / Xóa / Hiệu chỉnh)";
    tieuDeMucMenu[5]  = "f. In danh sách môn học (Tăng dần theo Tên MH)";
    tieuDeMucMenu[6]  = "g. Đăng ký lớp tín chỉ";
    tieuDeMucMenu[7]  = "h. Hủy lớp tín chỉ";
    tieuDeMucMenu[8]  = "i. Nhập điểm";
    tieuDeMucMenu[9]  = "j. In bảng điểm môn học của 1 lớp tín chỉ";
    tieuDeMucMenu[10] = "k. In điểm TB kết thúc khóa học theo tín chỉ";
    tieuDeMucMenu[11] = "l. In bảng điểm tổng kết các môn của 1 lớp";
    tieuDeMucMenu[12] = "0. Lưu dữ liệu và Thoát chương trình";

    // Mảng động các vùng nút bấm của Menu chính (Mỗi mục cao 1 dòng từ y=5 đến y=17)
    NutChuNhat* vungNutMenu = new NutChuNhat[SO_MUC_MENU];
    for (int i = 0; i < SO_MUC_MENU; ++i) {
        vungNutMenu[i] = { 3, 5 + i, 58, 1, i, tieuDeMucMenu[i] };
    }

    // Hàm chỉ vẽ đúng 1 dòng mục menu (chống nhấp nháy tuyệt đối)
    auto veMotMucMenu = [&](int i, bool dangDuocChon) {
        if (i < 0 || i >= SO_MUC_MENU) return;
        GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
        GiaoDienConsole::diDenToaDo(vungNutMenu[i].x, vungNutMenu[i].y);
        string tienTo = dangDuocChon ? " >> " : "    ";
        cout << GiaoDienConsole::canTrai(tienTo + vungNutMenu[i].label, vungNutMenu[i].w);
    };

    int mucDangChon = 0;
    bool dangChay = true;

    while (dangChay) {
        // Vẽ khung tĩnh của màn hình chính đúng 1 lần khi vào Menu
        GiaoDienConsole::xoaManHinh();
        GiaoDienConsole::veTieuDe();

        // Khung Menu bên trái
        GiaoDienConsole::veKhung(1, 2, 62, 24, XANH_NGOC_NHAT, DEN, "DANH MỤC CHỨC NĂNG");

        for (int i = 0; i < SO_MUC_MENU; ++i) {
            veMotMucMenu(i, i == mucDangChon);
        }

        // Khung Dữ liệu hiện tại của hệ thống bên phải
        GiaoDienConsole::veKhung(64, 2, 51, 24, XANH_NGOC_NHAT, DEN, "Dữ liệu hiện tại của hệ thống");
        int tongMH = demMonHoc(cayMonHoc);
        int tongSV = 0;
        for (int i = 0; i < danhSachLop.soLuong; ++i) tongSV += demSVTrongLop(danhSachLop.lop[i].dssv);
        int ltcDangMo = 0;
        for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) if (!danhSachLopTinChi.lopTC[i]->huyLop) ltcDangMo++;

        GiaoDienConsole::datMau(TRANG, DEN);
        GiaoDienConsole::diDenToaDo(67, 6);  cout << "- Số lượng Môn học     : "; GiaoDienConsole::datMau(VANG, DEN); cout << tongMH << " môn";
        GiaoDienConsole::datMau(TRANG, DEN);
        GiaoDienConsole::diDenToaDo(67, 9);  cout << "- Số lượng Lớp học     : "; GiaoDienConsole::datMau(VANG, DEN); cout << danhSachLop.soLuong << " lớp";
        GiaoDienConsole::datMau(TRANG, DEN);
        GiaoDienConsole::diDenToaDo(67, 12); cout << "- Tổng số Sinh viên    : "; GiaoDienConsole::datMau(VANG, DEN); cout << tongSV << " sinh viên";
        GiaoDienConsole::datMau(TRANG, DEN);
        GiaoDienConsole::diDenToaDo(67, 15); cout << "- Tổng số Lớp tín chỉ  : "; GiaoDienConsole::datMau(VANG, DEN); cout << danhSachLopTinChi.soLuong << " lớp (" << ltcDangMo << " đang mở)";

        GiaoDienConsole::diDenToaDo(4, 19);
        GiaoDienConsole::datMau(VANG, DEN);
        cout << "Lưu ý: Tìm và nhập tiếng Việt không dấu.";

        GiaoDienConsole::veThanhTrangThai("Chuột: Di chuyển & Click trái | Bàn phím: Mũi tên Lên/Xuống, Enter, ESC, phím a-l, 0: thoát");

        int mucChonTruoc = mucDangChon;
        bool canVeLaiToanBo = false;

        // Vòng lặp nhận sự kiện: CHỈ vẽ lại đúng 2 dòng khi đổi mục chọn
        while (!canVeLaiToanBo) {
            if (mucDangChon != mucChonTruoc) {
                veMotMucMenu(mucChonTruoc, false);
                veMotMucMenu(mucDangChon, true);
                mucChonTruoc = mucDangChon;
            }

            SuKienNhap suKien = GiaoDienConsole::docSuKien();
            int luaChonKichHoat = -1;

            if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
                mucDangChon = (mucDangChon - 1 + SO_MUC_MENU) % SO_MUC_MENU;
            } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
                mucDangChon = (mucDangChon + 1) % SO_MUC_MENU;
            } else if (suKien.type == EVT_KEY_HOME) {
                mucDangChon = 0;
            } else if (suKien.type == EVT_KEY_END) {
                mucDangChon = SO_MUC_MENU - 1;
            } else if (suKien.type == SK_PHIM_ENTER) {
                luaChonKichHoat = mucDangChon;
            } else if (suKien.type == SK_PHIM_ESC) {
                luaChonKichHoat = 12;
            } else if (suKien.type == SK_KY_TU) {
                char c = suKien.ch;
                if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
                if (c >= 'a' && c <= 'l') {
                    mucDangChon = c - 'a';
                    luaChonKichHoat = mucDangChon;
                } else if (c == '0') {
                    mucDangChon = 12;
                    luaChonKichHoat = 12;
                }
            } else if (suKien.type == SK_CHUOT_DI_CHUYEN) {
                for (int i = 0; i < SO_MUC_MENU; ++i) {
                    if (vungNutMenu[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                        mucDangChon = i;
                        break;
                    }
                }
            } else if (suKien.type == SK_CHUOT_TRAI) {
                for (int i = 0; i < SO_MUC_MENU; ++i) {
                    if (vungNutMenu[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                        mucDangChon = i;
                        luaChonKichHoat = i;
                        break;
                    }
                }
            }

            if (luaChonKichHoat != -1) {
                switch (luaChonKichHoat) {
                    case 0:  chucNangA_MoLopTinChi(cayMonHoc, danhSachLopTinChi, danhSachLop, lichSu); break;
                    case 1:  chucNangB_InDSSVDangKyLopTC(cayMonHoc, danhSachLopTinChi, danhSachLop); break;
                    case 2:  chucNangC_CapNhatSinhVienLop(cayMonHoc, danhSachLop, danhSachLopTinChi, lichSu); break;
                    case 3:  chucNangD_InDSSVTheoMaSV(danhSachLop); break;
                    case 4:  chucNangE_CapNhatMonHoc(cayMonHoc, danhSachLop, danhSachLopTinChi, lichSu); break;
                    case 5:  chucNangF_InDSMonHocTheoTen(cayMonHoc); break;
                    case 6:  chucNangG_DangKyLopTinChi(cayMonHoc, danhSachLop, danhSachLopTinChi); break;
                    case 7:  chucNangH_HuyLopTinChi(cayMonHoc, danhSachLop, danhSachLopTinChi); break;
                    case 8:  chucNangI_NhapDiem(cayMonHoc, danhSachLop, danhSachLopTinChi); break;
                    case 9:  chucNangJ_InBangDiemMonHoc(cayMonHoc, danhSachLop, danhSachLopTinChi); break;
                    case 10: chucNangK_InDiemTrungBinhKhoaHoc(cayMonHoc, danhSachLop, danhSachLopTinChi); break;
                    case 11: chucNangL_BangDiemTongKetLop(cayMonHoc, danhSachLop, danhSachLopTinChi); break;
                    case 12:
                        if (GiaoDienConsole::hopXacNhan("XÁC NHẬN THOÁT CHƯƠNG TRÌNH",
                                                     "Bạn có muốn lưu toàn bộ dữ liệu và thoát chương trình không?")) {
                            luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                            dangChay = false;
                        }
                        break;
                }
                canVeLaiToanBo = true;
            }
        }
    }

    delete[] vungNutMenu;
    delete[] tieuDeMucMenu;
    GiaoDienConsole::xoaManHinh();
    GiaoDienConsole::hienConTro(true);
}

