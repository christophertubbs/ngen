#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include <FeatureBuilder.hpp>
#include <FeatureVisitor.hpp>
#include "features/Features.hpp"

#include "realizations/catchment/Formulation_Manager.hpp"

#include <HY_HydroNexus.hpp>
#include <HY_Catchment.hpp>
#include <Formulation.hpp>
#include <HY_PointHydroNexus.hpp>

#include "NGenConfig.h"
#include "tshirt_params.h"

#include <FileChecker.h>
#include <boost/algorithm/string.hpp>
#include "CatchmentGenerator.hpp"
#include "NexusGenerator.hpp"

std::string catchmentDataFile = "";
std::string nexusDataFile = "";
std::string REALIZATION_CONFIG_PATH = "";

//TODO this is possible, but ASSUMES realizations based on feature geom type, so not quite ready for prime time
class RealizaitonVisitor : public geojson::FeatureVisitor {

};

// TODO: Avoid having to call this in favor of having this being a core part of the creation of both the initial collections
void prepare_features(geojson::GeoJSON& nexus, geojson::GeoJSON& catchments, bool validate=false)
{
  for(auto& feature: *nexus){
    feature->set_id(feature->get_property("ID").as_string());
    //std::cout << "Got Nexus Feature " << feature->get_id() << std::endl;
  }
  nexus->update_ids();
  //Now read the collection of catchments, iterate it and add them to the nexus collection
  //also link them by to->id
  //std::cout << "Iterating Catchment Features" << std::endl;
  for(auto& feature: *catchments){
    feature->set_id(feature->get_property("ID").as_string());
    nexus->add_feature(feature);
    //std::cout<<"Catchment "<<feature->get_id()<<" -> Nexus "<<feature->get_property("toID").as_string()<<std::endl;
  }
  std::string linkage = "toID";
  nexus->link_features_from_property(nullptr, &linkage);

  if(validate){
    //Convience check on linkage
    for(auto& feature: *nexus){
      //feature->set_id(feature->get_property("ID").as_string());
      //std::cout << "Got Nexus Feature " << feature->get_id() << std::endl;
      if( feature->get_id().substr(0, 3) == "cat") {
        auto downstream = feature->destination_features();
        if(downstream.size() > 1) {
          std::cerr << "catchment " << feature->get_id() << " has more than one downstream connection" << std::endl;
        }
        else if(downstream.size() == 0) {
            std::cerr << "catchment " << feature->get_id() << " has NO downstream connection" << std::endl;
        }
        else {
          std::cout <<"catchment feature " << feature->get_id() << " to nexus feature " << downstream[0]->get_id() << std::endl;
        }
       }
     }//end for(feature: nexus)
   }//end if(validate)
}

std::unordered_map<std::string, std::unique_ptr<HY_HydroNexus>> nexus_realizations;
std::unordered_map<std::string, std::string> catchment_to_nexus;
std::unordered_map<std::string, std::string> nexus_to_catchment;
std::unordered_map<std::string, std::string> nexus_from_catchment;
std::unordered_map<std::string, std::vector<double>> output_map;

std::unordered_map<std::string, std::ofstream> catchment_outfiles;
std::unordered_map<std::string, std::ofstream> nexus_outfiles;

//TODO move catchment int identity to relization, and update nexus to use string id
std::unordered_map<std::string, int> catchment_id;


pdm03_struct get_et_params() {
  // create the struct used for ET
    pdm03_struct pdm_et_data;
    pdm_et_data.scaled_distribution_fn_shape_parameter = 1.3;
    pdm_et_data.vegetation_adjustment = 0.99;
    pdm_et_data.model_time_step = 0.0;
    pdm_et_data.max_height_soil_moisture_storerage_tank = 400.0;
    pdm_et_data.maximum_combined_contents = pdm_et_data.max_height_soil_moisture_storerage_tank /
                                            (1.0 + pdm_et_data.scaled_distribution_fn_shape_parameter);
    return pdm_et_data;
}

typedef Simple_Lumped_Model_Realization _hymod;
typedef realization::Tshirt_Realization _tshirt;

