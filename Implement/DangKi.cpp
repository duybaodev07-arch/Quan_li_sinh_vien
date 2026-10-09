#include "DangKi.h"
#include<iostream>
#include<cstring>
#include <iomanip>
using namespace std;

PTRDK taoNodeDangKy(const DangKy& dk) {
    PTRDK newNode = new NodeDK;
    newNode->dk = dk;
    newNode->next = nullptr;

    return newNode;
}
PTRDK timDangKyTheoMaSV(PTRDK head, const char* maSV) {
    PTRDK cur = head;

    while (cur != nullptr) {
        if (strcmp(cur->dk.maSV, maSV) == 0)
            return cur;

        cur = cur->next;
    }

    return nullptr;
}
bool daDangKy(PTRDK head, const char* maSV) {
    return timDangKyTheoMaSV(head, maSV) != nullptr;
}
int demSoSVDangKy(PTRDK head) {
    int dem = 0;

    while (head != nullptr) {
        dem++;
        head = head-> next;
    }
    return dem;
}
void themCuoiDangKy(PTRDK& head, PTRDK newNode) {
    if (head == nullptr) {
        head = newNode;
        return;
    }
    PTRDK cur = head;
    while (cur->next != nullptr)
        cur = cur->next;

    cur->next = newNode;
}
bool themDangKy(PTRDK& head, const DangKy& dk) {
    if (daDangKy(head, dk.maSV))
        return false;

    PTRDK newNode = taoNodeDangKy(dk);
    themCuoiDangKy(head, newNode);
    return true;
}
bool xoaDangKy(PTRDK& head, const char* maSV) {
    if (head == nullptr)
        return false;

    if (strcmp(head->dk.maSV, maSV) == 0) {
        PTRDK temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    PTRDK prev = head;
    PTRDK cur = head->next;

    while (cur != nullptr) {
        if (strcmp(cur->dk.maSV, maSV) == 0) {
            prev->next = cur->next;
            delete cur;
            return true;
        }

        prev = cur;
        cur = cur->next;
    }

    return false;
}
void xoaTatCaDangKy(PTRDK& head) {
    while (head != nullptr) {
        PTRDK temp = head;
        head = head->next;
        delete temp;
    }
}
void inDangKy(const DangKy& dk) {
    cout << "Ma SV       : " << dk.maSV << endl;
    cout << "Diem        : " << dk.diem << endl;
    cout << "Huy dang ky : " << (dk.huyDangKy ? "Co" : "Khong") << endl;
}
