#ifndef __RAT_GeoBarFactory__
#define __RAT_GeoBarFactory__

#include <RAT/GeoSolidFactory.hh>

namespace RAT {

    class GeoBarFactory : public GeoSolidFactory {
        public:
        GeoBarFactory() : GeoSolidFactory("BarBase"){};
        virtual G4VSolid *ConstructSolid(DBLinkPtr table);
    };

}  // namespace RAT

#endif
