#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <random>
#include <set>
#include <string>
#include <vector>

class LanguageModel {
public:
    LanguageModel(int k, double alpha);

    // Reads the file and builds the counts. Throws on errors.
    void train_from_file(const std::string& filename);

    // P(c | w) = (n(w,c) + alpha) / (n(w) + alpha * |alphabet|)
    double probability(const std::string& w, char c) const;

    // Picks a starting k-gram, weighted by how often each k-gram occurs.
    std::string random_start(std::mt19937& rng) const;

    // Samples the character that follows w and stores it in c.
    // Returns false if the model is "stuck" (no transitions and alpha == 0).
    bool sample_next(const std::string& w, std::mt19937& rng, char& c) const;

    // Temporary, for debugging. Remove later.
    void print_counts() const;

private:
    int k_;
    double alpha_;
    std::map<std::string, int> freq_;                  // k-gram -> number of occurrences
    std::map<std::string, std::map<char, int>> next_;  // w -> (c -> n(w, c))
    std::set<char> alphabet_;                          // the distinct characters

    void train(const std::string& text);
};

#endif