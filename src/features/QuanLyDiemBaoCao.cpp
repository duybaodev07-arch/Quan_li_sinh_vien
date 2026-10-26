#include "ui/GiaoDienConsole.h"
#include "features/QuanLyDiemBaoCao.h"
#include "features/HamDungChung.h"

#include "storage/DocGhiDuLieu.h"


#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

// ============================================================================
void chucNangI_NhapDiem(PTRMH cayMonHoc, const DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi) {
    while (true) {
        int viTriLopTinChi = chonLopTinChiTheoThongTin(cayMonHoc, danhSachLopTinChi, "CHỨC NĂNG (i): NHẬP ĐIỂM LỚP TÍN CHỈ");
        if (viTriLopTinChi == -1) break;

        LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChi];
        if (lopTinChi->huyLop) {
            GiaoDienConsole::hopThongBao("CẢNH BÁO", "Lớp tín chỉ này đã bị hủy, không thể nhập điểm!", true);
            continue;
        }

        NodeMH* mh = timMonHoc(cayMonHoc, lopTinChi->maMH);
        string tenMonHoc = mh ? mh->mh.tenMH : lopTinChi->maMH;

        int soDangKyHieuLuc = demSVDangKyHieuLuc(lopTinChi->dssvdk);
        if (soDangKyHieuLuc == 0) {
            GiaoDienConsole::hopThongBao("THÔNG BÁO", "Lớp tín chỉ này chưa có sinh viên nào đăng ký còn hiệu lực!", true);
            continue;
        }

        struct BanGhiDiem {
            NodeDK* nutDangKy;
            string maSV;
            string ho;
            string ten;
            string chuoiDiem;
        };

        BanGhiDiem* cacBanGhiDiem = new BanGhiDiem[soDangKyHieuLuc];
        int count = 0;
        for (PTRDK p = lopTinChi->dssvdk; p != nullptr; p = p->next) {
            if (!p->dk.huyDangKy) {
                NodeSV* sv = timSinhVienToanTruong(danhSachLop, p->dk.maSV);
                cacBanGhiDiem[count].nutDangKy = p;
                cacBanGhiDiem[count].maSV = p->dk.maSV;
                cacBanGhiDiem[count].ho = sv ? sv->sv.ho : "(Không rõ)";
                cacBanGhiDiem[count].ten = sv ? sv->sv.ten : "";
                cacBanGhiDiem[count].chuoiDiem = dinhDangDiem(p->dk.diem, true);
                count++;
            }
        }

        int dongDangChon = 0;
        int dongTruoc = -1;
        int soDongMoiTrang = 13;
        int trangTruoc = -1;
        bool canVeLaiToanBo = true;
        bool oDiemDaSua = false;
        string thongBao = "";
        bool coLoi = false;

        NutChuNhat nutLuu = { 28, 23, 28, 1, 0, "[ LƯU BẢNG ĐIỂM (F2) ]" };
        NutChuNhat nutHuy = { 62, 23, 24, 1, 1, "[ QUAY LẠI (ESC) ]" };

        auto veMotDong = [&](int i, int viTriDau, bool dangDuocChon) {
            int r = i - viTriDau;
            if (r < 0 || r >= soDongMoiTrang) return;
            int toaDoDong = 8 + r;
            GiaoDienConsole::diDenToaDo(4, toaDoDong);
            if (i >= 0 && i < count) {
                GiaoDienConsole::datMau(dangDuocChon ? VANG : TRANG, DEN);
                cout << GiaoDienConsole::canPhai(" " + to_string(i + 1), 10)
                     << GiaoDienConsole::canPhai(cacBanGhiDiem[i].maSV, 20)
                     << GiaoDienConsole::canPhai(cacBanGhiDiem[i].ho, 38)
                     << GiaoDienConsole::canPhai(cacBanGhiDiem[i].ten, 20);

                GiaoDienConsole::diDenToaDo(92, toaDoDong);
                if (dangDuocChon) {
                    GiaoDienConsole::datMau(DEN, VANG);
                    cout << "[ " << GiaoDienConsole::canPhai(cacBanGhiDiem[i].chuoiDiem, 10) << " ]";
                } else {
                    GiaoDienConsole::datMau(XANH_NGOC_NHAT, XAM_DAM);
                    cout << "  " << GiaoDienConsole::canPhai(cacBanGhiDiem[i].chuoiDiem, 10) << "  ";
                }
            } else {
                GiaoDienConsole::datMau(XAM_NHAT, DEN);
                cout << string(106, ' ');
            }
        };

        auto veOdiem = [&](int i, int viTriDau) {
            int r = i - viTriDau;
            if (r < 0 || r >= soDongMoiTrang) return;
            int toaDoDong = 8 + r;
            GiaoDienConsole::diDenToaDo(92, toaDoDong);
            GiaoDienConsole::datMau(DEN, VANG);
            cout << "[ " << GiaoDienConsole::canPhai(cacBanGhiDiem[i].chuoiDiem, 10) << " ]";
        };

        while (true) {
            int tongSoTrang = (count + soDongMoiTrang - 1) / soDongMoiTrang;
            int trangHienTai = dongDangChon / soDongMoiTrang;
            int viTriDau = trangHienTai * soDongMoiTrang;
            int viTriCuoi = (viTriDau + soDongMoiTrang < count) ? (viTriDau + soDongMoiTrang) : count;

            if (trangHienTai != trangTruoc) canVeLaiToanBo = true;

            GiaoDienConsole::hienConTro(false);

            if (canVeLaiToanBo) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "BẢNG NHẬP ĐIỂM MÔN: " + tenMonHoc);

                GiaoDienConsole::datMau(VANG, DEN);
                GiaoDienConsole::diDenToaDo(4, 4);
                cout << "Niên khóa: " << lopTinChi->nienKhoa << "   |   Học kỳ: " << lopTinChi->hocKy << "   |   Nhóm: " << lopTinChi->nhom;

                GiaoDienConsole::datMau(VANG, LAM);
                GiaoDienConsole::diDenToaDo(4, 6);
                cout << GiaoDienConsole::canPhai(" STT", 10)
                     << GiaoDienConsole::canPhai("maSV", 20)
                     << GiaoDienConsole::canPhai("ho", 38)
                     << GiaoDienConsole::canPhai("ten", 20)
                     << GiaoDienConsole::canPhai("diem (0..10)", 18);

                for (int r = 0; r < soDongMoiTrang; ++r) {
                    int i = viTriDau + r;
                    veMotDong(i, viTriDau, i == dongDangChon);
                }

                GiaoDienConsole::veNut(nutLuu, false, TRANG, XANH_LA, DEN, VANG);
                GiaoDienConsole::veNut(nutHuy, false, TRANG, DO, DEN, VANG);

                GiaoDienConsole::veThanhTrangThai("Mũi tên Lên/Xuống/Enter hoặc Click chuột vào dòng để gõ điểm | F2: Lưu | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang), thongBao, coLoi);
                canVeLaiToanBo = false;
                dongTruoc = dongDangChon;
                trangTruoc = trangHienTai;
                oDiemDaSua = false;
            } else {
                if (dongDangChon != dongTruoc) {
                    veMotDong(dongTruoc, viTriDau, false);
                    veMotDong(dongDangChon, viTriDau, true);
                    dongTruoc = dongDangChon;
                    oDiemDaSua = false;
                } else if (oDiemDaSua) {
                    veOdiem(dongDangChon, viTriDau);
                    oDiemDaSua = false;
                }
            }

            // Đặt con trỏ nhấp nháy ngay tại ô điểm đang nhập
            int toaDoDongDangNhap = 8 + (dongDangChon - viTriDau);
            int toaDoConTroX = 94 + (int)cacBanGhiDiem[dongDangChon].chuoiDiem.size();
            GiaoDienConsole::diDenToaDo(toaDoConTroX, toaDoDongDangNhap);
            GiaoDienConsole::hienConTro(true);

            SuKienNhap suKien = GiaoDienConsole::docSuKien();
            GiaoDienConsole::hienConTro(false);

            if (suKien.type == SK_CHUOT_DI_CHUYEN) {
                continue;
            }

            int nutDuocBam = -1;
            if (suKien.type == SK_CHUOT_TRAI) {
                if (nutLuu.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) nutDuocBam = 0;
                else if (nutHuy.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) nutDuocBam = 1;
                else if (suKien.toaDoChuotX >= 4 && suKien.toaDoChuotX < 110 && suKien.toaDoChuotY >= 8 && suKien.toaDoChuotY < 8 + soDongMoiTrang) {
                    int viTriClick = viTriDau + (suKien.toaDoChuotY - 8);
                    if (viTriClick < viTriCuoi) {
                        dongDangChon = viTriClick;
                    }
                }
            }

            auto kiemTraDiemHienTai = [&]() -> bool {
                string s = GiaoDienConsole::catKhoangTrang(cacBanGhiDiem[dongDangChon].chuoiDiem);
                if (s.empty()) return true;
                float val = (float)atof(s.c_str());
                if (val < 0.0f || val > 10.0f) {
                    thongBao = "Lỗi dòng " + to_string(dongDangChon + 1) + ": Điểm phải nằm trong khoảng 0.0 đến 10.0!";
                    coLoi = true;
                    GiaoDienConsole::veThanhTrangThai("Mũi tên Lên/Xuống/Enter hoặc Click chuột vào dòng để gõ điểm | F2: Lưu | Trang " + to_string(trangHienTai + 1) + "/" + to_string(tongSoTrang), thongBao, coLoi);
                    return false;
                }
                return true;
            };

            if (suKien.type == SK_PHIM_ESC || nutDuocBam == 1) {
                break;
            } else if (suKien.type == SK_PHIM_LEN || suKien.type == SK_CON_LAN_LEN) {
                if (kiemTraDiemHienTai()) {
                    dongDangChon = (dongDangChon - 1 + count) % count;
                }
            } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_PHIM_ENTER || suKien.type == SK_CON_LAN_XUONG) {
                if (kiemTraDiemHienTai()) {
                    dongDangChon = (dongDangChon + 1) % count;
                }
            } else if (suKien.type == EVT_KEY_BACKSPACE) {
                if (!cacBanGhiDiem[dongDangChon].chuoiDiem.empty()) {
                    cacBanGhiDiem[dongDangChon].chuoiDiem.pop_back();
                    oDiemDaSua = true;
                }
            } else if (suKien.type == EVT_KEY_DELETE) {
                cacBanGhiDiem[dongDangChon].chuoiDiem = "";
                oDiemDaSua = true;
            } else if (suKien.type == SK_KY_TU) {
                char c = suKien.ch;
                string& s = cacBanGhiDiem[dongDangChon].chuoiDiem;
                if (s.size() < 5) {
                    if (c >= '0' && c <= '9') {
                        s += c;
                        oDiemDaSua = true;
                    } else if ((c == '.' || c == ',') && s.find('.') == string::npos) {
                        if (s.empty()) s = "0.";
                        else s += '.';
                        oDiemDaSua = true;
                    }
                }
            } else if (suKien.type == EVT_KEY_F2 || nutDuocBam == 0) {
                bool tatCaHopLe = true;
                for (int i = 0; i < count; ++i) {
                    string s = GiaoDienConsole::catKhoangTrang(cacBanGhiDiem[i].chuoiDiem);
                    if (!s.empty()) {
                        float v = (float)atof(s.c_str());
                        if (v < 0.0f || v > 10.0f) {
                            dongDangChon = i;
                            thongBao = "Điểm của SV " + cacBanGhiDiem[i].maSV + " không hợp lệ (phải từ 0 đến 10)!";
                            coLoi = true;
                            canVeLaiToanBo = true;
                            tatCaHopLe = false;
                            break;
                        }
                    }
                }
                if (tatCaHopLe) {
                    for (int i = 0; i < count; ++i) {
                        string s = GiaoDienConsole::catKhoangTrang(cacBanGhiDiem[i].chuoiDiem);
                        if (s.empty()) {
                            cacBanGhiDiem[i].nutDangKy->dk.diem = -1.0f;
                        } else {
                            float v = (float)atof(s.c_str());
                            cacBanGhiDiem[i].nutDangKy->dk.diem = roundf(v * 100.0f) / 100.0f;
                        }
                    }
                    luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
                    GiaoDienConsole::hopThongBao("THÀNH CÔNG", "Đã lưu bảng điểm lớp tín chỉ thành công!");
                    break;
                }
            }
        }

        delete[] cacBanGhiDiem;
    }
}

