#ifndef HY_HYDRONEXUS_H
#define HY_HYDRONEXUS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>


#include "HY_Catchment.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include "HY_HydroLocation.hpp"
#include <Forcing.h>

namespace bg = boost::geometry;

class HY_HydroNexus
{
    public:
        typedef long time_step_t;
        typedef std::vector< std::pair<std::string,double> > request_vector;

        HY_HydroNexus(int nexus_id_num, std::string nexus_id, int num_dowstream);
        virtual ~HY_HydroNexus();

        /** Increase the downstream flow for timestep_t by input amount*/
        virtual void add_upstream_flow(double val, std::shared_ptr<HY_Catchment> catchment, time_step_t timestep) = 0;

        /** get a precentage of the downstream flow at requested time_step. Record the requesting percentage*/
        virtual double get_downstream_flow(time_step_t timestep)=0;

        virtual double run_formulations(time_step_t time_step, double step_duration, void* pdm_data);

        /** get the units that the flows are described in */
        virtual std::string get_flow_units()=0;

        virtual void set_location(std::shared_ptr<HY_HydroLocation> location) {
            this->location = location;
        }

        void add_receiving_catchment(std::shared_ptr<HY_Catchment> catchment) {
            this->recieving_catchments.emplace(catchment->get_id(), catchment);
        }

        void add_contributing_catchment(std::shared_ptr<HY_Catchment> catchment) {
            this->contributing_catchments.emplace(catchment->get_id(), catchment);
        }

        std::shared_ptr<HY_Catchment> get_receiving_catchment(std::string identifier);
        std::shared_ptr<HY_Catchment> get_contributing_catchment(std::string identifier);

    protected:
        void add_downstream_request(time_step_t timestep, std::string identifier, double percent_flow) {
            request_vector request;
            request.push_back(std::pair<std::string, double>(identifier, percent_flow));
            this->downstream_requests.emplace(timestep, request);
        }

        virtual void write_output(time_step_t timestep, double value) {
            if (!this->output_stream.is_open()) {
                this->output_stream.open(this->output_file_name, std::ios::trunc);
            }
            this->output_stream << timestep << "," << value << std::endl;
        }

        virtual std::map<std::string, std::shared_ptr<HY_Catchment>> get_receiving_catchments() const {
            return this->recieving_catchments;
        }

        virtual std::map<std::string, std::shared_ptr<HY_Catchment>> get_contributing_catchments() const {
            return this->contributing_catchments;
        }

        virtual std::unordered_map<time_step_t, request_vector> get_downstream_requests() const {
            return this->downstream_requests;
        }

        virtual std::unordered_map<time_step_t, double> get_summed_flows() const {
            return this->summed_flows;
        }

        virtual std::unordered_map<time_step_t, double> get_total_requests() const {
            return this->total_requests;
        }

        virtual std::map<std::shared_ptr<HY_Catchment>, double> get_catchment_flow_percentages() {
            double percent_left = 100.0;
            std::map<std::shared_ptr<HY_Catchment>, double> flow_percentages;
            std::vector<std::shared_ptr<HY_Catchment>> missing_flow_percentages;

            for (std::pair<std::string, std::shared_ptr<HY_Catchment>> idAndCatchment : this->recieving_catchments) {
                if (idAndCatchment.second->get_flow_percentage() < 0) {
                    missing_flow_percentages.push_back(idAndCatchment.second);
                }
                else {
                    flow_percentages.emplace(idAndCatchment.second, idAndCatchment.second->get_flow_percentage());
                    percent_left -= idAndCatchment.second->get_flow_percentage();
                }
            }

            if (not missing_flow_percentages.empty()) {
                double uniform_amount_left = percent_left / missing_flow_percentages.size();

                for (std::shared_ptr<HY_Catchment> catchment : missing_flow_percentages) {
                    flow_percentages.emplace(catchment, uniform_amount_left);
                }
            }

            return flow_percentages;
        }

        std::ofstream output_stream;
        std::map<std::string, std::shared_ptr<HY_Catchment>> recieving_catchments;
        std::map<std::string, std::shared_ptr<HY_Catchment>> contributing_catchments;

        std::shared_ptr<HY_HydroLocation> location;

        double area;

        std::unordered_map<time_step_t, request_vector > downstream_requests;
        std::unordered_map<time_step_t, double> summed_flows;
        std::unordered_map<time_step_t, double> total_requests;
    private:
        unsigned long id_number;
        std::string id;

        std::string output_file_name;

        int number_of_downstream_catchments;
};

#endif // HY_HYDRONEXUS_H
