#include "ui/GiaoDienConsole.h"
#include "features/QuanLyLopSinhVien.h"
#include "model/LichSuThaoTac.h"
#include "features/TienIchChung.h"

#include "storage/DocGhiDuLieu.h"
#include "model/SoSanhChuoi.h"


#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

void chucNangC_CapNhatSinhVienLop(PTRMH& root, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi, QuanLyLichSu& lichSu) {
    while (true) {
        string maLop = chonLopNhanh(danhSachLop, true, false, root, &danhSachLopTinChi, &lichSu);
        if (maLop.empty()) break;

        int viTriLop = timLop(danhSachLop, maLop.c_str());
        if (viTriLop == -1) continue;
        Lop& lop = danhSachLop.lop[viTriLop];

        int mucDangChon = 0;
        string tuKhoaMaSV;
        string tuKhoaHoTen;
        string tuKhoaSoDienThoai;
        string tuKhoaPhai;
        int oLocDangChon = 0;
        size_t viTriConTroOLoc[4] = {};
        int soDongMoiTrang = 14;
        string thongBao = "";
        bool coLoi = false;
        bool canVeLaiToanBo = true;
        bool canVeLaiCacOLoc = true;
        bool canVeLaiBang = true;
        bool canVeLaiThanhTrangThai = true;
        int mucChonTruoc = -1;
        int trangTruoc = -1;
        NodeSV* cacDongDaVe[14] = {};
        bool cacDongDaChonDaVe[14] = {};
        bool daVeThongBaoKhongKetQua = false;
        int nutDangTro = -1;
        int nutTruocDo = -1;

        int soNut = 3;
        NutChuNhat* cacNut = new NutChuNhat[soNut];
        cacNut[0] = { 6,  23, 28, 1, 0, "[ + NHẬP SV MỚI (Ins) ]" };
        cacNut[1] = { 53, 23, 24, 1, 4, "[ F3: HOÀN TÁC ]" };
        cacNut[2] = { 90, 23, 20, 1, 3, "[ ĐỔI LỚP (ESC) ]" };

        auto veCacOLoc = [&]() {
            const int toaDoX[] = {4, 32, 64, 91};
            const int chieuRong[] = {25, 29, 25, 21};
            const string nhan[] = {"Mã SV: ", "Họ/Tên: ", "SĐT: ", "Phái: "};
            const string* giaTri[] = {&tuKhoaMaSV, &tuKhoaHoTen, &tuKhoaSoDienThoai, &tuKhoaPhai};
            for (int i = 0; i < 4; ++i) {
                GiaoDienConsole::diDenToaDo(toaDoX[i], 4);
                GiaoDienConsole::datMau(oLocDangChon == i ? DEN : TRANG, oLocDangChon == i ? VANG : XANH_NGOC);
                int doRongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
                size_t soKyTuHienThi = static_cast<size_t>(max(0, chieuRong[i] - doRongNhan));
                size_t dauHienThi = viTriConTroOLoc[i] >= soKyTuHienThi && soKyTuHienThi > 0
                    ? viTriConTroOLoc[i] - soKyTuHienThi + 1 : 0;
                string noiDung = nhan[i] + giaTri[i]->substr(dauHienThi, soKyTuHienThi);
                int doDai = GiaoDienConsole::doDaiUtf8(noiDung);
                cout << noiDung << string(max(0, chieuRong[i] - doDai), ' ');
            }
        };

        auto datViTriConTroOLoc = [&]() {
            const int toaDoX[] = {4, 32, 64, 91};
            const int chieuRong[] = {25, 29, 25, 21};
            const string nhan[] = {"Mã SV: ", "Họ/Tên: ", "SĐT: ", "Phái: "};
            int doRongNhan = GiaoDienConsole::doDaiUtf8(nhan[oLocDangChon]);
            int soKyTuHienThi = max(0, chieuRong[oLocDangChon] - doRongNhan);
            size_t dauHienThi = viTriConTroOLoc[oLocDangChon] >= static_cast<size_t>(soKyTuHienThi) && soKyTuHienThi > 0
                ? viTriConTroOLoc[oLocDangChon] - static_cast<size_t>(soKyTuHienThi) + 1 : 0;
            int x = toaDoX[oLocDangChon] + doRongNhan
                + static_cast<int>(min(viTriConTroOLoc[oLocDangChon] - dauHienThi, static_cast<size_t>(soKyTuHienThi)));
            int cotCuoi = toaDoX[oLocDangChon] + chieuRong[oLocDangChon] - 1;
            if (x > cotCuoi) x = cotCuoi;
            GiaoDienConsole::diDenToaDo(x, 4);
            GiaoDienConsole::hienConTro(true);
        };

        while (true) {
            int soSinhVien = demSVTrongLop(lop.dssv);
            NodeSV** mangSinhVien = (soSinhVien > 0) ? new NodeSV*[soSinhVien] : nullptr;
            int idx = 0;
            for (PTRSV p = lop.dssv; p != nullptr; p = p->next) {
                string maSV = GiaoDienConsole::vietHoa(p->sv.maSV);
                string hoTen = GiaoDienConsole::vietHoa(string(p->sv.ho) + " " + p->sv.ten);
                string soDienThoai = p->sv.soDT;
                string phai = GiaoDienConsole::vietHoa(p->sv.phai);
                if (phai == "NỮ") phai = "NU";
                string phaiCanTim = GiaoDienConsole::vietHoa(tuKhoaPhai);
                if (phaiCanTim == "NỮ") phaiCanTim = "NU";
                if ((!tuKhoaMaSV.empty() && maSV.find(GiaoDienConsole::vietHoa(tuKhoaMaSV)) == string::npos) ||
                    (!tuKhoaHoTen.empty() && hoTen.find(GiaoDienConsole::vietHoa(tuKhoaHoTen)) == string::npos) ||
                    (!tuKhoaSoDienThoai.empty() && soDienThoai.find(tuKhoaSoDienThoai) == string::npos) ||
                    (!tuKhoaPhai.empty() && phai.find(phaiCanTim) == string::npos)) continue;
                mangSinhVien[idx++] = p;
            }
            soSinhVien = idx;

            if (mucDangChon >= soSinhVien) mucDangChon = (soSinhVien > 0) ? soSinhVien - 1 : 0;
            int tongSoTrang = (soSinhVien == 0) ? 1 : (soSinhVien + soDongMoiTrang - 1) / soDongMoiTrang;
            int trangHienTai = mucDangChon / soDongMoiTrang;
            int viTriDau = trangHienTai * soDongMoiTrang;
            int viTriCuoi = (viTriDau + soDongMoiTrang < soSinhVien) ? (viTriDau + soDongMoiTrang) : soSinhVien;

            if (trangHienTai != trangTruoc) {
                canVeLaiBang = true;
                canVeLaiThanhTrangThai = true;
                trangTruoc = trangHienTai;
            }

            auto veMotDong = [&](int i, bool dangDuocChon) {
                int r = i - viTriDau;
                if (r < 0 || r >= soDongMoiTrang) return;
                GiaoDienConsole::diDenToaDo(4, 7 + r);
                if (i >= 0 && i < soSinhVien) {
                    SinhVien& sv = mangSinhVien[i]->sv;
                    GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
                    cout << GiaoDienConsole::canTrai(" " + to_string(i + 1), 7)
                         << GiaoDienConsole::canTrai(sv.maSV, 18);
                    GiaoDienConsole::datMau(dangDuocChon ? DEN : XANH_NGOC, dangDuocChon ? VANG : DEN); cout << "|";
                    GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
                    cout << GiaoDienConsole::canTrai(sv.ho, 33);
                    GiaoDienConsole::datMau(dangDuocChon ? DEN : XANH_NGOC, dangDuocChon ? VANG : DEN); cout << "|";
                    GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
                    cout << GiaoDienConsole::canTrai(sv.ten, 15);
                    GiaoDienConsole::datMau(dangDuocChon ? DEN : XANH_NGOC, dangDuocChon ? VANG : DEN); cout << "|";
                    GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
                    cout << GiaoDienConsole::canTrai(sv.phai, 11);
                    GiaoDienConsole::datMau(dangDuocChon ? DEN : XANH_NGOC, dangDuocChon ? VANG : DEN); cout << "|";
                    GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
                    cout << GiaoDienConsole::canTrai(sv.soDT, 19);
                } else {
                    GiaoDienConsole::datMau(XAM_NHAT, DEN);
                    cout << string(108, ' ');
                }
            };

            if (canVeLaiToanBo) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, string("DANH SÁCH SINH VIÊN LỚP ") + lop.maLop);

                veCacOLoc();

                GiaoDienConsole::datMau(VANG, LAM);
                GiaoDienConsole::diDenToaDo(4, 5);
                cout << GiaoDienConsole::canTrai(" STT", 7)
                     << GiaoDienConsole::canTrai("MÃ SINH VIÊN", 18);
                GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
                GiaoDienConsole::datMau(VANG, LAM);
                cout << GiaoDienConsole::canTrai("HỌ VÀ TÊN ĐỆM", 33);
                GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
                GiaoDienConsole::datMau(VANG, LAM);
                cout << GiaoDienConsole::canTrai("TÊN", 15);
                GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
                GiaoDienConsole::datMau(VANG, LAM);
                cout << GiaoDienConsole::canTrai("PHÁI", 11);
                GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
                GiaoDienConsole::datMau(VANG, LAM);
                cout << GiaoDienConsole::canTrai("SỐ ĐIỆN THOẠI", 19);

                GiaoDienConsole::diDenToaDo(4, 6);
                GiaoDienConsole::datMau(XANH_NGOC_NHAT, DEN);
                cout << string(108, '-');

                for (int r = 0; r < soDongMoiTrang; ++r) {
                    int i = viTriDau + r;
                    veMotDong(i, i == mucDangChon);
                    cacDongDaVe[r] = i < soSinhVien ? mangSinhVien[i] : nullptr;
                    cacDongDaChonDaVe[r] = i < soSinhVien && i == mucDangChon;
                }
                if (soSinhVien == 0) {
                    GiaoDienConsole::diDenToaDo(4, 10);
                    GiaoDienConsole::datMau(DO_NHAT, DEN);
                    cout << GiaoDienConsole::canGiua("Không tìm thấy sinh viên phù hợp với điều kiện tìm kiếm.", 108);
                }
                daVeThongBaoKhongKetQua = soSinhVien == 0;

                for (int i = 0; i < soNut; ++i) {
                    GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
                }

                string moTaUndo = LichSuThaoTac::moTaThaoTacCGanNhat(lichSu);
                GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang) + " | F3: hoàn tác [" + moTaUndo + "] | Enter sửa | Ins thêm | Esc đổi lớp | Tìm không dấu", thongBao, coLoi);
                thongBao = "";
                canVeLaiToanBo = false;
                canVeLaiCacOLoc = false;
                canVeLaiBang = false;
                canVeLaiThanhTrangThai = false;
                mucChonTruoc = mucDangChon;
                trangTruoc = trangHienTai;
                nutTruocDo = nutDangTro;
            } else {
                if (canVeLaiCacOLoc) {
                    veCacOLoc();
                    canVeLaiCacOLoc = false;
                }
                if (canVeLaiBang) {
                    for (int r = 0; r < soDongMoiTrang; ++r) {
                        int i = viTriDau + r;
                        NodeSV* dongMoi = i < soSinhVien ? mangSinhVien[i] : nullptr;
                        bool dangChonMoi = i < soSinhVien && i == mucDangChon;
                        if (dongMoi != cacDongDaVe[r] || dangChonMoi != cacDongDaChonDaVe[r] ||
                            (r == 3 && daVeThongBaoKhongKetQua != (soSinhVien == 0))) {
                            veMotDong(i, dangChonMoi);
                            cacDongDaVe[r] = dongMoi;
                            cacDongDaChonDaVe[r] = dangChonMoi;
                        }
                    }
                    if (soSinhVien == 0) {
                        GiaoDienConsole::diDenToaDo(4, 10);
                        GiaoDienConsole::datMau(DO_NHAT, DEN);
                        cout << GiaoDienConsole::canGiua("Không tìm thấy sinh viên phù hợp với điều kiện tìm kiếm.", 108);
                    }
                    daVeThongBaoKhongKetQua = soSinhVien == 0;
                    mucChonTruoc = mucDangChon;
                    canVeLaiBang = false;
                }
                if (mucDangChon != mucChonTruoc) {
                    veMotDong(mucChonTruoc, false);
                    veMotDong(mucDangChon, true);
                    mucChonTruoc = mucDangChon;
                }
                if (nutDangTro != nutTruocDo) {
                    for (int i = 0; i < soNut; ++i) {
                        GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
                    }
                    nutTruocDo = nutDangTro;
                }
                if (canVeLaiThanhTrangThai) {
                    string moTaUndo = LichSuThaoTac::moTaThaoTacCGanNhat(lichSu);
                    GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang) + " | F3: hoàn tác [" + moTaUndo + "] | Enter sửa | Ins thêm | Esc đổi lớp | Tìm không dấu", thongBao, coLoi);
                    canVeLaiThanhTrangThai = false;
                }
            }

            datViTriConTroOLoc();
            SuKienNhap suKien = GiaoDienConsole::docSuKien();
            int hanhDong = -1;

            if (suKien.type == SK_CHUOT_DI_CHUYEN) {
                nutDangTro = -1;
                for (int i = 0; i < soNut; ++i) {
                    if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                        nutDangTro = i;
                        break;
                    }
                }
                if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                    int viTriClick = viTriDau + (suKien.toaDoChuotY - 7);
                    if (viTriClick < viTriCuoi) mucDangChon = viTriClick;
                }
                if (mangSinhVien) delete[] mangSinhVien;
                continue;
            } else if (suKien.type == SK_CHUOT_TRAI) {
                for (int i = 0; i < soNut; ++i) {
                    if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                        hanhDong = cacNut[i].id;
                        break;
                    }
                }
                if (suKien.toaDoChuotY == 4) {
                    const int dau[] = {4, 32, 64, 91};
                    const int rong[] = {25, 29, 25, 21};
                    for (int i = 0; i < 4; ++i) {
                        if (suKien.toaDoChuotX >= dau[i] && suKien.toaDoChuotX < dau[i] + rong[i]) {
                            oLocDangChon = i;
                            canVeLaiCacOLoc = true;
                            break;
                        }
                    }
                }
                if (hanhDong == -1 && suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                    int viTriClick = viTriDau + (suKien.toaDoChuotY - 7);
                    if (viTriClick < viTriCuoi) {
                        mucDangChon = viTriClick;
                        hanhDong = 1;
                    }
                }
            }

            if (suKien.type == EVT_KEY_F3 || hanhDong == 4) {
                const string moTaUndo = LichSuThaoTac::moTaThaoTacCGanNhat(lichSu);
                string thongBaoUndo;
                bool daHoanTac = LichSuThaoTac::hoanTacThaoTacC(
                    lichSu, root, danhSachLop, danhSachLopTinChi, thongBaoUndo);
                thongBao = daHoanTac ? "Đã hoàn tác: " + moTaUndo : thongBaoUndo;
                coLoi = !daHoanTac;
                canVeLaiToanBo = true;
                canVeLaiThanhTrangThai = true;
            } else if (suKien.type == SK_PHIM_ESC || hanhDong == 3) {
                if (mangSinhVien) delete[] mangSinhVien;
                luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                break;
            } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
                if (soSinhVien > 0) mucDangChon = (mucDangChon - 1 + soSinhVien) % soSinhVien;
            } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_CON_LAN_XUONG) {
                if (soSinhVien > 0) mucDangChon = (mucDangChon + 1) % soSinhVien;
            } else if (suKien.type == EVT_KEY_INSERT || hanhDong == 0) {
                int soLuongDaThem = 0;
                while (true) {
                    TruongNhap* f = new TruongNhap[5];
                    f[0] = { "Mã SV (Để rỗng = Dừng)", "", 15, TRUONG_MA, 0, 0, 0 };
                    f[1] = { "Họ và tên đệm", "", 30, TRUONG_CHU, 0, 0, 0 };
                    f[2] = { "Tên sinh viên", "", 15, TRUONG_CHU, 0, 0, 0 };
                    f[3] = { "Phái (Nam/Nu)", "Nam", 6, TRUONG_CHU, 0, 0, 0 };
                    f[4] = { "Số điện thoại", "", 12, TRUONG_DIEN_THOAI, 0, 0, 0 };

                    string thongBaoLoi = "Bỏ trống ô Mã SV và nhấn Enter/Lưu để dừng nhập mới.";
                    bool dungVongLap = false;
                    while (true) {
                        if (!chayBieuMau(string("THÊM SV VÀO LỚP ") + lop.maLop + " (ĐÃ THÊM: " + to_string(soLuongDaThem) + ")", f, 5, thongBaoLoi)) {
                            dungVongLap = true;
                            break;
                        }
                        string maSV = GiaoDienConsole::vietHoa(f[0].value);
                        if (maSV.empty()) {
                            dungVongLap = true;
                            break;
                        }
                        string ho = GiaoDienConsole::vietHoaDauTu(f[1].value);
                        string ten = GiaoDienConsole::vietHoaDauTu(f[2].value);
                        string phai = GiaoDienConsole::vietHoaDauTu(f[3].value);
                        string sdt = GiaoDienConsole::catKhoangTrang(f[4].value);

                        if (ho.empty() || ten.empty()) {
                            thongBaoLoi = "Họ và Tên sinh viên không được để trống!";
                            continue;
                        }
                        if (soSanhKhongPhanBietHoaThuong(phai.c_str(), "Nam") != 0 && soSanhKhongPhanBietHoaThuong(phai.c_str(), "Nu") != 0 && soSanhKhongPhanBietHoaThuong(phai.c_str(), "Nữ") != 0) {
                            thongBaoLoi = "Phái chỉ nhận giá trị 'Nam' hoặc 'Nu'!";
                            continue;
                        }
                        if (sdt.size() < 9) {
                            thongBaoLoi = "Số điện thoại phải có từ 9 đến 12 chữ số!";
                            continue;
                        }
                        int viTriLopDaCo = -1;
                        if (timSinhVienToanTruong(danhSachLop, maSV.c_str(), &viTriLopDaCo) != nullptr) {
                            thongBaoLoi = "Mã SV '" + maSV + "' đã tồn tại ở lớp " + danhSachLop.lop[viTriLopDaCo].maLop + "!";
                            continue;
                        }

                        SinhVien sinhVienMoi = taoSinhVien(maSV.c_str(), ho.c_str(), ten.c_str(), phai.c_str(), sdt.c_str());
                        themSVCoThuTu(lop.dssv, sinhVienMoi);
                        LichSuThaoTac::ghiNhanThemSinhVien(lichSu, lop.maLop, sinhVienMoi);
                        soLuongDaThem++;
                        luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                        break;
                    }
                    delete[] f;
                    if (dungVongLap) break;
                }
                thongBao = "Đã kết thúc nhập mới (Thêm thành công " + to_string(soLuongDaThem) + " sinh viên).";
                coLoi = false;
                canVeLaiToanBo = true;
            } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_SHIFT_TAB) {
                int buoc = suKien.type == SK_PHIM_SHIFT_TAB ? -1 : 1;
                oLocDangChon = (oLocDangChon + buoc + 4) % 4;
                canVeLaiCacOLoc = true;
            } else if (suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI) {
                string* tuKhoa[] = {&tuKhoaMaSV, &tuKhoaHoTen, &tuKhoaSoDienThoai, &tuKhoaPhai};
                if (suKien.type == SK_PHIM_TRAI && viTriConTroOLoc[oLocDangChon] > 0) --viTriConTroOLoc[oLocDangChon];
                else if (suKien.type == SK_PHIM_PHAI && viTriConTroOLoc[oLocDangChon] < tuKhoa[oLocDangChon]->size()) ++viTriConTroOLoc[oLocDangChon];
            } else if (suKien.type == EVT_KEY_HOME) {
                viTriConTroOLoc[oLocDangChon] = 0;
            } else if (suKien.type == EVT_KEY_END) {
                string* tuKhoa[] = {&tuKhoaMaSV, &tuKhoaHoTen, &tuKhoaSoDienThoai, &tuKhoaPhai};
                viTriConTroOLoc[oLocDangChon] = tuKhoa[oLocDangChon]->size();
            } else if (suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
                string* tuKhoa[] = {&tuKhoaMaSV, &tuKhoaHoTen, &tuKhoaSoDienThoai, &tuKhoaPhai};
                if (suKien.type == EVT_KEY_BACKSPACE && viTriConTroOLoc[oLocDangChon] > 0) {
                    tuKhoa[oLocDangChon]->erase(viTriConTroOLoc[oLocDangChon] - 1, 1);
                    --viTriConTroOLoc[oLocDangChon];
                } else if (suKien.type == EVT_KEY_DELETE && viTriConTroOLoc[oLocDangChon] < tuKhoa[oLocDangChon]->size()) {
                    tuKhoa[oLocDangChon]->erase(viTriConTroOLoc[oLocDangChon], 1);
                }
                mucDangChon = 0; canVeLaiCacOLoc = true; canVeLaiBang = true; canVeLaiThanhTrangThai = true;
            } else if (suKien.type == SK_KY_TU) {
                string* tuKhoa[] = {&tuKhoaMaSV, &tuKhoaHoTen, &tuKhoaSoDienThoai, &tuKhoaPhai};
                const size_t doDaiToiDa[] = {15, 50, 12, 6};
                char kyTu = suKien.ch;
                bool hopLe = oLocDangChon == 2 ? (kyTu >= '0' && kyTu <= '9') : (kyTu >= 32 && kyTu <= 126);
                if (hopLe && tuKhoa[oLocDangChon]->size() < doDaiToiDa[oLocDangChon]) {
                    if (oLocDangChon == 0 && kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
                    tuKhoa[oLocDangChon]->insert(viTriConTroOLoc[oLocDangChon], 1, kyTu);
                    ++viTriConTroOLoc[oLocDangChon];
                }
                mucDangChon = 0; canVeLaiCacOLoc = true; canVeLaiBang = true; canVeLaiThanhTrangThai = true;
            } else if ((suKien.type == SK_PHIM_ENTER || hanhDong == 1) && soSinhVien > 0) {
                SinhVien sinhVienCu = mangSinhVien[mucDangChon]->sv;
                TruongNhap* f = new TruongNhap[5];
                f[0] = { "Mã SV (Cố định)", sinhVienCu.maSV, 15, TRUONG_CHI_DOC, 0, 0, 0 };
                f[1] = { "Họ và tên đệm", sinhVienCu.ho, 30, TRUONG_CHU, 0, 0, 0 };
                f[2] = { "Tên sinh viên", sinhVienCu.ten, 15, TRUONG_CHU, 0, 0, 0 };
                f[3] = { "Phái (Nam/Nu)", sinhVienCu.phai, 6, TRUONG_CHU, 0, 0, 0 };
                f[4] = { "Số điện thoại", sinhVienCu.soDT, 12, TRUONG_DIEN_THOAI, 0, 0, 0 };

                string thongBaoLoi = "";
                while (true) {
                    bool daChonXoa = false;
                    if (!chayBieuMau(string("HIỆU CHỈNH SINH VIÊN: ") + sinhVienCu.maSV, f, 5, thongBaoLoi, "[ LƯU (Enter) ]", "[ HỦY (ESC) ]", "[ XÓA SV ]", &daChonXoa)) {
                        if (daChonXoa) {
                            if (kiemTraSVDaDangKyLTC(danhSachLopTinChi, sinhVienCu.maSV)) {
                                thongBaoLoi = "Không thể xóa SV '" + string(sinhVienCu.maSV) + "' vì đã đăng ký lớp tín chỉ!";
                                continue;
                            }
                            if (GiaoDienConsole::hopXacNhan("XÁC NHẬN XÓA SINH VIÊN", "Xóa sinh viên " + string(sinhVienCu.maSV) + " khỏi lớp " + lop.maLop + "?")) {
                                LichSuThaoTac::ghiNhanXoaSinhVien(lichSu, lop.maLop, sinhVienCu);
                                xoaSVTheoMa(lop.dssv, sinhVienCu.maSV);
                                luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                                thongBao = "Đã xóa sinh viên " + string(sinhVienCu.maSV) + " thành công!";
                                coLoi = false;
                            }
                            break;
                        }
                        break;
                    }

                    string ho = GiaoDienConsole::vietHoaDauTu(f[1].value);
                    string ten = GiaoDienConsole::vietHoaDauTu(f[2].value);
                    string phai = GiaoDienConsole::vietHoaDauTu(f[3].value);
                    string sdt = GiaoDienConsole::catKhoangTrang(f[4].value);

                    if (ho.empty() || ten.empty()) {
                        thongBaoLoi = "Họ và Tên sinh viên không được để trống!";
                        continue;
                    }
                    if (soSanhKhongPhanBietHoaThuong(phai.c_str(), "Nam") != 0 && soSanhKhongPhanBietHoaThuong(phai.c_str(), "Nu") != 0 && soSanhKhongPhanBietHoaThuong(phai.c_str(), "Nữ") != 0) {
                        thongBaoLoi = "Phái chỉ nhận giá trị 'Nam' hoặc 'Nu'!";
                        continue;
                    }
                    if (sdt.size() < 9) {
                        thongBaoLoi = "Số điện thoại phải từ 9 đến 12 chữ số!";
                        continue;
                    }

                    SinhVien sinhVienMoi = taoSinhVien(sinhVienCu.maSV, ho.c_str(), ten.c_str(), phai.c_str(), sdt.c_str());
                    xoaSVTheoMa(lop.dssv, sinhVienCu.maSV);
                    themSVCoThuTu(lop.dssv, sinhVienMoi);
                    LichSuThaoTac::ghiNhanSuaSinhVien(lichSu, lop.maLop, sinhVienCu, sinhVienMoi);
                    luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
                    thongBao = string("Đã hiệu chỉnh sinh viên ") + sinhVienCu.maSV + " và sắp xếp lại theo Tên!";
                    coLoi = false;
                    break;
                }
                delete[] f;
                canVeLaiToanBo = true;
            }

            if (mangSinhVien) delete[] mangSinhVien;
        }
        delete[] cacNut;
    }
}

