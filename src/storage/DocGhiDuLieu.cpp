#include "storage/DocGhiDuLieu.h"

#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cmath>
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
            if (i + 1 < danhSachLop.soLuong) out << "\n";
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
            if (i + 1 < danhSachLopTinChi.soLuong) out << "\n";
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

// Đọc dòng tiếp theo có nội dung, bỏ qua các dòng trống dùng để phân nhóm dữ liệu.
bool docDongKhongRong(ifstream& tep, string& dong) {
    while (getline(tep, dong)) {
        while (!dong.empty() && dong.back() == '\r') dong.pop_back();
        if (dong.find_first_not_of(" \t\r\n") != string::npos) return true;
    }
    return false;
}

static bool docSoNguyen(const string& chuoi, int& giaTri) {
    if (chuoi.empty()) return false;
    char* cuoi = nullptr;
    long ketQua = strtol(chuoi.c_str(), &cuoi, 10);
    if (cuoi == chuoi.c_str() || *cuoi != '\0' || ketQua < 0 || ketQua > 100000) return false;
    giaTri = static_cast<int>(ketQua);
    return true;
}

static bool docDiem(const string& chuoi, float& giaTri) {
    if (chuoi.empty()) return false;
    char* cuoi = nullptr;
    float ketQua = strtof(chuoi.c_str(), &cuoi);
    if (cuoi == chuoi.c_str() || *cuoi != '\0' || !std::isfinite(ketQua) ||
        (ketQua < 0.0f && ketQua != -1.0f) || ketQua > 10.0f) return false;
    giaTri = ketQua;
    return true;
}

static int demTruong(const string& dong) {
    return 1 + static_cast<int>(count(dong.begin(), dong.end(), '|'));
}

static bool dongConDuLieu(ifstream& tep) {
    string dong;
    while (getline(tep, dong)) {
        while (!dong.empty() && dong.back() == '\r') dong.pop_back();
        if (dong.find_first_not_of(" \t\r\n") != string::npos) return true;
    }
    return false;
}

