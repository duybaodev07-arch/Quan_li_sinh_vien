#include "Input.h"
#include<string>
#include <cctype>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace std;

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
    return (stream >> value) && !(stream >> kyTu) && isfinite(value);
}

[[noreturn]] void baoHetDauVao() {
    throw runtime_error("Ket thuc luong nhap (EOF)");
}
}

bool nhapMa(char* ma, int kichThuoc) {
    while (true) {
        if (!docDong(ma, kichThuoc)) {
            if (cin.eof()) return false;
            continue;
        }
        if (ma[0] == '\0') return false;

        bool hopLe = true;
        for (int i = 0; ma[i] != '\0'; ++i) {
            const unsigned char kyTu = static_cast<unsigned char>(ma[i]);
            if (isalpha(kyTu)) ma[i] = static_cast<char>(toupper(kyTu));
            else if (!isdigit(kyTu)) { hopLe = false; break; }
        }
        if (hopLe) return true;
        cout << "Ma chi duoc chua chu cai va chu so! Vui long nhap lai: ";
    }
}

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

float nhapSoThuc() {
    string input;
    float value;
    while (true) {
        if (!getline(cin, input)) baoHetDauVao();
        if (tachSoThuc(input, value)) return value;
        cout << "Sai dinh dang! Vui long nhap so thuc huu han: ";
    }
}

float nhapDiem() {
    string input;
    float diem;
    while (true) {
        if (!getline(cin, input)) baoHetDauVao();
        if (!tachSoThuc(input, diem)) {
            cout << "Sai dinh dang! Vui long nhap lai: ";
            continue;
        }
        if (diem < 0 || diem > 10) {
            cout << "Diem phai tu 0 den 10! Vui long nhap lai: ";
            continue;
        }
        return diem;
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
void nhapPhai(char* phai, int kichThuoc) {
    string input;

    while (true) {
        cout << "Nhap phai (Nam/Nu): ";
        if (!getline(cin, input)) baoHetDauVao();
        string normalized = input;
        for (char& ch : normalized) ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
        if (normalized == "nam" || normalized == "nu") {
            const char* value = normalized == "nam" ? "Nam" : "Nu";
            if (strlen(value) >= static_cast<size_t>(kichThuoc)) throw invalid_argument("Bo dem phai qua nho");
            strcpy(phai, value);
            return;
        }

        cout << "Vui long nhap Nam hoac Nu.\n";
    }
}
