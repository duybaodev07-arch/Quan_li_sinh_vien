#include "nhapLieu.h"
#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>

std::string chuanHoaKhoangTrang(const std::string& chuoi) {
    std::istringstream input(chuoi);
    std::string tu, ketQua;
    while (input >> tu) { if (!ketQua.empty()) ketQua += ' '; ketQua += tu; }
    return ketQua;
}

std::string docChuoi(const std::string& loiNhac, bool choPhepRong) {
    while (true) {
        std::cout << loiNhac;
        std::string ketQua;
        if (!std::getline(std::cin, ketQua)) return {};
        ketQua = chuanHoaKhoangTrang(ketQua);
        if (choPhepRong || !ketQua.empty()) return ketQua;
        std::cout << "Du lieu khong duoc de trong. Vui long nhap lai.\n";
    }
}

std::string docChuoiGioiHan(const std::string& loiNhac, std::size_t doDaiToiDa) {
    while (true) {
        std::string ketQua = docChuoi(loiNhac);
        if (ketQua.empty()) return {};
        if (ketQua.size() <= doDaiToiDa) return ketQua;
        std::cout << "Du lieu qua dai, toi da " << doDaiToiDa << " byte.\n";
    }
}

bool nhapMa(std::string& ma, int doDaiToiDa, const std::string& loiNhac, bool choPhepRong) {
    while (true) {
        std::cout << loiNhac;
        std::string tam;
        if (!std::getline(std::cin, tam)) return false;
        tam = chuanHoaKhoangTrang(tam);
        if (tam.empty() && choPhepRong) { ma.clear(); return false; }
        bool hopLe = !tam.empty() && tam.size() <= static_cast<std::size_t>(doDaiToiDa);
        for (char& c : tam) {
            const unsigned char uc = static_cast<unsigned char>(c);
            if (uc >= 128) { hopLe = false; break; }
            if (!std::isalnum(uc)) { hopLe = false; break; }
            c = static_cast<char>(std::toupper(uc));
        }
        if (hopLe) { ma = tam; return true; }
        std::cout << "Ma chi gom chu va so, toi da " << doDaiToiDa << " ky tu.\n";
    }
}

bool nhapSoNguyen(int& giaTri, int min, int max, const std::string& loiNhac) {
    if (min > max) return false;
    while (true) {
        std::cout << loiNhac;
        std::string dong;
        if (!std::getline(std::cin, dong)) return false;
        std::istringstream input(dong);
        long long tam;
        char du;
        if ((input >> tam) && !(input >> du) && tam >= min && tam <= max) { giaTri = static_cast<int>(tam); return true; }
        std::cout << "Vui long nhap so nguyen trong khoang [" << min << ", " << max << "].\n";
    }
}

bool nhapSoThuc(double& giaTri, double min, double max, const std::string& loiNhac) {
    while (true) {
        std::cout << loiNhac;
        std::string dong;
        if (!std::getline(std::cin, dong)) return false;
        std::istringstream input(dong);
        double tam;
        char du;
        if ((input >> tam) && !(input >> du) && std::isfinite(tam) && tam >= min && tam <= max) { giaTri = tam; return true; }
        std::cout << "Vui long nhap so trong khoang [" << min << ", " << max << "].\n";
    }
}

bool xacNhan(const std::string& loiNhac) {
    while (true) {
        std::cout << loiNhac << " (y/n): ";
        std::string dong;
        if (!std::getline(std::cin, dong)) return false;
        if (dong == "y" || dong == "Y") return true;
        if (dong == "n" || dong == "N") return false;
        std::cout << "Chi nhap y hoac n.\n";
    }
}
void dungManHinh() { std::cout << "\nNhan Enter de tiep tuc..."; std::string dong; std::getline(std::cin, dong); }
