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

double LanguageModel::probability(const std::string& w, char c) const {
    int n_w = 0;   // n(w): how many times w was followed by something
    int n_wc = 0;  // n(w, c): how many times w was followed by c

    auto it = next_.find(w);
    if (it != next_.end()) {
        for (const auto& [ch, n] : it->second) {
            n_w += n;
        }
        auto jt = it->second.find(c);
        if (jt != it->second.end()) {
            n_wc = jt->second;
        }
    }

    double denominator = n_w + alpha_ * alphabet_.size();
    if (denominator == 0.0) {
        return 0.0;  // alpha == 0 and w has no transitions: no distribution
    }
    return (n_wc + alpha_) / denominator;
}

std::string LanguageModel::random_start(std::mt19937& rng) const {
    std::vector<std::string> grams;
    std::vector<int> weights;
    for (const auto& [w, n] : freq_) {
        grams.push_back(w);
        weights.push_back(n);
    }
    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return grams[dist(rng)];
}

bool LanguageModel::sample_next(const std::string& w, std::mt19937& rng, char& c) const {
    std::vector<char> chars;
    std::vector<double> weights;
    double total = 0.0;

    auto it = next_.find(w);  // may not exist if w never had a follower
    for (char ch : alphabet_) {
        double count = 0.0;
        if (it != next_.end()) {
            auto jt = it->second.find(ch);
            if (jt != it->second.end()) {
                count = jt->second;
            }
        }
        double weight = count + alpha_;   // n(w,c) + alpha
        chars.push_back(ch);
        weights.push_back(weight);
        total += weight;
    }

    if (total <= 0.0) {
        return false;  // stuck: nothing can follow w
    }
    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    c = chars[dist(rng)];
    return true;
}