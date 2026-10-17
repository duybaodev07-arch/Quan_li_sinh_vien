#include "DangKi.h"

#include <cstring>

namespace {
PTRDK taoNodeDangKy(const DangKy& dk) {
    PTRDK node = new NodeDK{};
    node->dk = dk;
    return node;
}

void themCuoiDangKy(PTRDK& head, PTRDK newNode) {
    if (newNode == nullptr) return;
    newNode->next = nullptr;
    if (head == nullptr) {
        head = newNode;
        return;
    }
    PTRDK cur = head;
    while (cur->next != nullptr) cur = cur->next;
    cur->next = newNode;
}
}

PTRDK timDangKyTheoMaSV(PTRDK head, const char* maSV) {
    if (maSV == nullptr) return nullptr;
    for (PTRDK cur = head; cur != nullptr; cur = cur->next) {
        if (std::strcmp(cur->dk.maSV, maSV) == 0) return cur;
    }
    return nullptr;
}

bool daDangKy(PTRDK head, const char* maSV) {
    PTRDK node = timDangKyTheoMaSV(head, maSV);
    return node != nullptr && !node->dk.huyDangKy;
}

int demSoSVDangKy(PTRDK head) {
    int count = 0;
    for (PTRDK cur = head; cur != nullptr; cur = cur->next) {
        if (!cur->dk.huyDangKy) ++count;
    }
    return count;
}

bool themDangKy(PTRDK& head, const DangKy& dk) {
    PTRDK existing = timDangKyTheoMaSV(head, dk.maSV);
    if (existing != nullptr) {
        if (!existing->dk.huyDangKy) return false; // sinh viên đang đki
        // Khoi phuc dang ky, giu nguyen diem da luu.
        existing->dk.huyDangKy = false;
        return true;
    }
    themCuoiDangKy(head, taoNodeDangKy(dk));
    return true;
}

bool huyDangKyTheoMaSV(PTRDK head, const char* maSV) {
    PTRDK node = timDangKyTheoMaSV(head, maSV);
    if (node == nullptr || node->dk.huyDangKy) return false;
    node->dk.huyDangKy = true;
    return true;
}

void xoaTatCaDangKy(PTRDK& head) {
    while (head != nullptr) {
        PTRDK next = head->next;
        delete head;
        head = next;
    }
}
