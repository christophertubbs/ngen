#ifndef SIMPLESUMACCUMULATOR_H
#define SIMPLESUMACCUMULATOR_H

#include "HY_HydroNexus.hpp"
#include "ResponseAccumulator.hpp"

class SimpleSumAccumulator : ResponseAccumulator {
    SimpleSumAccumulator(){}
    ~SimpleSumAccumulator(){}
    double accumulate(std::vector<double> responses, HY_HydroNexus* nexus) {
        double total = 0;

        for (int response_index = 0; response_index < responses.size(); response_index++) {
            total += responses[response_index];
        }

        return total;
    }
}

#endif