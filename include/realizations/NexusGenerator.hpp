#ifndef NEXUS_GENERATOR_HPP
#define NEXUS_GENERATOR_HPP

#include <string>
#include <iostream>
#include <map>
#include <memory>

#include <geojson/FeatureBuilder.hpp>
#include <geojson/FeatureCollection.hpp>

#include "HY_HydroLocation.hpp"

namespace realization {
    class NexusGenerator {
        public:
            NexusGenerator(std::string definition_path, map<std::string, std::shared_ptr<HY_Catchment>> catchments){
                this->geospatial_definition = geojson::read(definition_path);
                this->catchments = catchments;
            }

            NexusGenerator(std::stringstream definition_data, map<std::string, std::shared_ptr<HY_Catchment>> catchments) {
                this->geospatial_definition = geojson::read(definition_data);
                this->catchments = catchments;
            }

            map<std::string, HY_HydroLocation> generate_nexi() {
                map<std::string, HY_HydroLocation> nexi;

                for (geojson::Feature feature : *(this->geospatial_definition)) {
                    geojson::JSONProperty feature_type_property = feature->get_properties().at("featureType");

                    if (feature_type_property.as_string() == "nexus") {
                        HY_PointHydroNexus nexus_realization(0, feature->get_id(), feature->get_destination_length());
                        std::string nexusTypeName = feature->get_properties().at("nexusType").as_string();
                        HY_HydroLocationType location_type = get_HydroLocationType_by_name(nexusTypeName);
                        //HY_HydroLocation location()
                    }

                }
                return nexi;
            }

        private:
            geojson::GeoJSON geospatial_definition;
            map<std::string, std::shared_ptr<HY_Catchment>> catchments;
    };
}
#endif