// ============================================================================
// CHỨC NĂNG (j): IN BẢNG ĐIỂM MÔN HỌC CỦA 1 LỚP TÍN CHỈ
// ============================================================================

// ============================================================================
void chucNangJ_InBangDiemMonHoc(PTRMH cayMonHoc, const DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi) {
    while (true) {
        int viTriLopTinChi = chonLopTinChiTheoThongTin(cayMonHoc, danhSachLopTinChi, "CHỨC NĂNG (j): IN BẢNG ĐIỂM MÔN HỌC CỦA 1 LỚP TÍN CHỈ");
        if (viTriLopTinChi == -1) break;

        LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[viTriLopTinChi];
        NodeMH* mh = timMonHoc(cayMonHoc, lopTinChi->maMH);
        string tenMonHoc = mh ? mh->mh.tenMH : lopTinChi->maMH;

        int soDangKyHieuLuc = demSVDangKyHieuLuc(lopTinChi->dssvdk);
        struct DongDiemMonHoc {
            string maSV;
            string ho;
            string ten;
            float diem;
        };
        DongDiemMonHoc* cacDong = (soDangKyHieuLuc > 0) ? new DongDiemMonHoc[soDangKyHieuLuc] : nullptr;
        int soDong = 0;
        for (PTRDK p = lopTinChi->dssvdk; p != nullptr; p = p->next) {
            if (!p->dk.huyDangKy) {
                NodeSV* sv = timSinhVienToanTruong(danhSachLop, p->dk.maSV);
                cacDong[soDong].maSV = p->dk.maSV;
                cacDong[soDong].ho = sv ? sv->sv.ho : "(Không rõ)";
                cacDong[soDong].ten = sv ? sv->sv.ten : "";
                cacDong[soDong].diem = p->dk.diem;
                soDong++;
            }
        }

        int trang = 0;
        int soDongMoiTrang = 13;
        int tongSoTrang = (soDong == 0) ? 1 : (soDong + soDongMoiTrang - 1) / soDongMoiTrang;
        int trangTruoc = -1;
        NutChuNhat nutQuayLai = { 46, 23, 24, 1, 0, "[ QUAY LẠI (ESC) ]" };

        while (true) {
            if (trang != trangTruoc) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "BẢNG ĐIỂM MÔN HỌC " + GiaoDienConsole::vietHoa(tenMonHoc));

                GiaoDienConsole::datMau(TRANG, DEN);
                GiaoDienConsole::diDenToaDo(4, 4);
                cout << GiaoDienConsole::canGiua("Niên khóa : " + string(lopTinChi->nienKhoa) + "     Học kỳ : " + to_string(lopTinChi->hocKy) + "     Nhóm : " + to_string(lopTinChi->nhom), 108);

                GiaoDienConsole::datMau(VANG, LAM);
                GiaoDienConsole::diDenToaDo(4, 6);
                cout << GiaoDienConsole::canPhai("STT", 10)
                     << GiaoDienConsole::canPhai("maSV", 22)
                     << GiaoDienConsole::canPhai("ho", 38)
                     << GiaoDienConsole::canPhai("ten", 20)
                     << GiaoDienConsole::canPhai("diem", 18);

                int viTriDau = trang * soDongMoiTrang;
                int viTriCuoi = (viTriDau + soDongMoiTrang < soDong) ? (viTriDau + soDongMoiTrang) : soDong;

                if (soDong == 0) {
                    GiaoDienConsole::datMau(DO_NHAT, DEN);
                    GiaoDienConsole::diDenToaDo(4, 10);
                    cout << GiaoDienConsole::canGiua("Không có sinh viên nào trong danh sách lớp tín chỉ này!", 108);
                } else {
                    for (int r = 0; r < soDongMoiTrang; ++r) {
                        int i = viTriDau + r;
                        GiaoDienConsole::diDenToaDo(4, 8 + r);
                        if (i < viTriCuoi) {
                            GiaoDienConsole::datMau(TRANG, DEN);
                            cout << GiaoDienConsole::canPhai(to_string(i + 1), 10)
                                 << GiaoDienConsole::canPhai(cacDong[i].maSV, 22)
                                 << GiaoDienConsole::canPhai(cacDong[i].ho, 38)
                                 << GiaoDienConsole::canPhai(cacDong[i].ten, 20)
                                 << GiaoDienConsole::canPhai(dinhDangDiem(cacDong[i].diem), 18);
                        } else {
                            cout << string(108, ' ');
                        }
                    }
                }

                GiaoDienConsole::veNut(nutQuayLai, true, TRANG, XAM_DAM, DEN, VANG);
                GiaoDienConsole::veThanhTrangThai("Mũi tên Lên/Xuống hoặc Cuộn chuột để chuyển trang | Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | ESC/Enter: Quay lại");
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

        if (cacDong) delete[] cacDong;
    }
}

