#pragma once
#include "Const.h"
struct MonHoc {
    char maMH[MAX_MAMH];
    char tenMH[MAX_TENMH];
    int soTCLT;
    int soTCTH;
};
struct NodeMH { 
    // Danh sách môn học, BST xếp theo mã
    MonHoc mh;
    NodeMH* left = nullptr;
    NodeMH* right = nullptr;
};
typedef NodeMH* PTRMH;
/* do đề có các câu thêm, xóa, chỉnh cần tìm maMH trong cây xem có
bị trùng ko, nên cây xếp theo maMH, còn 1 câu in ra theo tên thì xử lí riêng.
-> 3 ý cần mã
*/

//==================== KHỞI TẠO ====================
void khoiTaoDanhSachMonHoc(PTRMH& root);
// ==================== NHẬP ==================== 
bool nhapMonHoc(MonHoc& mh); // user nhập tt môn học, nhập rỗng thoát
void nhapThongTinHieuChinh(MonHoc& mh); // dùng cho chỉnh info, ko cho sửa maMH
// ==================== TẠO NODE ====================
PTRMH taoNodeMonHoc(const MonHoc& mh); // Các hàm sau gọi nhớ check maMH tồn tại chưa, cái này gán sv dô luôn
// ==================== TÌM KIẾM ====================
PTRMH timMonHocTheoMa(PTRMH root, const char* maMH);
// ==================== THÊM ====================
bool themMonHocVaoDSMH(PTRMH& root, const MonHoc& mh); // thêm mh vào cây bst, check trùng mã mới thêm
void themMonHoc(PTRMH& root); // gọi cái này ở menu: gồm nhập mh và thêm mh vào cây, nhập liên tục
// ==================== HIỆU CHỈNH ====================
bool hieuChinhMonHoc(PTRMH root, const char* maMH); // tìm mh theo mã và cho chỉnh (nếu có), gọi hàm nhập tt hiệu chỉnh
// ==================== XÓA ====================
PTRMH timNodeNhoNhatBenPhai(PTRMH root); // tìm node nhỏ nhất bên phải để copy sv lên node cần xóa, trong giáo trinh
bool xoaMonHoc(PTRMH& root, const char* maMH); // 
// ==================== IN ====================
void inMonHoc(const MonHoc& mh);
void inDanhSachMonHocTheoMa(PTRMH root); 
// In theo tên tăng dần -> yc của đề
void demSoMonHoc(PTRMH root, int& n);
void duaMonHocVaoMang(PTRMH root, MonHoc mh[], int& n);
bool monHocDungTruoc(const MonHoc& a, const MonHoc& b);
void inDanhSachMonHocTheoTen(PTRMH root);
// ====================Giải phóng===============
void giaiPhongMonHoc(PTRMH& root);
// ==================== FILE ====================
bool ghiFileMonHoc(PTRMH root); // hàm này chỉ gọi trả T/F
bool docFileMonHoc(PTRMH& root); // nhớ xóa hết mh trc khi đọc 