// ============================================================================
// CHỨC NĂNG (d): IN DSSV CỦA 1 LỚP THEO THỨ TỰ ALPHABET TĂNG DẦN CỦA MÃ SV
// ============================================================================

// ============================================================================
void chucNangD_InDSSVTheoMaSV(DanhSachLop& danhSachLop) {
    while (true) {
        string maLop = chonLopNhanh(danhSachLop, false);
        if (maLop.empty()) break;

        int viTriLop = timLop(danhSachLop, maLop.c_str());
        if (viTriLop == -1) continue;
        Lop& lop = danhSachLop.lop[viTriLop];

        int soSinhVien = demSVTrongLop(lop.dssv);
        SinhVien* danhSachSinhVien = (soSinhVien > 0) ? new SinhVien[soSinhVien] : nullptr;
        int idx = 0;
        for (PTRSV p = lop.dssv; p != nullptr; p = p->next) {
            danhSachSinhVien[idx++] = p->sv;
        }

        for (int i = 0; i < soSinhVien - 1; ++i) {
            for (int j = i + 1; j < soSinhVien; ++j) {
                if (soSanhKhongPhanBietHoaThuong(danhSachSinhVien[i].maSV, danhSachSinhVien[j].maSV) > 0) {
                    SinhVien sinhVienTam = danhSachSinhVien[i];
                    danhSachSinhVien[i] = danhSachSinhVien[j];
                    danhSachSinhVien[j] = sinhVienTam;
                }
            }
        }

        string tuKhoa[4];
        size_t viTriConTro[4] = {};
        int oDangChon = 0;
        int mucDangChon = 0;
        const int soDongMoiTrang = 12;
        int trang = 0;
        const int toaDoX[] = {4, 32, 62, 88};
        const int chieuRong[] = {25, 28, 24, 22};
        const string nhan[] = {"Mã SV: ", "Họ/Tên: ", "Phái: ", "SĐT: "};
        const size_t doDaiToiDa[] = {15, 50, 6, 12};
        int tongNam = 0;
        int tongNu = 0;
        for (int i = 0; i < soSinhVien; ++i) {
            string phai = GiaoDienConsole::vietHoa(danhSachSinhVien[i].phai);
            if (phai == "NAM") ++tongNam;
            else if (phai == "NU" || phai == "NỮ") ++tongNu;
        }
        int* sinhVienLoc = (soSinhVien > 0) ? new int[soSinhVien] : nullptr;
        int soSinhVienLoc = 0;
        bool canVeKhung = true;
        bool canVeOLoc = true;
        bool canVeBang = true;
        bool canVeThongKe = true;
        bool canVeTrangThai = true;
        int trangCu = -1;
        NutChuNhat nutQuayLai = { 46, 23, 24, 1, 0, "[ QUAY LẠI (ESC) ]" };

        auto capNhatKetQuaTim = [&]() {
            soSinhVienLoc = 0;
            for (int i = 0; i < soSinhVien; ++i) {
                string ma = GiaoDienConsole::vietHoa(danhSachSinhVien[i].maSV);
                string hoTen = GiaoDienConsole::vietHoa(string(danhSachSinhVien[i].ho) + " " + danhSachSinhVien[i].ten);
                string phai = GiaoDienConsole::vietHoa(danhSachSinhVien[i].phai);
                if (phai == "NỮ") phai = "NU";
                string tuKhoaPhai = GiaoDienConsole::vietHoa(tuKhoa[2]);
                if (tuKhoaPhai == "NỮ") tuKhoaPhai = "NU";
                if ((!tuKhoa[0].empty() && ma.find(GiaoDienConsole::vietHoa(tuKhoa[0])) == string::npos) ||
                    (!tuKhoa[1].empty() && hoTen.find(GiaoDienConsole::vietHoa(tuKhoa[1])) == string::npos) ||
                    (!tuKhoaPhai.empty() && phai.find(tuKhoaPhai) == string::npos) ||
                    (!tuKhoa[3].empty() && string(danhSachSinhVien[i].soDT).find(tuKhoa[3]) == string::npos)) continue;
                sinhVienLoc[soSinhVienLoc++] = i;
            }
        };

        auto veMotOLoc = [&](int i) {
            GiaoDienConsole::diDenToaDo(toaDoX[i], 5);
            GiaoDienConsole::datMau(oDangChon == i ? DEN : TRANG, oDangChon == i ? VANG : XANH_NGOC);
            int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
            size_t soKyTuHienThi = static_cast<size_t>(max(0, chieuRong[i] - rongNhan));
            size_t dauHienThi = viTriConTro[i] >= soKyTuHienThi && soKyTuHienThi > 0
                ? viTriConTro[i] - soKyTuHienThi + 1 : 0;
            string noiDung = nhan[i] + tuKhoa[i].substr(dauHienThi, soKyTuHienThi);
            int doDai = GiaoDienConsole::doDaiUtf8(noiDung);
            cout << noiDung << string(max(0, chieuRong[i] - doDai), ' ');
        };

        auto datViTriConTro = [&]() {
            int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[oDangChon]);
            size_t soKyTuHienThi = static_cast<size_t>(max(0, chieuRong[oDangChon] - rongNhan));
            size_t dauHienThi = viTriConTro[oDangChon] >= soKyTuHienThi && soKyTuHienThi > 0
                ? viTriConTro[oDangChon] - soKyTuHienThi + 1 : 0;
            int x = toaDoX[oDangChon] + rongNhan + static_cast<int>(min(viTriConTro[oDangChon] - dauHienThi, soKyTuHienThi));
            int cotCuoi = toaDoX[oDangChon] + chieuRong[oDangChon] - 1;
            if (x > cotCuoi) x = cotCuoi;
            GiaoDienConsole::diDenToaDo(x, 5);
            GiaoDienConsole::hienConTro(true);
        };

        auto veDong = [&](int dongTrang, bool duocChon) {
            int viTri = trang * soDongMoiTrang + dongTrang;
            GiaoDienConsole::diDenToaDo(4, 9 + dongTrang);
            if (viTri < soSinhVienLoc) {
                const SinhVien& sv = danhSachSinhVien[sinhVienLoc[viTri]];
                GiaoDienConsole::datMau(duocChon ? DEN : TRANG, duocChon ? VANG : DEN);
                cout << GiaoDienConsole::canTrai(" " + to_string(viTri + 1), 7)
                     << GiaoDienConsole::canTrai(sv.maSV, 18);
                GiaoDienConsole::datMau(duocChon ? DEN : XANH_NGOC, duocChon ? VANG : DEN); cout << "|";
                GiaoDienConsole::datMau(duocChon ? DEN : TRANG, duocChon ? VANG : DEN);
                cout << GiaoDienConsole::canTrai(sv.ho, 33);
                GiaoDienConsole::datMau(duocChon ? DEN : XANH_NGOC, duocChon ? VANG : DEN); cout << "|";
                GiaoDienConsole::datMau(duocChon ? DEN : TRANG, duocChon ? VANG : DEN);
                cout << GiaoDienConsole::canTrai(sv.ten, 15);
                GiaoDienConsole::datMau(duocChon ? DEN : XANH_NGOC, duocChon ? VANG : DEN); cout << "|";
                GiaoDienConsole::datMau(duocChon ? DEN : TRANG, duocChon ? VANG : DEN);
                cout << GiaoDienConsole::canTrai(sv.phai, 11);
                GiaoDienConsole::datMau(duocChon ? DEN : XANH_NGOC, duocChon ? VANG : DEN); cout << "|";
                GiaoDienConsole::datMau(duocChon ? DEN : TRANG, duocChon ? VANG : DEN);
                cout << GiaoDienConsole::canTrai(sv.soDT, 19);
            } else {
                GiaoDienConsole::datMau(TRANG, DEN);
                cout << string(108, ' ');
            }
        };

        auto veBang = [&]() {
            int soTrang = max(1, (soSinhVienLoc + soDongMoiTrang - 1) / soDongMoiTrang);
            if (trang >= soTrang) trang = soTrang - 1;
            GiaoDienConsole::datMau(VANG, LAM);
            GiaoDienConsole::diDenToaDo(4, 7);
            cout << GiaoDienConsole::canTrai(" STT", 7)
                 << GiaoDienConsole::canTrai("MÃ SINH VIÊN", 18);
            GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
            GiaoDienConsole::datMau(VANG, LAM); cout << GiaoDienConsole::canTrai("HỌ VÀ TÊN ĐỆM", 33);
            GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
            GiaoDienConsole::datMau(VANG, LAM); cout << GiaoDienConsole::canTrai("TÊN", 15);
            GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
            GiaoDienConsole::datMau(VANG, LAM); cout << GiaoDienConsole::canTrai("PHÁI", 11);
            GiaoDienConsole::datMau(XANH_NGOC, LAM); cout << "|";
            GiaoDienConsole::datMau(VANG, LAM); cout << GiaoDienConsole::canTrai("SỐ ĐIỆN THOẠI", 19);
            GiaoDienConsole::datMau(XANH_NGOC_NHAT, DEN);
            GiaoDienConsole::diDenToaDo(4, 8);
            cout << string(108, '-');
            for (int r = 0; r < soDongMoiTrang; ++r) veDong(r, trang * soDongMoiTrang + r == mucDangChon);
            if (soSinhVienLoc == 0) {
                GiaoDienConsole::diDenToaDo(4, 13);
                GiaoDienConsole::datMau(DO_NHAT, DEN);
                cout << GiaoDienConsole::canGiua("Không tìm thấy sinh viên phù hợp với điều kiện tìm kiếm.", 108);
            }
            GiaoDienConsole::datMau(TRANG, DEN);
        };

        while (true) {
            capNhatKetQuaTim();
            int tongSoTrang = max(1, (soSinhVienLoc + soDongMoiTrang - 1) / soDongMoiTrang);
            if (mucDangChon >= soSinhVienLoc) mucDangChon = max(0, soSinhVienLoc - 1);
            trang = soSinhVienLoc > 0 ? mucDangChon / soDongMoiTrang : 0;
            if (canVeKhung) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, string("DANH SÁCH SINH VIÊN LỚP ") + lop.maLop + " - " + lop.tenLop);
                canVeKhung = false;
                canVeOLoc = canVeThongKe = canVeBang = canVeTrangThai = true;
            }
            if (canVeOLoc) {
                for (int i = 0; i < 4; ++i) veMotOLoc(i);
                canVeOLoc = false;
            }
            if (canVeThongKe) {
                GiaoDienConsole::diDenToaDo(4, 4);
                GiaoDienConsole::datMau(VANG, DEN);
                cout << GiaoDienConsole::canTrai("Tổng sinh viên: " + to_string(soSinhVien) + "   Nam: " + to_string(tongNam) + "   Nữ: " + to_string(tongNu), 108);
                canVeThongKe = false;
            }
            if (canVeBang || trang != trangCu) {
                veBang();
                canVeBang = false;
                trangCu = trang;
                canVeTrangThai = true;
            }
            if (canVeTrangThai) {
                GiaoDienConsole::veNut(nutQuayLai, true, TRANG, XAM_DAM, DEN, VANG);
                GiaoDienConsole::veThanhTrangThai("Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | Tìm SV | ↑/↓ chọn | PgUp/PgDn trang | Enter/Esc về | Nhập không dấu");
                canVeTrangThai = false;
            }

            int viTriChuotTruoc = mucDangChon;
            datViTriConTro();
            SuKienNhap suKien = GiaoDienConsole::docSuKien();
            bool canLocLai = false;
            if (suKien.type == SK_PHIM_ESC || suKien.type == SK_PHIM_ENTER ||
                (suKien.type == SK_CHUOT_TRAI && nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY))) {
                GiaoDienConsole::hienConTro(false);
                delete[] sinhVienLoc;
                delete[] danhSachSinhVien;
                break;
            } else if (suKien.type == SK_CHUOT_DI_CHUYEN || suKien.type == SK_CHUOT_TRAI) {
                if (suKien.toaDoChuotY == 5 && suKien.type == SK_CHUOT_TRAI) {
                    for (int i = 0; i < 4; ++i) {
                        if (suKien.toaDoChuotX >= toaDoX[i] && suKien.toaDoChuotX < toaDoX[i] + chieuRong[i]) {
                            oDangChon = i;
                            int rongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
                            size_t soKyTuHienThi = static_cast<size_t>(max(0, chieuRong[i] - rongNhan));
                            size_t dauHienThi = viTriConTro[i] >= soKyTuHienThi && soKyTuHienThi > 0
                                ? viTriConTro[i] - soKyTuHienThi + 1 : 0;
                            size_t viTriClick = dauHienThi + static_cast<size_t>(max(0, suKien.toaDoChuotX - toaDoX[i] - rongNhan));
                            viTriConTro[i] = min(viTriClick, tuKhoa[i].size());
                            canVeOLoc = true;
                            break;
                        }
                    }
                }
                if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 9 && suKien.toaDoChuotY < 9 + soDongMoiTrang) {
                    int viTri = trang * soDongMoiTrang + suKien.toaDoChuotY - 9;
                    if (viTri < soSinhVienLoc) mucDangChon = viTri;
                }
                if (mucDangChon != viTriChuotTruoc) {
                    int dongCu = viTriChuotTruoc % soDongMoiTrang;
                    int dongMoi = mucDangChon % soDongMoiTrang;
                    if (viTriChuotTruoc / soDongMoiTrang != mucDangChon / soDongMoiTrang) canVeBang = true;
                    else {
                        veDong(dongCu, false);
                        veDong(dongMoi, true);
                    }
                }
            } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_SHIFT_TAB) {
                int buoc = suKien.type == SK_PHIM_SHIFT_TAB ? -1 : 1;
                oDangChon = (oDangChon + buoc + 4) % 4;
                canVeOLoc = true;
            } else if (suKien.type == SK_PHIM_TRAI) {
                if (viTriConTro[oDangChon] > 0) --viTriConTro[oDangChon];
            } else if (suKien.type == SK_PHIM_PHAI) {
                if (viTriConTro[oDangChon] < tuKhoa[oDangChon].size()) ++viTriConTro[oDangChon];
            } else if (suKien.type == EVT_KEY_HOME) {
                viTriConTro[oDangChon] = 0;
                canVeOLoc = false;
            } else if (suKien.type == EVT_KEY_END) {
                viTriConTro[oDangChon] = tuKhoa[oDangChon].size();
                canVeOLoc = false;
            } else if (suKien.type == EVT_KEY_BACKSPACE || suKien.type == EVT_KEY_DELETE) {
                if (suKien.type == EVT_KEY_BACKSPACE && viTriConTro[oDangChon] > 0) {
                    tuKhoa[oDangChon].erase(viTriConTro[oDangChon] - 1, 1);
                    --viTriConTro[oDangChon];
                    canLocLai = true;
                } else if (suKien.type == EVT_KEY_DELETE && viTriConTro[oDangChon] < tuKhoa[oDangChon].size()) {
                    tuKhoa[oDangChon].erase(viTriConTro[oDangChon], 1);
                    canLocLai = true;
                }
            } else if (suKien.type == SK_KY_TU) {
                char kyTu = suKien.ch;
                bool hopLe = oDangChon == 3 ? (kyTu >= '0' && kyTu <= '9') : (kyTu >= 32 && kyTu <= 126);
                if (hopLe && tuKhoa[oDangChon].size() < doDaiToiDa[oDangChon]) {
                    if (oDangChon == 0 && kyTu >= 'a' && kyTu <= 'z') kyTu = static_cast<char>(kyTu - 'a' + 'A');
                    tuKhoa[oDangChon].insert(viTriConTro[oDangChon], 1, kyTu);
                    ++viTriConTro[oDangChon];
                    canLocLai = true;
                }
            } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_PHIM_XUONG) {
                if (soSinhVienLoc > 0) {
                    int buoc = suKien.type == SK_PHIM_LEN ? -1 : 1;
                    mucDangChon = (mucDangChon + buoc + soSinhVienLoc) % soSinhVienLoc;
                    if (mucDangChon / soDongMoiTrang != trang) canVeBang = true;
                    else {
                        veDong(viTriChuotTruoc % soDongMoiTrang, false);
                        veDong(mucDangChon % soDongMoiTrang, true);
                    }
                    canVeTrangThai = true;
                }
            } else if (suKien.type == EVT_KEY_PGDN || suKien.type == EVT_KEY_PGUP) {
                if (soSinhVienLoc > 0) {
                    int buoc = suKien.type == EVT_KEY_PGDN ? soDongMoiTrang : -soDongMoiTrang;
                    mucDangChon = max(0, min(soSinhVienLoc - 1, mucDangChon + buoc));
                    trang = mucDangChon / soDongMoiTrang;
                    canVeBang = canVeTrangThai = true;
                }
            } else if (suKien.type == SK_CON_LAN_XUONG || suKien.type == SK_CON_LAN_LEN) {
                if (soSinhVienLoc > 0) {
                    int buoc = suKien.type == SK_CON_LAN_XUONG ? 1 : -1;
                    mucDangChon = (mucDangChon + buoc + soSinhVienLoc) % soSinhVienLoc;
                    trang = mucDangChon / soDongMoiTrang;
                    canVeBang = canVeTrangThai = true;
                }
            }
            if (canLocLai) {
                capNhatKetQuaTim();
                mucDangChon = 0;
                trang = 0;
                canVeOLoc = canVeBang = canVeTrangThai = true;
            }
        }
    }
}



