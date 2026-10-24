# Quan ly diem sinh vien theo he tin chi

Chuong trinh C++17 quan ly mon hoc, lop, sinh vien, lop tin chi, dang ky va diem thi. Du lieu duoc luu tu dong trong thu muc `data` tinh tu thu muc chay chuong trinh.

## Cau truc

- `include/`: khai bao lop du lieu va giao dien cac module.
- `src/`: cai dat nhap lieu, nghiep vu, luu doc file va chuong trinh chinh.
- `data/`: tu dong tao cac file `MONHOC.txt`, `LOP.txt`, `LOPTC.txt` khi luu lan dau.

## Bien dich tren Windows (MinGW-w64)

Mo PowerShell tai thu muc project va chay lenh:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/*.cpp -o QuanLySinhVien.exe
.\QuanLySinhVien.exe
```

Hoac chay `build.bat` bang MinGW-w64. Chay file exe tu thu muc goc project de chuong trinh tim dung thu muc `data`.

## Chuc nang

Quan ly them/xoa/hieu chinh mon hoc, lop, sinh vien va lop tin chi; dang ky va huy dang ky; tu dong huy lop khong du si so sau khi xac nhan; nhap diem; in danh sach dang ky, bang diem mon hoc, diem trung binh theo tin chi va bang diem tong ket theo lop. Danh sach sinh vien trong lop duoc giu theo thu tu ten tang dan; bang tong ket lay diem thi cao nhat cua tung sinh vien cho moi mon.

Sinh vien co lich su dang ky khong bi xoa khoi du lieu de tranh lam dut khoa tham chieu cua bang diem. Lop tin chi co lich su dang ky cung duoc bao ve tuong tu.
