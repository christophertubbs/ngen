#ifndef RESPONSEACCUMULATOR_H
#define RESPONSEACCUMULATOR_H

#include <vector>

#include "HY_HydroNexus.hpp"

class ResponseAccumulator {
    virtual double accumulate(std::vector<double> responses, HY_HydroNexus* nexus) = 0;
};

#endif
