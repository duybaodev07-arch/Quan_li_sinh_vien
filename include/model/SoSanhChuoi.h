#pragma once

#ifdef _WIN32
#include <string.h>

inline int soSanhKhongPhanBietHoaThuong(const char* chuoiA, const char* chuoiB) {
    return _stricmp(chuoiA, chuoiB);
}
#else
#include <strings.h>

inline int soSanhKhongPhanBietHoaThuong(const char* chuoiA, const char* chuoiB) {
    return strcasecmp(chuoiA, chuoiB);
}
#endif
