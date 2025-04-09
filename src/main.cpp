#include <iostream>
#include <string>
#include <vector>
#include "DataFetcher.hpp"

void printHelp() {
    std::cout << "Usage:\n"
              << "  -v  Show current Ubuntu LTS version\n"
              << "  -h  Show help\n"
              << "  -l  List items\n"
              << "  -i disk_image  Return the hash of input disk1.img\n";
}

void printLTSVersion(DataFetcher& df) {
    df.fetchData();
    std::cout << df.getCurrentLTSVersion() << std::endl;
}

void printSupportedReleases(DataFetcher& df) {
    df.fetchData();
    auto versions = df.getSupportedReleases();
    std::cout << "[";
    for (auto i = versions->begin(); i < versions->end(); ++i) {
        std::cout << *i;
        if (i + 1 != versions->end()) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

void printChecksum(DataFetcher& df, const std::string& imageName) {
    df.fetchData();
    std::cout << df.getChecksum(imageName) << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No option provided. Type -h for help.\n";
        return 1;
    }

    std::string choice = argv[1];
    DataFetcher df;

    if (choice == "-v") {
        printLTSVersion(df);
    } else if (choice == "-h") {
        printHelp();
    } else if (choice == "-l") {
        printSupportedReleases(df);
    } else if (choice == "-i") {
        if (argc < 3) {
            std::cerr << "Error: -i requires a value (e.g., a disk image file).\n";
            return 1;
        }
        printChecksum(df, argv[2]);
    } else {
        std::cerr << "Unknown option. Type -h for help.\n";
    }

    return 0;
}
