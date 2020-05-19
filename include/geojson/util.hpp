#ifndef GEOJSON_UTIL_H
#define GEOJSON_UTIL_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>

namespace geojson {
    template<typename T>
    static bool contains(const std::vector<T*> &container, const T *value) {
        return not (std::find(container.begin(), container.end(), value) == container.end());
    }

    template<typename T>
    static bool contains(const std::vector<T> &container, const T value) {
        return not (std::find(container.begin(), container.end(), value) == container.end());
    }

    template<typename T, class Predicate>
    static T find_first(const std::vector<T> &container, Predicate search_function) {
        auto result = std::find_if(container.begin(), container.end(), search_function);

        if (result == container.end()) {
            return nullptr;
        }
        else {
            return *result;
        }
    }

    template<typename T, class Predicate, class Assigner>
    static std::vector<T> filter(const std::vector<T> &container, Predicate filter_condition, Assigner assignment_statement = [](T to_keep){return to_keep;}) {
        std::vector<T> resultant;

        for (T element : container) {
            if (filter_condition(element)) {
                resultant.push_back(assignment_statement(element));
            }
        }

        return resultant;
    }
}
#endif  // GEOJSON_UTIL_H