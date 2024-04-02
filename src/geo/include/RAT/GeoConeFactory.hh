#ifndef __RAT_GeoConeFactory__
#define __RAT_GeoConeFactory__

#include <RAT/GeoSolidFactory.hh>

namespace RAT {

    class GeoConeFactory : public GeoSolidFactory {
        public:
        GeoConeFactory() : GeoSolidFactory("TrdCone"){};
        virtual G4VSolid *ConstructSolid(DBLinkPtr table);
    };

}  // namespace RAT

#endif
