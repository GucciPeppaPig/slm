#include "model.h"

int main() {
    LanguageModel m(2, 0.0);
    m.train_from_file("test.txt");
    m.print_counts();
    return 0;
}