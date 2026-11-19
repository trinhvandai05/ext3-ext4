// Xây dựng lại các chức năng cơ bản của API stat() trong Linux
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;


class MyStat {
public:
    
    // path của file hoặc directory
    void show_path(const fs::path& path) { 
        cout << "Path: " << path << '\n';
    }

    // tính kích thước của file thông qua địa chỉ logical và physical của inode
    void get_size(const fs::path& path);

    // lấy inode của file hoặc directory
    void get_inode(const fs::path& path);

    void get_uid(const fs::path& path);

    void get_gid(const fs::path& path);

    void get_permissions(const fs::path& path);
private:
    fs::path m_path;
    unit32_t m_inode;
    unit32_t m_uid;

    unit32_t m_gid;
    unit32_t m_permissions;
    
};
    

int main(){
    return 0;
}



