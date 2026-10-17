#include "Input.h"
#include <string>
#include <cctype>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace std;

// ============================================================================
// 1. NAMESPACE ẨN: CHỨA CÁC HÀM BỔ TRỢ NỀN TẢNG (Hạn chế phạm vi nội bộ)
// ============================================================================
namespace {
bool docDong(char* buffer, int kichThuoc) {
    if (buffer == nullptr || kichThuoc <= 0) {
        throw invalid_argument("Buffer nhap khong hop le");
    }

    if (cin.getline(buffer, kichThuoc)) return true;
    if (cin.eof()) return false;

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Vuot qua so ky tu cho phep! Vui long nhap lai: ";
    return false;
}

bool tachSoNguyen(const string& input, int& value) {
    istringstream stream(input);
    char kyTu;
    return (stream >> value) && !(stream >> kyTu);
}

bool tachSoThuc(const string& input, float& value) {
    istringstream stream(input);
    char kyTu;
    return (stream >> value) && !(stream >> kyTu) && std::isfinite(value);
}

[[noreturn]] void baoHetDauVao() {
    throw runtime_error("Ket thuc luong nhap (EOF)");
}
}

// ============================================================================
// 2. NHÓM CÁC HÀM NHẬP CHUỖI CƠ BẢN & MÃ (Dùng cho Mã SV, Lớp, Môn Học, Số ĐT)
// ============================================================================
void nhapChuoi(char* str, int kichThuoc) {
    while (true) {
        if (!docDong(str, kichThuoc)) {
            if (cin.eof()) baoHetDauVao();
            continue;
        }
        if (str[0] == '\0') {
            cout << "Khong duoc de trong! Vui long nhap lai: ";
            continue;
        }
        if (strchr(str, '|') != nullptr) {
            cout << "Khong duoc chua ky tu |! Vui long nhap lai: ";
            continue;
        }
        return;
    }
}
void nhapChuoiChoPhepRong(char* str, int kichThuoc) {
    while (true) {
        if (docDong(str, kichThuoc)) {
            if (strchr(str, '|') == nullptr) return;
            cout << "Khong duoc chua ky tu |! Vui long nhap lai: ";
            continue;
        }
        if (cin.eof()) baoHetDauVao();
    }
}
void chuanHoaMa(char* ma) {
    if (ma == nullptr) return;

    for (int i = 0; ma[i] != '\0'; ++i) {
        const unsigned char kyTu = static_cast<unsigned char>(ma[i]);
        ma[i] = static_cast<char>(toupper(kyTu));
    }
}
bool laMaHopLe(const std::string& ma) {
    if (ma.empty()) return false;
    const char* kyTuMaDuocPhep = "-()+";
    for (unsigned char kyTu : ma) {
        if (std::isalnum(kyTu)) continue;
        if (std::strchr(kyTuMaDuocPhep, static_cast<int>(kyTu)) != nullptr) continue;
        return false;
    }
    return true;
}
bool laTenHopLe(const std::string& ten) {
    if (ten.empty()) return false;
    bool coKyTuKhongPhaiKhoangTrang = false;
    const char* kyTuDuocPhep = "+#-:.()";
    for (unsigned char kyTu : ten) {
        if (kyTu == '|') return false;
        if (std::isalnum(kyTu) || kyTu > 127) {
            coKyTuKhongPhaiKhoangTrang = true;
            continue;
        }
        if (kyTu == ' ') continue;
        if (std::strchr(kyTuDuocPhep, static_cast<int>(kyTu)) != nullptr) continue;
        return false;
    }
    return coKyTuKhongPhaiKhoangTrang;
}
bool nhapMa(char* ma, int kichThuoc) {
    while (true) {
        if (!docDong(ma, kichThuoc)) {
            if (cin.eof()) return false;
            continue;
        }
        if (ma[0] == '\0') return false;

        if (laMaHopLe(ma)) {
            chuanHoaMa(ma);
            return true;
        }
        cout << "Ma chi chua chu, so va cac ky tu -()+! Vui long nhap lai: ";
    }
}
void nhapMaBatBuoc(char* ma, int kichThuoc) {
    while (true) {
        if (nhapMa(ma, kichThuoc)) {
            return;
        }
        if (cin.eof()) {
            baoHetDauVao();
        }
        cout << "Ma khong duoc de trong! Vui long nhap lai: ";
    }
}
bool laSoDienThoaiHopLe(const char* soDT) {
    if (soDT == nullptr || soDT[0] == '\0') return false;
    for (int i = 0; soDT[i] != '\0'; ++i) {
        if (soDT[i] < '0' || soDT[i] > '9') return false;
    }
    return true;
}
void nhapSoDienThoai(char* soDT, int kichThuoc) {
    while (true) {
        nhapChuoi(soDT, kichThuoc);
        if (laSoDienThoaiHopLe(soDT)) return;
        cout << "So dien thoai chi duoc chua chu so! Vui long nhap lai: ";
    }
}

// ============================================================================
// 3. NHÓM CÁC HÀM NHẬP TÊN CHUYÊN BIỆT (Tên Môn Học, Họ Tên Sinh Viên)
// ============================================================================

