////////////////////////////////////////////////////////////////////////////////////////////////////
/*!\fn doEmcEmbedEvent
  \author Alexandre Suaide
  */
class TChain;
TChain *chain=0;

//void doEmcEmbedEvent(int nevents = 10,char* file="../st_zerobias_7103008_raw_1110001.event.root",Bool_t print = kTRUE);
//void doEmcEmbedEvent(int nevents = 10,char* file="st_zerobias_7072058_raw_1110001.event.root",Bool_t print = kTRUE)

//void doTest(const char* fileInDir="./out_mb__100_20151202/")
//void doTest(const char* fileInDir="./out_ht_20160329/")
void doTest(int uncertainty = 0)
{
	//TH1F *hMcVertexZ = new TH1F("mcVertexZ","mcVertexZ;Vz^{mc} (cm)",400,-200,200);
	//	TH2F *hMcVertexXY = new TH2F("mcVertexXY","mcVertexXY;Vx^{mc} (cm);Vy^{mc} (cm)",40,-2,2,40,-2,2);
	//	TH1F *hMcJpsiPol = new TH1F("hMcJpsiPol","Theta",300,0,TMath::Pi());
	//	TH2F *hMcJpsiPolThetaPt = new TH2F("hMcJpsiPolThetaPt","Pt vs Cos(#theta)",300,-1,1,300,0,10);
	//	TH1F *hHtJpsiPol = new TH1F("hHtJpsiPol","Theta",300,0,TMath::Pi());
	//	TH2F *hHtJpsiPolThetaPt = new TH2F("hHtJpsiPolThetaPt","Pt vs Cos(#theta)",300,-1,1,300,0,10);
	const char* fileInDir="/star/u/huangbc/data01/analysis/jpsi/run12/polarization/checkForSiwei/checkEff/out_ht_20160329/";

	if(gClassTable->GetID("TTable") < 0) {
		gSystem->Load("libStar");
		gSystem->Load("libPhysics");
	}
	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();

	gSystem->Load("StMyElectronMaker");

	gSystem->Load("StMyJpsiEffMakerSmearing66");

	cout<<"Makers are loadded"<<endl;

	// create chain    
	chMc = new TChain("mcT");

	void* dir = gSystem->OpenDirectory(gSystem->ExpandPathName(fileInDir));
	int nruns=0;
	char *file_name;
	TString Tname;
	const int nFilesMax = 10000;
	char file_list[nFilesMax][256];
	do {
		file_name = (char*)gSystem->GetDirEntry(dir);
		Tname=file_name;
		if(file_name && Tname.Contains("myminimc.root")&&Tname.Contains("emb")) {
			sprintf(file_list[nruns],"%s/%s",fileInDir,file_name);
			chMc->Add(file_list[nruns]);
			cout << " read in " << file_list[nruns] << endl;
			nruns++;
		}
	} while (file_name && nruns<=nFilesMax);

	int iret = 0;
	int nb=0;
	cout << chMc->GetEntries() << " events in chain" << endl;
	int nevents = chMc->GetEntries();
	//nevents = 100000;
	//	cout<<"nevents ========"<< nevents <<endl;

	cout<<"uncertainty====="<<uncertainty<<endl;


	StChain *chain = new StChain;
	StMyJpsiEffMaker *myJpsiEff = new StMyJpsiEffMaker("myJpsiEff",chMc, uncertainty);
	//end of my analysis maker

	chain->Init();
	// do the event loop    
	int istat = 0;
	int iev = 0;
	while ( istat!=2 && istat!=3 && istat!=4 && iev<=nevents ) {
		chain->Clear();
		//cout << "Start to process event number "<<iev <<endl;
		istat = chain->Make();
		//emcAssociation->printMaps();
		if(iev%1000==0) 
			cout << "Finished processing event number "<<iev <<endl;
		iev++;
	}
	chain->Finish();

}
