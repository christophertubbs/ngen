#ifndef HY_HYDROLOCATIONTYPE_H
#define HY_HYDROLOCATIONTYPE_H

#include <string>

#include <boost/algorithm/string.hpp>

enum HY_HydroLocationType
{
    barrage,
    bifurcation,
    catchmentOutlet,
    confluence,
    dam,
    diversionOfWater,
    extractionWell,
    forkNexus,
    hydrometricStation,
    inflitrationWell,
    injectionWell,
    inletStructure,
    intake,
    outletStructure,
    ponor,
    pourPoint,
    rapids,
    referenceClimatologicalStation,
    riverMouth,
    sinkhole,
    source,
    waterfall,
    weir,
    undefined
};

inline HY_HydroLocationType get_HydroLocationType_by_name(std::string locationtype_name) {
    std::string name = boost::algorithm::to_lower_copy(locationtype_name);

    if (name == "barrage") {
        return HY_HydroLocationType::barrage;
    }
    else if (name == "bifurcation") {
        return HY_HydroLocationType::bifurcation;
    }
    else if (name == "catchmentOutlet") {
        return HY_HydroLocationType::catchmentOutlet;
    }
    else if (name == "confluence") {
        return HY_HydroLocationType::confluence;
    }
    else if (name == "dam") {
        return HY_HydroLocationType::dam;
    }
    else if (name == "diversionOfWater") {
        return HY_HydroLocationType::diversionOfWater;
    }
    else if (name == "extractionWell") {
        return HY_HydroLocationType::extractionWell;
    }
    else if (name == "fork") {
        return HY_HydroLocationType::forkNexus;
    }
    else if (name == "hydrometricStation") {
        return HY_HydroLocationType::hydrometricStation;
    }
    else if (name == "inflitrationWell") {
        return HY_HydroLocationType::inflitrationWell;
    }
    else if (name == "injectionWell") {
        return HY_HydroLocationType::injectionWell;
    }
    else if (name == "inletStructure") {
        return HY_HydroLocationType::inletStructure;
    }
    else if (name == "intake") {
        return HY_HydroLocationType::intake;
    }
    else if (name == "outletStructure") {
        return HY_HydroLocationType::outletStructure;
    }
    else if (name == "ponor") {
        return HY_HydroLocationType::ponor;
    }
    else if (name == "pourPoint") {
        return HY_HydroLocationType::pourPoint;
    }
    else if (name == "rapids") {
        return HY_HydroLocationType::rapids;
    }
    else if (name == "referenceClimatologicalStation") {
        return HY_HydroLocationType::referenceClimatologicalStation;
    }
    else if (name == "riverMouth") {
        return HY_HydroLocationType::riverMouth;
    }
    else if (name == "sinkhole") {
        return HY_HydroLocationType::sinkhole;
    }
    else if (name == "source") {
        return HY_HydroLocationType::source;
    }
    else if (name == "waterfall") {
        return HY_HydroLocationType::waterfall;
    }
    else if (name == "weir") {
        return HY_HydroLocationType::weir;
    }

    return HY_HydroLocationType::undefined;
}

#endif // HY_HYDROLOCATIONTYPE_H
