#include <iostream>
#include <filesystem>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fs.h>
#include <linux/fiemap.h>

using namespace std;
namespace fs = filesystem;

vector<fiemap_extent> get_extents(const fs::path& path) {
    vector<fiemap_extent> result;

    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0) {
        perror("open");
        return result;
    }

    const int MAX = 128;
    fiemap* fm = (fiemap*)calloc(
        1, sizeof(fiemap) + MAX * sizeof(fiemap_extent)
    );

    if (fm == nullptr) {
        close(fd);
        return result;
    }

    fm->fm_start = 0;
    fm->fm_length = FIEMAP_MAX_OFFSET;
    fm->fm_extent_count = MAX;
    fm->fm_flags = FIEMAP_FLAG_SYNC;

    if (ioctl(fd, FS_IOC_FIEMAP, fm) == -1) {
        perror("FIEMAP");
    } else {
        for (unsigned int i = 0; i < fm->fm_mapped_extents; i++) {
            result.push_back(fm->fm_extents[i]);
        }

        if (fm->fm_mapped_extents == MAX &&
            !(fm->fm_extents[MAX - 1].fe_flags & FIEMAP_EXTENT_LAST)) {
            cerr << "Chỉ lấy 128 extent đầu tiên\n";
        }
    }

    free(fm);
    close(fd);
    return result;
}

void show_metadata(const fs::path& path){
    struct stat st;

    if (stat(path.c_str(), &st) == -1) {
        perror("stat");
        return;
    }

    cout << "\nMetadata\n";
    cout << "File: " << path << '\n';
    cout << "Size: " << st.st_size << " bytes\n";
    cout << "Inode: " << st.st_ino << '\n';
    cout << "UID: " << st.st_uid << '\n';
    cout << "GID: " << st.st_gid << '\n';
    cout << "Permissions: " << oct
         << (st.st_mode & 0777) << dec << '\n';
    cout << "Links: " << st.st_nlink << '\n';
    cout << "Modified time: " << st.st_mtime << '\n';
};

void show_inode_extent(const fs::path& path){
    struct stat st;

    if (stat(path.c_str(), &st) == -1) {
        perror("stat");
        return;
    }

    cout << "\nInode/Extent\n";
    cout << "Inode: " << st.st_ino << '\n';

    vector<fiemap_extent> extents = get_extents(path);

    cout << "Extents: " << extents.size() << '\n';

    for (size_t i = 0; i < extents.size(); i++) {
        cout << "Extent " << i << ":\n";
        cout << "  Logical offset: "
             << extents[i].fe_logical << " bytes\n";
        cout << "  Physical offset: "
             << extents[i].fe_physical << " bytes\n";
        cout << "  Length: "
             << extents[i].fe_length << " bytes\n";
        cout << "  Flags: " << extents[i].fe_flags << '\n';
    }
};

void show_block(const fs::path& path){
    struct statvfs info;

    if (statvfs(path.c_str(), &info) == -1) {
        perror("statvfs");
        return;
    }

    unsigned long long block_size = info.f_frsize;

    cout << "\nBlock Mapping\n";
    cout << "Block size: " << block_size << " bytes\n";

    vector<fiemap_extent> extents = get_extents(path);

    for (const auto& e : extents) {
        if (e.fe_flags & (FIEMAP_EXTENT_UNKNOWN |
                          FIEMAP_EXTENT_DELALLOC |
                          FIEMAP_EXTENT_ENCODED)) {
            cout << "Không ánh xạ được (flags: "
                 << e.fe_flags << ")\n";
            continue;
        }

        cout << "Logical block: "
             << e.fe_logical / block_size << '\n';

        cout << "Physical block: "
             << e.fe_physical / block_size << '\n';

        cout << "Number of blocks: "
             << (e.fe_length + block_size - 1) / block_size
             << '\n';
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }

    show_metadata(argv[1]);
    show_inode_extent(argv[1]);
    show_block(argv[1]);

    return 0;
}

