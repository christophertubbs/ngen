#ifndef CATCHMENT_REALIZATION_H
#define CATCHMENT_REALIZATION_H

#include <memory>

#include "Realization.hpp"
#include <HY_CatchmentArea.hpp>

namespace realization {

    class Catchment_Realization : public Realization, public HY_CatchmentArea {
        public:
            Catchment_Realization(std::string id, forcing_params forcing_config) : Realization(id), HY_CatchmentArea(forcing_config) {};

            Catchment_Realization(std::string id) : Realization(id){};

            virtual double get_response(double input_flux, time_step_t t, time_step_t dt, void* et_params) = 0;

            virtual std::string* get_required_parameters() = 0;
            
            virtual void create_realization(boost::property_tree::ptree &config, geojson::PropertyMap *global = nullptr) = 0;

            virtual ~Catchment_Realization(){};
    };
}
#endif // CATCHMENT_REALIZATION_H