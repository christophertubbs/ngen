#include "gtest/gtest.h"
#include <Realization_Manager.hpp>
#include <Catchment_Realization.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class Realization_Manager_Test : public ::testing::Test {

    protected:


    Realization_Manager_Test() {

    }

    ~Realization_Manager_Test() override {

    }

    void SetUp() override {

    }

    void TearDown() override {

    }

};

const double EPSILON = 0.0000001;

const std::string EXAMPLE = "{ "
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
                "\"crosswalk_path\": \"./data/sugar_creek/crosswalk_subset.json\" "
            "} "
        "}, "
        "\"forcing\": { "
            "\"file_pattern\": \".*{{ID}}.*.csv\", "
            "\"path\": \"./data/sugar_creek/forcing/\", "
            "\"start_time\": \"2015-12-01 00:00:00\", "
            "\"end_time\": \"2015-12-30 23:00:00\" "
        "} "
    "}, "
    "\"catchments\": { "
        "\"wat-88\": { "
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
                        "\"crosswalk_path\": \"./data/sugar_creek/crosswalk_subset.json\" "
                    "} "
                "}, "
                "\"forcing\": { "
                    "\"path\": \"./data/sugar_creek/forcing/cat-88_2015-12-01 00_00_00_2015-12-30 23_00_00.csv\", "
                    "\"start_time\": \"2015-12-01 00:00:00\", "
                    "\"end_time\": \"2015-12-30 23:00:00\" "
                "} "
        "}, "
        "\"wat-89\": { "
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
                "\"path\": \"./data/sugar_creek/forcing/cat-89_2015-12-01 00_00_00_2015-12-30 23_00_00.csv\", "
                "\"start_time\": \"2015-12-01 00:00:00\", "
                "\"end_time\": \"2015-12-30 23:00:00\" "
            "} "
        "} "
    "} "
"}";

TEST_F(Realization_Manager_Test, basic_reading) {
    std::stringstream stream;
    stream << EXAMPLE;

    realization::Realization_Manager manager = realization::Realization_Manager(stream);

    ASSERT_TRUE(manager.is_empty());
    manager.read();

    ASSERT_EQ(manager.get_size(), 2);

    ASSERT_TRUE(manager.contains("wat-88"));
    ASSERT_TRUE(manager.contains("wat-89"));
}