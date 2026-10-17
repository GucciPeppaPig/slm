#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <set>
#include <string>

class LanguageModel {
public:
    LanguageModel(int k, double alpha);

    // Reads the file and builds the counts. Throws on errors.
    void train_from_file(const std::string& filename);

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