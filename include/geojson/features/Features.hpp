#ifndef GEOJSON_FEATURES_H
#define GEOJSON_FEATURES_H

#include "FeatureBase.hpp"
#include "PointFeature.hpp"
#include "LineStringFeature.hpp"
#include "PolygonFeature.hpp"
#include "MultiPointFeature.hpp"
#include "MultiLineStringFeature.hpp"
#include "MultiPolygonFeature.hpp"
#include "CollectionFeature.hpp"

#include <exception>

namespace geojson {
    namespace features {
        static Feature create_feature(
            coordinate_t coordinate,
            std::string new_id = "",
            PropertyMap new_properties = PropertyMap(),
            std::vector<double> new_bounding_box = std::vector<double>(),
            std::vector<FeatureBase*> upstream_features = std::vector<FeatureBase*>(),
            std::vector<FeatureBase*> downstream_features = std::vector<FeatureBase*>(),
            std::map<std::string, JSONProperty> members = std::map<std::string, JSONProperty>()
        ) {
            return std::make_shared<PointFeature>(
                coordinate,
                new_id,
                new_properties,
                new_bounding_box,
                upstream_features,
                downstream_features,
                members
            );
        }

        static Feature create_feature(
            linestring_t linestring,
            std::string new_id = "",
            PropertyMap new_properties = PropertyMap(),
            std::vector<double> new_bounding_box = std::vector<double>(),
            std::vector<FeatureBase*> upstream_features = std::vector<FeatureBase*>(),
            std::vector<FeatureBase*> downstream_features = std::vector<FeatureBase*>(),
            std::map<std::string, JSONProperty> members = std::map<std::string, JSONProperty>()
        ) {
            return std::make_shared<LineStringFeature>(
                linestring,
                new_id,
                new_properties,
                new_bounding_box,
                upstream_features,
                downstream_features,
                members
            );
        }

        static Feature create_feature(
            polygon_t polygon,
            std::string new_id = "",
            PropertyMap new_properties = PropertyMap(),
            std::vector<double> new_bounding_box = std::vector<double>(),
            std::vector<FeatureBase*> upstream_features = std::vector<FeatureBase*>(),
            std::vector<FeatureBase*> downstream_features = std::vector<FeatureBase*>(),
            std::map<std::string, JSONProperty> members = std::map<std::string, JSONProperty>()
        ) {
            return std::make_shared<PolygonFeature>(
                polygon,
                new_id,
                new_properties,
                new_bounding_box,
                upstream_features,
                downstream_features,
                members
            );
        }

        static Feature create_feature(
            multipoint_t multipoint,
            std::string new_id = "",
            PropertyMap new_properties = PropertyMap(),
            std::vector<double> new_bounding_box = std::vector<double>(),
            std::vector<FeatureBase*> upstream_features = std::vector<FeatureBase*>(),
            std::vector<FeatureBase*> downstream_features = std::vector<FeatureBase*>(),
            std::map<std::string, JSONProperty> members = std::map<std::string, JSONProperty>()
        ) {
            return std::make_shared<MultiPointFeature>(
                multipoint,
                new_id,
                new_properties,
                new_bounding_box,
                upstream_features,
                downstream_features,
                members
            );
        }

        static Feature create_feature(
            multilinestring_t multilinestring,
            std::string new_id = "",
            PropertyMap new_properties = PropertyMap(),
            std::vector<double> new_bounding_box = std::vector<double>(),
            std::vector<FeatureBase*> upstream_features = std::vector<FeatureBase*>(),
            std::vector<FeatureBase*> downstream_features = std::vector<FeatureBase*>(),
            std::map<std::string, JSONProperty> members = std::map<std::string, JSONProperty>()
        ) {
            return std::make_shared<MultiLineStringFeature>(
                multilinestring,
                new_id,
                new_properties,
                new_bounding_box,
                upstream_features,
                downstream_features,
                members
            );
        }

        static Feature create_feature(
            multipolygon_t multipolygon,
            std::string new_id = "",
            PropertyMap new_properties = PropertyMap(),
            std::vector<double> new_bounding_box = std::vector<double>(),
            std::vector<FeatureBase*> upstream_features = std::vector<FeatureBase*>(),
            std::vector<FeatureBase*> downstream_features = std::vector<FeatureBase*>(),
            std::map<std::string, JSONProperty> members = std::map<std::string, JSONProperty>()
        ) {
            return std::make_shared<MultiPolygonFeature>(
                multipolygon,
                new_id,
                new_properties,
                new_bounding_box,
                upstream_features,
                downstream_features,
                members
            );
        }

        static Feature create_feature(
            std::vector<geojson::geometry> geometry_collection,
            std::string new_id = "",
            PropertyMap new_properties = PropertyMap(),
            std::vector<double> new_bounding_box = std::vector<double>(),
            std::vector<FeatureBase*> upstream_features = std::vector<FeatureBase*>(),
            std::vector<FeatureBase*> downstream_features = std::vector<FeatureBase*>(),
            std::map<std::string, JSONProperty> members = std::map<std::string, JSONProperty>()
        ) {
            return std::make_shared<CollectionFeature>(
                geometry_collection,
                new_id,
                new_properties,
                new_bounding_box,
                upstream_features,
                downstream_features,
                members
            );
        }

        static Feature extract_feature(Feature original) {
            Feature new_feature;

            switch (original->get_type()) {
                case FeatureType::GeometryCollection:
                    new_feature = create_feature(
                        original->get_geometry_collection(),
                        original->get_id(),
                        original->get_properties(),
                        original->get_bounding_box()
                    );
                    break;
                case FeatureType::LineString:
                    new_feature = create_feature(
                        original->geometry<linestring_t>(),
                        original->get_id(),
                        original->get_properties(),
                        original->get_bounding_box()
                    );
                    break;
                case FeatureType::MultiLineString:
                    new_feature = create_feature(
                        original->geometry<multilinestring_t>(),
                        original->get_id(),
                        original->get_properties(),
                        original->get_bounding_box()
                    );
                    break;
                case FeatureType::MultiPoint:
                    new_feature = create_feature(
                        original->geometry<multipoint_t>(),
                        original->get_id(),
                        original->get_properties(),
                        original->get_bounding_box()
                    );
                    break;
                case FeatureType::MultiPolygon:
                    new_feature = create_feature(
                        original->geometry<multipolygon_t>(),
                        original->get_id(),
                        original->get_properties(),
                        original->get_bounding_box()
                    );
                    break;
                case FeatureType::Point:
                    new_feature = create_feature(
                        original->geometry<coordinate_t>(),
                        original->get_id(),
                        original->get_properties(),
                        original->get_bounding_box()
                    );
                    break;
                case FeatureType::Polygon:
                    new_feature = create_feature(
                        original->geometry<polygon_t>(),
                        original->get_id(),
                        original->get_properties(),
                        original->get_bounding_box()
                    );
                case FeatureType::None:
                    std::string message;

                    if (original->get_id() != "") {
                        message = "The partially defined feature named '" + original->get_id() + 
                            "' cannot be extracted. A Geometry must be present.";
                    }
                    else {
                        message = "Partially defined features cannot be extracted. A geometry must be present.";
                    }

                    throw std::invalid_argument(
                        message
                    );
            }

            for (std::string member_key : original->keys()) {
                new_feature->set(member_key, JSONProperty(original->get(member_key)));
            }

            return new_feature;
        }
    }
}
#endif // GEOJSON_FEATURE_H