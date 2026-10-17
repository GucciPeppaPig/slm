#include "generator.h"

Generator::Generator(const LanguageModel& model)
    : model_(model), rng_(std::random_device{}()) {}

std::string Generator::generate(std::size_t length) {
    std::string w = model_.random_start(rng_);   // step 1: starting k-gram
    std::string out = w;                         // step 2: output it

    while (out.size() < length) {
        char c;
        if (!model_.sample_next(w, rng_, c)) {   // step 3: sample, or stop if stuck
            break;
        }
        out += c;                                // step 4: output c
        w = w.substr(1) + c;                     // step 5: slide the window
    }
    return out;
}