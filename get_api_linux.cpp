#include <filesystem>
using namespace std;
namespace fs = filesystem;

void show_metadata(const fs::path& path);
void show_inode_extent(const fs::path& path);
void show_block(const fs::path& path);

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

