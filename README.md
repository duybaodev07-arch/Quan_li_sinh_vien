# Quan ly sinh vien

Du an C++17 dung `char[]` va `struct`.

## Build bang CMake

Mo PowerShell tai thu muc du an va chay (neu da cai CMake):

```powershell
cmake -S . -B build
cmake --build build
.\build\QuanLySinhVien.exe
```

Voi VS Code va MinGW o `C:\mingw64`, bam `Ctrl+Shift+B` de build ca chín file `.cpp`. Chay chuong trinh khi thu muc lam viec la thu muc du an de du lieu trong `data/` duoc doc va luu dung.

Du lieu mon hoc, lop/sinh vien va lop tin chi duoc luu lan luot tai `data/MONHOC.txt`, `data/LOP.txt`, va `data/LOPTC.txt`. Chuong trinh doc khi khoi dong va luu khi thoat. Khong sua truc tiep cac file nay khi chuong trinh dang chay.

## Cau truc

- `include/`: khai bao struct, hang so va ham.
- `src/`: cai dat va `main`.
- `data/`: du lieu mau.
- `CMakeLists.txt`: cau hinh build toan bo chuong trinh.
- `.vscode/`: task build va IntelliSense.
