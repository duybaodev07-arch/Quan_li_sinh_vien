#include "DangKi.h"

#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

PTRDK taoNodeDangKy(const DangKy& dk) {
    PTRDK node = new NodeDK{};
    node->dk = dk;
    return node;
}

PTRDK timDangKyTheoMaSV(PTRDK head, const char* maSV) {
    for (PTRDK cur = head; cur != nullptr; cur = cur->next) {
        if (strcmp(cur->dk.maSV, maSV) == 0) return cur;
    }
    return nullptr;
}

bool daDangKyConHieuLuc(PTRDK head, const char* maSV) {
    PTRDK node = timDangKyTheoMaSV(head, maSV);
    return node != nullptr && !node->dk.huyDangKy;
}

// Giu ten ham cu de cac module hien co co the dung tiep; chi tinh dang ky con hieu luc.
bool daDangKy(PTRDK head, const char* maSV) {
    return daDangKyConHieuLuc(head, maSV);
}

int demSoSVDangKy(PTRDK head) {
    int count = 0;
    for (PTRDK cur = head; cur != nullptr; cur = cur->next) {
        if (!cur->dk.huyDangKy) ++count;
    }
    return count;
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

bool themDangKy(PTRDK& head, const DangKy& dk) {
    PTRDK existing = timDangKyTheoMaSV(head, dk.maSV);
    if (existing != nullptr) {
        if (!existing->dk.huyDangKy) return false;
        existing->dk.huyDangKy = false;
        existing->dk.diem = dk.diem;
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

bool khoiPhucDangKyTheoMaSV(PTRDK head, const char* maSV) {
    PTRDK node = timDangKyTheoMaSV(head, maSV);
    if (node == nullptr || !node->dk.huyDangKy) return false;
    node->dk.huyDangKy = false;
    return true;
}

bool xoaDangKy(PTRDK& head, const char* maSV) {
    PTRDK prev = nullptr;
    PTRDK cur = head;
    while (cur != nullptr && strcmp(cur->dk.maSV, maSV) != 0) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    if (prev == nullptr) head = cur->next;
    else prev->next = cur->next;
    delete cur;
    return true;
}

void xoaTatCaDangKy(PTRDK& head) {
    while (head != nullptr) {
        PTRDK next = head->next;
        delete head;
        head = next;
    }
}

void inDangKy(const DangKy& dk) {
    cout << "+----------------+----------+-------------+\n";
    cout << "| Ma SV          | Diem     | Trang thai  |\n";
    cout << "+----------------+----------+-------------+\n";
    cout << "| " << left << setw(14) << dk.maSV << " | "
         << right << setw(8) << fixed << setprecision(2) << dk.diem << " | "
         << left << setw(11) << (dk.huyDangKy ? "Da huy" : "Dang ky") << " |\n";
    cout << "+----------------+----------+-------------+\n";
}
