#include <iostream>
#include <random>

#include "model.h"

int main() {
    LanguageModel m(2, 1.0);
    m.train_from_file("test.txt");

    std::cout << "P(a|ab) = " << m.probability("ab", 'a') << '\n';
    std::cout << "P(b|ba) = " << m.probability("ba", 'b') << '\n';
    std::cout << "P(c|ac) = " << m.probability("ac", 'c') << '\n';

    std::mt19937 rng{std::random_device{}()};
    std::string w = m.random_start(rng);
    std::cout << "start: " << w << '\n';

    char c;
    if (m.sample_next(w, rng, c)) {
        std::cout << "next: " << c << '\n';
    } else {
        std::cout << "stuck\n";
    }
    return 0;
}