bool docTatCaDuLieu(PTRMH& root, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi, string& thongBaoLoi) {
    ifstream tepMonHoc(FILE_MONHOC);
    ifstream tepLop(FILE_LOP_SV);
    ifstream tepLopTinChi(FILE_LOPTC_DK);

    auto loiVaXoaDuLieuDocDo = [&](const string& noiDung) {
        thongBaoLoi = noiDung;
        giaiPhongBST(root);
        giaiPhongDanhSachLop(danhSachLop);
        giaiPhongDanhSachLopTinChi(danhSachLopTinChi);
        return false;
    };

    if (!tepMonHoc.is_open()) return loiVaXoaDuLieuDocDo(string("Không mở được ") + FILE_MONHOC);
    if (!tepLop.is_open()) return loiVaXoaDuLieuDocDo(string("Không mở được ") + FILE_LOP_SV);
    if (!tepLopTinChi.is_open()) return loiVaXoaDuLieuDocDo(string("Không mở được ") + FILE_LOPTC_DK);

    string cacMuc[12];
    string dong;
    int soLuong = 0;
    int soTruong = 0;

    // 1. Đọc danh sách môn học
    if (!docDongKhongRong(tepMonHoc, dong) || !docSoNguyen(dong, soLuong))
        return loiVaXoaDuLieuDocDo("Sai dòng đếm môn học trong data/data_monhoc.txt.");
    for (int i = 0; i < soLuong; ++i) {
        if (!docDongKhongRong(tepMonHoc, dong))
            return loiVaXoaDuLieuDocDo("Thiếu dòng môn học trong data/data_monhoc.txt.");
        soTruong = tachChuoi(dong, '|', cacMuc, 12);
        int tinChiLyThuyet = 0, tinChiThucHanh = 0;
        if (demTruong(dong) != 4 || soTruong != 4 || cacMuc[0].empty() || cacMuc[1].empty() ||
            cacMuc[0].size() >= MAX_MAMH || cacMuc[1].size() >= MAX_TENMH ||
            !docSoNguyen(cacMuc[2], tinChiLyThuyet) || !docSoNguyen(cacMuc[3], tinChiThucHanh) ||
            tinChiLyThuyet + tinChiThucHanh <= 0) {
            return loiVaXoaDuLieuDocDo("Sai định dạng bản ghi môn học trong data/data_monhoc.txt.");
        }
        MonHoc monHoc = taoMonHoc(cacMuc[0].c_str(), cacMuc[1].c_str(), tinChiLyThuyet, tinChiThucHanh);
        if (!themMonHoc(root, monHoc))
            return loiVaXoaDuLieuDocDo("Mã môn học bị trùng trong data/data_monhoc.txt.");
    }
    if (dongConDuLieu(tepMonHoc)) return loiVaXoaDuLieuDocDo("Có dữ liệu thừa trong data/data_monhoc.txt.");
    tepMonHoc.close();

    // 2. Đọc danh sách lớp & sinh viên
    if (!docDongKhongRong(tepLop, dong) || !docSoNguyen(dong, soLuong) || soLuong > MAX_LOP)
        return loiVaXoaDuLieuDocDo("Sai dòng đếm lớp trong data/data_lop_sinhvien.txt.");
    for (int i = 0; i < soLuong; ++i) {
        if (!docDongKhongRong(tepLop, dong))
            return loiVaXoaDuLieuDocDo("Thiếu dòng thông tin lớp trong data/data_lop_sinhvien.txt.");
        soTruong = tachChuoi(dong, '|', cacMuc, 12);
        int soSV = 0;
        if (demTruong(dong) != 3 || soTruong != 3 || cacMuc[0].empty() || cacMuc[1].empty() ||
            cacMuc[0].size() >= MAX_MALOP || cacMuc[1].size() >= MAX_TENLOP ||
            !docSoNguyen(cacMuc[2], soSV)) {
            return loiVaXoaDuLieuDocDo("Sai định dạng dòng lớp trong data/data_lop_sinhvien.txt.");
        }
        for (int lop = 0; lop < danhSachLop.soLuong; ++lop) {
            if (strcmp(danhSachLop.lop[lop].maLop, cacMuc[0].c_str()) == 0)
                return loiVaXoaDuLieuDocDo("Mã lớp bị trùng trong data/data_lop_sinhvien.txt.");
        }
        int idx = danhSachLop.soLuong++;
        danhSachLop.lop[idx] = taoLop(cacMuc[0].c_str(), cacMuc[1].c_str());
        for (int j = 0; j < soSV; ++j) {
            if (!docDongKhongRong(tepLop, dong))
                return loiVaXoaDuLieuDocDo("Thiếu dòng sinh viên trong data/data_lop_sinhvien.txt.");
            soTruong = tachChuoi(dong, '|', cacMuc, 12);
            if (demTruong(dong) != 5 || soTruong != 5 || cacMuc[0].empty() || cacMuc[1].empty() ||
                cacMuc[2].empty() || cacMuc[3].empty() || cacMuc[4].empty() ||
                cacMuc[0].size() >= MAX_MASV || cacMuc[1].size() >= MAX_HO ||
                cacMuc[2].size() >= MAX_TEN || cacMuc[3].size() >= MAX_PHAI || cacMuc[4].size() >= MAX_SODT ||
                timSinhVienToanTruong(danhSachLop, cacMuc[0].c_str()) != nullptr) {
                return loiVaXoaDuLieuDocDo("Sai định dạng hoặc trùng mã sinh viên trong data/data_lop_sinhvien.txt.");
            }
            SinhVien sinhVien = taoSinhVien(cacMuc[0].c_str(), cacMuc[1].c_str(), cacMuc[2].c_str(), cacMuc[3].c_str(), cacMuc[4].c_str());
            themSVCoThuTu(danhSachLop.lop[idx].dssv, sinhVien);
        }
    }
    if (dongConDuLieu(tepLop)) return loiVaXoaDuLieuDocDo("Có dữ liệu thừa trong data/data_lop_sinhvien.txt.");
    tepLop.close();

    // 3. Đọc danh sách lớp tín chỉ & đăng ký
    if (!docDongKhongRong(tepLopTinChi, dong))
        return loiVaXoaDuLieuDocDo("Thiếu dòng đếm lớp tín chỉ trong data/data_loptinchi_dangky.txt.");
    soTruong = tachChuoi(dong, '|', cacMuc, 12);
    int maLopTCKeTiep = 0;
    if (demTruong(dong) != 2 || soTruong != 2 || !docSoNguyen(cacMuc[0], soLuong) || soLuong > MAX_LOP_TC ||
        !docSoNguyen(cacMuc[1], maLopTCKeTiep) || maLopTCKeTiep < 1)
        return loiVaXoaDuLieuDocDo("Sai dòng đếm lớp tín chỉ trong data/data_loptinchi_dangky.txt.");

    int maLopTCMax = 0;
    for (int i = 0; i < soLuong; ++i) {
        if (!docDongKhongRong(tepLopTinChi, dong))
            return loiVaXoaDuLieuDocDo("Thiếu dòng thông tin lớp tín chỉ trong data/data_loptinchi_dangky.txt.");
        soTruong = tachChuoi(dong, '|', cacMuc, 12);
        int maLopTC = 0, hocKy = 0, nhom = 0, minSV = 0, maxSV = 0, daHuy = 0, soDangKy = 0;
        if (demTruong(dong) != 9 || soTruong != 9 ||
            !docSoNguyen(cacMuc[0], maLopTC) || maLopTC < 1 ||
            cacMuc[1].empty() || cacMuc[1].size() >= MAX_MAMH || timMonHoc(root, cacMuc[1].c_str()) == nullptr ||
            cacMuc[2].empty() || cacMuc[2].size() >= MAX_NIENKHOA ||
            !docSoNguyen(cacMuc[3], hocKy) || hocKy < 1 || hocKy > 3 ||
            !docSoNguyen(cacMuc[4], nhom) || nhom < 1 ||
            !docSoNguyen(cacMuc[5], minSV) || !docSoNguyen(cacMuc[6], maxSV) || maxSV < minSV ||
            !docSoNguyen(cacMuc[7], daHuy) || daHuy > 1 ||
            !docSoNguyen(cacMuc[8], soDangKy)) {
            return loiVaXoaDuLieuDocDo("Sai định dạng dòng lớp tín chỉ trong data/data_loptinchi_dangky.txt.");
        }
        if (timLopTinChiTheoMa(danhSachLopTinChi, maLopTC) != -1)
            return loiVaXoaDuLieuDocDo("Mã lớp tín chỉ bị trùng trong data/data_loptinchi_dangky.txt.");

        LopTinChi* lopTinChi = new LopTinChi();
        lopTinChi->maLopTC = maLopTC;
        strncpy(lopTinChi->maMH, cacMuc[1].c_str(), MAX_MAMH - 1);
        strncpy(lopTinChi->nienKhoa, cacMuc[2].c_str(), MAX_NIENKHOA - 1);
        lopTinChi->hocKy = hocKy;
        lopTinChi->nhom = nhom;
        lopTinChi->minSV = minSV;
        lopTinChi->maxSV = maxSV;
        lopTinChi->huyLop = daHuy != 0;

        for (int j = 0; j < soDangKy; ++j) {
            if (!docDongKhongRong(tepLopTinChi, dong)) {
                giaiPhongDSDK(lopTinChi->dssvdk);
                delete lopTinChi;
                return loiVaXoaDuLieuDocDo("Thiếu dòng đăng ký trong data/data_loptinchi_dangky.txt.");
            }
            soTruong = tachChuoi(dong, '|', cacMuc, 12);
            float diem = 0.0f;
            int huyDangKy = 0;
            if (demTruong(dong) != 3 || soTruong != 3 || cacMuc[0].empty() || cacMuc[0].size() >= MAX_MASV ||
                !docDiem(cacMuc[1], diem) || !docSoNguyen(cacMuc[2], huyDangKy) || huyDangKy > 1) {
                giaiPhongDSDK(lopTinChi->dssvdk);
                delete lopTinChi;
                return loiVaXoaDuLieuDocDo("Sai định dạng bản ghi đăng ký trong data/data_loptinchi_dangky.txt.");
            }
            NodeDK* nutMoi = new NodeDK{taoDangKy(cacMuc[0].c_str(), diem, huyDangKy != 0), nullptr};
            if (lopTinChi->dssvdk == nullptr) lopTinChi->dssvdk = nutMoi;
            else {
                PTRDK p = lopTinChi->dssvdk;
                while (p->next != nullptr) p = p->next;
                p->next = nutMoi;
            }
        }
        danhSachLopTinChi.lopTC[danhSachLopTinChi.soLuong++] = lopTinChi;
        if (maLopTC > maLopTCMax) maLopTCMax = maLopTC;
    }
    if (maLopTCKeTiep <= maLopTCMax)
        return loiVaXoaDuLieuDocDo("Mã lớp tín chỉ kế tiếp không hợp lệ trong data/data_loptinchi_dangky.txt.");
    if (dongConDuLieu(tepLopTinChi)) return loiVaXoaDuLieuDocDo("Có dữ liệu thừa trong data/data_loptinchi_dangky.txt.");
    tepLopTinChi.close();

    thongBaoLoi.clear();
    return true;
}




