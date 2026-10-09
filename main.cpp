#include "Menu.h"

int main() {
    PTRMH root = nullptr;
    static DanhSachLop dsLop{};
    static DanhSachLopTinChi dsLopTC{};

    menuChinh(root, dsLop, dsLopTC);

    return 0;
}