#include "gtest/gtest.h"
#include <Formulation_Manager.hpp>
#include <Catchment_Formulation.hpp>

#include <iostream>
#include <memory>

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class Formulation_Manager_Test : public ::testing::Test {

    protected:


    Formulation_Manager_Test() {

    }

    ~Formulation_Manager_Test() override {

    }

    void SetUp() override {

    }

    void TearDown() override {

    }

};

const double EPSILON = 0.0000001;

const std::string EXAMPLE_1 = "{ "
    "\"global\": { "
        "\"tshirt\": { "
            "\"maxsmc\": 0.81, "
            "\"wltsmc\": 1.0, "
            "\"satdk\": 0.48, "
            "\"satpsi\": 0.1, "
            "\"slope\": 0.58, "
            "\"scaled_distribution_fn_shape_parameter\": 1.3, "
            "\"multiplier\": 1.0, "
            "\"alpha_fc\": 1.0, "
            "\"Klf\": 0.0000672, "
            "\"Kn\": 0.1, "
            "\"nash_n\": 8, "
            "\"Cgw\": 1.08, "
            "\"expon\": 6.0, "
            "\"max_groundwater_storage_meters\": 16.0, "
            "\"nash_storage\": [ "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0 "
            "], "
            "\"soil_storage_percentage\": 1.0, "
            "\"groundwater_storage_percentage\": 1.0, "
            "\"timestep\": 3600, "
            "\"giuh\": { "
                "\"giuh_path\": \"./test/data/giuh/GIUH.json\", "
                "\"crosswalk_path\": \"./data/crosswalk.json\" "
            "} "
        "}, "
        "\"forcing\": { "
            "\"file_pattern\": \".*{{ID}}.*.csv\", "
            "\"path\": \"./data/forcing/\", "
            "\"start_time\": \"2015-12-01 00:00:00\", "
            "\"end_time\": \"2015-12-30 23:00:00\" "
        "} "
    "}, "
    "\"catchments\": { "
        "\"wat-88\": { "
            "\"simple_lumped\": { "
                "\"sr\": [ "
                    "1.0, "
                    "1.0, "
                    "1.0 "
                "], "
                "\"storage\": 1.0, "
                "\"max_storage\": 1000.0, "
                "\"a\": 1.0, "
                "\"b\": 10.0, "
                "\"Ks\": 0.1, "
                "\"Kq\": 0.01, "
                "\"n\": 3, "
                "\"t\": 0 "
            "}, "
            "\"forcing\": { "
                "\"path\": \"./data/forcing/cat-88_2015-12-01 00_00_00_2015-12-30 23_00_00.csv\", "
                "\"start_time\": \"2015-12-01 00:00:00\", "
                "\"end_time\": \"2015-12-30 23:00:00\" "
            "} "
        "}, "
        "\"wat-89\": { "
            "\"tshirt\": { "
                "\"maxsmc\": 0.81, "
                "\"wltsmc\": 1.0, "
                "\"satdk\": 0.48, "
                "\"satpsi\": 0.1, "
                "\"slope\": 0.58, "
                "\"scaled_distribution_fn_shape_parameter\": 1.3, "
                "\"multiplier\": 1.0, "
                "\"alpha_fc\": 1.0, "
                "\"Klf\": 0.0000672, "
                "\"Kn\": 0.1, "
                "\"nash_n\": 8, "
                "\"Cgw\": 1.08, "
                "\"expon\": 6.0, "
                "\"max_groundwater_storage_meters\": 16.0, "
                "\"nash_storage\": [ "
                "   1.0, "
                "   1.0, "
                "   1.0, "
                "   1.0, "
                "   1.0, "
                "   1.0, "
                "   1.0, "
                "   1.0 "
                "], "
                "\"soil_storage_percentage\": 1.0, "
                "\"groundwater_storage_percentage\": 1.0, "
                "\"timestep\": 3600, "
                "\"giuh\": { "
                    "\"giuh_path\": \"./test/data/giuh/GIUH.json\", "
                    "\"crosswalk_path\": \"./data/crosswalk.json\" "
                "} "
            "}, "
            "\"forcing\": { "
                "\"path\": \"./data/forcing/cat-89_2015-12-01 00_00_00_2015-12-30 23_00_00.csv\", "
                "\"start_time\": \"2015-12-01 00:00:00\", "
                "\"end_time\": \"2015-12-30 23:00:00\" "
            "} "
        "} "
    "} "
