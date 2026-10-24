#include "quanLy.h"
#include <filesystem>
#include <iostream>

int main() {
    try {
        HeThong heThong((std::filesystem::current_path() / "data").string());
        if (!heThong.khoiDong()) return 1;
        heThong.chay();
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Loi chuong trinh: " << ex.what() << '\n';
        return 1;
    }
}
