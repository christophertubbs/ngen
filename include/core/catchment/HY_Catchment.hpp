#ifndef HY_CATCHMENT_H
#define HY_CATCHMENT_H

#include <memory>
#include <string>
#include <vector>
#include <cmath>

#include "HY_CatchmentRealization.hpp"
#include "HY_HydroFeature.hpp"
#include <iostream>
#include <fstream>

class HY_HydroNexus;

class HY_Catchment : public HY_HydroFeature
{
    public:

        HY_Catchment();
        virtual ~HY_Catchment();

        void set_formulation(std::shared_ptr<HY_CatchmentRealization> formulation) {
            this->formulation = formulation;
        }

        void add_inflow_nexus(std::shared_ptr<HY_HydroNexus> nexus) {
            this->inflows.push_back(nexus);
        }

        void add_outflow_nexus(std::shared_ptr<HY_HydroNexus> nexus) {
            this->outflows.push_back(nexus);
        }

        void add_recieving_catchment(std::shared_ptr<HY_Catchment> catchment) {
            this->recieving_catchments.push_back(catchment);
        }

        void add_contributing_catchment(std::shared_ptr<HY_Catchment> catchment) {
            this->contributing_catchments.push_back(catchment);
        }

        void add_contained_catchment(std::shared_ptr<HY_Catchment> catchment) {
            this->contained_catchments.push_back(catchment);
        }

        void add_containing_catchment(std::shared_ptr<HY_Catchment> catchment) {
            this->containing_catchment = catchment;
        }

        void set_id(std::string id) {
            this->id = id;
        }

        std::string get_id() {
            return this->id;
        }

        void set_id_number(unsigned long id_number) {
            this->id_number = id_number;
        }

        unsigned long get_id_number() {
            return this->id_number;
        }

        std::shared_ptr<HY_CatchmentRealization> get_formulation() {
            return this->formulation;
        }

        void set_output(std::ofstream* output_stream) {
            this->output_stream = output_stream;
        }

        std::vector<std::shared_ptr<HY_HydroNexus>> get_outflows() const {
            return this->outflows;
        }

        double get_flow_percentage() {
            return this->flow_percentage;
        }

        void set_flow_percentage(double percentage) {
            this->flow_percentage = percentage;
        }
    protected:



    private:

        std::ofstream* output_stream;
        std::shared_ptr<HY_CatchmentRealization> formulation;

        // TODO: Do we want the realization set here if we're going to use the formulation?
        std::shared_ptr<HY_CatchmentRealization> realization;

        std::vector< std::shared_ptr<HY_HydroNexus> > inflows;
        std::vector< std::shared_ptr<HY_HydroNexus> > outflows;
        std::vector< std::shared_ptr<HY_Catchment> > recieving_catchments;
        std::vector<std::shared_ptr<HY_Catchment>> contributing_catchments;
        std::vector< std::shared_ptr<HY_Catchment> > contained_catchments;
        std::shared_ptr<HY_Catchment> containing_catchment;

        unsigned long id_number;
        std::string id;

        double flow_percentage = -1.0;
};

#endif // HY_CATCHMENT_H
