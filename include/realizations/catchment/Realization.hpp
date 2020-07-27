#ifndef REALIZATION_H
#define REALIZATION_H

#include <memory>
#include <string>
#include <map>
#include <exception>

#include "JSONProperty.hpp"
#include "Pdm03.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>

namespace realization {

    class Realization {
        public:
            typedef long time_step_t;

            Realization(std::string id) : id(id) {}
            
            virtual ~Realization(){};

            virtual std::string get_realization_type() = 0;

            virtual double get_response(double input_flux, time_step_t t, time_step_t dt, void* et_params) = 0;

            // The neccessity of this function is in question
            virtual void add_time(time_t t, double n){};

            std::string get_id() const {
                return this->id;
            }
            
            virtual void create_realization(boost::property_tree::ptree &config, geojson::PropertyMap *global = nullptr) = 0;

        protected:
            
            virtual std::string* get_required_parameters() = 0;

            virtual geojson::PropertyMap interpret_parameters(boost::property_tree::ptree &config, geojson::PropertyMap *global = nullptr) {
                geojson::PropertyMap options;

                for (auto &realization_parameter : config) {
                    options.emplace(realization_parameter.first, geojson::JSONProperty(realization_parameter.first, realization_parameter.second));
                }

                if (global != nullptr) {
                    for(auto &global_option : *global) {
                        if (options.count(global_option.first) == 0) {
                            options.emplace(global_option.first, global_option.second);
                        }
                    }
                }

                validate_parameters(options);

                return options;
            }

            virtual void validate_parameters(geojson::PropertyMap options) {
                std::vector<std::string> missing_parameters;
                std::string *required_parameters = get_required_parameters();
                int required_parameter_count = sizeof(required_parameters) / sizeof(required_parameters[0]);

                for (int parameter_index = 0; parameter_index < required_parameter_count; parameter_index++) {
                    std::string parameter = required_parameters[parameter_index];
                    
                    if (options.count(parameter) != 0) {
                        missing_parameters.push_back(parameter);
                    }
                }

                if (missing_parameters.size() > 0) {
                    std::string message = "A " + get_realization_type() + " realization cannot be created; the following parameters are missing: ";

                    for (int missing_parameter_index = 0; missing_parameter_index < missing_parameters.size(); missing_parameter_index++) {
                        message += missing_parameters[missing_parameter_index];

                        if (missing_parameter_index < missing_parameters.size() - 1) {
                            message += ", ";
                        }
                    }
                    
                    throw std::runtime_error(message);
                }
            }

            std::string id;
    };

}
#endif // REALIZATION_H