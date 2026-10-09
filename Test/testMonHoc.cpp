#include "MonHoc.h"
#include "Input.h"
#include <cstring>
#include <iostream>

using namespace std;

int main() {
    PTRMH root;
    khoiTaoDanhSachMonHoc(root);

    int luaChon;

    do {
        cout << "\n=========TEST MON HOC=========\n";
        cout << "1. Them mon hoc\n";
        cout << "2. Hieu chinh mon hoc\n";
        cout << "3. Xoa mon hoc\n";
        cout << "4. Tim thong tin mon hoc\n";
        cout << "5. In danh sach mon hoc theo ma\n";
        cout << "6. In danh sach mon hoc theo ten\n";
        cout << "7. Ghi file\n";
        cout << "8. Doc file\n";
        cout << "9. Xoa tat ca mon hoc\n";
        cout << "0. Thoat chuong trinh\n"; 

        cout << "\n";
        cout << "Nhap lua chon: ";
        luaChon = nhapSoNguyenKhongAm();

        cout << "\n";
        
        switch(luaChon) {
            case 1: {
                themMonHoc(root);
                cout << "Da nhap xong!\n";
                system("pause");
                break;
            }
            case 2: {
                char maMH[MAX_MAMH];
                cout << "Nhap ma mon hoc can dieu chinh(rong de thoat): ";
                if(nhapMa(maMH, MAX_MAMH) == false) {
                    break;
                }
                if(hieuChinhMonHoc(root, maMH)) {
                    cout << "Hieu chinh thanh cong!\n";
                }
                else 
                    cout << "Khong tim thay mon hoc!\n";

                system("pause");
                break;
            }
            case 3: {
                char maMH[MAX_MAMH];
                while(true) {
                    cout << "Nhap ma mon hoc can xoa(Chuoi rong de thoat): ";
                    nhapChuoiChoPhepRong(maMH, MAX_MAMH);

                    if(strcmp(maMH, "") == 0) {
                        cout << "Thoat thanh cong!\n";
                        break;
                    }
                    if(xoaMonHoc(root, maMH)) {
                        cout << "Xoa thanh cong\n";
                    }
                    else 
                        cout << "Khong tim thay mon hoc\n";
                    cout << "\n";
                }
                system("pause");
                break;
            }
            case 4: {
                char maMH[MAX_MAMH];
                cout << "Nhap ma mon hoc can tim: ";
                nhapChuoi(maMH, MAX_MAMH);
                PTRMH found = timMonHocTheoMa(root, maMH);
                if(found) {
                    cout << "Da tim thay mon hoc\n\n";
                    inMonHoc(found->mh);
                }
                else 
                    cout << "Khong tim thay mon hoc\n";

                system("pause");
                break;
            }
            case 5: {
                if(root == nullptr) {
                    cout << "Danh sach mon hoc rong!\n";
                }
                else 
                    inDanhSachMonHocTheoMa(root);

                system("pause");
                break;
            }
            case 6: {
                if(root == nullptr) {
                    cout << "Danh sach mon hoc rong!\n";
                }
                else 
                    inDanhSachMonHocTheoTen(root);

                system("pause");
                break;
            }
            case 7: {
                if(ghiFileMonHoc(root)) {
                    cout << "Ghi du lieu ra file thanh cong!\n";
                }
                else 
                    cout << "Khong mo duoc file!\n";
                system("pause");
                break;
            }
            case 8: {
                if(docFileMonHoc(root)) {
                    cout << "Da xoa du lieu dang co!\n";
                    cout << "Doc du lieu tu file thanh cong!\n";
                }
                else 
                    cout << "Khong mo duoc file!\n";
                system("pause");
                break;
            }
            case 9: {
                if(root == nullptr) {
                    cout << "Danh sach mon hoc rong!\n";
                    system("pause");
                    break;
                }

                cout << "Ban co thuc su muon xoa tat ca mon hoc? (Y/N)\n";
                cout << "Nhap lua chon: ";
                char luaChon = nhapXacNhan();

                if(luaChon == 'Y') {
                    giaiPhongMonHoc(root);
                    cout << "Xoa danh sach thanh cong!\n";
                }
                else 
                    cout << "Xoa khong thanh cong!\n";
                
                system("pause");
                break;
            }
            case 0: {
                giaiPhongMonHoc(root);
                cout << "Thoat thanh cong\n";
                system("pause");
                break;
            }
            default: {
                cout << "Lua chon khong hop le! Chon lai!\n";
                system("pause");
                break;
            }
            
        }
        if(luaChon != 0) {
            system("cls");
        }
    } while(luaChon != 0); 
}