"}";

const std::string EXAMPLE_2 = "{ "
    "\"global\": { "
        "\"tshirt_c\": { "
            "\"maxsmc\": 0.81, "
            "\"wltsmc\": 1.0, "
            "\"satdk\": 0.48, "
            "\"satpsi\": 0.1, "
            "\"slope\": 0.58, "
            "\"scaled_distribution_fn_shape_parameter\": 1.3, "
            "\"multiplier\": 1.0, "
            "\"alpha_fc\": 1.0, "
            "\"Klf\": 0.0000672, "
            "\"Kn\": 0.1, "
            "\"nash_n\": 8, "
            "\"Cgw\": 1.08, "
            "\"expon\": 6.0, "
            "\"max_groundwater_storage_meters\": 16.0, "
            "\"nash_storage\": [ "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0 "
            "], "
            "\"soil_storage_percentage\": 1.0, "
            "\"groundwater_storage_percentage\": 1.0, "
            "\"timestep\": 3600, "
            "\"giuh\": { "
                "\"giuh_path\": \"./test/data/giuh/GIUH.json\", "
                "\"crosswalk_path\": \"./data/crosswalk.json\" "
            "} "
        "}, "
        "\"forcing\": { "
            "\"file_pattern\": \".*{{ID}}.*.csv\", "
            "\"path\": \"./data/forcing/\", "
            "\"start_time\": \"2015-12-01 00:00:00\", "
            "\"end_time\": \"2015-12-30 23:00:00\" "
        "} "
    "}, "
    "\"catchments\": { "
        "\"wat-88\": { "
            "\"simple_lumped\": { "
                "\"sr\": [ "
                    "1.0, "
                    "1.0, "
                    "1.0 "
                "], "
                "\"storage\": 1.0, "
                "\"max_storage\": 1000.0, "
                "\"a\": 1.0, "
                "\"b\": 10.0, "
                "\"Ks\": 0.1, "
                "\"Kq\": 0.01, "
                "\"n\": 3, "
                "\"t\": 0 "
            "}, "
            "\"forcing\": { "
                "\"path\": \"./data/forcing/cat-88_2015-12-01 00_00_00_2015-12-30 23_00_00.csv\", "
                "\"start_time\": \"2015-12-01 00:00:00\", "
                "\"end_time\": \"2015-12-30 23:00:00\" "
            "} "
        "}, "
        "\"wat-89\": { "
            "\"tshirt_c\": { "
                "\"maxsmc\": 0.81, "
                "\"wltsmc\": 1.0, "
                "\"satdk\": 0.48, "
                "\"satpsi\": 0.1, "
                "\"slope\": 0.58, "
                "\"scaled_distribution_fn_shape_parameter\": 1.3, "
                "\"multiplier\": 1.0, "
                "\"alpha_fc\": 1.0, "
                "\"Klf\": 0.0000672, "
                "\"Kn\": 0.1, "
                "\"nash_n\": 8, "
                "\"Cgw\": 1.08, "
                "\"expon\": 6.0, "
                "\"max_groundwater_storage_meters\": 16.0, "
                "\"nash_storage\": [ "
                "   1.0, "
                "   1.0, "
                "   1.0, "
                "   1.0, "
                "   1.0, "
                "   1.0, "
                "   1.0, "
                "   1.0 "
                "], "
                "\"soil_storage_percentage\": 1.0, "
                "\"groundwater_storage_percentage\": 1.0, "
                "\"timestep\": 3600, "
                "\"giuh\": { "
                    "\"giuh_path\": \"./test/data/giuh/GIUH.json\", "
                    "\"crosswalk_path\": \"./data/crosswalk.json\" "
                "} "
            "}, "
            "\"forcing\": { "
                "\"path\": \"./data/forcing/cat-89_2015-12-01 00_00_00_2015-12-30 23_00_00.csv\", "
                "\"start_time\": \"2015-12-01 00:00:00\", "
                "\"end_time\": \"2015-12-30 23:00:00\" "
            "} "
        "} "
    "} "
