#include "storage/DocGhiDuLieu.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"

using namespace std;

const char* FILE_MONHOC = "data/data_monhoc.txt";
const char* FILE_LOP_SV = "data/data_lop_sinhvien.txt";
const char* FILE_LOPTC_DK = "data/data_loptinchi_dangky.txt";

// Duyệt NLR để ghi cây BST môn học ra file
void ghiMonHocNLR(PTRMH root, ofstream& out) {
    if (root == nullptr) return;
    out << root->mh.maMH << "|"
        << root->mh.tenMH << "|"
        << root->mh.soTCLT << "|"
        << root->mh.soTCTH << "\n";
    ghiMonHocNLR(root->left, out);
    ghiMonHocNLR(root->right, out);
}

bool luuTatCaDuLieu(PTRMH root, const DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi) {
    // 1. Lưu danh sách môn học
    {
        ofstream out(FILE_MONHOC);
        if (!out.is_open()) return false;
        int soMH = demMonHoc(root);
        out << soMH << "\n";
        ghiMonHocNLR(root, out);
        out.close();
    }

    // 2. Lưu danh sách lớp và sinh viên của từng lớp
    {
        ofstream out(FILE_LOP_SV);
        if (!out.is_open()) return false;
        out << danhSachLop.soLuong << "\n";
        for (int i = 0; i < danhSachLop.soLuong; ++i) {
            int soSV = demSVTrongLop(danhSachLop.lop[i].dssv);
            out << danhSachLop.lop[i].maLop << "|" << danhSachLop.lop[i].tenLop << "|" << soSV << "\n";
            for (PTRSV p = danhSachLop.lop[i].dssv; p != nullptr; p = p->next) {
                out << p->sv.maSV << "|"
                    << p->sv.ho << "|"
                    << p->sv.ten << "|"
                    << p->sv.phai << "|"
                    << p->sv.soDT << "\n";
            }
        }
        out.close();
    }

    // 3. Lưu danh sách lớp tín chỉ và danh sách đăng ký
    {
        ofstream out(FILE_LOPTC_DK);
        if (!out.is_open()) return false;
        out << danhSachLopTinChi.soLuong << "|" << layMaLopTCKeTiep(danhSachLopTinChi) << "\n";
        for (int i = 0; i < danhSachLopTinChi.soLuong; ++i) {
            LopTinChi* lopTinChi = danhSachLopTinChi.lopTC[i];
            int soDangKy = demTongDangKy(lopTinChi->dssvdk);
            out << lopTinChi->maLopTC << "|"
                << lopTinChi->maMH << "|"
                << lopTinChi->nienKhoa << "|"
                << lopTinChi->hocKy << "|"
                << lopTinChi->nhom << "|"
                << lopTinChi->minSV << "|"
                << lopTinChi->maxSV << "|"
                << (lopTinChi->huyLop ? 1 : 0) << "|"
                << soDangKy << "\n";
            for (PTRDK p = lopTinChi->dssvdk; p != nullptr; p = p->next) {
                out << p->dk.maSV << "|"
                    << p->dk.diem << "|"
                    << (p->dk.huyDangKy ? 1 : 0) << "\n";
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
void khoiTaoDuLieuMau(PTRMH& root, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi) {
    // 1. Môn học mẫu (BST)
    themMonHoc(root, taoMonHoc("INT1306", "Cau truc du lieu va giai thuat", 3, 1));
    themMonHoc(root, taoMonHoc("INT1154", "Tin hoc co so 2", 2, 1));
    themMonHoc(root, taoMonHoc("INT1332", "Lap trinh huong doi tuong", 2, 1));
    themMonHoc(root, taoMonHoc("INT1313", "Co so du lieu", 3, 1));
    themMonHoc(root, taoMonHoc("INT1336", "Mang may tinh", 3, 0));
    themMonHoc(root, taoMonHoc("INT1319", "He dieu hanh", 2, 1));
    themMonHoc(root, taoMonHoc("BAS1203", "Giai tich 1", 3, 0));
    themMonHoc(root, taoMonHoc("BAS1201", "Dai so tuyen tinh", 3, 0));

    // 2. Lớp học & Sinh viên mẫu
    danhSachLop.lop[danhSachLop.soLuong++] = taoLop("D22CQCN01", "Cong nghe thong tin 01 - K22");
    themSVCoThuTu(danhSachLop.lop[0].dssv, taoSinhVien("N22DCCN005", "Nguyen Van", "An", "Nam", "0901234501"));
    themSVCoThuTu(danhSachLop.lop[0].dssv, taoSinhVien("N22DCCN001", "Tran Thi", "Bich", "Nu", "0901234502"));
    themSVCoThuTu(danhSachLop.lop[0].dssv, taoSinhVien("N22DCCN008", "Le Hoang", "Cuong", "Nam", "0901234503"));
    themSVCoThuTu(danhSachLop.lop[0].dssv, taoSinhVien("N22DCCN003", "Pham Minh", "Dung", "Nam", "0901234504"));
    themSVCoThuTu(danhSachLop.lop[0].dssv, taoSinhVien("N22DCCN002", "Hoang Thu", "Ha", "Nu", "0901234505"));
    themSVCoThuTu(danhSachLop.lop[0].dssv, taoSinhVien("N22DCCN009", "Vu Quang", "Huy", "Nam", "0901234506"));
    themSVCoThuTu(danhSachLop.lop[0].dssv, taoSinhVien("N22DCCN004", "Dang Ngoc", "Lan", "Nu", "0901234507"));
    themSVCoThuTu(danhSachLop.lop[0].dssv, taoSinhVien("N22DCCN007", "Bui Duc", "Minh", "Nam", "0901234508"));
    themSVCoThuTu(danhSachLop.lop[0].dssv, taoSinhVien("N22DCCN006", "Doan Thanh", "Tuan", "Nam", "0901234509"));

    danhSachLop.lop[danhSachLop.soLuong++] = taoLop("D22CQCN02", "Cong nghe thong tin 02 - K22");
    themSVCoThuTu(danhSachLop.lop[1].dssv, taoSinhVien("N22DCCN102", "Nguyen Hoang", "Anh", "Nam", "0912345601"));
    themSVCoThuTu(danhSachLop.lop[1].dssv, taoSinhVien("N22DCCN105", "Le Thi", "Mai", "Nu", "0912345602"));
    themSVCoThuTu(danhSachLop.lop[1].dssv, taoSinhVien("N22DCCN101", "Tran Quoc", "Khanh", "Nam", "0912345603"));
    themSVCoThuTu(danhSachLop.lop[1].dssv, taoSinhVien("N22DCCN104", "Pham Gia", "Phong", "Nam", "0912345604"));
    themSVCoThuTu(danhSachLop.lop[1].dssv, taoSinhVien("N22DCCN103", "Vo Thi", "Thao", "Nu", "0912345605"));

    danhSachLop.lop[danhSachLop.soLuong++] = taoLop("D22CQAT01", "An toan thong tin 01 - K22");
    themSVCoThuTu(danhSachLop.lop[2].dssv, taoSinhVien("N22DCAT002", "Ly Minh", "Khoa", "Nam", "0923456701"));
    themSVCoThuTu(danhSachLop.lop[2].dssv, taoSinhVien("N22DCAT001", "Trinh Bao", "Ngoc", "Nu", "0923456702"));
    themSVCoThuTu(danhSachLop.lop[2].dssv, taoSinhVien("N22DCAT003", "Ngo Tuan", "Kiet", "Nam", "0923456703"));

    // 3. Lớp tín chỉ & Đăng ký mẫu
    auto taoLTC = [&](const char* maMH, const char* nienKhoa, int hocKy, int nhom, int soSinhVienToiThieu, int soSinhVienToiDa) -> LopTinChi* {
        LopTinChi* lopTinChi = new LopTinChi();
        lopTinChi->maLopTC = layMaLopTCKeTiep(danhSachLopTinChi);
        strncpy(lopTinChi->maMH, maMH, MAX_MAMH - 1); lopTinChi->maMH[MAX_MAMH - 1] = '\0';
        strncpy(lopTinChi->nienKhoa, nienKhoa, MAX_NIENKHOA - 1); lopTinChi->nienKhoa[MAX_NIENKHOA - 1] = '\0';
        lopTinChi->hocKy = hocKy;
        lopTinChi->nhom = nhom;
        lopTinChi->minSV = soSinhVienToiThieu;
        lopTinChi->maxSV = soSinhVienToiDa;
        lopTinChi->huyLop = false;
        danhSachLopTinChi.lopTC[danhSachLopTinChi.soLuong++] = lopTinChi;
        return lopTinChi;
    };

    // LTC 1: INT1306 (CTDL&GT) - 2025-2026 - HK1 - Nhóm 1
    LopTinChi* lopTinChi1 = taoLTC("INT1306", "2025-2026", 1, 1, 3, 50);
    themDangKy(lopTinChi1->dssvdk, taoDangKy("N22DCCN001", 8.5f, false));
    themDangKy(lopTinChi1->dssvdk, taoDangKy("N22DCCN002", 7.5f, false));
    themDangKy(lopTinChi1->dssvdk, taoDangKy("N22DCCN003", 9.0f, false));
    themDangKy(lopTinChi1->dssvdk, taoDangKy("N22DCCN004", 6.5f, false));
    themDangKy(lopTinChi1->dssvdk, taoDangKy("N22DCCN005", 8.0f, false));
    themDangKy(lopTinChi1->dssvdk, taoDangKy("N22DCCN006", 7.0f, true)); // Đã hủy ĐK

    // LTC 2: INT1332 (LTHĐT) - 2025-2026 - HK1 - Nhóm 1
    LopTinChi* lopTinChi2 = taoLTC("INT1332", "2025-2026", 1, 1, 3, 45);
    themDangKy(lopTinChi2->dssvdk, taoDangKy("N22DCCN001", 9.0f, false));
    themDangKy(lopTinChi2->dssvdk, taoDangKy("N22DCCN002", 8.0f, false));
    themDangKy(lopTinChi2->dssvdk, taoDangKy("N22DCCN005", 4.5f, false)); // Điểm thấp, học lại ở HK2
    themDangKy(lopTinChi2->dssvdk, taoDangKy("N22DCCN101", 8.5f, false));

    // LTC 3: INT1313 (CSDL) - 2025-2026 - HK1 - Nhóm 1
    LopTinChi* lopTinChi3 = taoLTC("INT1313", "2025-2026", 1, 1, 3, 50);
    themDangKy(lopTinChi3->dssvdk, taoDangKy("N22DCCN001", 7.5f, false));
    themDangKy(lopTinChi3->dssvdk, taoDangKy("N22DCCN003", 8.5f, false));
    themDangKy(lopTinChi3->dssvdk, taoDangKy("N22DCCN005", 9.0f, false));
    themDangKy(lopTinChi3->dssvdk, taoDangKy("N22DCCN007", 8.0f, false));

    // LTC 4: INT1336 (Mạng MT) - 2025-2026 - HK1 - Nhóm 2 (Số SV ĐK = 1 < minSV = 5 -> để test chức năng Hủy lớp TC)
    LopTinChi* lopTinChi4 = taoLTC("INT1336", "2025-2026", 1, 2, 5, 40);
    themDangKy(lopTinChi4->dssvdk, taoDangKy("N22DCCN008", -1.0f, false));

    // LTC 5: INT1332 (LTHĐT) - 2025-2026 - HK2 - Nhóm 1 (SV N22DCCN005 học cải thiện điểm lên 8.5)
    LopTinChi* lopTinChi5 = taoLTC("INT1332", "2025-2026", 2, 1, 2, 40);
    themDangKy(lopTinChi5->dssvdk, taoDangKy("N22DCCN005", 8.5f, false));
    themDangKy(lopTinChi5->dssvdk, taoDangKy("N22DCCN008", 7.0f, false));
    themDangKy(lopTinChi5->dssvdk, taoDangKy("N22DCCN009", 8.0f, false));

    luuTatCaDuLieu(root, danhSachLop, danhSachLopTinChi);
}

void docTatCaDuLieu(PTRMH& root, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi) {
    ifstream tepMonHoc(FILE_MONHOC);
    ifstream tepLop(FILE_LOP_SV);
    ifstream tepLopTinChi(FILE_LOPTC_DK);

    if (!tepMonHoc.is_open() || !tepLop.is_open() || !tepLopTinChi.is_open()) {
        khoiTaoDuLieuMau(root, danhSachLop, danhSachLopTinChi);
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
                MonHoc mh = taoMonHoc(cacMuc[0], cacMuc[1], atoi(cacMuc[2].c_str()), atoi(cacMuc[3].c_str()));
                themMonHoc(root, mh);
            }
        }
    }
    tepMonHoc.close();

    // 2. Đọc danh sách lớp & sinh viên
    if (getline(tepLop, dong) && !dong.empty()) {
        int soLop = atoi(dong.c_str());
        for (int i = 0; i < soLop && danhSachLop.soLuong < MAX_LOP; ++i) {
            if (!getline(tepLop, dong) || dong.empty()) break;
            if (tachChuoi(dong, '|', cacMuc, 10) >= 3) {
                string maLop = cacMuc[0];
                string tenLop = cacMuc[1];
                int soSV = atoi(cacMuc[2].c_str());
                int idx = danhSachLop.soLuong++;
                danhSachLop.lop[idx] = taoLop(maLop, tenLop);
                for (int j = 0; j < soSV; ++j) {
                    if (!getline(tepLop, dong) || dong.empty()) break;
                    if (tachChuoi(dong, '|', cacMuc, 10) >= 5) {
                        SinhVien sv = taoSinhVien(cacMuc[0], cacMuc[1], cacMuc[2], cacMuc[3], cacMuc[4]);
                        themSVCoThuTu(danhSachLop.lop[idx].dssv, sv);
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
            
            for (int i = 0; i < soLTC && danhSachLopTinChi.soLuong < MAX_LOP_TC; ++i) {
                if (!getline(tepLopTinChi, dong) || dong.empty()) break;
                if (tachChuoi(dong, '|', cacMuc, 12) >= 9) {
                    LopTinChi* lopTinChi = new LopTinChi();
                    lopTinChi->maLopTC = atoi(cacMuc[0].c_str());
                    strncpy(lopTinChi->maMH, cacMuc[1].c_str(), MAX_MAMH - 1); lopTinChi->maMH[MAX_MAMH - 1] = '\0';
                    strncpy(lopTinChi->nienKhoa, cacMuc[2].c_str(), MAX_NIENKHOA - 1); lopTinChi->nienKhoa[MAX_NIENKHOA - 1] = '\0';
                    lopTinChi->hocKy = atoi(cacMuc[3].c_str());
                    lopTinChi->nhom = atoi(cacMuc[4].c_str());
                    lopTinChi->minSV = atoi(cacMuc[5].c_str());
                    lopTinChi->maxSV = atoi(cacMuc[6].c_str());
                    lopTinChi->huyLop = (atoi(cacMuc[7].c_str()) != 0);
                    int soDangKy = atoi(cacMuc[8].c_str());

                    for (int j = 0; j < soDangKy; ++j) {
                        if (!getline(tepLopTinChi, dong) || dong.empty()) break;
                        if (tachChuoi(dong, '|', cacMuc, 10) >= 3) {
                            string maSV = cacMuc[0];
                            float diem = (float)atof(cacMuc[1].c_str());
                            bool huy = (atoi(cacMuc[2].c_str()) != 0);
                            // Nối vào cuối DSLK
                            if (lopTinChi->dssvdk == nullptr) {
                                lopTinChi->dssvdk = new NodeDK{taoDangKy(maSV, diem, huy), nullptr};
                            } else {
                                PTRDK p = lopTinChi->dssvdk;
                                while (p->next != nullptr) p = p->next;
                                p->next = new NodeDK{taoDangKy(maSV, diem, huy), nullptr};
                            }
                        }
                    }
                    danhSachLopTinChi.lopTC[danhSachLopTinChi.soLuong++] = lopTinChi;
                }
            }
        }
    }
    tepLopTinChi.close();

    delete[] cacMuc;
}




