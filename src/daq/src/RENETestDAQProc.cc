/* SplitEVDAQ is an extension of the SimpleDAQ that converts hits into multiple
 * triggers, and places those in new events (not subevents) to better simulate
 * data and produce combined datasets.
 */
#include <G4ThreeVector.hh>
#include <RAT/DS/DigitPMT.hh>
#include <RAT/DS/MCPMT.hh>
#include <RAT/DS/PMT.hh>
#include <RAT/DS/RunStore.hh>
#include <RAT/RENETestDAQProc.hh>
#include <algorithm>
#include <vector>

namespace RAT {

RENETestDAQProc::RENETestDAQProc() : Processor("renetestdaq") {
  // Trigger Specifications
std::cout<<"YUN1---------------------------------------------------------hhhhhhhhhhh"<<std::endl;
  ldaq = DB::Get()->GetLink("DAQ", "SplitEVDAQ");
  fEventCounter = 0;
  fPulseWidth = ldaq->GetD("pulse_width");
  fTriggerThreshold = ldaq->GetD("trigger_threshold");
  fTriggerWindow = ldaq->GetD("trigger_window");
  fPmtLockout = ldaq->GetD("pmt_lockout");
  fTriggerLockout = ldaq->GetD("trigger_lockout");
  fTriggerResolution = ldaq->GetD("trigger_resolution");
  fLookback = ldaq->GetD("lookback");
  fMaxHitTime = ldaq->GetD("max_hit_time");
  fTriggerOnNoise = ldaq->GetI("trigger_on_noise");
  fDigitizerType = ldaq->GetS("digitizer_name");
  fDigitize = ldaq->GetZ("digitize");
  fAnalyze = ldaq->GetZ("analyze");

  fDigitizer = new Digitizer(fDigitizerType);
  fWaveformAnalysis = new WaveformAnalysis();
}

void RENETestDAQProc::BeginOfRun(DS::Run *run) {
std::cout<<"YUN2---------------------------------------------------------hhhhhhhhhh"<<std::endl;
  if (fDigitize) {
    DS::PMTInfo *pmtinfo = run->GetPMTInfo();
    const size_t numModels = pmtinfo->GetModelCount();
    for (size_t i = 0; i < numModels; i++) {
      const std::string modelName = pmtinfo->GetModelName(i);
      fDigitizer->AddWaveformGenerator(modelName);
    }
  }
}

Processor::Result RENETestDAQProc::DSEvent(DS::Root *ds) {
std::cout<<"YUN3---------------------------------------------------------hhhhhhhhhh"<<std::endl;
  // This DAQ will convert hits on PMTs through time into trigger pulses
  // which can fire a global event trigger. Each trigger will correspond
  // to a single sub-event in the datastructure.
  // Not included yet
  // - Noise on the trigger pulse height, rise-time, etc
  // - Disciminator on charge (all hits assumed to trigger)
  
  DS::MC *mc = ds->GetMC();
  DS::Run *run = DS::RunStore::Get()->GetRun(ds);
  DS::PMTInfo *pmtinfo = run->GetPMTInfo();
  // Prune the previous EV branchs if one exists
  if (ds->ExistEV()) ds->PruneEV();

/////////////////////////////////////////////////////////////////////////////////////////////////////
// declare several std::vectors
  std::vector<int> iArray, iArraySort;       // index
  std::vector<double> tArray, tArraySort;    // front end time
  std::vector<double> qArray, qArraySort;    // charge
  std::vector<double> idArray, idArraySort;  // PMT ID
  std::vector<double> phArray, phArraySort;  // photon count ...we are not using this...
  std::vector<double> htArray, htArraySort;  // hit time     ...we are not using this either...
///////////////////////////////////////////////////////////////////////////////
std::vector<double> QArrayV;
std::vector<double> QArrayRawV;
std::vector<unsigned long> fNPEArrayV;


int numSub = 0;
    
    std::vector<int> subIndex;
    std::vector<double> clusterTime;
    std::vector<double> triggerTime;
    
    std::vector<unsigned long> startIndex;
    
    std::vector<double> WaveV;
    
    double timeWindow = 500.0; //ns
double voltageThreshold = 500.; //mV
   double samplingTime = 2.; //ns
   double delayTime = -50.; //ns

   double maxFrontEndTime = 0.;
   for (int imcpmt = 0; imcpmt < mc->GetMCPMTCount(); imcpmt++) {
    DS::MCPMT *mcpmt0 = mc->GetMCPMT(imcpmt);
    if (mcpmt0->GetMCPhotonCount() > 0) {
      for (int i = 0; i < mcpmt0->GetMCPhotonCount(); i++) {
      	if((mcpmt0->GetMCPhoton(i)->GetFrontEndTime()) > maxFrontEndTime){
      		maxFrontEndTime = (mcpmt0->GetMCPhoton(i)->GetFrontEndTime());
      	}	
      }
    }
  }
  int sizeOfQAV = TMath::Floor(maxFrontEndTime/samplingTime)+1+25;
  std::cout<<"sizeOfQAV "<<sizeOfQAV<<std::endl;
  // get the information for 1 full event and put them in std::vectors
  for (int imcpmt = 0; imcpmt < mc->GetMCPMTCount(); imcpmt++) {
 std::cout<<"--------------------------------mc->GetMCPMTCount() "<<mc->GetMCPMTCount() <<std::endl;
   numSub = 0;
   subIndex.clear();
   clusterTime.clear();
   startIndex.clear();
   iArray.clear();
   triggerTime.clear();
   iArraySort.clear();
   qArray.clear();
   qArraySort.clear();
   idArray.clear();
   idArraySort.clear();
   phArray.clear();
   phArraySort.clear();
   htArray.clear();
   htArraySort.clear();
   tArray.clear();
   tArraySort.clear();
   		
		
   //startIndex.push_back(0);
    DS::MCPMT *mcpmt = mc->GetMCPMT(imcpmt);

    if (mcpmt->GetMCPhotonCount() > 0) {
      for (int i = 0; i < mcpmt->GetMCPhotonCount(); i++) {
        tArray.push_back(mcpmt->GetMCPhoton(i)->GetFrontEndTime());
        tArraySort.push_back(mcpmt->GetMCPhoton(i)->GetFrontEndTime());
        qArray.push_back(mcpmt->GetMCPhoton(i)->GetCharge());
        idArray.push_back(mcpmt->GetID());
        iArray.push_back(i);
        phArray.push_back(mcpmt->GetMCPhotonCount());
        htArray.push_back(mcpmt->GetMCPhoton(i)->GetHitTime());
      }
    }
      std::cout<<"--------------------------------tArray.size() "<<tArray.size()<<std::endl;
      std::cout<<"--------------------------------tArraySort.size() "<<tArraySort.size()<<std::endl;
      std::cout<<"--------------------------------qArray.size() "<<qArray.size()<<std::endl;
      std::cout<<"--------------------------------idArray.size() "<<idArray.size()<<std::endl;
      std::cout<<"--------------------------------iArray.size() "<<iArray.size()<<std::endl;
      std::cout<<"--------------------------------phArray.size() "<<phArray.size()<<std::endl;
      std::cout<<"--------------------------------htArray.size() "<<htArray.size()<<std::endl;
      
      
      
  double timeStart = 0.;
  double timeNow = tArraySort[0];
//clusterTime.push_back(timeNow);
  // then sort these events based on the front end time
  sort(tArraySort.begin(), tArraySort.end());

  // create iArraySort to keep track of the indexes
  for (unsigned long q = 0; q < tArraySort.size(); q++) {
    for (unsigned long qq = 0; qq < tArray.size(); qq++) {
      if (tArraySort[q] == tArray[qq]) {
        iArraySort.push_back(iArray[qq]);
      }
    }
  }

  // sort the rest of the std::vectors
  //std::cout<<"--------------------------------tArraySort.size() "<<tArraySort.size()<<std::endl;
  for (unsigned long m = 0; m < tArraySort.size(); m++) {
 // std::cout<<"--------------------------------iArraySort[m] "<<m<<std::endl;
 // std::cout<<"--------------------------------iArraySort[m] "<<m<<" "<<iArraySort[m]<<" "<<tArraySort.size()<<" qArray.size() "<<qArray.size()<<std::endl;
    qArraySort.push_back(qArray[iArraySort[m]]);
    // std::cout<<"--------------------------------qArray[iArraySort[m]] "<<qArray[iArraySort[m]]<<std::endl;
    idArraySort.push_back(idArray[iArraySort[m]]);
    // std::cout<<"--------------------------------idArray[iArraySort[m]] "<<idArray[iArraySort[m]]<<std::endl;
    phArraySort.push_back(phArray[iArraySort[m]]);
    // std::cout<<"--------------------------------phArray[iArraySort[m]] "<<phArray[iArraySort[m]]<<std::endl;
    htArraySort.push_back(htArray[iArraySort[m]]);
    // std::cout<<"--------------------------------htArray[iArraySort[m]] "<<htArray[iArraySort[m]]<<std::endl;
   // std::cout<<"--------------------------------"<<std::endl;
  }

    
  // if there is at least 1 event ...
 std::cout<<"-------------------------------- tArraySort.size()"<< tArraySort.size()<<std::endl;
  if (tArraySort.size() > 0) {
    // group these events and get bunch of subevents. the cluster time will be
    // the earliest time among the PMTs.

///////////////////////////////////////////////////////////////////////////////////////////



QArrayV.clear();
fNPEArrayV.clear();
QArrayRawV.clear();

QArrayV.resize(sizeOfQAV, 0.);
fNPEArrayV.resize(sizeOfQAV, 0);
QArrayRawV.resize(sizeOfQAV, 0.);
std::cout<<"sizeOfQAV "<<sizeOfQAV<<std::endl;
for (unsigned long a = 0; a < tArraySort.size(); a++) {
//std::cout<<"tArraySort[a] "<< tArraySort[a]<<" TMath::Floor(tArraySort[a]/samplingTime) "<<TMath::Floor(tArraySort[a]/samplingTime)<<std::endl;
    //QArrayV[TMath::Floor(tArraySort[a]/samplingTime)] +=qArraySort[a];  
    
    for(int TTSi = 0; TTSi < 25 ; TTSi++){
    	QArrayV[TMath::Floor(tArraySort[a]/samplingTime)+TTSi] +=qArraySort[a]*TMath::LogNormal(TTSi, 0.5, 0., 2.5);
    }    
    
    QArrayRawV[TMath::Floor(tArraySort[a]/samplingTime)] +=qArraySort[a];
    fNPEArrayV[TMath::Floor(tArraySort[a]/samplingTime)] +=1; 
    }

//test
for (unsigned long a = 0; a < QArrayV.size(); a++) {
    //std::cout<<"QArray: "<<a<<" "<< QArrayV[a]<<" fNPEArrayV: "<< fNPEArrayV[a]<<" QArrayV[a]*50/samplingTime "<<QArrayV[a]*50/samplingTime<<std::endl; 
    }

int triggerKey = 0;
for (unsigned long a = 0; a < QArrayV.size(); a++) {
//if(a%4 == 0) std::cout<<"--------------------------------a "<<a <<" numSub "<<numSub<<" QArrayV[a]*50/samplingTime "<<QArrayV[a]*50/samplingTime<<" voltageThreshold "<<voltageThreshold<<" triggerKey "<<triggerKey<<std::endl;

	if( QArrayV[a]*50/samplingTime >= voltageThreshold && triggerKey == 0 ){
		triggerKey = 1;		
		timeNow = a*samplingTime;
		timeStart = a*samplingTime+delayTime;
		if(timeStart < 0) timeStart = 0;
		subIndex.push_back(numSub);
		clusterTime.push_back(timeStart);
		triggerTime.push_back(timeNow);
		startIndex.push_back(TMath::Floor(timeStart/2));
		std::cout<<"--------------------------------timeStart "<<timeStart<<" timeNow "<<timeNow<<" TMath::Floor(timeStart/2) "<<TMath::Floor(timeStart/2)<<std::endl;
		numSub++;
	}
//if(a%4 == 0) std::cout<<"--------------------------------samplingTime*a "<<samplingTime*a <<" timeNow "<<timeNow<<" timeWindow "<<timeWindow<<std::endl;
	if(triggerKey == 1 && (samplingTime*a-timeNow > timeWindow)){
		triggerKey = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////

    std::cout<<"--------------------------------numSub "<<numSub<<" subIndex "<<subIndex.size()<<std::endl;
}
std::cout<<"--------------------------------numSubcheck1 imcpmt "<<imcpmt<<std::endl;
std::cout<<"--------------------------------numSubcheck2 "<<mc->GetMCPMTCount()<<std::endl;

    // ok, we have the number of subevents now (numSub+1). Let's set things up!
   double qSum = 0.0;
//numSub + 1 ?
  for (int b = 0; b < numSub; b++) {
    
     DS::EV *ev = ds->AddNewEV();
      DS::PMT *pmt;
     fEventCounter += 1;

      if (b > 0) {
       ev->SetDeltaT(clusterTime[b] - clusterTime[b - 1]);
      } else {
       ev->SetDeltaT(clusterTime[0]);
      }
      ev->SetCalibratedTriggerTime(clusterTime[b]);
      ev->SetID(subIndex[b]);
     // std::cout<<"--------------------------------ev->SetID(subIndex[b]); b "<<b<<" subIndex[b] "<<subIndex[b]<<" startIndex[b] "<<startIndex[b]<<" QArrayV[b] "<<QArrayV[b]<<std::endl;
      qSum = 0.0;
       int pmtID = mcpmt->GetID();
       WaveV.clear();
          pmt = ev->AddNewPMT();
          pmt->SetID(pmtID);
          pmt->SetTime(startIndex[b]*2);
          pmt->SetTriggerTime(triggerTime[b]);
          std::cout<<std::endl;
         std::cout<<"--------------------------------C3 startIndex.size() "<<startIndex.size()<<" subIndex[b] "<<subIndex[b]<<" startIndex[b] "<<startIndex[b]<<" triggerTime[b] "<<triggerTime[b]<<std::endl;
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////      
         for(int getEVi = 0; getEVi < TMath::Floor(timeWindow/2) ; getEVi++){
            
               
                if((startIndex[b]+getEVi)>=QArrayV.size()){
                	WaveV.push_back(QArrayV[QArrayV.size()-1]*50/samplingTime);
         		qSum += QArrayV[QArrayV.size()-1];
                }else{         
         		WaveV.push_back(QArrayV[startIndex[b]+getEVi]*50/samplingTime);
         		qSum += QArrayV[startIndex[b]+getEVi];
         	}
         	//std::cout<<"-----------------C5 QQArrayV[startIndex[b]+getEVi]*50/samplingTime "<<QArrayV[startIndex[b]+getEVi]*50/samplingTime<<" QArrayRawV "<<QArrayRawV[startIndex[b]+getEVi]*50/samplingTime<<" fNPEArrayV "<<fNPEArrayV[startIndex[b]+getEVi]<<std::endl;
         }
         
         pmt->SetWaveVector(WaveV);
         
         
   ///////////////////////////////////////////////////////////////////////////////////////////////////      
          pmt->SetCharge(qSum);
        std::cout<<"--------------------------------C4 QArrayV.size() "<< QArrayV.size()<<std::endl;
	  if (fDigitize) {
	  std::cout<<"--------------------------------fDigitize "<<startIndex[b]*2<<std::endl;
         	 fDigitizer->DigitizePMT(mcpmt, pmtID, startIndex[b]*2, pmtinfo);
         	 if (fAnalyze) {
         	   DS::DigitPMT *digitpmt = ev->AddNewDigitPMT();
         	   digitpmt->SetID(pmtID);
          	  fWaveformAnalysis->RunAnalysis(digitpmt, pmtID, fDigitizer);
         	 }
            }
      

	 if (fDigitize) {
    	  fDigitizer->DigitizeSum(ev);
   	 }
   	 
//std::cout<<"--------------------------------fDigitize9"<<std::endl;
      ev->SetTotalCharge(qSum);
    }
    }
return Processor::OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
 

void RENETestDAQProc::SetD(std::string param, double value) {
std::cout<<"YUN4---------------------------------------------------------hhhhhhhhhhhhhh"<<std::endl;
  if (param == "pulse_width")
    fPulseWidth = value;
  else if (param == "trigger_threshold")
    fTriggerThreshold = value;
  else if (param == "trigger_window")
    fTriggerWindow = value;
  else if (param == "pmt_lockout")
    fPmtLockout = value;
  else if (param == "trigger_lockout")
    fTriggerLockout = value;
  else if (param == "trigger_resolution")
    fTriggerResolution = value;
  else if (param == "lookback")
    fLookback = value;
  else if (param == "max_hit_time")
    fMaxHitTime = value;
  else
    throw ParamUnknown(param);
}

void RENETestDAQProc::SetI(std::string param, int value) {
std::cout<<"YUN5---------------------------------------------------------hhhhhhhhhhhh"<<std::endl;
  if (param == "trigger_on_noise")
    fTriggerOnNoise = value;
  else
    throw ParamUnknown(param);
}

}  // namespace RAT
