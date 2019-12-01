#include "FTPdriver.h"

using boost::filesystem::all_all;

int main(int argc, char* argv[]) {
    FTPdriver result(argc == 2 ? boost::filesystem::path(argv[1])
                               : boost::filesystem::current_path());
    std::cout << result.FTPresult() << std::endl;

}
