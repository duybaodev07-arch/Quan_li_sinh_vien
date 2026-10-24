@echo off
chcp 65001 >nul
where mingw32-make >nul 2>nul
if %errorlevel% equ 0 (
  mingw32-make run
) else (
  where make >nul 2>nul
  if %errorlevel% neq 0 (
    echo Khong tim thay mingw32-make hoac make. Hay cai MinGW Make va them vao PATH.
    pause
    exit /b 1
  )
  make run
)
if %errorlevel% neq 0 (
  echo Build hoac chay chuong trinh that bai.
  pause
  exit /b %errorlevel%
)