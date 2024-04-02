#include <CLHEP/Units/SystemOfUnits.h>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Trd.hh>
#include <G4ThreeVector.hh>
#include <G4TwoVector.hh>
#include <G4SubtractionSolid.hh>
#include <G4UnionSolid.hh>
#include <RAT/GeoVetoFactory.hh>


namespace RAT {

    G4VSolid *GeoVetoFactory::ConstructSolid(DBLinkPtr table) {
        std::string volume_name = table->GetIndex();

        std::string name_PS("PS"), name_LG_u("LG_u"), name_LG_d("LG_d"); //up & down
        std::string name_PMT("PMT");

        G4double height_PS = table->GetD("PS_height");
        G4double length_PS = table->GetD("PS_length");
        G4double height_LG = table->GetD("LG_height");
        G4double height_PMT = table->GetD("PMT_height");
        G4double radius_PMT = table->GetD("PMT_radius");
        G4double thick = table->GetD("thick");


        //Translation
        G4ThreeVector Rel_Trans_LG_u(0*CLHEP::mm , 0*CLHEP::mm, 0.5*(height_PS+height_LG)*CLHEP::mm);
        G4ThreeVector Rel_Trans_LG_d(0*CLHEP::mm , 0*CLHEP::mm, -0.5*(height_PS+height_LG)*CLHEP::mm);
        G4ThreeVector Rel_Trans_PMT_u(0*CLHEP::mm , 0*CLHEP::mm, (0.5*height_PS+height_LG+0.5*height_PMT)*CLHEP::mm);
        G4ThreeVector Rel_Trans_PMT_d(0*CLHEP::mm , 0*CLHEP::mm, -(0.5*height_PS+height_LG+0.5*height_PMT)*CLHEP::mm);


        G4Box * PS_box = new G4Box(volume_name+"_"+name_PS, 0.5*length_PS*CLHEP::mm, 0.5*thick*CLHEP::mm, 0.5*height_PS*CLHEP::mm );
        // G4Trd -> {dxa, dxb, dya, dyb, dz}/2        
        G4Trd * LG_Trd_up = new G4Trd(volume_name+"_"+name_LG_u, 0.5*length_PS*CLHEP::mm, radius_PMT*CLHEP::mm, 0.5*thick*CLHEP::mm, 0.5*thick*CLHEP::mm, 0.5*height_LG*CLHEP::mm);
        G4Trd * LG_Trd_dowm = new G4Trd(volume_name+"_"+name_LG_d, radius_PMT*CLHEP::mm, 0.5*length_PS*CLHEP::mm, 0.5*thick*CLHEP::mm, 0.5*thick*CLHEP::mm, 0.5*height_LG*CLHEP::mm);
        G4Box * PMT_box = new G4Box(volume_name+"_"+name_PMT, radius_PMT*CLHEP::mm, radius_PMT*CLHEP::mm,0.5*height_PMT*CLHEP::mm);
       

        G4UnionSolid * PS_uni_LG_u = new G4UnionSolid(volume_name+"_PS_uni_LG_u", PS_box,LG_Trd_up,0,Rel_Trans_LG_u);
        G4UnionSolid * PS_uni_LG_u_d = new G4UnionSolid(volume_name+"_PS_uni_LG_u_d", PS_uni_LG_u,LG_Trd_dowm,0,Rel_Trans_LG_d);
        G4UnionSolid * PS_uni_LG_u_d_PMT_u = new G4UnionSolid(volume_name+"_PS_uni_LG_u_d_PMT_u", PS_uni_LG_u_d,PMT_box,0,Rel_Trans_PMT_u);
        G4UnionSolid * PS_uni_LG_u_d_PMT_u_d = new G4UnionSolid(volume_name+"_PS_uni_LG_u_d_PMT_u_d", PS_uni_LG_u_d_PMT_u,PMT_box,0,Rel_Trans_PMT_d);

        return PS_uni_LG_u_d_PMT_u_d;
    }




}
