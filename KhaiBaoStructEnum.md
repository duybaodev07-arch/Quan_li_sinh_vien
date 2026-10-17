# Khai báo `struct` và `enum`

Tài liệu này tổng hợp các khai báo tìm thấy trong `include/`. Mã được giữ theo định nghĩa gốc và chia theo file nguồn.

**Lưu ý:** Đây là tài liệu tham khảo, không phải header để include vào chương trình. Các kiểu đã được khai báo trong header gốc; include lại nội dung này sẽ gây định nghĩa trùng.

## `include/SinhVien.h`

```cpp
struct SinhVien {
    char maSV[MAX_MASV];
    char ho[MAX_HO];
    char ten[MAX_TEN];
    char phai[MAX_PHAI];
    char soDT[MAX_SODT];
};

struct NodeSV {
    // dslk don, xếp theo tên
    SinhVien sv;
    NodeSV* next = nullptr;
};
```

## `include/DangKi.h`

```cpp
struct DangKy {
    char maSV[MAX_MASV];
    float diem;
    bool huyDangKy;
};

struct NodeDK {
    DangKy dk;
    NodeDK* next = nullptr;
};
```

## `include/MonHoc.h`

```cpp
struct MonHoc {
    char maMH[MAX_MAMH];
    char tenMH[MAX_TENMH];
    int soTCLT;
    int soTCTH;
};

struct NodeMH {
    MonHoc mh;
    NodeMH* left = nullptr;
    NodeMH* right = nullptr;
};

enum class KetQuaDocMonHoc {
    ThanhCong,
    KhongTimThayFile,
    KhongMoDuocFile,
    SaiDinhDang,
    MaMonHocTrung,
    TinChiNgoaiKhoang,
    LoiDocFile
};
```

## `include/Lop.h`

```cpp
struct Lop {
    char maLop[MAX_MALOP];
    char tenLop[MAX_TENLOP];
    PTRSV dssv; // trỏ đến đầu dssv(head)
};

struct DanhSachLop {
    Lop lop[MAX_LOP];
    int soLuong;
};
```

## `include/LopTinChi.h`

```cpp
struct LopTinChi {
    int maLopTC = 0;
    char maMH[MAX_MAMH] = "";
    char nienKhoa[MAX_NIENKHOA] = "";
    int hocKy = 0;
    int nhom = 0;
    int minSV = 0;
    int maxSV = 0;
    bool huyLop = false;
    PTRDK dssvdk = nullptr;
};

struct DanhSachLopTinChi {
    PTRLTC lopTC[MAX_LOP_TC] = {};
    int soLuong = 0;
};
```

## Các kiểu con trỏ liên quan

Các bí danh này không phải `struct` hay `enum`, nhưng được dùng trong một số trường của các cấu trúc ở trên. Giữ đúng kiểu khai báo trong mã nguồn:

```cpp
// include/SinhVien.h
typedef NodeSV* PTRSV;

// include/MonHoc.h
typedef NodeMH* PTRMH;

// include/DangKi.h
using PTRDK = NodeDK*;

// include/LopTinChi.h
using PTRLTC = LopTinChi*;
```