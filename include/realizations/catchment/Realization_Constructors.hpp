#ifndef NGEN_REALIZATION_CONSTRUCTORS_H
#define NGEN_REALIZATION_CONSTRUCTORS_H

#include "Realization.hpp"
#include <JSONProperty.hpp>
#include <exception>

#include <boost/property_tree/ptree.hpp>

// Realizations
#include "Tshirt_Realization.hpp"
#include "Simple_Lumped_Model_Realization.hpp"

namespace realization {


    typedef std::shared_ptr<Realization> (*constructor)(std::string, forcing_params);

    template<class T>
    static constructor create_realization_constructor() {
        return [](std::string id, forcing_params forcing_config) -> std::shared_ptr<Realization>{
            return std::make_shared<T>(id, forcing_config);
        };
    };

    static std::map<std::string, constructor> realizations = {
        {"tshirt", create_realization_constructor<Tshirt_Realization>()},
        {"simple_lumped", create_realization_constructor<Simple_Lumped_Model_Realization>()}
    };

    static bool realization_exists(std::string realization_type) {
        return realizations.count(realization_type) > 0;
    }

    static std::shared_ptr<Realization> construct_realization(std::string realization_type, std::string identifier, forcing_params &forcing_config) {
        constructor realization_constructor = realizations.at(realization_type);
        return realization_constructor(identifier, forcing_config);
    };

    static std::string get_realization_key(boost::property_tree::ptree &tree) {
        for (auto &node : tree) {
            if (realization_exists(node.first)) {
                return node.first;
            }
        }

        throw std::runtime_error("No valid realization was described in the passed in tree.");
    }
}

#endif // NGEN_REALIZATION_CONSTRUCTORS_H