void nhapTen(char* str, int kichThuoc) {
    while (true) {
        nhapChuoi(str, kichThuoc);
        if (laTenHopLe(str)) return;
        cout << "Ten mon hoc chua ky tu khong hop le! (Chi cho phep chu, so, khoang trang va +#-:.()). Vui long nhap lai: ";
    }
}
void nhapHoTenSinhVien(char* str, int kichThuoc) {
    while (true) {
        nhapChuoi(str, kichThuoc); 

        bool hopLe = true;
        for (int i = 0; str[i] != '\0'; ++i) {
            const unsigned char kyTu = static_cast<unsigned char>(str[i]);
            
            if (kyTu == ' ' || kyTu > 127 || isalpha(kyTu)) {
                continue;
            }
            hopLe = false;
            break;
        }

        if (hopLe) return; 
        
        cout << "Ho ten sinh vien chi duoc chua chu cai va khoang trang! Vui long nhap lai: ";
    }
}

// ============================================================================
// 4. NHÓM CÁC HÀM LOGIC ĐẶC THÙ (Niên Khóa, Phái)
// ============================================================================

void nhapNienKhoa(char* nienKhoa, int kichThuoc) {
    if (nienKhoa == nullptr || kichThuoc < 10) {
        throw invalid_argument("Bo dem nien khoa phai co it nhat 10 ky tu");
    }
    string input;

    while (true) {
        cout << "Nhap nien khoa (YYYY-YYYY): ";
        if (!getline(cin, input)) baoHetDauVao();

        bool dungDinhDang = (input.length() == 9 && input[4] == '-');
        for (int i = 0; dungDinhDang && i < 9; ++i) {
            if (i == 4) continue;
            dungDinhDang = isdigit(static_cast<unsigned char>(input[i])) != 0;
        }

        if (!dungDinhDang) {
            cout << "Nien khoa phai co dang YYYY-YYYY.\n";
            continue;
        }

        const int namTruoc = stoi(input.substr(0, 4));
        const int namSau = stoi(input.substr(5, 4));
        if (namSau < namTruoc) {
            cout << "Nam sau khong duoc nho hon nam truoc.\n";
            continue;
        }

        if (input.length() >= static_cast<size_t>(kichThuoc)) {
            throw invalid_argument("Bo dem nien khoa qua nho");
        }

        strcpy(nienKhoa, input.c_str());
        return;
    }
}


void nhapPhai(char* phai, int kichThuoc) {
    if (phai == nullptr || kichThuoc <= 0) {
        throw invalid_argument("Bo dem phai khong hop le");
    }
    string input;
    while (true) {
        cout << "Nhap phai (Nam/Nu): ";
        if (!getline(cin, input)) baoHetDauVao();
        
        string normalized = input;
        for (char& ch : normalized) ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
        
        if (normalized == "nam" || normalized == "nu") {
            const char* value = (normalized == "nam") ? "Nam" : "Nu";
            if (strlen(value) >= static_cast<size_t>(kichThuoc)) {
                throw invalid_argument("Bo dem phai qua nho");
            }
            strcpy(phai, value);
            return;
        }
        cout << "Vui long nhap Nam hoac Nu.\n";
    }
}

// ============================================================================
// 5. NHÓM CÁC HÀM SỐ HỌC (Số Nguyên, Số Thực, Điểm)
// ============================================================================
int nhapSoNguyenKhongAm() {
    string input;
    int value;
    while (true) {
        if (!getline(cin, input)) baoHetDauVao();
        if (!tachSoNguyen(input, value)) {
            cout << "Sai dinh dang! Vui long nhap so nguyen: ";
            continue;
        }
        if (value < 0) {
            cout << "Gia tri khong duoc am! Vui long nhap lai: ";
            continue;
        }
        return value;
    }
}

int nhapSoNguyenTrongKhoang(int min, int max) {
    if (min > max) throw invalid_argument("Khoang gia tri khong hop le");
    string input;
    int value;
    while (true) {
        if (!getline(cin, input)) baoHetDauVao();
        if (!tachSoNguyen(input, value)) {
            cout << "Sai dinh dang! Vui long nhap lai: ";
            continue;
        }
        if (value < min || value > max) {
            cout << "Gia tri phai tu " << min << " den " << max << "! Vui long nhap lai: ";
            continue;
        }
        return value;
    }
}
float nhapSoThuc() {
    string input;
    float value;
    while (true) {
        if (!getline(cin, input)) baoHetDauVao();
        if (tachSoThuc(input, value)) return value;
        cout << "Sai dinh dang! Vui long nhap so thuc huu han: ";
    }
}
float nhapDiem(float minDiem, float maxDiem) {
    if (!std::isfinite(minDiem) || !std::isfinite(maxDiem) || minDiem > maxDiem) {
        throw invalid_argument("Khoang diem khong hop le");
    }
    string input;
    float diem;
    while (true) {
        if (!getline(cin, input)) baoHetDauVao();
        if (!tachSoThuc(input, diem)) {
            cout << "Sai dinh dang! Vui long nhap lai: ";
            continue;
        }
        if (diem < minDiem || diem > maxDiem) {
            cout << "Diem phai nam trong khoang tu " << minDiem << " den " << maxDiem << "! Vui long nhap lai: ";
            continue;
        }
        return diem;
    }
}

// ============================================================================
// 6. NHÓM CÁC HÀM XÁC NHẬN LỰA CHỌN (Y/N, Trạng Thái Hủy)
// ============================================================================
char nhapXacNhan() {
    string input;
    while (true) {
        if (!getline(cin, input)) baoHetDauVao();
        if (input.length() == 1) {
            const char luaChon = static_cast<char>(toupper(static_cast<unsigned char>(input[0])));
            if (luaChon == 'Y' || luaChon == 'N') return luaChon;
        }
        cout << "Vui long chi nhap Y hoac N: ";
    }
}
bool nhapTrangThaiHuy() {
    return nhapXacNhan() == 'Y';
}
