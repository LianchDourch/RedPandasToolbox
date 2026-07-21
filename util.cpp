#include "util.h"

int Util::randomGaussianInt(int min, int max) {
    if (min > max) std::swap(min, max);

    const double mean = (min + max) / 2.0;
    const double stddev = (max - min) / 6.0; // 99.7% des valeurs seront dans [min, max] (~±3σ)

    int result;
    do {
        // Box-Muller transform
        double u1 = QRandomGenerator::global()->generateDouble();
        double u2 = QRandomGenerator::global()->generateDouble();

        double z0 = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2);
        double value = mean + z0 * stddev;
        result = static_cast<int>(std::round(value));
    } while (result < min || result > max); // Rejeter les valeurs hors intervalle

    return result;
}