int main(int argc, char *argv[]) {
    std::cout << "Hello there " << ngen_VERSION_MAJOR << "."
              << ngen_VERSION_MINOR << "."
              << ngen_VERSION_PATCH << std::endl;
    std::ios::sync_with_stdio(false);


        //Pull a few "options" form the cli input, this is a temporary solution to CLI parsing!
        //Use "positional args"
        //arg 0 is program name
        //arg 1 is catchment_data file path
        //arg 2 is catchment subset ids, comma seperated string of ids (no spaces!), "" for all
        //arg 3 is nexus_data file path
        //arg 4 is nexus subset ids, comma seperated string of ids (no spaces!), "" for all
        //arg 5 is realization config path

        std::vector<string> catchment_subset_ids;
        std::vector<string> nexus_subset_ids;

        if( argc < 6) {
            std::cout << "Missing required args:" << std::endl;
            std::cout << argv[0] << " <catchment_data_path> <catchment subset ids> <nexus_data_path> <nexus subset ids>"
                      << " <realization_config_path>" << std::endl;
            if(argc > 3) {
                std::cout << std::endl << "Note:" << std::endl
                          << "Arguments for <catchment subset ids> and <nexus subset ids> must be given." << std::endl
                          << "Use empty string (\"\") as explicit argument when no subset is needed." << std::endl;
            }
            exit(-1);
        }
        else {
          bool error = false;

          if( !utils::FileChecker::file_is_readable(argv[1]) ) {
            std::cout<<"catchment data path "<<argv[1]<<" not readable"<<std::endl;
            error = true;
          }
          else{ catchmentDataFile = argv[1]; }

          if( !utils::FileChecker::file_is_readable(argv[3]) ) {
            std::cout<<"nexus data path "<<argv[3]<<" not readable"<<std::endl;
            error = true;
          }
          else { nexusDataFile = argv[3]; }

          if( !utils::FileChecker::file_is_readable(argv[5]) ) {
            std::cout<<"realization config path "<<argv[5]<<" not readable"<<std::endl;
            error = true;
          }
          else{ REALIZATION_CONFIG_PATH = argv[5]; }

          if(error) exit(-1);
          //split the subset strings into vectors
          boost::split(catchment_subset_ids, argv[2], [](char c){return c == ','; } );
          boost::split(nexus_subset_ids, argv[4], [](char c){return c == ','; } );
          //If a single id or no id is passed, the subset vector will have size 1 and be the id or the ""
          //if we get an empy string, pop it from the subset list.
          if(nexus_subset_ids.size() == 1 && nexus_subset_ids[0] == "") nexus_subset_ids.pop_back();
          if(catchment_subset_ids.size() == 1 && catchment_subset_ids[0] == "") catchment_subset_ids.pop_back();
        }

    //Read the collection of nexus
    std::cout << "Building Nexus collection" << std::endl;

    // TODO: Instead of iterating through a collection of FeatureBase objects mapping to nexi, we instead want to iterate through HY_HydroLocation objects
    geojson::GeoJSON nexus_collection = geojson::read(nexusDataFile, nexus_subset_ids);
    std::cout << "Building Catchment collection" << std::endl;

    // TODO: Instead of iterating through a collection of FeatureBase objects mapping to catchments, we instead want to iterate through HY_Catchment objects
    geojson::GeoJSON catchment_collection = geojson::read(catchmentDataFile, catchment_subset_ids);

    prepare_features(nexus_collection, catchment_collection, !true);

    // TODO: Have these formulations attached to the prior HY_Catchment objects
    realization::Formulation_Manager manager = realization::Formulation_Manager(REALIZATION_CONFIG_PATH);
    manager.read(catchment_collection, utils::getStdOut());

    realization::CatchmentGenerator catchment_generator(catchmentDataFile, &manager, utils::getStdOut());
    map<std::string, std::shared_ptr<HY_Catchment>> generated_catchments = catchment_generator.generate_catchments();
    realization::NexusGenerator nexus_generator(nexusDataFile, generated_catchments);

    //TODO don't really need catchment_collection once catchments are added to nexus collection
    catchment_collection.reset();
    for(auto& feature : *nexus_collection)
    {
      std::string feat_id = feature->get_id();
      //FIXME rework how we use IDs to NOT force parsing???
      //Skipping IDs that aren't "real" i.e. have a  NA id
      if (feat_id.substr(4) == "NA") continue;

      // We need a better way to identify catchments vs nexi
      if( feat_id.substr(0, 3) == "cat" ){
        catchment_outfiles[feat_id].open(feature->get_id()+"_output.csv", std::ios::trunc);

        if(feature->get_number_of_destination_features() == 1)
        {
          catchment_to_nexus[feat_id] = feature->destination_features()[0]->get_id();
        }
        else
        {
          //TODO
        }
        catchment_id[feat_id] = std::stoi(feat_id.substr(4));
      }else{
        //Create nexus realization, add to map
        int num = std::stoi( feat_id.substr(4) );
        nexus_outfiles[feat_id].open("./"+feature->get_id()+"_output.csv", std::ios::trunc);

        nexus_realizations[feat_id] = std::make_unique<HY_PointHydroNexus>(
                                      HY_PointHydroNexus(num, feat_id,
                                                         feature->get_number_of_destination_features()));
       if(feature->get_number_of_destination_features() == 1)
       {
         nexus_to_catchment[feat_id] = feature->destination_features()[0]->get_id();
       }
       else if(feature->get_number_of_origination_features() == 1)
       {
         nexus_from_catchment[feat_id] = feature->origination_features()[0]->get_id();
       }
       else{
         //TODO
       }
       output_map[feat_id] = std::vector<double>();
      }

    }

    std::cout<<"Running Models"<<std::endl;

    pdm03_struct pdm_et_data = get_et_params();

    //Now loop some time, iterate catchments, do stuff for 720 hourly time steps
    for(int time_step = 0; time_step < 720; time_step++)
    {
      std::cout<<"Time step "<<time_step<<std::endl;

      for (HY_HydroNexus nexus : nexus_generator) {
        nexus.run_formulations(time_step, 3600.0, &pdm_et_data);
      }

      for (std::pair<std::string, std::shared_ptr<realization::Formulation>> formulation_pair : manager ) {
        std::string formulation_name = formulation_pair.first;
        std::shared_ptr<realization::Formulation> formulation = formulation_pair.second;

        //get the formulation result
        double response = formulation->get_response(0, time_step, 3600.0, &pdm_et_data);

        std::string target_nexus_id = catchment_to_nexus[formulation_name];

        // Tell the user what the result was in m/s
        std::cout<< "\tCatchment "<< formulation_name << " contributing " << response << " m/s to " << target_nexus_id << std::endl;

        // Record the resulting value to the catchments output file
        std::ofstream catchment_output = catchment_outfiles[formulation_name];
        catchment_output << time_step << ", " << response << std::endl;

        // Determine the greater impact that the channel had on the following nexus
        geojson::Feature nexus_feature = nexus_collection->get_feature(formulation_name);
        geojson::multipolygon_t nexus_geometry = nexus_feature->geometry<geojson::multipolygon_t>();
        double nexus_area = boost::geometry::area(nexus_geometry);

        response = response * nexus_area;

        std::cout << "\t\tThe modified response is: " << response << std::endl;

        //update the nexus with this flow
        std::unique_ptr<HY_HydroNexus> target_nexus = nexus_realizations[target_nexus_id];
        target_nexus->add_upstream_flow(response, formulation_name, time_step);
      }

      for(std::pair<std::string, std::unique_ptr<HY_HydroNexus>> id_nexus: nexus_realizations)
      {
        //TODO this ID isn't all that important is it?  And really it should connect to
        //the downstream waterbody the way we are using it, so consider if this is needed
        //it works for now though, so keep it
        std::string nexus_name = id_nexus.first;
        std::unique_ptr<HY_HydroNexus> nexus = id_nexus.second;

        // Get the ID for the catchment that sent values here
        // WARNING: Makes the assumption that only one catchment supplies values
        std::string source_catchment_name = nexus_from_catchment[nexus_name];
        int catchment_id = catchment_id[source_catchment_name];

        std::cout << "nexusID: " << catchment_id << std::endl;

        // Get the contribution of the collected catchment to this nexus' downstream flow
        double contribution_at_timestep = nexus->get_downstream_flow(catchment_id, time_step, 100.0);

        std::ofstream nexus_output_file = nexus_outfiles[nexus_name];

        if(nexus_output_file.is_open())
        {
          // Record how much the catchment contributed to this nexus' downstream flow
          nexus_output_file << time_step << ", " << contribution_at_timestep << std::endl;
        }

        std::cout << "\tNexus " << nexus_name << " has " << contribution_at_timestep << " m^3/s" << std::endl;

        // Update the overall collection of results
        std::vector<double> output_record = output_map[nexus_name];
        output_record.push_back(contribution_at_timestep);
      }
    }

    /*
        The basic driving algorithm looks something like this:

        Iteration 1: Only read catchments and nexus
        Read configuration;
        From configuration, read inputs
        inputs: id, realization enum, input file (geojson), forcing provider (enum)

        loop:
          id -> read geojson -> lookup by id -> construct catchment realization and nexus
        TODO catchment realizations will need to construct forcing objects (implement forcing provider interface) upon constructuion
        One INDEPDENT timestep
        run catchment A -> forcing.get(time, dt, <grid?> ) -> run_model(time, dt, forcing)
        run catchment B

        Iteration 2: Read waterbodies, build linkage between nexus/waterbody, apply routing, multiple timesteps

    */
}
