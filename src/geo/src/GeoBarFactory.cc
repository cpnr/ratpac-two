#include <CLHEP/Units/SystemOfUnits.h>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Trd.hh>
#include <G4ThreeVector.hh>
#include <G4TwoVector.hh>
#include <G4SubtractionSolid.hh>
#include <RAT/GeoBarFactory.hh>
#include <cmath>

namespace RAT {

    G4VSolid *GeoBarFactory::ConstructSolid(DBLinkPtr table) {
        std::string volume_name = table->GetIndex();
        std::string name_Trd("Trd_bar"), name_Tube("Tube_bar");
        std::string name_Window("Window"), name_Gap("Gap");

        // only Bar
        G4double GC_size = table->GetD("GC_size"); // 2L(GC_size) x 2L x 2H = GC box size
        G4double r_max = table->GetD("r_max"); // 275
        G4double wing_size = table->GetD("wing_size"); // 70
        G4double bar_size = table->GetD("bar_size"); // 175
        G4double thick = table->GetD("thick");
        G4double Gsin = bar_size/r_max;
        G4double Gcos = sqrt(1-pow(Gsin,2));

        // Window
        G4double window_gap_size = table->GetD("window_gap_size"); // 20
        G4double window_wing_size = table->GetD("window_wing_size"); // 20
        G4double window_bar_size = table->GetD("window_bar_size"); // 130
        G4double window_height_size = table->GetD("window_height_size"); // 160
       
        // Window from bottom
        G4double window_pos_from_bottom = table->GetD("window_pos_from_bottom"); // 120


        // Translation
        G4ThreeVector Rel_Trans(0 , 0, (GC_size - 0.5*(GC_size-r_max*Gcos) )*CLHEP::mm);
        G4ThreeVector Rel_Trans_Window(0 , 0, -(0.5*(GC_size-r_max*Gcos)-window_pos_from_bottom)*CLHEP::mm);

        // Rotation
        G4RotationMatrix* Rotation = new G4RotationMatrix();
        Rotation->rotateX(90*CLHEP::deg); // 0~360 
        Rotation->rotateY(0*CLHEP::deg);
        Rotation->rotateZ(0*CLHEP::deg);


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


        G4Trd * Trd_bar = new G4Trd(volume_name+"_"+name_Trd, (bar_size+wing_size)*CLHEP::mm, bar_size*CLHEP::mm,0.5*thick*CLHEP::mm,0.5*thick*CLHEP::mm, 0.5*(GC_size-r_max*Gcos)*CLHEP::mm);
        // {dxa, dxb, dya, dyb, dz}/2 

        G4Tubs * cyl = new G4Tubs(volume_name+"_"+name_Tube, r_min*CLHEP::mm, r_max*CLHEP::mm, thick*CLHEP::mm, phi_start, phi_delta);

        G4SubtractionSolid * TrdPlane = new  G4SubtractionSolid(volume_name+"_Bar", Trd_bar, cyl, Rotation, Rel_Trans);

        G4Trd * Trd_window = new G4Trd(volume_name+"_"+name_Window, 0.5*(window_gap_size + 2*window_bar_size + 2*window_wing_size)*CLHEP::mm, 0.5*(window_gap_size + 2*window_bar_size)*CLHEP::mm, 0.6*thick*CLHEP::mm, 0.6*thick*CLHEP::mm, 0.5*window_height_size);

        G4Box * Gap_window = new G4Box(volume_name+"_"+name_Gap, 0.5*window_gap_size, 0.7*thick, 0.6*window_height_size);

        G4SubtractionSolid * Window = new  G4SubtractionSolid(volume_name+"_Window_sub_gap", Trd_window, Gap_window);

        G4SubtractionSolid * Bar_sub_Window = new G4SubtractionSolid(volume_name+"_Bar_sub_Window",TrdPlane,Window,0,Rel_Trans_Window); 

        return Bar_sub_Window;
    }




}
