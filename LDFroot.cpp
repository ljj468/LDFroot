#include "LDFroot.h"

int main(int argc, char *argv[]){
  // check for correct std inputs
  if(argc != 3){
    cout << "Usage: LDFroot [input file] [output file]" << endl;
    return -1;
  }
  
  // open input file
  ifstream input_file(argv[1]);
  if(!input_file.is_open()){
    cout << "Cannot open file " << argv[1] << endl;
    return -2;
  }
  
  //-------Read input file parameters-------
  
  // filename for LDF data file
  string get_directory;
  input_file >> get_directory; // input

  // output file
  string output_filename;
  input_file >> output_filename; // 4th input
  
  // maximum number of events
  UInt_t EndEvent = 0;
  input_file >> EndEvent; // 5th input

  //------------------------------------------
    
  // LDFDataReader object
  LDFDataReader *get  = new LDFDataReader();
 //------------Data------------------- 

  // Root file open
  TFile *fOut  = new TFile(argv[2], "RECREATE");

  TTree *tneuf = new TTree("AutoTree", "New tree...");

  //---------Make branches------------

get->openFile(argv[1]);
/*  if(IsOpen == -1){
    exit(1);
  }*/

    
  tneuf->Branch("data", get->data, "data[8]/i");




TFile *fOutScal  = new TFile(argv[2], "RECREATE");

  TTree *tneufScal = new TTree("AutoTree", "New tree...");

  //---------Make branches------------


/*  if(IsOpen == -1){
    exit(1);
  }*/

    
//  tneufScal->Branch("scal", get->scal, "Scal[8]/i");






int recordID=0;
//type=get->readOneWord();
bool ldf_eof=false;
// event loop


while(recordID%10!=8&&recordID%10!=7){

//	if(recordID%10==2){
//		get->readOneWord();
//		bool next_event=true;
	
//		while(recordID==1||recordID==0||recordID==9){
//			recordID=get->readDataEvent();
//			if(recordID/10>0){
				
//				tneuf->Fill();
//			}
//			next_event=(recordID%10==1);
			

//			}
//	}
	//else if(type%10==5){
	
	//get->readScalarEvent();
	//get->parseScalars();
	//	}
	else if((recordID%10>1)&&(recordID%10<8)){
	
	get->toNextRecord();
	recordID=get->readOneWord();	
	}
	
	else{

	recordID=get->readOneWord();
		
	}
	
 }

  // save ROOT file
  fOut->Write();
  fOut->Close();

  return 0;
}
