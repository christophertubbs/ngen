#ifndef CATCHMENT_GENERATOR_HPP
#define CATCHMENT_GENERATOR_HPP

#include <string>
#include <iostream>
#include <map>

#include <geojson/FeatureBuilder.hpp>
#include <geojson/FeatureCollection.hpp>
#include "Formulation_Manager.hpp"

#include "HY_Catchment.hpp"

namespace realization {
    class CatchmentGenerator {
        public:
            CatchmentGenerator(std::string definition_path, realization::Formulation_Manager* formulation_manager, utils::StreamHandler output_stream){
                this->geospatial_definition = geojson::read(definition_path);
                this->formulation_manager = formulation_manager;
                this->formulation_manager->read(this->geospatial_definition, output_stream);
            }

            CatchmentGenerator(std::stringstream definition_data, realization::Formulation_Manager* formulation_manager, utils::StreamHandler output_stream) {
                this->geospatial_definition = geojson::read(definition_data);
                this->formulation_manager = formulation_manager;
                this->formulation_manager->read(this->geospatial_definition, output_stream);
            }

            map<std::string, std::shared_ptr<HY_Catchment>> generate_catchments() {
                map<std::string, std::shared_ptr<HY_Catchment>> catchments;

                for (geojson::Feature feature : *(this->geospatial_definition)) {
                    std::shared_ptr<HY_Catchment> catchment = std::make_shared<HY_Catchment>();
                    catchment->set_id(feature->get_id());
                    std::shared_ptr<realization::Catchment_Formulation> formulation = this->formulation_manager->get_formulation(feature->get_id());
                    catchment->set_formulation(formulation);
                    catchments.emplace(catchment->get_id(), catchment);
                }

                // Iterate through every catchment and link to every matched catchment
                for (std::pair<std::string, std::shared_ptr<HY_Catchment>> idAndCatchment : catchments ) {
                    geojson::Feature feature = this->geospatial_definition->get_feature(idAndCatchment.first);
                    std::shared_ptr<HY_Catchment> catchment = idAndCatchment.second;

                    for (geojson::FeatureBase* destination_feature : feature->destination_features() ) {
                        if (catchments.count(destination_feature->get_id()) > 0) {
                            // Might want to throw an exception here since it SHOULD be here, but we're not going to sweat it right now
                            std::shared_ptr<HY_Catchment> destination_catchment = catchments.at(destination_feature->get_id());
                            catchment->add_recieving_catchment(destination_catchment);
                        }
                    }

                    for (geojson::FeatureBase* origination_feature : feature->origination_features()) {
                        if (catchments.count(origination_feature->get_id()) > 0) {
                            std::shared_ptr<HY_Catchment> origination_catchment = catchments.at(origination_feature->get_id());
                            catchment->add_contributing_catchment(origination_catchment);
                        }
                    }
                }
                return catchments;
            }

        private:
            geojson::GeoJSON geospatial_definition;
            realization::Formulation_Manager* formulation_manager;
    };
}
#endif