#ifndef HY_POINTHYDRONEXUS_H
#define HY_POINTHYDRONEXUS_H

#include <HY_HydroNexus.hpp>

#include <memory>
#include <unordered_map>
#include <unordered_set>

class HY_PointHydroNexus : public HY_HydroNexus
{
    public:
        HY_PointHydroNexus(int nexus_id_number, std::string nexus_id, int num_downstream);
        virtual ~HY_PointHydroNexus();

        /** get the request percentage of downstream flow through this nexus at timestep t. */
        double get_downstream_flow(time_step_t timestep);

        /** add flow to this nexus for timestep t. */
        void add_upstream_flow(double value, std::shared_ptr<HY_Catchment> catchment, time_step_t timestep);

        /** get the units that flows are represented in. */
        std::string get_flow_units();

        void set_mintime(time_step_t);

    protected:

        virtual std::map<std::string, std::shared_ptr<HY_Catchment>> get_receiving_catchments() const;

        virtual std::map<std::string, std::shared_ptr<HY_Catchment>> get_contributing_catchments() const;

        virtual std::unordered_map<time_step_t, request_vector> get_downstream_requests() const;

        virtual std::unordered_map<time_step_t, double> get_summed_flows() const;

        virtual std::unordered_map<time_step_t, double> get_total_requests() const;
    private:

        typedef std::vector< std::pair<std::string,double> > id_flow_vector;
        typedef std::unordered_map<std::shared_ptr<HY_Catchment>, double> flow_map;

        /** The current downstream flow through this Point Nexus.*/
        std::unordered_map<time_step_t, id_flow_vector > upstream_flows;
        std::unordered_map<time_step_t, flow_map> timed_flows;

        time_step_t min_timestep{0};
        std::unordered_set<time_step_t> completed;

};

#endif // HY_POINTHYDRONEXUS_H
