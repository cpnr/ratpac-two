#ifndef __RAT_GeoConeCutFactory__
#define __RAT_GeoConeCutFactory__

#include <RAT/GeoSolidArrayFactoryBase.hh>

namespace RAT {
class GeoConeCutFactory : public GeoSolidArrayFactoryBase {
 public:
  GeoConeCutFactory() : GeoSolidArrayFactoryBase("ConeCut"){};
  virtual G4VPhysicalVolume *Construct(DBLinkPtr table);
};

}  // namespace RAT

#endif
