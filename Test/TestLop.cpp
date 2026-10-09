#include <iostream>
#include "Input.h"
#include "SinhVien.h"
#include "Lop.h"
using namespace std;

int main() {
    static DanhSachLop dsLop; // tránh crack
    khoiTaoDanhSachLop(dsLop);
    PTRSV head;
    khoiTaoDanhSachSinhVien(head);
    int luaChon;
    do {
        cout << "\n===========TEST LOP==========\n";
        cout << "1. Them lop\n";
        cout << "2. Cap nhat sinh vien 1 lop\n";
        cout << "3. Hieu chinh lop\n";
        cout << "4. Xoa lop\n";
        cout << "5. Tim lop\n";
        cout << "6. Tim sinh vien trong danh sach lop\n";
        cout << "7. In danh sach lop\n";
        cout << "8. Xoa tat ca lop trong danh sach\n";
        cout << "9. Ghi file\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "\n";
        cout << "Nhap lua chon: ";
        luaChon = nhapSoNguyenKhongAm();
        cout << "\n";
        switch(luaChon) {
            case 1: {
                Lop lop;
                nhapLop(lop);

                if(themLop(dsLop, lop)) {
                    cout << "Them lop thanh cong!\n";
                    cout << "Nhap sinh vien them vao danh sach: \n\n";
                }
                else 
                    cout << "Them lop that bai!\n";

                themSinhVien(lop.dssv);
                
                system("pause");
                break;
            }
            case 2: {
                
            }
            case 3: {
                cout << "Nhap ma lop can cap nhat: ";

                char maLop[MAX_LOP];

                nhapChuoi(maLop, MAX_MALOP);
                if(hieuChinhLop(dsLop, maLop) == true) {
                    cout << "Hieu chinh lop thanh cong!\n";
                }
                else 
                    cout << "Khong tim thay ma lop!\n";

                break;
            }
            
        }
        if(luaChon == 0) {
            system("cls");
        }
    } while(luaChon != 0);
}