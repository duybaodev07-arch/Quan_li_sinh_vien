#include "storage/DocGhiDuLieu.h"

#include <fstream>
#include <sstream>
#include "model/CauTrucDuLieu.h"

using namespace std;

const char* FILE_MONHOC = "data/data_monhoc.txt";
const char* FILE_LOP_SV = "data/data_lop_sinhvien.txt";
const char* FILE_LOPTC_DK = "data/data_loptinchi_dangky.txt";

// Duyệt NLR để ghi cây BST môn học ra file
void ghiMonHocNLR(PTRMH cay, ofstream& out) {
    if (cay == nullptr) return;
    out << cay->duLieu.MAMH << "|"
        << cay->duLieu.TENMH << "|"
        << cay->duLieu.STCLT << "|"
        << cay->duLieu.STCTH << "\n";
    ghiMonHocNLR(cay->nutTrai, out);
    ghiMonHocNLR(cay->nutPhai, out);
}

bool luuTatCaDuLieu(PTRMH cayMonHoc, const DSLop& danhSachLop, const DSLopTinChi& danhSachLopTinChi) {
    // 1. Lưu danh sách môn học
    {
        ofstream out(FILE_MONHOC);
        if (!out.is_open()) return false;
        int soMH = demMonHoc(cayMonHoc);
        out << soMH << "\n";
        ghiMonHocNLR(cayMonHoc, out);
        out.close();
    }

    // 2. Lưu danh sách lớp và sinh viên của từng lớp
    {
        ofstream out(FILE_LOP_SV);
        if (!out.is_open()) return false;
        out << danhSachLop.n << "\n";
        for (int i = 0; i < danhSachLop.n; ++i) {
            int soSV = demSVTrongLop(danhSachLop.cacNutDuLieu[i].dssv);
            out << danhSachLop.cacNutDuLieu[i].MALOP << "|" << danhSachLop.cacNutDuLieu[i].TENLOP << "|" << soSV << "\n";
            for (PTRSV p = danhSachLop.cacNutDuLieu[i].dssv; p != nullptr; p = p->tiepTheo) {
                out << p->duLieu.MASV << "|"
                    << p->duLieu.HO << "|"
                    << p->duLieu.TEN << "|"
                    << p->duLieu.PHAI << "|"
                    << p->duLieu.SODT << "\n";
            }
        }
        out.close();
    }

    // 3. Lưu danh sách lớp tín chỉ và danh sách đăng ký
    {
        ofstream out(FILE_LOPTC_DK);
        if (!out.is_open()) return false;
        out << danhSachLopTinChi.n << "|" << danhSachLopTinChi.nextMaLopTC << "\n";
        for (int i = 0; i < danhSachLopTinChi.n; ++i) {
            LopTinChi* lopTinChi = danhSachLopTinChi.cacNutDuLieu[i];
            int soDangKy = demTongDangKy(lopTinChi->dsSinhVienDangKy);
            out << lopTinChi->MALOPTC << "|"
                << lopTinChi->MAMH << "|"
                << lopTinChi->nienKhoa << "|"
                << lopTinChi->hocKy << "|"
                << lopTinChi->nhom << "|"
                << lopTinChi->soSvMin << "|"
                << lopTinChi->soSvMax << "|"
                << (lopTinChi->huyLop ? 1 : 0) << "|"
                << soDangKy << "\n";
            for (PTRDK p = lopTinChi->dsSinhVienDangKy; p != nullptr; p = p->tiepTheo) {
                out << p->duLieu.MASV << "|"
                    << p->duLieu.DIEM << "|"
                    << (p->duLieu.huyDangKy ? 1 : 0) << "\n";
            }
        }
        out.close();
    }

    return true;
}

// Tách chuỗi theo ký tự phân cách '|' vào mảng động string*
int tachChuoi(const string& dong, char kyTuPhanCach, string* cacMuc, int soMucToiDa) {
    int count = 0;
    string dongDaLamSach = dong;
    while (!dongDaLamSach.empty() && (dongDaLamSach.back() == '\r' || dongDaLamSach.back() == '\n')) {
        dongDaLamSach.pop_back();
    }
    stringstream ss(dongDaLamSach);
    string muc;
    while (getline(ss, muc, kyTuPhanCach) && count < soMucToiDa) {
        while (!muc.empty() && muc.back() == '\r') muc.pop_back();
        cacMuc[count++] = muc;
    }
    return count;
}

