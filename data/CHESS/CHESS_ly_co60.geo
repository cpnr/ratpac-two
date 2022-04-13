/////////////////////
// DARK BOX
/////////////////////

{
  name: "GEO",
  index: "world",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 1, // omitted for visualization
  mother: "",
  type: "box",
  size: [3000.0,3000.0,600.0], //mm, half-lenght
  material: "air",
}

{
  name: "GEO",
  index: "darkbox",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 1, // omitted for visualization
  mother: "world",
  type: "box",
  size: [762.0,762.0,508.0], //mm, half-lenght
  material: "acrylic_black", //acrylic_black
  surface: "acrylic_black",
  color: [0.5, 0.2, 0.1, 0.1],
}

{
  name: "GEO",
  index: "inner",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "darkbox",
  type: "box",
  size: [711.2,711.2,457.2], //mm, half-lenght
  material: "air",
  color: [0.0, 0.0, 0.0, 0.1],
}

/////////////////////////////
// CONTAINER
/////////////////////////////

// {
//   name: "GEO",
//   index: "ruler",
//   valid_begin: [0, 0],
//   valid_end: [0, 0],
//   invisible: 0, // omitted for visualization
//   mother: "inner",
//   type: "box",
//   size: [57.15, 6.5, 6.5], //mm, half-lenght
//   position: [91.5, -32.0, -250.91], //114.3mm from PMT
//   rotation:  [0.0, 0.0, -34.4],
//   material: "air",
//   color: [0.0, 0.0, 0.0, 0.1],
// }

{
  name: "GEO",
  index: "mask",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "inner",
  type: "box",
  size: [13.0,13.0,0.5], //mm, half-lenght
  position: [138.7, 0.2, -272.41], //114.3mm from PMT
  rotation:  [0.0, 0.0, -34.4],
  material: "acrylic_black",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "base",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "mask",
  type: "box",
  size: [6.5,6.5,0.5], //mm, half-lenght
  position: [0.0, 0.0, 0.0], //114.3mm from PMT
  rotation:  [0.0, 0.0, 0.0],
  material: "quartz",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "vessel",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "inner",
  type: "box",
  size: [6.5,6.5,22.0], //mm, half-lenght
  position: [138.7, 0.2, -249.91], //114.3mm from PMT
  rotation:  [0.0, 0.0, -124.4],
  material: "quartz",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "cap",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "vessel",
  type: "tube",
  position: [0.0, 0.0, 20.5], //1inch from vessel
  rotation:  [0.0, 0.0, 0.0],
  r_max: 4.0,
  size_z: 1.5, //half height
  material: "acrylic_white",
  surface: "acrylic_white",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "content",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "vessel",
  type: "box",
  size: [5.0,5.0,20.0], //mm, half-lenght
  position: [0.0,0.0,-1.0],
  material: "water",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "vessel_wall_0",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "vessel",
  type: "box",
  size: [0.75,6.5,22.0], //mm, half-lenght
  position: [5.75, 0.0, 0.0], //114.3mm from PMT
  material: "quartz_matte",
  surface: "quartz_matte",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "vessel_wall_1",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "vessel",
  type: "box",
  size: [0.75,6.5,22.0], //mm, half-lenght
  position: [-5.75, 0.0, 0.0], //114.3mm from PMT
  material: "quartz_matte",
  surface: "quartz_matte",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "vessel_floor",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "vessel",
  type: "box",
  size: [5.0,5.0,0.5], //mm, half-lenght
  position: [0.0, 0.0, -21.5], //114.3mm from PMT
  material: "quartz_matte",
  surface: "quartz_matte",
  color: [0.0, 0.0, 0.0, 0.1],
}

/////////////////////////////
// RADIACTIVE SOURCES
/////////////////////////////

//TOP
{
  name: "GEO",
  index: "button_source",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "inner",
  type: "tube",
  position: [159.7, 14.6, -270.91], //1inch from vessel
  rotation:  [0.0, 0.0, 0.0],
  r_max: 12.808,
  size_z: 1.587, //half height
  material: "acrylic_white",
  surface: "acrylic_white",
  color: [0.1, 1.0, 0.3, 0.8],
}

{
  name: "GEO",
  index: "source",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization 
  mother: "button_source",
  type: "tube",
  position: [0.0, 0.0, -1.157],
  r_max: 3.175,
  size_z: 0.05, //half height
  material: "acrylic_white", //strontium, acrylic_white
  surface: "acrylic_white",
  color: [0.1, 1.0, 1.0, 0.8],
}




/////////////////////
// ACRYLIC BLOCK
/////////////////////
{
  name: "GEO",
  index: "block",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "inner",
  type: "box",
  size: [160.0,160.0,32.5],
  position: [-398.0, -367.0, -270.0],
  material: "acrylic_black",
  color: [0.1, 0.3, 0.8, 0.1],
}

/////////////////////
// COVER
/////////////////////
{
  name: "GEO",
  index: "cover_air",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "block",
  type: "box",
  size: [160.0,160.0,0.05],
  position: [0.0,0.0,32.45],
  material: "felt", //air
  surface: "felt", //none
  color: [0.5, 0.2, 0.1, 0.1],
}

{
  name: "GEO",
  index: "cover",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "block",
  type: "box",
  size: [160.0,160.0,0.45],
  position: [0.0,0.0,31.95],
  material: "felt",
  surface: "felt",
  color: [0.5, 0.2, 0.1, 0.1],
}