"}";

const std::string EXAMPLE_3 = "{ "
    "\"global\": { "
        "\"tshirt_c\": { "
            "\"maxsmc\": 0.81, "
            "\"wltsmc\": 1.0, "
            "\"satdk\": 0.48, "
            "\"satpsi\": 0.1, "
            "\"slope\": 0.58, "
            "\"scaled_distribution_fn_shape_parameter\": 1.3, "
            "\"multiplier\": 1.0, "
            "\"alpha_fc\": 1.0, "
            "\"Klf\": 0.0000672, "
            "\"Kn\": 0.1, "
            "\"nash_n\": 8, "
            "\"Cgw\": 1.08, "
            "\"expon\": 6.0, "
            "\"max_groundwater_storage_meters\": 16.0, "
            "\"nash_storage\": [ "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0, "
                "1.0 "
            "], "
            "\"soil_storage_percentage\": 1.0, "
            "\"groundwater_storage_percentage\": 1.0, "
            "\"timestep\": 3600, "
            "\"giuh\": { "
                "\"giuh_path\": \"./test/data/giuh/GIUH.json\", "
                "\"crosswalk_path\": \"./data/crosswalk.json\" "
            "} "
        "}, "
        "\"forcing\": { "
            "\"file_pattern\": \".*{{ID}}.*.csv\", "
            "\"path\": \"./data/forcing/\", "
            "\"start_time\": \"2015-12-01 00:00:00\", "
            "\"end_time\": \"2015-12-30 23:00:00\" "
        "} "
    "}, "
    "\"catchments\": { "
        "\"wat-88\": { "
            "\"simple_lumped\": { "
                "\"sr\": [ "
                    "1.0, "
                    "1.0, "
                    "1.0 "
                "], "
                "\"storage\": 1.0, "
                "\"max_storage\": 1000.0, "
                "\"a\": 1.0, "
                "\"b\": 10.0, "
                "\"Ks\": 0.1, "
                "\"Kq\": 0.01, "
                "\"n\": 3, "
                "\"t\": 0 "
            "}, "
            "\"forcing\": { "
                "\"path\": \"./data/forcing/cat-88_2015-12-01 00_00_00_2015-12-30 23_00_00.csv\", "
                "\"start_time\": \"2015-12-01 00:00:00\", "
                "\"end_time\": \"2015-12-30 23:00:00\" "
            "} "
        "}, "
        "\"wat-89\": { "
            "\"tshirt_c\": { "
                "\"maxsmc\": 0.439, "
                "\"wltsmc\": 0.066, "
                "\"satdk\": 0.00000338, "
                "\"satpsi\": 0.355, "
                "\"slope\": 1.0, "
                // This is the 'b' (or 'bb') param
                "\"scaled_distribution_fn_shape_parameter\": 4.05, "
                "\"multiplier\": 1000.0, "
                "\"alpha_fc\": 0.33, "
                "\"Klf\": 1.70352, "
                "\"Kn\": 0.03, "
                "\"nash_n\": 2, "
                "\"Cgw\": 0.01, "
                "\"expon\": 6.0, "
                "\"max_groundwater_storage_meters\": 16.0, "
                "\"nash_storage\": [ "
                "   0.0, "
                "   0.0 "
                "], "
                "\"soil_storage_percentage\": 0.667, "
                "\"groundwater_storage_percentage\": 0.5, "
                "\"giuh\": { "
                    "\"cdf_ordinates\": ["
                    "    0.06,"
                    "    0.51,"
                    "    0.28,"
                    "    0.12,"
                    "    0.03"
                    "]"
                "} "
            "}, "
            "\"forcing\": { "
                "\"path\": \"./data/forcing/cat-89_2015-12-01 00_00_00_2015-12-30 23_00_00.csv\", "
                "\"start_time\": \"2015-12-01 00:00:00\", "
                "\"end_time\": \"2015-12-30 23:00:00\" "
            "} "
        "} "
    "} "
"}";

