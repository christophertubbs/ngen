#include <vector>

#include "HY_HydroNexus.hpp"

HY_HydroNexus::HY_HydroNexus(int nexus_id_num, std::string nexus_id, int num_downstream)
{
   this->id_number = nexus_id_num;
   this->id = nexus_id;
   this->number_of_downstream_catchments = num_downstream;
   this->output_file_name = "./" + nexus_id + "_output.csv";
}

double HY_HydroNexus::run_formulations(time_step_t time_step, double step_duration, void* pdm_data){
    for (std::pair<std::string, std::shared_ptr<HY_Catchment>> contributing_catchment : this->contributing_catchments) {
        double response = contributing_catchment.second->get_formulation()->get_response(0.0, time_step, step_duration, pdm_data) * this->area;
        this->add_upstream_flow(response, contributing_catchment.second, time_step);
    }

    std::vector<double> downstream_flows;

    double downstream_flow = this->get_downstream_flow(time_step);

    /*for (std::pair<std::string, std::shared_ptr<HY_Catchment>> recieving_catchment : this->recieving_catchments) {
        double downstream_flow = this->get_downstream_flow(recieving_catchment.second->get_id(), time_step, 100.0);
        downstream_flows.push_back(downstream_flow);
    }*/

    /*double overall_response = this->response_accumulator->accumulate(downstream_flows, this);
    this->write_output(time_step, overall_response);
    return overall_response;*/
    return downstream_flow;
}

HY_HydroNexus::~HY_HydroNexus()
{
    if (this->output_stream.is_open()) {
        this->output_stream.close();
    }
}
