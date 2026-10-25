#include "ui/GiaoDienConsole.h"
#include "features/QuanLyLopSinhVien.h"
#include "features/HamDungChung.h"

#include "storage/DocGhiDuLieu.h"


#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

void chucNangC_CapNhatSinhVienLop(PTRMH cayMonHoc, DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi) {
    while (true) {
        string maLop = chonLopNhanh(danhSachLop, true);
        if (maLop.empty()) break;

        int viTriLop = timLop(danhSachLop, maLop);
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

        int soNut = 2;
        NutChuNhat* cacNut = new NutChuNhat[soNut];
        cacNut[0] = { 6,  23, 28, 1, 0, "[ + NHẬP SV MỚI (Ins) ]" };
        cacNut[1] = { 90, 23, 20, 1, 3, "[ ĐỔI LỚP (ESC) ]" };

        auto veCacOLoc = [&]() {
            const int toaDoX[] = {4, 32, 64, 91};
            const int chieuRong[] = {25, 29, 25, 21};
            const string nhan[] = {"Mã SV: ", "Họ/Tên: ", "SĐT: ", "Phái: "};
            const string* giaTri[] = {&tuKhoaMaSV, &tuKhoaHoTen, &tuKhoaSoDienThoai, &tuKhoaPhai};
            for (int i = 0; i < 4; ++i) {
                GiaoDienConsole::diDenToaDo(toaDoX[i], 4);
                GiaoDienConsole::datMau(oLocDangChon == i ? DEN : TRANG, oLocDangChon == i ? VANG : XANH_NGOC);
                int doRongNhan = GiaoDienConsole::doDaiUtf8(nhan[i]);
                string noiDung = nhan[i] + giaTri[i]->substr(0, max(0, chieuRong[i] - doRongNhan));
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
            int x = toaDoX[oLocDangChon] + doRongNhan
                + static_cast<int>(min(viTriConTroOLoc[oLocDangChon], static_cast<size_t>(soKyTuHienThi)));
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
                    cout << GiaoDienConsole::canPhai(" " + to_string(i + 1), 8)
                         << GiaoDienConsole::canPhai(sv.maSV, 18)
                         << GiaoDienConsole::canPhai(sv.ho, 34)
                         << GiaoDienConsole::canPhai(sv.ten, 16)
                         << GiaoDienConsole::canPhai(sv.phai, 12)
                         << GiaoDienConsole::canPhai(sv.soDT, 20);
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
                cout << GiaoDienConsole::canPhai(" STT", 8)
                     << GiaoDienConsole::canPhai("MÃ SINH VIÊN", 18)
                     << GiaoDienConsole::canPhai("HỌ VÀ TÊN ĐỆM", 34)
                     << GiaoDienConsole::canPhai("TÊN", 16)
                     << GiaoDienConsole::canPhai("PHÁI", 12)
                     << GiaoDienConsole::canPhai("SỐ ĐIỆN THOẠI", 20);

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

                GiaoDienConsole::veThanhTrangThai("Gõ để tìm mã/tên/SĐT/phái | Tab/←→: đổi ô | Enter: hiệu chỉnh | Ins: thêm SV | ESC: đổi lớp | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang), thongBao, coLoi);
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
                    GiaoDienConsole::veThanhTrangThai("Gõ để tìm mã/tên/SĐT/phái | Tab/←→: đổi ô | Enter: hiệu chỉnh | Ins: thêm SV | ESC: đổi lớp | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang), thongBao, coLoi);
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

            if (suKien.type == SK_PHIM_ESC || hanhDong == 3) {
                if (mangSinhVien) delete[] mangSinhVien;
                luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
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
                        if (_stricmp(phai.c_str(), "Nam") != 0 && _stricmp(phai.c_str(), "Nu") != 0 && _stricmp(phai.c_str(), "Nữ") != 0) {
                            thongBaoLoi = "Phái chỉ nhận giá trị 'Nam' hoặc 'Nu'!";
                            continue;
                        }
                        if (sdt.size() < 9) {
                            thongBaoLoi = "Số điện thoại phải có từ 9 đến 12 chữ số!";
                            continue;
                        }
                        int viTriLopDaCo = -1;
                        if (timSinhVienToanTruong(danhSachLop, maSV, &viTriLopDaCo) != nullptr) {
                            thongBaoLoi = "Mã SV '" + maSV + "' đã tồn tại ở lớp " + danhSachLop.lop[viTriLopDaCo].maLop + "!";
                            continue;
                        }

                        themSVCoThuTu(lop.dssv, taoSinhVien(maSV, ho, ten, phai, sdt));
                        soLuongDaThem++;
                        luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                        break;
                    }
                    delete[] f;
                    if (dungVongLap) break;
                }
                thongBao = "Đã kết thúc nhập mới (Thêm thành công " + to_string(soLuongDaThem) + " sinh viên).";
                coLoi = false;
                canVeLaiToanBo = true;
            } else if (suKien.type == SK_PHIM_TAB || suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI) {
                int buoc = suKien.type == SK_PHIM_TRAI ? -1 : 1;
                oLocDangChon = (oLocDangChon + buoc + 4) % 4;
                canVeLaiCacOLoc = true;
            } else if (suKien.type == EVT_KEY_BACKSPACE) {
                string* tuKhoa[] = {&tuKhoaMaSV, &tuKhoaHoTen, &tuKhoaSoDienThoai, &tuKhoaPhai};
                if (viTriConTroOLoc[oLocDangChon] > 0) {
                    tuKhoa[oLocDangChon]->erase(viTriConTroOLoc[oLocDangChon] - 1, 1);
                    --viTriConTroOLoc[oLocDangChon];
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
                                xoaSVTheoMa(lop.dssv, sinhVienCu.maSV);
                                luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
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
                    if (_stricmp(phai.c_str(), "Nam") != 0 && _stricmp(phai.c_str(), "Nu") != 0 && _stricmp(phai.c_str(), "Nữ") != 0) {
                        thongBaoLoi = "Phái chỉ nhận giá trị 'Nam' hoặc 'Nu'!";
                        continue;
                    }
                    if (sdt.size() < 9) {
                        thongBaoLoi = "Số điện thoại phải từ 9 đến 12 chữ số!";
                        continue;
                    }

                    xoaSVTheoMa(lop.dssv, sinhVienCu.maSV);
                    themSVCoThuTu(lop.dssv, taoSinhVien(sinhVienCu.maSV, ho, ten, phai, sdt));
                    luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
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

        int viTriLop = timLop(danhSachLop, maLop);
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
                if (_stricmp(danhSachSinhVien[i].maSV, danhSachSinhVien[j].maSV) > 0) {
                    SinhVien sinhVienTam = danhSachSinhVien[i];
                    danhSachSinhVien[i] = danhSachSinhVien[j];
                    danhSachSinhVien[j] = sinhVienTam;
                }
            }
        }

        int trang = 0;
        int soDongMoiTrang = 14;
        int tongSoTrang = (soSinhVien == 0) ? 1 : (soSinhVien + soDongMoiTrang - 1) / soDongMoiTrang;
        int trangTruoc = -1;
        NutChuNhat nutQuayLai = { 46, 23, 24, 1, 0, "[ QUAY LẠI (ESC) ]" };

        while (true) {
            if (trang != trangTruoc) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, string("DANH SÁCH SINH VIÊN LỚP ") + lop.maLop + " - " + lop.tenLop);

                GiaoDienConsole::datMau(VANG, LAM);
                GiaoDienConsole::diDenToaDo(4, 5);
                cout << GiaoDienConsole::canPhai(" STT", 8)
                     << GiaoDienConsole::canPhai("MÃ SINH VIÊN", 20)
                     << GiaoDienConsole::canPhai("HỌ VÀ TÊN ĐỆM", 34)
                     << GiaoDienConsole::canPhai("TÊN", 16)
                     << GiaoDienConsole::canPhai("PHÁI", 12)
                     << GiaoDienConsole::canPhai("SỐ ĐIỆN THOẠI", 18);

                int viTriDau = trang * soDongMoiTrang;
                int viTriCuoi = (viTriDau + soDongMoiTrang < soSinhVien) ? (viTriDau + soDongMoiTrang) : soSinhVien;

                if (soSinhVien == 0) {
                    GiaoDienConsole::datMau(DO_NHAT, DEN);
                    GiaoDienConsole::diDenToaDo(4, 10);
                    cout << GiaoDienConsole::canGiua("Lớp này hiện chưa có sinh viên nào!", 108);
                } else {
                    for (int r = 0; r < soDongMoiTrang; ++r) {
                        int i = viTriDau + r;
                        GiaoDienConsole::diDenToaDo(4, 7 + r);
                        if (i < viTriCuoi) {
                            GiaoDienConsole::datMau(TRANG, DEN);
                            cout << GiaoDienConsole::canPhai(" " + to_string(i + 1), 8)
                                 << GiaoDienConsole::canPhai(danhSachSinhVien[i].maSV, 20)
                                 << GiaoDienConsole::canPhai(danhSachSinhVien[i].ho, 34)
                                 << GiaoDienConsole::canPhai(danhSachSinhVien[i].ten, 16)
                                 << GiaoDienConsole::canPhai(danhSachSinhVien[i].phai, 12)
                                 << GiaoDienConsole::canPhai(danhSachSinhVien[i].soDT, 18);
                        } else {
                            cout << string(108, ' ');
                        }
                    }
                }

                GiaoDienConsole::veNut(nutQuayLai, true, TRANG, XAM_DAM, DEN, VANG);
                GiaoDienConsole::veThanhTrangThai("Lên/Xuống hoặc Cuộn chuột để lật trang | Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | ESC/Enter: Quay lại");
                trangTruoc = trang;
            }

            SuKienNhap suKien = GiaoDienConsole::docSuKien();
            if (suKien.type == SK_PHIM_ESC || suKien.type == SK_PHIM_ENTER) break;
            if (suKien.type == SK_CHUOT_TRAI && nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) break;
            if (suKien.type == SK_PHIM_XUONG || suKien.type == EVT_KEY_PGDN || suKien.type == SK_CON_LAN_XUONG) {
                if (trang + 1 < tongSoTrang) trang++;
            } else if (suKien.type == SK_PHIM_LEN || suKien.type == EVT_KEY_PGUP || suKien.type == SK_CON_LAN_LEN) {
                if (trang > 0) trang--;
            }
        }

        if (danhSachSinhVien) delete[] danhSachSinhVien;
    }
}

// ============================================================================
// CHỨC NĂNG (e): NHẬP MÔN HỌC (Thêm / Xóa / Hiệu chỉnh trên Cây BST)
// ============================================================================


