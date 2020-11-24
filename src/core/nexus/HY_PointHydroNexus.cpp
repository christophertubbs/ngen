#include "HY_PointHydroNexus.hpp"

#include <boost/exception/all.hpp>

typedef boost::error_info<struct tag_errmsg, std::string> errmsg_info;

struct invalid_downstream_request : public boost::exception, public std::exception
{
  const char *what() const noexcept { return "All downstream catchments can not request more than 100% of flux in total"; }
};

struct add_to_summed_nexus : public boost::exception, public std::exception
{
  const char *what() const noexcept { return "Can not add water to a summed point nexus"; }
};

struct request_from_empty_nexus : public boost::exception, public std::exception
{
  const char *what() const noexcept { return "Can not release water from an empty nexus"; }
};

struct completed_time_step : public boost::exception, public std::exception
{
  const char *what() const noexcept { return "Can not operate on a completed time step"; }
};

struct invalid_time_step : public boost::exception, public std::exception
{
  const char *what() const noexcept { return "Time step before minimum time step requested"; }
};

HY_PointHydroNexus::HY_PointHydroNexus(int nexus_id_number, std::string nexus_id, int num_downstream) : HY_HydroNexus(nexus_id_number, nexus_id, num_downstream), upstream_flows()
{

}

HY_PointHydroNexus::~HY_PointHydroNexus()
{
    //dtor
}

double HY_PointHydroNexus::get_downstream_flow(time_step_t timestep, HY_Catchment* catchment)
{

    if ( timestep < min_timestep ) {
        // This parameter is saying that we're at, say time step 2, but the earliest we could ever possibly be is 6
        BOOST_THROW_EXCEPTION(invalid_time_step());
    }
    if ( completed.find(timestep) != completed.end() ) {
        // If we find evidence that we've completed this timestep, we exit out
        BOOST_THROW_EXCEPTION(completed_time_step());
    }

    auto upstream_flows_at_timestep = upstream_flows.find(timestep);

    double percent_flow = catchment->get_flow_percentage();

    if ( percent_flow > 100.0)
    {
        // no downstream may ever recieve more than 100% of flows

        BOOST_THROW_EXCEPTION(invalid_downstream_request());
    }
    else if ( upstream_flows_at_timestep == upstream_flows.end() )
    {
        // there are no recorded flows for this time.
        // throw exception

        BOOST_THROW_EXCEPTION(request_from_empty_nexus() );
    }
    else
    {
        if ( this->get_summed_flows().find(timestep) == this->get_summed_flows().end() )
        {
            // the flows have not been summed calculate the sum
            // and store it into summed_flows
            double sum {};
            for(std::pair<std::string, double> idAndflow : upstream_flows_at_timestep->second )
            {
                sum += idAndflow.second;
            }

            this->get_summed_flows()[timestep] = sum;

            // mark downstream request with the amount of flow requested
            // and the catchment making the request

            request_vector v;
            v.push_back(std::pair<std::string,double>(catchment_id,percent_flow));
            this->get_downstream_requests()[timestep] = v;

            // record the total requests for this time
            this->get_total_requests()[timestep] = percent_flow;

            // release flux
            return sum * (percent_flow / 100);
        }
        else
        {
            // flows have been summed so some water has allready been release

            if ( this->get_total_requests()[timestep] + percent_flow > 100.0 )
            {
                    // if the amount of flow allready released plus the amount
                    // of this release is greater than 100 throw an error
                    BOOST_THROW_EXCEPTION(invalid_downstream_request());
            }
            else
            {
                // update the total_request for this timesteo
                this->get_total_requests()[timestep] += percent_flow;

                // add this request to recorded downstream requests
                this->get_downstream_requests()[timestep].push_back(std::pair<std::string,double>(catchment_id,percent_flow));

                double released_flux = this->get_summed_flows()[timestep] * (percent_flow / 100.0);

                if (100.0 - this->get_total_requests()[timestep] < 0.00005 )
                {
                    // all water has been requested remove bookeeping

                    upstream_flows.erase(upstream_flows.find(timestep));
                    this->get_downstream_requests().erase(this->get_downstream_requests().find(timestep));
                    this->get_summed_flows().erase(this->get_summed_flows().find(timestep));
                    this->get_total_requests().erase(this->get_total_requests().find(timestep));

                    completed.emplace(timestep);
                }

                return released_flux;
            }
        }
    }
}

void HY_PointHydroNexus::add_upstream_flow(double value, std::shared_ptr<HY_Catchment> catchment, time_step_t timestep)
{
    if ( timestep < min_timestep ) {
        BOOST_THROW_EXCEPTION(invalid_time_step());
    }
    else if ( completed.find(timestep) != completed.end() ) {
        BOOST_THROW_EXCEPTION(completed_time_step());
    }

    auto flowsForTimestep = upstream_flows.find(timestep);

    if (  flowsForTimestep == upstream_flows.end() )
    {
        // case 1 there are no upstream flow for this time
        // create a new vector of flow id pairs and add the current flow
        // and catchment id to the vector then insert the vector

        id_flow_vector v;
        v.push_back(std::pair<std::string,double>(catchment_id,value));
        upstream_flows[timestep] = v;
    }
    else
    {
        if ( this->get_summed_flows().find(timestep) == this->get_summed_flows().end() )
        {
            // case 2 there are no summed flow for the time
            // this means there have been no downstream request and we can add water

             flowsForTimestep->second.push_back(std::pair<std::string,double>(catchment_id,value));
        }
        else
        {
            // case 3 summed flows exist we can not add water for a time step when
            // one or more catchments have made downstream requests

             BOOST_THROW_EXCEPTION(add_to_summed_nexus());
         }
    }
}

std::string HY_PointHydroNexus::get_flow_units()
{
    return std::string("m3/s");
}

void HY_PointHydroNexus::set_mintime(time_step_t t)
{
    min_timestep = t;

    // remove expired time steps from completed
    for( auto& t: completed)
    {
        if ( t < min_timestep )
        {
            completed.erase(t);
        }
    }

    // C++ 2014 would allow this do be done with a single lambda
    auto clearIdValueCollections = [](int minimum_timestep, std::unordered_map<long, id_flow_vector>& idsToValueCollection)
    {
        for( std::pair<long, id_flow_vector> idToValues : idsToValueCollection)
        {
            if ( idToValues.first < minimum_timestep )
            {
                idsToValueCollection.erase(idToValues.first);
            }
        }
    };

    // C++ 2014 would allow this do be done with a single lambda
    auto clearStepsToValues = [](int minimum_timestep, std::unordered_map<long,double>& stepsToValues)
    {
        for(std::pair<long, double> stepToValue: stepsToValues)
        {
            if ( stepToValue.first < minimum_timestep )
            {
                stepsToValues.erase(stepToValue.first);
            }
        }
    };

    // remove expired time steps from all maps
    clearIdValueCollections(min_timestep, downstream_requests);
    clearIdValueCollections(min_timestep, upstream_flows);

    clearStepsToValues(min_timestep, summed_flows);
    clearStepsToValues(min_timestep, total_requests);

}