// ============================================================================
// CHỨC NĂNG (k): IN ĐIỂM TRUNG BÌNH KẾT THÚC KHÓA HỌC CỦA 1 LỚP THEO SỐ TÍN CHỈ
// ============================================================================

// ============================================================================
void chucNangK_InDiemTrungBinhKhoaHoc(PTRMH cayMonHoc, DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi) {
    int soMonHoc = demMonHoc(cayMonHoc);
    MonHoc* mangMonHoc = (soMonHoc > 0) ? new MonHoc[soMonHoc] : nullptr;
    int idxMH = 0;
    bstSangMang(cayMonHoc, mangMonHoc, idxMH);

    while (true) {
        string maLop = chonLopNhanh(danhSachLop, false);
        if (maLop.empty()) break;

        int viTriLop = timLop(danhSachLop, maLop.c_str());
        if (viTriLop == -1) continue;
        Lop& lop = danhSachLop.lop[viTriLop];

        int soSinhVien = demSVTrongLop(lop.dssv);
        struct DongDiemTrungBinh {
            string maSV;
            string ho;
            string ten;
            float diemTB;
            bool coDiem;
        };
        DongDiemTrungBinh* cacDong = (soSinhVien > 0) ? new DongDiemTrungBinh[soSinhVien] : nullptr;
        float* diemCaoNhatTheoMon = (soMonHoc > 0) ? new float[soMonHoc] : nullptr;

        int viTriSinhVien = 0;
        for (PTRSV p = lop.dssv; p != nullptr; p = p->next) {
            cacDong[viTriSinhVien].maSV = p->sv.maSV;
            cacDong[viTriSinhVien].ho = p->sv.ho;
            cacDong[viTriSinhVien].ten = p->sv.ten;

            for (int m = 0; m < soMonHoc; ++m) diemCaoNhatTheoMon[m] = -1.0f;

            for (int l = 0; l < danhSachLopTinChi.soLuong; ++l) {
                LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[l];
                if (lopTinChi->huyLop) continue;
                NodeDK* dk = timDangKy(lopTinChi->dssvdk, p->sv.maSV);
                if (dk != nullptr && !dk->dk.huyDangKy && dk->dk.diem >= 0.0f) {
                    for (int m = 0; m < soMonHoc; ++m) {
                        if (strcmp(mangMonHoc[m].maMH, lopTinChi->maMH) == 0) {
                            if (dk->dk.diem > diemCaoNhatTheoMon[m]) {
                                diemCaoNhatTheoMon[m] = dk->dk.diem;
                            }
                            break;
                        }
                    }
                }
            }

            float tongDiemNhanTC = 0.0f;
            int tongSoTC = 0;
            for (int m = 0; m < soMonHoc; ++m) {
                if (diemCaoNhatTheoMon[m] >= 0.0f) {
                    int soTC = mangMonHoc[m].soTCLT + mangMonHoc[m].soTCTH;
                    tongDiemNhanTC += diemCaoNhatTheoMon[m] * soTC;
                    tongSoTC += soTC;
                }
            }

            if (tongSoTC > 0) {
                cacDong[viTriSinhVien].coDiem = true;
                cacDong[viTriSinhVien].diemTB = tongDiemNhanTC / tongSoTC;
            } else {
                cacDong[viTriSinhVien].coDiem = false;
                cacDong[viTriSinhVien].diemTB = 0.0f;
            }
            viTriSinhVien++;
        }

        if (diemCaoNhatTheoMon) delete[] diemCaoNhatTheoMon;

        int trang = 0;
        int soDongMoiTrang = 13;
        int tongSoTrang = (soSinhVien == 0) ? 1 : (soSinhVien + soDongMoiTrang - 1) / soDongMoiTrang;
        int trangTruoc = -1;
        NutChuNhat nutQuayLai = { 46, 23, 24, 1, 0, "[ QUAY LẠI (ESC) ]" };

        while (true) {
            if (trang != trangTruoc) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "BẢNG THỐNG KÊ ĐIỂM TRUNG BÌNH KHÓA HỌC");

                GiaoDienConsole::datMau(TRANG, DEN);
                GiaoDienConsole::diDenToaDo(4, 4);
                cout << GiaoDienConsole::canGiua(string("Lớp : ") + lop.maLop + " - " + lop.tenLop, 108);

                GiaoDienConsole::datMau(VANG, LAM);
                GiaoDienConsole::diDenToaDo(4, 6);
                cout << GiaoDienConsole::canPhai("STT", 10)
                     << GiaoDienConsole::canPhai("maSV", 22)
                     << GiaoDienConsole::canPhai("ho", 38)
                     << GiaoDienConsole::canPhai("ten", 20)
                     << GiaoDienConsole::canPhai("Điểm TB", 18);

                int viTriDau = trang * soDongMoiTrang;
                int viTriCuoi = (viTriDau + soDongMoiTrang < soSinhVien) ? (viTriDau + soDongMoiTrang) : soSinhVien;

                for (int r = 0; r < soDongMoiTrang; ++r) {
                    int i = viTriDau + r;
                    GiaoDienConsole::diDenToaDo(4, 8 + r);
                    if (i < viTriCuoi) {
                        GiaoDienConsole::datMau(TRANG, DEN);
                        stringstream ss;
                        if (cacDong[i].coDiem) ss << fixed << setprecision(1) << cacDong[i].diemTB;
                        else ss << "Chưa có";
                        cout << GiaoDienConsole::canPhai(to_string(i + 1), 10)
                             << GiaoDienConsole::canPhai(cacDong[i].maSV, 22)
                             << GiaoDienConsole::canPhai(cacDong[i].ho, 38)
                             << GiaoDienConsole::canPhai(cacDong[i].ten, 20)
                             << GiaoDienConsole::canPhai(ss.str(), 18);
                    } else {
                        cout << string(108, ' ');
                    }
                }

                GiaoDienConsole::veNut(nutQuayLai, true, TRANG, XAM_DAM, DEN, VANG);
                GiaoDienConsole::veThanhTrangThai("Mũi tên Lên/Xuống hoặc Cuộn chuột để chuyển trang | Trang " + to_string(trang + 1) + "/" + to_string(tongSoTrang) + " | ESC/Enter: Quay lại");
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

        if (cacDong) delete[] cacDong;
    }

    if (mangMonHoc) delete[] mangMonHoc;
}

