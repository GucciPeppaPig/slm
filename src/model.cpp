#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

LanguageModel::LanguageModel(int k, double alpha) : k_(k), alpha_(alpha) {}

void LanguageModel::train_from_file(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("cannot open file '" + filename + "'");
    }
    std::ostringstream ss;
    ss << in.rdbuf();   // read the whole file, including spaces and newlines
    train(ss.str());
}

void LanguageModel::train(const std::string& text) {
    // Need at least one k-gram followed by a character.
    if (text.size() <= static_cast<std::size_t>(k_)) {
        throw std::runtime_error("training text is too short for k = " + std::to_string(k_));
    }

    for (char c : text) {
        alphabet_.insert(c);
    }

    for (std::size_t i = 0; i + k_ <= text.size(); ++i) {
        std::string w = text.substr(i, k_);
        ++freq_[w];
        // Only count a transition if a character follows this k-gram.
        if (i + k_ < text.size()) {
            ++next_[w][text[i + k_]];
        }
    }
}

void LanguageModel::print_counts() const {
    std::cout << "k-gram frequencies:\n";
    for (const auto& [w, n] : freq_) {
        std::cout << "  '" << w << "' " << n << '\n';
    }
    std::cout << "transitions:\n";
    for (const auto& [w, followers] : next_) {
        for (const auto& [c, n] : followers) {
            std::cout << "  '" << w << "' -> '" << c << "' " << n << '\n';
        }
    }
}