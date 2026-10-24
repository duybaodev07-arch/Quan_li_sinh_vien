#include "duLieu.h"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

namespace fs = std::filesystem;

bool ghiDuLieu(const std::string& thuMuc, const std::vector<MonHoc>& dsMonHoc, const DanhSachLop& dsLop, const DanhSachLopTinChi& dsLopTC) {
    std::error_code ec;
    fs::create_directories(thuMuc, ec);
    if (ec) { std::cerr << "Khong tao duoc thu muc du lieu: " << ec.message() << '\n'; return false; }
    const fs::path goc(thuMuc);
    const fs::path mhTmp = goc / "MONHOC.tmp", lopTmp = goc / "LOP.tmp", ltcTmp = goc / "LOPTC.tmp";
    {
        std::ofstream out(mhTmp, std::ios::binary | std::ios::trunc);
        if (!out) return false;
        out << dsMonHoc.size() << '\n';
        for (const auto& mh : dsMonHoc) out << std::quoted(mh.getMaMH()) << ' ' << std::quoted(mh.getTenMH()) << ' '
            << mh.getSoTCLT() << ' ' << mh.getSoTCTH() << '\n';
        if (!out) return false;
    }
    {
        std::ofstream out(lopTmp, std::ios::binary | std::ios::trunc);
        if (!out) return false;
        out << dsLop.getDanhSach().size() << '\n';
        for (const auto& lop : dsLop.getDanhSach()) {
            auto dssv = taoMangSinhVien(lop.getDSSV());
            out << std::quoted(lop.getMaLop()) << ' ' << std::quoted(lop.getTenLop()) << ' ' << dssv.size() << '\n';
            for (const auto& sv : dssv) out << std::quoted(sv.getMaSV()) << ' ' << std::quoted(sv.getHo()) << ' '
                << std::quoted(sv.getTen()) << ' ' << std::quoted(sv.getPhai()) << ' ' << std::quoted(sv.getSoDT()) << '\n';
        }
        if (!out) return false;
    }
    {
        std::ofstream out(ltcTmp, std::ios::binary | std::ios::trunc);
        if (!out) return false;
        out << dsLopTC.getDanhSach().size() << '\n';
        for (const auto& lop : dsLopTC.getDanhSach()) {
            out << lop.getMaLopTC() << ' ' << std::quoted(lop.getMaMH()) << ' ' << std::quoted(lop.getNienKhoa()) << ' '
                << lop.getHocKy() << ' ' << lop.getNhom() << ' ' << lop.getMinSV() << ' ' << lop.getMaxSV() << ' '
                << lop.getHuyLop() << ' ' << lop.getDSSVDK().size() << '\n';
            for (const auto& dk : lop.getDSSVDK()) out << std::quoted(dk.getMaSV()) << ' ' << std::setprecision(17)
                << dk.getDiem() << ' ' << dk.getHuyDangKy() << '\n';
        }
        if (!out) return false;
    }
    for (const auto& ten : {"MONHOC", "LOP", "LOPTC"}) {
        fs::path tmp = goc / (std::string(ten) + ".tmp");
        fs::path dst = goc / (std::string(ten) + ".txt");
        fs::rename(tmp, dst, ec);
        if (ec) { ec.clear(); fs::remove(dst, ec); ec.clear(); fs::rename(tmp, dst, ec); }
        if (ec) { std::cerr << "Khong ghi duoc " << dst.string() << ": " << ec.message() << '\n'; return false; }
    }
    return true;
}

bool docDuLieu(const std::string& thuMuc, std::vector<MonHoc>& dsMonHoc, DanhSachLop& dsLop, DanhSachLopTinChi& dsLopTC) {
    const fs::path goc(thuMuc);
    const fs::path mhFile = goc / "MONHOC.txt", lopFile = goc / "LOP.txt", ltcFile = goc / "LOPTC.txt";
    if (!fs::exists(mhFile) && !fs::exists(lopFile) && !fs::exists(ltcFile)) return true;
    std::vector<MonHoc> mhTam;
    DanhSachLop lopTam;
    DanhSachLopTinChi ltcTam;
    {
        std::ifstream in(mhFile, std::ios::binary);
        std::size_t n = 0;
        if (!in || !(in >> n) || n > 100000) { std::cerr << "File MONHOC.txt bi loi.\n"; return false; }
        for (std::size_t i = 0; i < n; ++i) {
            std::string ma, ten; unsigned tclt, tcth;
            if (!(in >> std::quoted(ma) >> std::quoted(ten) >> tclt >> tcth) || tclt > 1000 || tcth > 1000 || !themMonHoc(mhTam, MonHoc(ma, ten, tclt, tcth))) return false;
        }
    }
    {
        std::ifstream in(lopFile, std::ios::binary);
        std::size_t n = 0;
        if (!in || !(in >> n) || n > 10000) { std::cerr << "File LOP.txt bi loi.\n"; return false; }
        for (std::size_t i = 0; i < n; ++i) {
            std::string maLop, tenLop; std::size_t soSV;
            if (!(in >> std::quoted(maLop) >> std::quoted(tenLop) >> soSV) || soSV > 100000) return false;
            Lop lop(maLop, tenLop);
            for (std::size_t j = 0; j < soSV; ++j) {
                std::string maSV, ho, ten, phai, soDT;
                if (!(in >> std::quoted(maSV) >> std::quoted(ho) >> std::quoted(ten) >> std::quoted(phai) >> std::quoted(soDT)) || !lop.themSinhVien(SinhVien(maSV, ho, ten, phai, soDT))) return false;
            }
            if (!lopTam.themLop(lop)) return false;
        }
    }
    {
        std::ifstream in(ltcFile, std::ios::binary);
        std::size_t n = 0;
        if (!in || !(in >> n) || n > 10000) { std::cerr << "File LOPTC.txt bi loi.\n"; return false; }
        for (std::size_t i = 0; i < n; ++i) {
            int ma, ky, nhom, minSV, maxSV; bool huy; std::string maMH, khoa; std::size_t soDK;
            if (!(in >> ma >> std::quoted(maMH) >> std::quoted(khoa) >> ky >> nhom >> minSV >> maxSV >> huy >> soDK) || soDK > 100000 || minSV < 0 || maxSV < minSV || ky < 1) return false;
            LopTinChi lop(ma, maMH, khoa, ky, nhom, minSV, maxSV); lop.setHuyLop(huy);
            for (std::size_t j = 0; j < soDK; ++j) {
                std::string maSV; double diem; bool huyDK;
                if (!(in >> std::quoted(maSV) >> diem >> huyDK) || (diem != -1 && (diem < 0 || diem > 10))) return false;
                if (lop.timDangKyTheoMaSV(maSV)) return false;
                lop.getDSSVDK().emplace_back(maSV);
                auto* dk = lop.timDangKyTheoMaSV(maSV);
                if (diem >= 0) dk->setDiem(diem);
                dk->setHuyDangKy(huyDK);
            }
            if (!ltcTam.themLopTinChi(lop)) return false;
        }
    }
    dsMonHoc = std::move(mhTam);
    dsLop = std::move(lopTam);
    dsLopTC = std::move(ltcTam);
    return true;
}