// ============================================================================
// CHỨC NĂNG (l): BẢNG ĐIỂM TỔNG KẾT CÁC MÔN CỦA CÁC SINH VIÊN THUỘC 1 LỚP
// ============================================================================

// ============================================================================
void chucNangL_BangDiemTongKetLop(PTRMH cayMonHoc, DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi) {
    int soMonHoc = demMonHoc(cayMonHoc);
    MonHoc* mangMonHoc = (soMonHoc > 0) ? new MonHoc[soMonHoc] : nullptr;
    int idxMH = 0;
    bstSangMang(cayMonHoc, mangMonHoc, idxMH);

    while (true) {
        string maLop = chonLopNhanh(danhSachLop, false);
        if (maLop.empty()) break;

        int viTriLop = timLop(danhSachLop, maLop.c_str());
        if (viTriLop == -1) continue;
        Lop& lop = danhSachLop.lop[viTriLop];

        int soSinhVien = demSVTrongLop(lop.dssv);
        SinhVien* mangSinhVien = (soSinhVien > 0) ? new SinhVien[soSinhVien] : nullptr;
        int viTriSinhVien = 0;
        for (PTRSV p = lop.dssv; p != nullptr; p = p->next) {
            mangSinhVien[viTriSinhVien++] = p->sv;
        }

        float* diemCaoNhat = (soSinhVien > 0 && soMonHoc > 0) ? new float[soSinhVien * soMonHoc] : nullptr;
        for (int i = 0; i < soSinhVien * soMonHoc; ++i) diemCaoNhat[i] = -1.0f;

        for (int i = 0; i < soSinhVien; ++i) {
            for (int l = 0; l < danhSachLopTinChi.soLuong; ++l) {
                LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[l];
                if (lopTinChi->huyLop) continue;
                NodeDK* dk = timDangKy(lopTinChi->dssvdk, mangSinhVien[i].maSV);
                if (dk != nullptr && !dk->dk.huyDangKy && dk->dk.diem >= 0.0f) {
                    for (int m = 0; m < soMonHoc; ++m) {
                        if (strcmp(mangMonHoc[m].maMH, lopTinChi->maMH) == 0) {
                            float& cell = diemCaoNhat[i * soMonHoc + m];
                            if (dk->dk.diem > cell) {
                                cell = dk->dk.diem;
                            }
                            break;
                        }
                    }
                }
            }
        }

        int dongDauTrang = 0;
        int soDongMoiTrang = 13;
        int tongSoTrangDong = (soSinhVien == 0) ? 1 : (soSinhVien + soDongMoiTrang - 1) / soDongMoiTrang;

        int viTriCotDau = 0;
        int soCotMoiTrang = 5;
        int dongTrangTruoc = -1;
        int viTriCotTruoc = -1;

        NutChuNhat left = { 18, 23, 20, 1, 0, "[ << CỘT TRƯỚC ]" };
        NutChuNhat nutQuayLai = { 46, 23, 24, 1, 1, "[ QUAY LẠI (ESC) ]" };
        NutChuNhat right = { 78, 23, 20, 1, 2, "[ CỘT TIẾP >> ]" };

        while (true) {
            if (dongDauTrang != dongTrangTruoc || viTriCotDau != viTriCotTruoc) {
                GiaoDienConsole::xoaManHinh();
                GiaoDienConsole::veKhung(1, 2, 114, 24, XANH_NGOC_NHAT, DEN, "BẢNG ĐIỂM TỔNG KẾT");

                GiaoDienConsole::datMau(TRANG, DEN);
                GiaoDienConsole::diDenToaDo(4, 4);
                cout << GiaoDienConsole::canGiua(string("Lớp : ") + lop.maLop + " - " + lop.tenLop, 108);

                GiaoDienConsole::datMau(VANG, LAM);
                GiaoDienConsole::diDenToaDo(4, 6);
                cout << GiaoDienConsole::canPhai("STT", 6)
                     << GiaoDienConsole::canPhai("Mã SV", 14)
                     << GiaoDienConsole::canPhai("Họ Tên", 28);

                for (int c = 0; c < soCotMoiTrang; ++c) {
                    int viTriMonHoc = viTriCotDau + c;
                    if (viTriMonHoc < soMonHoc) {
                        cout << GiaoDienConsole::canGiua(mangMonHoc[viTriMonHoc].maMH, 12);
                    } else {
                        cout << string(12, ' ');
                    }
                }

                int dongBatDau = dongDauTrang * soDongMoiTrang;
                int dongKetThuc = (dongBatDau + soDongMoiTrang < soSinhVien) ? (dongBatDau + soDongMoiTrang) : soSinhVien;

                for (int r = 0; r < soDongMoiTrang; ++r) {
                    int i = dongBatDau + r;
                    GiaoDienConsole::diDenToaDo(4, 8 + r);
                    if (i < dongKetThuc) {
                        GiaoDienConsole::datMau(TRANG, DEN);
                        string hoTen = string(mangSinhVien[i].ho) + " " + mangSinhVien[i].ten;
                        cout << GiaoDienConsole::canPhai(to_string(i + 1), 6)
                             << GiaoDienConsole::canPhai(mangSinhVien[i].maSV, 14)
                             << GiaoDienConsole::canPhai(hoTen, 28);
                        for (int c = 0; c < soCotMoiTrang; ++c) {
                            int viTriMonHoc = viTriCotDau + c;
                            if (viTriMonHoc < soMonHoc) {
                                float sc = diemCaoNhat[i * soMonHoc + viTriMonHoc];
                                string s = (sc >= 0.0f) ? dinhDangDiem(sc) : "-";
                                GiaoDienConsole::datMau(sc >= 0.0f ? XANH_LA_NHAT : XAM_DAM, DEN);
                                cout << GiaoDienConsole::canGiua(s, 12);
                            } else {
                                cout << string(12, ' ');
                            }
                        }
                    } else {
                        cout << string(108, ' ');
                    }
                }

                GiaoDienConsole::veNut(left, false, TRANG, XAM_DAM, DEN, VANG);
                GiaoDienConsole::veNut(nutQuayLai, true, TRANG, XAM_DAM, DEN, VANG);
                GiaoDienConsole::veNut(right, false, TRANG, XAM_DAM, DEN, VANG);

                GiaoDienConsole::veThanhTrangThai("Mũi tên Trái/Phải: Cuộn cột Mã MH | Lên/Xuống: Cuộn trang SV (" + to_string(dongDauTrang + 1) + "/" + to_string(tongSoTrangDong) + ") | ESC: Quay lại");
                dongTrangTruoc = dongDauTrang;
                viTriCotTruoc = viTriCotDau;
            }

            SuKienNhap suKien = GiaoDienConsole::docSuKien();
            if (suKien.type == SK_PHIM_ESC || suKien.type == SK_PHIM_ENTER) break;
            if (suKien.type == SK_CHUOT_TRAI) {
                if (nutQuayLai.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) break;
                if (left.contains(suKien.toaDoChuotX, suKien.toaDoChuotY) && viTriCotDau > 0) viTriCotDau--;
                if (right.contains(suKien.toaDoChuotX, suKien.toaDoChuotY) && viTriCotDau + soCotMoiTrang < soMonHoc) viTriCotDau++;
            } else if (suKien.type == SK_PHIM_TRAI) {
                if (viTriCotDau > 0) viTriCotDau--;
            } else if (suKien.type == SK_PHIM_PHAI) {
                if (viTriCotDau + soCotMoiTrang < soMonHoc) viTriCotDau++;
            } else if (suKien.type == SK_PHIM_XUONG || suKien.type == EVT_KEY_PGDN || suKien.type == SK_CON_LAN_XUONG) {
                if (dongDauTrang + 1 < tongSoTrangDong) dongDauTrang++;
            } else if (suKien.type == SK_PHIM_LEN || suKien.type == EVT_KEY_PGUP || suKien.type == SK_CON_LAN_LEN) {
                if (dongDauTrang > 0) dongDauTrang--;
            }
        }

        if (diemCaoNhat) delete[] diemCaoNhat;
        if (mangSinhVien) delete[] mangSinhVien;
    }

    if (mangMonHoc) delete[] mangMonHoc;
}


