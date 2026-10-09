#include "Input.h"
#include <sstream>
#include <iostream>
#include <cstring>
#include <limits>
#include <cctype>

using namespace std;

bool nhapMa(char* ma, int kichThuoc) {
    while (true) {
        cin.getline(ma, kichThuoc);

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Vuot qua so ky tu cho phep! Vui long nhap lai: ";
            continue;
        }

        if (strlen(ma) == 0) {
            return false;
        }

        bool hopLe = true;

        for (int i = 0; ma[i] != '\0'; i++) {
            if (isalpha((unsigned char)ma[i])) {
                ma[i] = toupper((unsigned char)ma[i]);
            }
            else if (!isdigit((unsigned char)ma[i])) {
                hopLe = false;
                break;
            }
        }
        if (!hopLe) {
            cout << "Ma chi duoc chua chu cai va chu so! Vui long nhap lai: ";
            continue;
        }

        break;
    }
    return true;
}
void nhapChuoi(char* str, int kichThuoc) {
    while (true) {
        cin.getline(str, kichThuoc);

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Vuot qua so ky tu cho phep! Vui long nhap lai: ";
            continue;
        }

        if (strlen(str) == 0) {
            cout << "Khong duoc de trong! Vui long nhap lai: ";
            continue;
        }

        break;
    }
}
void nhapChuoiChoPhepRong(char* str, int kichThuoc) {
    while (true) {
        cin.getline(str, kichThuoc);

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Vuot qua so ky tu cho phep! Vui long nhap lai: ";
            continue;
        }
        break;
    }
}
int nhapSoNguyenKhongAm() {
    string input;
    int value;
    char kyTu;

    while (true) {
        getline(cin, input);
        stringstream ss(input);

        if (!(ss >> value) || (ss >> kyTu)) {
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
    char kyTu;

    while (true) {
        getline(cin, input);
        stringstream ss(input);

        if (!(ss >> value) || (ss >> kyTu)) {
            cout << "Sai dinh dang! Vui long nhap so thuc: ";
            continue;
        }

        return value;
    }
}
float nhapDiem() {
    float diem;

    while (true) {
        cin >> diem;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Sai dinh dang! Vui long nhap lai: ";
            continue;
        }

        if (diem < 0 || diem > 10) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Diem phai tu 0 den 10! Vui long nhap lai: ";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return diem;
    }
}
bool laSoDienThoaiHopLe(const char* soDT) {
    if (strlen(soDT) == 0)
        return false;

    for (int i = 0; soDT[i] != '\0'; i++) {
        if (soDT[i] < '0' || soDT[i] > '9')
            return false;
    }

    return true;
} 
void nhapSoDienThoai(char* soDT, int maxLength) {
    while (true) {
        nhapChuoi(soDT, maxLength);
        if (!laSoDienThoaiHopLe(soDT)) {
            cout << "So dien thoai chi duoc chua chu so! Vui long nhap lai: ";
            continue;
        }
        break;
    }
}
int nhapSoNguyenTrongKhoang(int min, int max) {
    int value;

    while (true) {
        cin >> value;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Sai dinh dang! Vui long nhap lai: ";
            continue;
        }

        if (value < min || value > max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Gia tri phai tu " << min << " den " << max << "! Vui long nhap lai: ";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}   
char nhapXacNhan() {
    string input;

    while (true) {
        getline(cin, input);

        if (input.length() == 1) {
            char luaChon = toupper((unsigned char)input[0]);

            if (luaChon == 'Y' || luaChon == 'N') {
                return luaChon;
            }
        }

        cout << "Vui long chi nhap Y hoac N: ";
    }
}