///////////////////////
// COSMIC TAGS
///////////////////////
{
  name: "GEO",
  index: "tag1",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "inner",
  type: "box",
  position: [-398.0, 700.0, -149.0], //out of the setup
  size: [10.0,10.0,55.0],
  material: "acrylic_black",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "tag1_scint",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "tag1",
  type: "tube",
  position: [0.0,0.0,-29.9],
  r_max: 5.0,
  size_z: 25.0,
  material: "scintillator",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "tag1_coat",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "tag1",
  type: "border",
  volume1: "tag1_scint",
  volume2: "tag1",
  surface: "mirror",
  reverse: 0,
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "tag2",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "inner",
  type: "box",
  position: [-398.0, -367.0, -357.5],
  size: [15.0,15.0,55.0],
  material: "acrylic_black",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "tag2_scint",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "tag2",
  type: "tube",
  position: [0.0,0.0,29.0],
  r_max: 5.0,
  size_z: 25.0,
  material: "scintillator",
  color: [0.0, 0.0, 0.0, 0.1],
}

{
  name: "GEO",
  index: "tag2_coat",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "tag2",
  type: "border",
  volume1: "tag2_scint",
  volume2: "tag2",
  surface: "mirror",
  reverse: 0,
  color: [0.0, 0.0, 0.0, 0.1],
}
/////////////////////////////////

//////////////////
// PMT holder
//////////////////
{
  name: "GEO",
  index: "pmtframe",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "inner",
  type: "box",
  size: [160.0,160.0,16.0],
  position: [-398.0, -367.0, -318.5],
  material: "acrylic_white", //aluminum
  surface: "acrylic_white", //aluminum
  color: [0.5, 0.0, 0.0, 0.5],
}

{
  name: "GEO",
  index: "pmtgrid",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "pmtframe",
  type: "box",
  size: [130.0,130.0,16.0],
  position: [0.0, 0.0, 0.0],
  material: "acrylic_white",
  surface: "acrylic_white",
  color: [0.0, 0.0, 0.0, 0.1],
}

//////////////////
// Muon panels
//////////////////
{
  name: "GEO",
  index: "panel_east",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "inner",
  type: "box",
  size: [25.0,500.0,250.0],
  position: [-686.2, -130.0, -180.0],
  material: "acrylic_black",
  surface: "acrylic_black",
  color: [0.0, 0.5, 0.5, 0.7],
}

{
  name: "GEO",
  index: "panel_north",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "inner",
  type: "box",
  size: [500.0,25.0,250.0],
  position: [-210.0, -660.0, -180.0],
  material: "acrylic_black",
  surface: "acrylic_black",
  color: [0.0, 0.5, 0.5, 0.7],
}

//////////////////
// PMTS
//////////////////
{
  name: "GEO",
  index: "ring_pmts",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  invisible: 0, // omitted for visualization
  mother: "pmtgrid",
  type: "pmtarray",
  pmt_model: "h11934", //h11934
  pmt_detector_type: "idpmt",
  sensitive_detector: "/mydet/pmt/inner",
  pos_table: "PMTINFO_CROSS_SIDE",
  orientation: "manual",
}

{
  name: "GEO",
  index: "light_pmts",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  mother: "inner",
  type: "pmtarray",
  pmt_model: "r7081_rev", //r7081_rev
  pmt_detector_type: "idpmt",
  sensitive_detector: "/mydet/pmt/inner",
  pos_table: "PMTINFO_LY",
  orientation: "point",
  orient_point: [91.0, -32.0, -215.0],
}

{
  name: "GEO",
  index: "topmuon_pmt",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  mother: "tag1",
  type: "pmtarray",
  pmt_model: "h3164-10",
  pmt_detector_type: "idpmt",
  sensitive_detector: "/mydet/pmt/tag1",
  pos_table: "PMTINFO_MUON_TOPTAG_OUT",
  orientation: "manual",
  orient_point: [-400.0, -400.0, -200.0],
}

{
  name: "GEO",
  index: "bottommuon_pmt",
  valid_begin: [0, 0],
  valid_end: [0, 0],
  mother: "tag2",
  type: "pmtarray",
  pmt_model: "h3164-10",
  pmt_detector_type: "idpmt",
  sensitive_detector: "/mydet/pmt/tag2",
  pos_table: "PMTINFO_MUON_BOTTOMTAG",
  orientation: "manual",
  orient_point: [-400.0, -400.0, -200.0],
}

{
   name: "GEO",
   index: "trigger_pmt",
   valid_begin: [0, 0],
   valid_end: [0, 0],
   mother: "inner",
   type: "pmtarray",
   pmt_model: "h11934",
   pmt_detector_type: "idpmt",
   sensitive_detector: "/mydet/pmt/inner",
   pos_table: "PMTINFO_TRIGGER_LY",
   orientation: "manual",
}

{
   name: "GEO",
   index: "cotrigger_pmt",
   valid_begin: [0, 0],
   valid_end: [0, 0],
   mother: "inner",
   type: "pmtarray",
   pmt_model: "h11934",
   pmt_detector_type: "idpmt",
   sensitive_detector: "/mydet/pmt/inner",
   pos_table: "PMTINFO_COTRIGGER",
   orientation: "point",
   orient_point: [-398.0, -367.0, -222.5],
}

{
   name: "GEO",
   index: "panels_pmts",
   valid_begin: [0, 0],
   valid_end: [0, 0],
   mother: "inner",
   type: "pmtarray",
   pmt_model: "h11934",
   pmt_detector_type: "idpmt",
   sensitive_detector: "/mydet/pmt/inner",
   pos_table: "PMTINFO_PANELS",
   orientation: "manual",
   orient_point: [0.0, 0.0, 400.0],

}

{
   name: "GEO",
   index: "control_channel",
   valid_begin: [0, 0],
   valid_end: [0, 0],
   mother: "inner",
   type: "pmtarray",
   pmt_model: "h11934",
   pmt_detector_type: "idpmt",
   sensitive_detector: "/mydet/pmt/inner",
   pos_table: "PMTINFO_XCH",
   orientation: "manual",
}