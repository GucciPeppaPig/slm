#ifndef GENERATOR_H
#define GENERATOR_H

#include <cstddef>
#include <random>
#include <string>

#include "model.h"

class Generator {
public:
    explicit Generator(const LanguageModel& model);

    // Generates text of up to `length` characters, starting with a k-gram.
    // The result can be shorter if the model gets stuck.
    std::string generate(std::size_t length);

private:
    const LanguageModel& model_;
    std::mt19937 rng_;
};

#endif