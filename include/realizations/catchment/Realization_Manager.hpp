#ifndef REALIZATION_MANAGER_H
#define REALIZATION_MANAGER_H

#include <memory>
#include <sstream>
#include <tuple>
#include <functional>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Realization_Constructors.hpp"

#include "GIUH.hpp"
#include "GiuhJsonReader.h"

namespace realization {

    class Realization_Manager {
        typedef std::tuple<std::string, std::string> dual_keys;

        public:
            Realization_Manager(std::stringstream &data) {
                boost::property_tree::ptree loaded_tree;
                boost::property_tree::json_parser::read_json(data, loaded_tree);
                this->tree = loaded_tree;
            }

            Realization_Manager(const std::string &file_path) {
                boost::property_tree::ptree loaded_tree;
                boost::property_tree::json_parser::read_json(file_path, loaded_tree);
                this->tree = loaded_tree;
            }

            Realization_Manager(boost::property_tree::ptree &loaded_tree) {
                this->tree = loaded_tree;
            }

            virtual ~Realization_Manager(){};

            virtual void read() {
                auto possible_global_config = tree.get_child_optional("global");

                if (possible_global_config) {
                    std::string realization_key = get_realization_key(*possible_global_config);
                    for(std::pair<std::string, boost::property_tree::ptree> node : *possible_global_config) {
                        if (node.first == "forcing") {
                            for (auto &forcing_parameter : node.second) {
                                this->global_forcing.emplace(
                                    forcing_parameter.first,
                                    geojson::JSONProperty(forcing_parameter.first, forcing_parameter.second)
                                );
                            }
                        }
                        else if (node.first == realization_key) {
                            this->global_realization_tree = node.second;

                            for (std::pair<std::string, boost::property_tree::ptree> global_setting : node.second) {
                                this->global_realization_parameters.emplace(
                                    global_setting.first,
                                    geojson::JSONProperty(global_setting.first, global_setting.second)
                                );
                            }
                        }
                    }
                }

                auto possible_catchment_configs = tree.get_child_optional("catchments");

                if (possible_catchment_configs) {
                    for (std::pair<std::string, boost::property_tree::ptree> catchment_config : *possible_catchment_configs) {
                        this->add_realization(
                            this->construct_realization_from_tree(
                                catchment_config.first,
                                catchment_config.second
                            )
                        );
                    }
                }
            }

            virtual void add_realization(std::shared_ptr<Realization> realization) {
                this->realizations.emplace(realization->get_id(), realization);
            }

            virtual std::shared_ptr<Realization> get_realization(std::string id) const {
                // TODO: Implement on-the-fly realization creation using global parameters
                return this->realizations.at(id);
            }

            virtual bool contains(std::string identifier) const {
                return this->realizations.count(identifier) > 0;
            }

            /**
             * @return The number of elements within the collection
             */
            virtual int get_size() {
                return this->realizations.size();
            }

            /**
             * @return Whether or not the collection is empty
             */
            virtual bool is_empty() {
                return this->realizations.empty();
            }

            virtual typename std::map<std::string, std::shared_ptr<Realization>>::const_iterator begin() const {
                return this->realizations.cbegin();
            }

            virtual typename std::map<std::string, std::shared_ptr<Realization>>::const_iterator end() const {
                return this->realizations.cend();
            }

        protected:
            std::shared_ptr<Realization> construct_realization_from_tree(
                std::string identifier,
                boost::property_tree::ptree &tree
            ) {
                std::string realization_type_key = get_realization_key(tree);

                boost::property_tree::ptree realization_config = tree.get_child(realization_type_key);

                auto possible_forcing = tree.get_child_optional("forcing");

                if (!possible_forcing) {
                    throw std::runtime_error("No forcing definition was found for " + identifier);
                }

                geojson::JSONProperty forcing_parameters("forcing", *possible_forcing);

                std::vector<std::string> missing_parameters;
                
                if (!forcing_parameters.has_key("path")) {
                    missing_parameters.push_back("path");
                }

                if (!forcing_parameters.has_key("start_time")) {
                    missing_parameters.push_back("start_time");
                }

                if (!forcing_parameters.has_key("end_time")) {
                    missing_parameters.push_back("end_time");
                }

                if (missing_parameters.size() > 0) {
                    std::string message = "A forcing configuration cannot be created for '" + identifier + "'; the following parameters are missing: ";

                    for (int missing_parameter_index = 0; missing_parameter_index < missing_parameters.size(); missing_parameter_index++) {
                        message += missing_parameters[missing_parameter_index];

                        if (missing_parameter_index < missing_parameters.size() - 1) {
                            message += ", ";
                        }
                    }
                    
                    throw std::runtime_error(message);
                }

                forcing_params forcing_config(
                    forcing_parameters.at("path").as_string(),
                    forcing_parameters.at("start_time").as_string(),
                    forcing_parameters.at("end_time").as_string()
                );

                std::shared_ptr<Realization> constructed_realization = construct_realization(realization_type_key, identifier, forcing_config);
                constructed_realization->create_realization(realization_config, &global_realization_parameters);
                return constructed_realization;
            }

            boost::property_tree::ptree tree;

            boost::property_tree::ptree global_realization_tree;

            geojson::PropertyMap global_realization_parameters;

            geojson::PropertyMap global_forcing;

            std::map<std::string, std::shared_ptr<Realization>> realizations;
    };
}
#endif // REALIZATION_MANAGER_H