#include <iostream>

int abstract_factory_main();
int builder_main();
int fabric_method_main();
int naive_singletone_main();
int singleton_main();

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "error: unexpected count of arguments" << std::endl;
        return 1;
    }

    if (argv[1] == std::string_view("--abstract_factory")) {
        return abstract_factory_main();
    }
    if (argv[1] == std::string_view("--builder")) {
        return builder_main();
    }
    if (argv[1] == std::string_view("--fabric_method")) {
        return fabric_method_main();
    }
    if (argv[1] == std::string_view("--naive_singletone")) {
        return naive_singletone_main();
    }
    if (argv[1] == std::string_view("--singleton")) {
        return singleton_main();
    }

    std::cerr << "error: unkwnown argument '" << argv[1] << "'" << std::endl;
    return 1;
}
