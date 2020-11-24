#ifndef GM_OBJECT_H
#define GM_OBJECT_H

#include <memory>

#include "Features.hpp"

class GM_Object
{
    public:
        GM_Object();
        virtual ~GM_Object();

        void set_feature(geojson::Feature feature) {
            this->feature = feature;
        }
    protected:
        geojson::Feature feature;

    private:
};

#endif // GM_OBJECT_H
