#include <CLHEP/Units/SystemOfUnits.h>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Trd.hh>
#include <G4ThreeVector.hh>
#include <G4TwoVector.hh>
#include <G4SubtractionSolid.hh>
#include <RAT/GeoConeFactory.hh>
 
namespace RAT {

    G4VSolid *GeoConeFactory::ConstructSolid(DBLinkPtr table) {
        std::string volume_name = table->GetIndex();
        std::string name_Trd_in("Trd_in"), name_Trd_out("Trd_out"), name_tub("tub");
        const std::vector<double> &size_in = table->GetDArray("size_in"); // {dxa, dxb, dya, dyb}/2
        const std::vector<double> &size_out = table->GetDArray("size_out"); // {dxa, dxb, dya, dyb}/2
        G4double r_max = table->GetD("r_max");
        G4double size_z = table->GetD("size_z");
        // Optional parameters
        G4double r_min = 0.0;
        try {
            r_min = table->GetD("r_min");
        } 
        catch (DBNotFoundError &e) {
        };

        G4double phi_start = 0.0;
        try {
            phi_start = table->GetD("phi_start") * CLHEP::deg;
        } 
        catch (DBNotFoundError &e) {
        };

        G4double phi_delta = CLHEP::twopi;
        try {
            phi_delta = table->GetD("phi_delta") * CLHEP::deg;
        } 
        catch (DBNotFoundError &e) {
        };
        G4Trd *Trd_in = new G4Trd(volume_name+"_"+name_Trd_in, size_in[0]*CLHEP::mm, size_in[1]*CLHEP::mm, size_in[2]*CLHEP::mm, size_in[3]*CLHEP::mm, size_z*CLHEP::mm);

        G4Trd *Trd_out = new G4Trd(volume_name+"_"+name_Trd_out, size_out[0]*CLHEP::mm, size_out[1]*CLHEP::mm, size_out[2]*CLHEP::mm, size_out[3]*CLHEP::mm, size_z*CLHEP::mm);

        G4Tubs *cyl = new G4Tubs(volume_name+"+"+name_tub, r_min*CLHEP::mm, r_max*CLHEP::mm, size_z*CLHEP::mm, phi_start, phi_delta);

        G4SubtractionSolid * TrdPlane = new  G4SubtractionSolid(volume_name+"_TrdPlane", Trd_out, Trd_in);
        return new G4SubtractionSolid(volume_name, TrdPlane, cyl);
    }




}