TEST_F(Formulation_Manager_Test, basic_reading_1) {
    std::stringstream stream;
    stream << EXAMPLE_1;

    std::ostream* raw_pointer = &std::cout;
    std::shared_ptr<std::ostream> s_ptr(raw_pointer, [](void*) {});
    utils::StreamHandler catchment_output(s_ptr);

    realization::Formulation_Manager manager = realization::Formulation_Manager(stream);

    ASSERT_TRUE(manager.is_empty());
    manager.read(catchment_output);

    ASSERT_EQ(manager.get_size(), 2);

    ASSERT_TRUE(manager.contains("wat-88"));
    ASSERT_TRUE(manager.contains("wat-89"));
}

TEST_F(Formulation_Manager_Test, basic_reading_2) {
    std::stringstream stream;
    stream << EXAMPLE_2;

    std::ostream* raw_pointer = &std::cout;
    std::shared_ptr<std::ostream> s_ptr(raw_pointer, [](void*) {});
    utils::StreamHandler catchment_output(s_ptr);

    realization::Formulation_Manager manager = realization::Formulation_Manager(stream);

    ASSERT_TRUE(manager.is_empty());
    manager.read(catchment_output);

    ASSERT_EQ(manager.get_size(), 2);

    ASSERT_TRUE(manager.contains("wat-88"));
    ASSERT_TRUE(manager.contains("wat-89"));
}

TEST_F(Formulation_Manager_Test, basic_run_1) {
    std::stringstream stream;
    stream << EXAMPLE_1;
    
    std::ostream* raw_pointer = &std::cout; 
    std::shared_ptr<std::ostream> s_ptr(raw_pointer, [](void*) {});
    utils::StreamHandler catchment_output(s_ptr);

    realization::Formulation_Manager manager = realization::Formulation_Manager(stream);
    manager.read(catchment_output);

    std::map<std::string, std::map<long, double>> calculated_results;

    pdm03_struct pdm_et_data;
    pdm_et_data.scaled_distribution_fn_shape_parameter = 1.3;
    pdm_et_data.vegetation_adjustment = 0.99;
    pdm_et_data.model_time_step = 0.0;
    pdm_et_data.max_height_soil_moisture_storerage_tank = 400.0;
    pdm_et_data.maximum_combined_contents = pdm_et_data.max_height_soil_moisture_storerage_tank / (1.0+pdm_et_data.scaled_distribution_fn_shape_parameter);

    double dt = 3600.0;

    for (std::pair<std::string, std::shared_ptr<realization::Formulation>> formulation : manager) {
        if (calculated_results.count(formulation.first) == 0) {
            calculated_results.emplace(formulation.first, std::map<long, double>());
        }

        double calculation;

        for (long t = 0; t < 4; t++) {            
            calculation = formulation.second->get_response(
                0,
                t,
                dt,
                &pdm_et_data
            );

            calculated_results.at(formulation.first).emplace(t, calculation);
        }
    }
}

TEST_F(Formulation_Manager_Test, basic_run_3) {
    std::stringstream stream;
    stream << EXAMPLE_3;

    std::ostream* raw_pointer = &std::cout;
    std::shared_ptr<std::ostream> s_ptr(raw_pointer, [](void*) {});
    utils::StreamHandler catchment_output(s_ptr);

    realization::Formulation_Manager manager = realization::Formulation_Manager(stream);
    manager.read(catchment_output);

    std::vector<double> inputs = {10.0 / 1000, 0.0, 0.0};
    std::vector<double> expected_results = {191.108626 / 1000, 177.181102 / 1000, 165.234198 / 1000};

    std::vector<double> actual_results(expected_results.size());

    for (int i = 0; i < inputs.size(); i++) {
        // Remember that for the Tshirt_C_Realization, the timestep sizes are implicit
        actual_results[i] = manager.get_formulation("wat-89")->get_response(inputs[i], 3600, 3600, nullptr);
    }

    for (int i = 0; i < actual_results.size(); i++) {
        double actual = actual_results[i];
        double error_margin = actual * 0.001;
        double expected = expected_results[i];
        double diff = actual > expected ? actual - expected : expected - actual;
        ASSERT_LE(diff, error_margin);
    }
}
