#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "generator.h"
#include "model.h"

namespace {

// Helpers that only main.cpp uses.
int parse_positive_int(const std::string& text, const std::string& name) {
    std::size_t pos = 0;
    int value = 0;
    try {
        value = std::stoi(text, &pos);
    } catch (const std::exception&) {
        pos = 0;  // not a number at all; handled below
    }
    if (pos == 0 || pos != text.size() || value <= 0) {
        throw std::invalid_argument(name + " must be a positive integer, got '" + text + "'");
    }
    return value;
}

double parse_alpha(const std::string& text) {
    std::size_t pos = 0;
    double value = 0.0;
    try {
        value = std::stod(text, &pos);
    } catch (const std::exception&) {
        pos = 0;
    }
    if (pos == 0 || pos != text.size() || value < 0.0) {
        throw std::invalid_argument("option -a needs a non-negative number, got '" + text + "'");
    }
    return value;
}

}  // namespace

int main(int argc, char* argv[]) {
    try {
        double alpha = 0.0;
        int i = 1;

        // Options come first.
        while (i < argc && argv[i][0] == '-') {
            std::string option = argv[i];
            if (option == "-a") {
                if (i + 1 >= argc) {
                    throw std::invalid_argument("option -a needs a value");
                }
                alpha = parse_alpha(argv[i + 1]);
                i += 2;
            } else {
                throw std::invalid_argument("unknown option '" + option + "'");
            }
        }

        // Exactly three arguments must remain: k, file, length.
        if (argc - i != 3) {
            throw std::invalid_argument("wrong number of arguments\nusage: slm [-a alpha] k file length");
        }

        int k = parse_positive_int(argv[i], "k");
        std::string filename = argv[i + 1];
        int length = parse_positive_int(argv[i + 2], "output length");
        if (length < k) {
            throw std::invalid_argument("output length must be at least k");
        }

        LanguageModel model(k, alpha);
        model.train_from_file(filename);   // throws if the file can't be opened or is too short

        Generator generator(model);
        std::cout << generator.generate(length) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "slm: " << e.what() << '\n';
        return 1;
    }
    return 0;
}