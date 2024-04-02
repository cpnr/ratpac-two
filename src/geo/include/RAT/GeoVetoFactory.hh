#ifndef __RAT_GeoVetoFactory__
#define __RAT_GeoVetoFactory__

#include <RAT/GeoSolidFactory.hh>

namespace RAT {

    class GeoVetoFactory : public GeoSolidFactory {
        public:
        GeoVetoFactory() : GeoSolidFactory("VetoBlock"){};
        virtual G4VSolid *ConstructSolid(DBLinkPtr table);
    };

}  // namespace RAT

#endif
