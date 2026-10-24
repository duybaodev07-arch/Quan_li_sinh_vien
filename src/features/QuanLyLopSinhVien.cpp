#include "features/QuanLyLopSinhVien.h"

using namespace std;

void chucNangC_CapNhatSinhVienLop(PTRMH cayMonHoc, DSLop& danhSachLop, const DSLopTinChi& danhSachLopTinChi) {
    while (true) {
        string maLop = chonLopNhanh(danhSachLop, true);
        if (maLop.empty()) break;

        int viTriLop = timLop(danhSachLop, maLop);
        if (viTriLop == -1) continue;
        Lop& lop = danhSachLop.cacNutDuLieu[viTriLop];

        int mucDangChon = 0;
        int soDongMoiTrang = 14;
        string thongBao = "";
        bool coLoi = false;
        bool canVeLaiToanBo = true;
        int mucChonTruoc = -1;
        int trangTruoc = -1;
        int nutDangTro = -1;
        int nutTruocDo = -1;

        int soNut = 4;
        NutChuNhat* cacNut = new NutChuNhat[soNut];
        cacNut[0] = { 6,  23, 28, 1, 0, "[ + NHẬP SV MỚI (Ins) ]" };
        cacNut[1] = { 38, 23, 24, 1, 1, "[ HIỆU CHỈNH (Enter) ]" };
        cacNut[2] = { 66, 23, 20, 1, 2, "[ XÓA SV (Del) ]" };
        cacNut[3] = { 90, 23, 20, 1, 3, "[ ĐỔI LỚP (ESC) ]" };

        while (true) {
            int soSinhVien = demSVTrongLop(lop.dssv);
            NodeSV** mangSinhVien = (soSinhVien > 0) ? new NodeSV*[soSinhVien] : nullptr;
            int idx = 0;
            for (PTRSV p = lop.dssv; p != nullptr; p = p->tiepTheo) {
                mangSinhVien[idx++] = p;
            }

            if (mucDangChon >= soSinhVien) mucDangChon = (soSinhVien > 0) ? soSinhVien - 1 : 0;
            int tongSoTrang = (soSinhVien == 0) ? 1 : (soSinhVien + soDongMoiTrang - 1) / soDongMoiTrang;
            int trangHienTai = mucDangChon / soDongMoiTrang;
            int viTriDau = trangHienTai * soDongMoiTrang;
            int viTriCuoi = (viTriDau + soDongMoiTrang < soSinhVien) ? (viTriDau + soDongMoiTrang) : soSinhVien;

            if (trangHienTai != trangTruoc) canVeLaiToanBo = true;

            auto veMotDong = [&](int i, bool dangDuocChon) {
                int r = i - viTriDau;
                if (r < 0 || r >= soDongMoiTrang) return;
                GiaoDienConsole::diDenToaDo(4, 7 + r);
                if (i >= 0 && i < soSinhVien) {
                    SinhVien& sv = mangSinhVien[i]->duLieu;
                    GiaoDienConsole::datMau(dangDuocChon ? DEN : TRANG, dangDuocChon ? VANG : DEN);
                    cout << GiaoDienConsole::canPhai(" " + to_string(i + 1), 8)
                         << GiaoDienConsole::canPhai(sv.MASV, 18)
                         << GiaoDienConsole::canPhai(sv.HO, 34)
                         << GiaoDienConsole::canPhai(sv.TEN, 16)
                         << GiaoDienConsole::canPhai(sv.PHAI, 12)
                         << GiaoDienConsole::canPhai(sv.SODT, 20);
                } else {
                    GiaoDienConsole::datMau(XAM_NHAT, DEN);
                    cout << string(108, ' ');
                }
            };

            if (canVeLaiToanBo) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH SINH VIÊN LỚP " + lop.MALOP);

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
                }

                for (int i = 0; i < soNut; ++i) {
                    GiaoDienConsole::veNut(cacNut[i], i == nutDangTro, TRANG, XAM_DAM, DEN, VANG);
                }

                GiaoDienConsole::veThanhTrangThai("Ins: Nhập SV liên tục (Mã SV rỗng để dừng) | Enter: Sửa | Del: Xóa | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang), thongBao, coLoi);
                thongBao = "";
                canVeLaiToanBo = false;
                mucChonTruoc = mucDangChon;
                trangTruoc = trangHienTai;
                nutTruocDo = nutDangTro;
            } else {
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
            }

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
                if (hanhDong == -1 && suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 112 && suKien.toaDoChuotY >= 7 && suKien.toaDoChuotY < 7 + soDongMoiTrang) {
                    int viTriClick = viTriDau + (suKien.toaDoChuotY - 7);
                    if (viTriClick < viTriCuoi) {
                        if (mucDangChon == viTriClick) hanhDong = 1;
                        else mucDangChon = viTriClick;
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
                        if (!chayBieuMau("THÊM SV VÀO LỚP " + lop.MALOP + " (ĐÃ THÊM: " + to_string(soLuongDaThem) + ")", f, 5, thongBaoLoi)) {
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
                            thongBaoLoi = "Mã SV '" + maSV + "' đã tồn tại ở lớp " + danhSachLop.cacNutDuLieu[viTriLopDaCo].MALOP + "!";
                            continue;
                        }

                        themSVCoThuTu(lop.dssv, SinhVien(maSV, ho, ten, phai, sdt));
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
            } else if ((suKien.type == SK_PHIM_ENTER || hanhDong == 1) && soSinhVien > 0) {
                SinhVien sinhVienCu = mangSinhVien[mucDangChon]->duLieu;
                TruongNhap* f = new TruongNhap[5];
                f[0] = { "Mã SV (Cố định)", sinhVienCu.MASV, 15, TRUONG_CHI_DOC, 0, 0, 0 };
                f[1] = { "Họ và tên đệm", sinhVienCu.HO, 30, TRUONG_CHU, 0, 0, 0 };
                f[2] = { "Tên sinh viên", sinhVienCu.TEN, 15, TRUONG_CHU, 0, 0, 0 };
                f[3] = { "Phái (Nam/Nu)", sinhVienCu.PHAI, 6, TRUONG_CHU, 0, 0, 0 };
                f[4] = { "Số điện thoại", sinhVienCu.SODT, 12, TRUONG_DIEN_THOAI, 0, 0, 0 };

                string thongBaoLoi = "";
                while (true) {
                    if (!chayBieuMau("HIỆU CHỈNH SINH VIÊN: " + sinhVienCu.MASV, f, 5, thongBaoLoi)) break;

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

                    xoaSVTheoMa(lop.dssv, sinhVienCu.MASV);
                    themSVCoThuTu(lop.dssv, SinhVien(sinhVienCu.MASV, ho, ten, phai, sdt));
                    luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                    thongBao = "Đã hiệu chỉnh sinh viên " + sinhVienCu.MASV + " và sắp xếp lại theo Tên!";
                    coLoi = false;
                    break;
                }
                delete[] f;
                canVeLaiToanBo = true;
            } else if ((suKien.type == EVT_KEY_DELETE || hanhDong == 2) && soSinhVien > 0) {
                string maSV = mangSinhVien[mucDangChon]->duLieu.MASV;
                if (kiemTraSVDaDangKyLTC(danhSachLopTinChi, maSV)) {
                    thongBao = "Không thể xóa SV '" + maSV + "' vì đã đăng ký lớp tín chỉ!";
                    coLoi = true;
                } else {
                    if (GiaoDienConsole::hopXacNhan("XÁC NHẬN XÓA SINH VIÊN",
                                                 "Bạn có chắc chắn muốn xóa sinh viên " + maSV + " khỏi lớp " + lop.MALOP + "?")) {
                        xoaSVTheoMa(lop.dssv, maSV);
                        luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                        thongBao = "Đã xóa sinh viên " + maSV + " thành công!";
                        coLoi = false;
                    }
                }
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
void chucNangD_InDSSVTheoMaSV(DSLop& danhSachLop) {
    while (true) {
        string maLop = chonLopNhanh(danhSachLop, false);
        if (maLop.empty()) break;

        int viTriLop = timLop(danhSachLop, maLop);
        if (viTriLop == -1) continue;
        Lop& lop = danhSachLop.cacNutDuLieu[viTriLop];

        int soSinhVien = demSVTrongLop(lop.dssv);
        SinhVien* arr = (soSinhVien > 0) ? new SinhVien[soSinhVien] : nullptr;
        int idx = 0;
        for (PTRSV p = lop.dssv; p != nullptr; p = p->tiepTheo) {
            arr[idx++] = p->duLieu;
        }

        for (int i = 0; i < soSinhVien - 1; ++i) {
            for (int j = i + 1; j < soSinhVien; ++j) {
                if (_stricmp(arr[i].MASV.c_str(), arr[j].MASV.c_str()) > 0) {
                    SinhVien tmp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = tmp;
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
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "DANH SÁCH SINH VIÊN LỚP " + lop.MALOP + " - " + lop.TENLOP);

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
                                 << GiaoDienConsole::canPhai(arr[i].MASV, 20)
                                 << GiaoDienConsole::canPhai(arr[i].HO, 34)
                                 << GiaoDienConsole::canPhai(arr[i].TEN, 16)
                                 << GiaoDienConsole::canPhai(arr[i].PHAI, 12)
                                 << GiaoDienConsole::canPhai(arr[i].SODT, 18);
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

        if (arr) delete[] arr;
    }
}

// ============================================================================
// CHỨC NĂNG (e): NHẬP MÔN HỌC (Thêm / Xóa / Hiệu chỉnh trên Cây BST)
// ============================================================================