// Khởi tạo dữ liệu mẫu phong phú khi chạy lần đầu
void khoiTaoDuLieuMau(PTRMH& cayMonHoc, DSLop& danhSachLop, DSLopTinChi& danhSachLopTinChi) {
    // 1. Môn học mẫu (BST)
    themMonHoc(cayMonHoc, MonHoc("INT1306", "Cau truc du lieu va giai thuat", 3, 1));
    themMonHoc(cayMonHoc, MonHoc("INT1154", "Tin hoc co so 2", 2, 1));
    themMonHoc(cayMonHoc, MonHoc("INT1332", "Lap trinh huong doi tuong", 2, 1));
    themMonHoc(cayMonHoc, MonHoc("INT1313", "Co so du lieu", 3, 1));
    themMonHoc(cayMonHoc, MonHoc("INT1336", "Mang may tinh", 3, 0));
    themMonHoc(cayMonHoc, MonHoc("INT1319", "He dieu hanh", 2, 1));
    themMonHoc(cayMonHoc, MonHoc("BAS1203", "Giai tich 1", 3, 0));
    themMonHoc(cayMonHoc, MonHoc("BAS1201", "Dai so tuyen tinh", 3, 0));

    // 2. Lớp học & Sinh viên mẫu
    danhSachLop.cacNutDuLieu[danhSachLop.n++] = Lop("D22CQCN01", "Cong nghe thong tin 01 - K22");
    themSVCoThuTu(danhSachLop.cacNutDuLieu[0].dssv, SinhVien("N22DCCN005", "Nguyen Van", "An", "Nam", "0901234501"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[0].dssv, SinhVien("N22DCCN001", "Tran Thi", "Bich", "Nu", "0901234502"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[0].dssv, SinhVien("N22DCCN008", "Le Hoang", "Cuong", "Nam", "0901234503"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[0].dssv, SinhVien("N22DCCN003", "Pham Minh", "Dung", "Nam", "0901234504"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[0].dssv, SinhVien("N22DCCN002", "Hoang Thu", "Ha", "Nu", "0901234505"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[0].dssv, SinhVien("N22DCCN009", "Vu Quang", "Huy", "Nam", "0901234506"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[0].dssv, SinhVien("N22DCCN004", "Dang Ngoc", "Lan", "Nu", "0901234507"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[0].dssv, SinhVien("N22DCCN007", "Bui Duc", "Minh", "Nam", "0901234508"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[0].dssv, SinhVien("N22DCCN006", "Doan Thanh", "Tuan", "Nam", "0901234509"));

    danhSachLop.cacNutDuLieu[danhSachLop.n++] = Lop("D22CQCN02", "Cong nghe thong tin 02 - K22");
    themSVCoThuTu(danhSachLop.cacNutDuLieu[1].dssv, SinhVien("N22DCCN102", "Nguyen Hoang", "Anh", "Nam", "0912345601"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[1].dssv, SinhVien("N22DCCN105", "Le Thi", "Mai", "Nu", "0912345602"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[1].dssv, SinhVien("N22DCCN101", "Tran Quoc", "Khanh", "Nam", "0912345603"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[1].dssv, SinhVien("N22DCCN104", "Pham Gia", "Phong", "Nam", "0912345604"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[1].dssv, SinhVien("N22DCCN103", "Vo Thi", "Thao", "Nu", "0912345605"));

    danhSachLop.cacNutDuLieu[danhSachLop.n++] = Lop("D22CQAT01", "An toan thong tin 01 - K22");
    themSVCoThuTu(danhSachLop.cacNutDuLieu[2].dssv, SinhVien("N22DCAT002", "Ly Minh", "Khoa", "Nam", "0923456701"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[2].dssv, SinhVien("N22DCAT001", "Trinh Bao", "Ngoc", "Nu", "0923456702"));
    themSVCoThuTu(danhSachLop.cacNutDuLieu[2].dssv, SinhVien("N22DCAT003", "Ngo Tuan", "Kiet", "Nam", "0923456703"));

    // 3. Lớp tín chỉ & Đăng ký mẫu
    auto taoLTC = [&](const char* maMH, const char* nienKhoa, int hocKy, int nhom, int soSinhVienToiThieu, int soSinhVienToiDa) -> LopTinChi* {
        LopTinChi* lopTinChi = new LopTinChi();
        lopTinChi->MALOPTC = danhSachLopTinChi.nextMaLopTC++;
        strncpy(lopTinChi->MAMH, maMH, 10); lopTinChi->MAMH[10] = '\0';
        strncpy(lopTinChi->nienKhoa, nienKhoa, 11); lopTinChi->nienKhoa[11] = '\0';
        lopTinChi->hocKy = hocKy;
        lopTinChi->nhom = nhom;
        lopTinChi->soSvMin = soSinhVienToiThieu;
        lopTinChi->soSvMax = soSinhVienToiDa;
        lopTinChi->huyLop = false;
        danhSachLopTinChi.cacNutDuLieu[danhSachLopTinChi.n++] = lopTinChi;
        return lopTinChi;
    };

    // LTC 1: INT1306 (CTDL&GT) - 2025-2026 - HK1 - Nhóm 1
    LopTinChi* lopTinChi1 = taoLTC("INT1306", "2025-2026", 1, 1, 3, 50);
    themDangKy(lopTinChi1->dsSinhVienDangKy, DangKy("N22DCCN001", 8.5f, false));
    themDangKy(lopTinChi1->dsSinhVienDangKy, DangKy("N22DCCN002", 7.5f, false));
    themDangKy(lopTinChi1->dsSinhVienDangKy, DangKy("N22DCCN003", 9.0f, false));
    themDangKy(lopTinChi1->dsSinhVienDangKy, DangKy("N22DCCN004", 6.5f, false));
    themDangKy(lopTinChi1->dsSinhVienDangKy, DangKy("N22DCCN005", 8.0f, false));
    themDangKy(lopTinChi1->dsSinhVienDangKy, DangKy("N22DCCN006", 7.0f, true)); // Đã hủy ĐK

    // LTC 2: INT1332 (LTHĐT) - 2025-2026 - HK1 - Nhóm 1
    LopTinChi* lopTinChi2 = taoLTC("INT1332", "2025-2026", 1, 1, 3, 45);
    themDangKy(lopTinChi2->dsSinhVienDangKy, DangKy("N22DCCN001", 9.0f, false));
    themDangKy(lopTinChi2->dsSinhVienDangKy, DangKy("N22DCCN002", 8.0f, false));
    themDangKy(lopTinChi2->dsSinhVienDangKy, DangKy("N22DCCN005", 4.5f, false)); // Điểm thấp, học lại ở HK2
    themDangKy(lopTinChi2->dsSinhVienDangKy, DangKy("N22DCCN101", 8.5f, false));

    // LTC 3: INT1313 (CSDL) - 2025-2026 - HK1 - Nhóm 1
    LopTinChi* lopTinChi3 = taoLTC("INT1313", "2025-2026", 1, 1, 3, 50);
    themDangKy(lopTinChi3->dsSinhVienDangKy, DangKy("N22DCCN001", 7.5f, false));
    themDangKy(lopTinChi3->dsSinhVienDangKy, DangKy("N22DCCN003", 8.5f, false));
    themDangKy(lopTinChi3->dsSinhVienDangKy, DangKy("N22DCCN005", 9.0f, false));
    themDangKy(lopTinChi3->dsSinhVienDangKy, DangKy("N22DCCN007", 8.0f, false));

    // LTC 4: INT1336 (Mạng MT) - 2025-2026 - HK1 - Nhóm 2 (Số SV ĐK = 1 < soSvMin = 5 -> để test chức năng Hủy lớp TC)
    LopTinChi* lopTinChi4 = taoLTC("INT1336", "2025-2026", 1, 2, 5, 40);
    themDangKy(lopTinChi4->dsSinhVienDangKy, DangKy("N22DCCN008", -1.0f, false));

    // LTC 5: INT1332 (LTHĐT) - 2025-2026 - HK2 - Nhóm 1 (SV N22DCCN005 học cải thiện điểm lên 8.5)
    LopTinChi* lopTinChi5 = taoLTC("INT1332", "2025-2026", 2, 1, 2, 40);
    themDangKy(lopTinChi5->dsSinhVienDangKy, DangKy("N22DCCN005", 8.5f, false));
    themDangKy(lopTinChi5->dsSinhVienDangKy, DangKy("N22DCCN008", 7.0f, false));
    themDangKy(lopTinChi5->dsSinhVienDangKy, DangKy("N22DCCN009", 8.0f, false));

    luuTatCaDuLieu(cayMonHoc, danhSachLop, danhSachLopTinChi);
}

void docTatCaDuLieu(PTRMH& cayMonHoc, DSLop& danhSachLop, DSLopTinChi& danhSachLopTinChi) {
    ifstream tepMonHoc(FILE_MONHOC);
    ifstream tepLop(FILE_LOP_SV);
    ifstream tepLopTinChi(FILE_LOPTC_DK);

    if (!tepMonHoc.is_open() || !tepLop.is_open() || !tepLopTinChi.is_open()) {
        khoiTaoDuLieuMau(cayMonHoc, danhSachLop, danhSachLopTinChi);
        return;
    }

    string* cacMuc = new string[12];
    string dong;

    // 1. Đọc danh sách môn học
    if (getline(tepMonHoc, dong) && !dong.empty()) {
        int soMH = atoi(dong.c_str());
        for (int i = 0; i < soMH; ++i) {
            if (!getline(tepMonHoc, dong) || dong.empty()) break;
            if (tachChuoi(dong, '|', cacMuc, 10) >= 4) {
                MonHoc mh(cacMuc[0].c_str(), cacMuc[1].c_str(), atoi(cacMuc[2].c_str()), atoi(cacMuc[3].c_str()));
                themMonHoc(cayMonHoc, mh);
            }
        }
    }
    tepMonHoc.close();

    // 2. Đọc danh sách lớp & sinh viên
    if (getline(tepLop, dong) && !dong.empty()) {
        int soLop = atoi(dong.c_str());
        for (int i = 0; i < soLop && danhSachLop.n < MAX_LOP; ++i) {
            if (!getline(tepLop, dong) || dong.empty()) break;
            if (tachChuoi(dong, '|', cacMuc, 10) >= 3) {
                string maLop = cacMuc[0];
                string tenLop = cacMuc[1];
                int soSV = atoi(cacMuc[2].c_str());
                int idx = danhSachLop.n++;
                danhSachLop.cacNutDuLieu[idx] = Lop(maLop, tenLop);
                for (int j = 0; j < soSV; ++j) {
                    if (!getline(tepLop, dong) || dong.empty()) break;
                    if (tachChuoi(dong, '|', cacMuc, 10) >= 5) {
                        SinhVien sv(cacMuc[0], cacMuc[1], cacMuc[2], cacMuc[3], cacMuc[4]);
                        themSVCoThuTu(danhSachLop.cacNutDuLieu[idx].dssv, sv);
                    }
                }
            }
        }
    }
    tepLop.close();

    // 3. Đọc danh sách lớp tín chỉ & đăng ký
    if (getline(tepLopTinChi, dong) && !dong.empty()) {
        if (tachChuoi(dong, '|', cacMuc, 10) >= 2) {
            int soLTC = atoi(cacMuc[0].c_str());
            danhSachLopTinChi.nextMaLopTC = atoi(cacMuc[1].c_str());
            for (int i = 0; i < soLTC && danhSachLopTinChi.n < MAX_LOPTC; ++i) {
                if (!getline(tepLopTinChi, dong) || dong.empty()) break;
                if (tachChuoi(dong, '|', cacMuc, 12) >= 9) {
                    LopTinChi* lopTinChi = new LopTinChi();
                    lopTinChi->MALOPTC = atoi(cacMuc[0].c_str());
                    strncpy(lopTinChi->MAMH, cacMuc[1].c_str(), 10); lopTinChi->MAMH[10] = '\0';
                    strncpy(lopTinChi->nienKhoa, cacMuc[2].c_str(), 11); lopTinChi->nienKhoa[11] = '\0';
                    lopTinChi->hocKy = atoi(cacMuc[3].c_str());
                    lopTinChi->nhom = atoi(cacMuc[4].c_str());
                    lopTinChi->soSvMin = atoi(cacMuc[5].c_str());
                    lopTinChi->soSvMax = atoi(cacMuc[6].c_str());
                    lopTinChi->huyLop = (atoi(cacMuc[7].c_str()) != 0);
                    int soDangKy = atoi(cacMuc[8].c_str());

                    if (lopTinChi->MALOPTC >= danhSachLopTinChi.nextMaLopTC) {
                        danhSachLopTinChi.nextMaLopTC = lopTinChi->MALOPTC + 1;
                    }

                    for (int j = 0; j < soDangKy; ++j) {
                        if (!getline(tepLopTinChi, dong) || dong.empty()) break;
                        if (tachChuoi(dong, '|', cacMuc, 10) >= 3) {
                            string maSV = cacMuc[0];
                            float diem = (float)atof(cacMuc[1].c_str());
                            bool huy = (atoi(cacMuc[2].c_str()) != 0);
                            // Nối vào cuối DSLK
                            if (lopTinChi->dsSinhVienDangKy == nullptr) {
                                lopTinChi->dsSinhVienDangKy = new NodeDK(DangKy(maSV, diem, huy));
                            } else {
                                PTRDK p = lopTinChi->dsSinhVienDangKy;
                                while (p->tiepTheo != nullptr) p = p->tiepTheo;
                                p->tiepTheo = new NodeDK(DangKy(maSV, diem, huy));
                            }
                        }
                    }
                    danhSachLopTinChi.cacNutDuLieu[danhSachLopTinChi.n++] = lopTinChi;
                }
            }
        }
    }
    tepLopTinChi.close();

    delete[] cacMuc;
}




