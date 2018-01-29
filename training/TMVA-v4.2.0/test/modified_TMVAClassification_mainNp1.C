// @(#)root/tmva $Id$
/**********************************************************************************
 * Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l ./TMVAClassification.C\(\"Fisher,Likelihood\"\)                     *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set of classifiers is used.                      *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 * Launch the GUI via the command:                                                *
 *                                                                                *
 *    root -l ./TMVAGui.C                                                         *
 *                                                                                *
 **********************************************************************************/

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include <fstream>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

using namespace std;





int TMVAClassification_main(TString variable_name, TString type, std::vector<TString> variables_names,int nTrees = 200, float MinNodeSize = 0.05, int maxDepth = 4)
{
	TString myMethodList = ""; 
   // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
   // if you use your private .rootrc, or run from a different directory, please copy the
   // corresponding lines from .rootrc

   // methods to be processed can be given as an argument; use format:
   //
   // mylinux~> root -l TMVAClassification.C\(\"myMethod1,myMethod2,myMethod3\"\)
   //
   // if you like to use a method via the plugin mechanism, we recommend using
   //
   // mylinux~> root -l TMVAClassification.C\(\"P_myMethod\"\)
   // (an example is given for using the BDT as plugin (see below),
   // but of course the real application is when you write your own
   // method based)

   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   Use["Fisher"]          = 0;
   Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   // --- Boosted Decision Trees
   Use["BDTB"]            = 0; // decorrelation + Adaptive Boost
   Use["BDTG"]            = 0; 
   Use["BDT"]            = 1; 
   // 
   // ---------------------------------------------------------------


   std::cout << "==> Start TMVAClassification" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return 1;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Here the preparation phase begins

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
 //  TString outfileName( "TMVA_bjet_new_powheg.root" );

	TString end = "axis2jet2q";
	
//    TString outfileName("output/v25/TMVA_main_v25_Nm1_"+variable_name+type+end+".root" );
   TString outfileName("output/TMVA_main_v25_Np1_"+variable_name+type+end+".root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
	ofstream out;
// 	out.open("output/v25/txt/TMVA_main_v25_Nm1_"+variable_name+type+end+".txt"); 
	out.open("output/TMVA_main_v25_Np1_"+variable_name+type+end+".txt"); 

        
   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory is 
   // the only TMVA object you have to interact with
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in
   // front of the "Silent" argument in the option string
 //  char factoryName[80];
//	sprintf(factoryName,"TMVA_13TeV_%s_%s_parameters_nTrees%i_MinNodeSize%.2f_maxDepth%i",type,end,nTrees,maxDepth);
   TMVA::Factory *factory = new TMVA::Factory( "Classification", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;P;G,D:AnalysisType=Classification" );

   // If you wish to modify default settings
   // (please check "src/Config.h" to see all available global options)
   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   
		TString weightDirName = "weights/TMVAClassification_BDTG_";
		weightDirName.Append(variable_name);
		weightDirName.Append(type);
		weightDirName.Append(end);
		weightDirName.Append("_v25");
  	   (TMVA::gConfig().GetIONames()).fWeightFileDir = weightDirName;
  	   

   // Define the input variables that shall be used for the MVA training
   // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
   // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
  // factory->AddVariable( "myvar1 := var1+var2", 'F' );
  // factory->AddVariable( "myvar2 := var1-var2", "Expression 2", "", 'F' );



//second itereatin final order "Mqq" "DeltaPhiQQ",  "SoftN5", "HTsoft", "CSV1", "CSV2","DeltaEtaQB1", "DeltaEtaQB2",  "qgl1_VBF", "qgl2_VBF"  "Jet5_pt"

//	const int max_variables_number=20;
    //	TString variables_names[max_variables_number]={ "Mqq", "DeltaEtaQQ", "Jet2q_pt" ,"Jet1q_pt", "Jet1q_leadTrackPt", "Jet2q_leadTrackPt", "axis2_jet1", "axis2_jet2", "qq_pt", "Jet3_pt", "RptHard", "Zll_pt", "Zll_zstar"}; 
    //	TString variables_names[max_variables_number]={ "Mqq", "DeltaEtaQQ", "Jet2q_pt" ,"Jet1q_pt", "Jet1q_leadTrackPt", "Jet2q_leadTrackPt", "axis2_jet1", "axis2_jet2", "qq_pt", "RptHard", "Zll_pt", "Zll_zstar"}; 
    //	TString variables_names[max_variables_number]={ "Mqq", "DeltaEtaQQ", "Jet2q_pt",  "axis2_jet1", "axis2_jet2", "qq_pt", "RptHard", "Zll_zstar"}; 
//   TString variables_names[max_variables_number]={ "Mqq", "DeltaEtaQQ","Jet2q_pt", "axis2_jet1","axis2_jet2", "qq_pt", "Zll_zstar"}; 
    //	TString variables_names[max_variables_number]={ "Mqq", "DeltaEtaQQ",   "axis2_jet1" , "axis2_jet2", "Zll_zstar"}; 

//         TString variables_names[max_variables_number]={ "Mqq", "DeltaEtaQQ", "q1_eta", "ll_pt", "ll_mass","met_pt", "EWKHTsoft", "qq_pt", "RptHard", "softActivityEWK_njets5", "btagCMVA", "qgl_1q", "qgl_2q", "cosThetaStar", "ll_zstar"}; 

//         TString variables_names[max_variables_number]={ "Mqq", "DeltaEtaQQ", "q1_eta", "ll_pt", "ll_mass","met_pt", "EWKHTsoft", "qq_pt", "RptHard", "softActivityEWK_njets5", "btagCSV", "qgl_1q", "qgl_2q", "ll_zstar", "ll_ystar", "Jet1q_pt",  "Jet2q_pt", "ll_eta", "cosThetaPlane", "cosThetaStarJet"}
         
//         TString variables_names[max_variables_number]={ "Mqq", "DeltaEtaQQ", "q1_eta", "ll_pt", "ll_mass","met_pt", "EWKHTsoft", "RptHard", "btagCSV", "qgl_1q", "qgl_2q", "ll_zstar", "ll_ystar", "Jet1q_pt",  "Jet2q_pt", "ll_eta"}
                  
                  

 

//         factory->AddVariable(variables_names[i], "", "", 'F' );
        
        
        
        
        
        
        
       out<<"We used variables : "<<endl;
	for (int i=0;i<variables_names.size();i++){ 
        
        factory->AddVariable(variables_names[i], "", "", 'F' );
	out<<variables_names[i]<<"   ,   ";
        }
        if (variable_name.CompareTo("nomore")!=0){
        factory->AddVariable(variable_name, "", "", 'F' );
        }
        
        
        
       

   // You can add so-called "Spectator variables", which are not used in the MVA training,
   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
   // input variables, the response values of all trained MVAs, and the spectator variables
  // factory->AddSpectator( "spec1 := var1*2",  "Spectator 1", "units", 'F' );
  // factory->AddSpectator( "spec2 := var1*3",  "Spectator 2", "units", 'F' );

   // Read training and test data
   // (it is also possible to use ASCII format as input -> see TMVA Users Guide)
// 	TString fname_signal_test ="main_tmva_tree_EWK_LL_JJ_v25"+type+"_test_split_incl.root";
// 	TString fname_bg_test ="main_tmva_tree_DYJetstoLL_madgraph_v25"+type+"_test_split_incl.root";
// 	TString fname_signal_train ="main_tmva_tree_EWK_LL_JJ_v25"+type+"_train_split_incl.root";
// 	TString fname_bg_train ="main_tmva_tree_DYJetstoLL_madgraph_v25"+type+"_train_split_incl.root";

//	TString fname_signal_test ="/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/main_tmva_tree_VBF_HToMuMu_v25mu_QCDScalenom_JESnom_509894Events.root";
	TString fname_signal_test ="/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/training_17gen/main_tmva_tree_VBF_HToMuMu_v25mu_QCDScalenom_JESnom.root";
	
	
	
//	TString fname_bg_test ="/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/main_tmva_tree_DYJetstoLL_amc_Filter105_v25mu_QCDScalenom_JESnom.root";main_tmva_tree_DYJetsToLL_M-105To160-madgraphMLM_v25mu_QCDScalenom_JESnom.rootbkgDY
    TString fname_bg_test ="/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/training_17gen/main_tmva_tree_DYJetsToLL_M-105To160-madgraphMLM_v25mu_QCDScalenom_JESnom.root";
  
//sftp://abonavit@lxplus.cern.ch/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/training_11_gen
      //  /afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src
//   TString fname_signal_train ="/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/main_tmva_tree_VBF_HToMuMu_v25mu_QCDScalenom_JESnom_509894Events.root";	
   TString fname_signal_train ="/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/training_17gen/main_tmva_tree_VBF_HToMuMu_v25mu_QCDScalenom_JESnom.root";
   //"/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/var_recuperate_Signal.root";	
   
   //
  // bkgDY"/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/var_recuperate_Bkg.root"; 
   
//   TString fname_bg_train ="/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/";   
   TString fname_bg_train ="/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/training_17gen/main_tmva_tree_DYJetsToLL_M-105To160-madgraphMLM_v25mu_QCDScalenom_JESnom.root" ;
   //"/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/var_recuperate_Bkg.root";   
   //"/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/var_recuperate_Bkg.root";
   TFile *input_signal_train = TFile::Open( fname_signal_train );   
   TFile *input_bg_train = TFile::Open( fname_bg_train ); 
 //TTree *signal_train     = (tree*)input_signal_train->Get("TMVA");    //scommentate oggi 15dic2017
   //TTree *bg_train = (TTree*)input_bg_train->Get("TMVA");//scommentate oggi 15dic2017
    //factory->AddSignalTree    ( signal_train,     signalWeight  );   //scommentate oggi 15dic2017
 //factory->AddBackgroundTree( bg_train, bgWeight );//scommentate oggi 15dic2017
   
  
    
  /* if (gSystem->AccessPathName( fname_signal_train )) { // file does not exist in local directory
		cout<<"input file "<< fname_signal_train<<" doesn't exist!"<<endl;
		//break;
	}
   if (gSystem->AccessPathName( fname_bg_train)) { // file does not exist in local directory
		cout<<"input file "<< fname_bg_train<<" doesn't exist!"<<endl;
		//break;
	}

   
   */


   
   TFile *input_signal_test = TFile::Open( fname_signal_test );

   TFile *input_bg_test = TFile::Open( fname_bg_test );
 //TFile *input_signal_train = TFile::Open( fname_signal_train );//scommentate oggi 15dic2017
  //TFile *input_bg_train = TFile::Open( fname_bg_train );//scommentate oggi 15dic2017

   std::cout << "--- TMVAClassification       : Using input signal file: " << input_signal_train->GetName() << std::endl;
   std::cout << "--- TMVAClassification       : Using input bg file: " << input_bg_train->GetName() << std::endl;
   

   // --- Register the training and test trees

   TTree *signal_train = (TTree*)input_signal_train->Get("TMVA");
   TTree *bg_train = (TTree*)input_bg_train->Get("TMVA");
   TTree *signal_test = (TTree*)input_signal_test->Get("TMVA");//scommentate oggi 15dic2017
   TTree *bg_test = (TTree*)input_bg_test->Get("TMVA");//scommentate oggi 15dic2017
   
   cout <<"signal_train"<<signal_train<<endl;
   cout <<"bg_train"<<bg_train<<endl;
   cout <<"signal_test"<<signal_test<<endl;
   cout <<"bg_test"<<bg_test<<endl;
   // global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0;
   Double_t bgWeight = 1.0;
   
   // You can add an arbitrary number of signal or background trees
   factory->AddSignalTree    ( signal_train,     signalWeight );
  factory->AddBackgroundTree( bg_train, bgWeight ); //commentate il 22gennaio 2018 per avere un sample solo
  // factory->AddSignalTree    ( signal_train,     signalWeight,TMVA::Types::kTraining );
   //factory->AddBackgroundTree( bg_train, bgWeight,TMVA::Types::kTraining );
  // factory->AddSignalTree    ( signal_test,     signalWeight,TMVA::Types::kTesting );
 //  factory->AddBackgroundTree( bg_test, bgWeight,TMVA::Types::kTesting );
 
   
   //factory->AddSignalTree    ( signal_test,     signalWeight,TMVA::Types::kTesting  );//scommentate oggi 15dic2017
   //factory->AddBackgroundTree( bg_test, bgWeight ,TMVA::Types::kTesting);//scommentate oggi 15dic2017
//	factory->SetSignalWeightExpression("weightMVA");//scommentate oggi 15dic2017
	//factory->SetBackgroundWeightExpression("weightMVA");//scommentate oggi 15dic2017
   TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";
   



   // Tell the factory how to use the training and testing events
   //
   // If no numbers of events are given, half of the events in the tree are used 
   // for training, and the other half for testing:
   //    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
   // To also specify the number of testing events, use:
   //    factory->PrepareTrainingAndTestTree( mycut,
   //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
   factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                                    "nTrain_Signal=15000:nTrain_Background=15000:NormMode=NumEvents:!V" );
 // factory->PrepareTrainingAndTestTree( mycuts, mycutb,"SplitMode=Random:NormMode=NumEvents:!V" );  //june Option
//factory->PrepareTrainingAndTestTree( mycuts, mycutb,
//                                        "nTrain_Signal=100000:nTrain_Background=100000:SplitMode=Block:NormMode=NumEvents:!V" );
//   factory->PrepareTrainingAndTestTree( mycuts, mycutb,
//                                         "nTrain_Signal=14000:nTrain_Background=14000:SplitMode=Random:NormMode=NumEvents:!V" );


   // ---- Book MVA methods
   //
   // Please lookup the various method configuration options in the corresponding cxx files, eg:
           // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

   // Fisher discriminant (same as LD)
   if (Use["Fisher"])
      factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );


   if (Use["MLPBNN"])
      factory->BookMethod( TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators


   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTD",
                           "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );
//GOOD UP TO HERE

	char parString[500];
	sprintf(parString,"!H:!V:NTrees=%i:MinNodeSize=%.2f:BoostType=Grad:Shrinkage=.1:UseBaggedBoost:BaggedSampleFraction=0.6:MaxDepth=%i:nCuts=20",nTrees,MinNodeSize,maxDepth); //correct parString
  // sprintf(parString,"!H:!V:NTrees=%i:MinNodeSize=%.2f:BoostType=Grad:Shrinkage=.1:UseBaggedBoost:BaggedSampleFraction=0.6:MaxDepth=%i:nCuts=20",nTrees,MinNodeSize,maxDepth);
	// sprintf(parString,"!H:!V:NTrees=%i:MinNodeSize=%.2f:MaxDepth=%i:BoostType=AdaBoost:AdaBoostBeta=0.1:SeparationType=GiniIndex:NegWeightTreatment=IgnoreNegWeightsInTraining:PruneMethod=NoPruning",nTrees,MinNodeSize,maxDepth);

	cout<<parString<<endl;

   if (Use["BDT"]) {//
           std::cout << " HERE 5 "  << std::endl;
      factory->BookMethod( TMVA::Types::kBDT, "BDTG",parString );
           std::cout << " HERE 6 "  << std::endl;
        }
           std::cout << " HERE 7 "  << std::endl;
	out<<parString<<endl;
	//	   "!H:!V:NTrees=150:MinNodeSize=0.06%:BoostType=Grad:Shrinkage=.1:nCuts=20:MaxDepth=3:NegWeightTreatment=IgnoreNegWeightsInTraining:PruneMethod=NoPruning" );
	//	   "!H:!V:NTrees=200:MinNodeSize=5%:BoostType=AdaBoost:AdaBoostBeta=0.6:nCuts=-1:MaxDepth=3:NegWeightTreatment=IgnoreNegWeightsInTraining:PruneMethod=NoPruning" );
   
	// For an example of the category classifier usage, see: TMVAClassificationCategory

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can optimize the setting (configuration) of the MVAs using the set of training events

   // ---- STILL EXPERIMENTAL and only implemented for BDT's ! 
   // factory->OptimizeAllMethods("SigEffAt001","Scan");
   // factory->OptimizeAllMethods("ROCIntegral","FitGA");

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs



        
   // Train MVAs using the set of training events
   factory->TrainAllMethods();

      std::cout << " HERE 4 "  << std::endl;
   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

      std::cout << " HERE 5 "  << std::endl;
   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   
      std::cout << " HERE 66 "  << std::endl;

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

  delete factory;
cout <<"giulio1"<<endl;
   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVAGui( outfileName );
//cout<< "out"<<out<<endl;
cout <<"giulio12"<<endl;
	out.close();
	cout <<"giulio13"<<endl;
	
}



void modified_TMVAClassification_mainNp1()
{

const int max_variables_number=39;
const int primary_variables_number=9;


TString variables_names_array_primary[primary_variables_number]={"ll_mass", "Mqq", "RptHard","ll_zstar","softActivityEWK_njets5","ll_pt","W_mass_virtual2","W_Pt_virtual1","diffMassWWH"}
//TString variables_names_array_primary[primary_variables_number]={"ll_mass", "Mqq", "RptHard", "DeltaEtaQQ", "ll_pt", "ll_eta", "EWKHTsoft", "Jet2q_pt"}; //variabili giugno
//, "RptHard","thetastarW1", "W_mass_virtual1", "W_Pt_virtual1", "EWKHTsoft"
//{"ll_mass", "Mqq","W_Pt_virtual2", "RptHard","thetastarHtoWW","DeltaEtaQQ","W_eta_virtual1","E_parton1","EWKHTsoft","theta1"}
TString variables_names_array[max_variables_number]={"Inv_mass","ll_mass","energytot","W_mass_virtual1","W_mass_virtual2","qgl_1q","qgl_2q" ,"thetastarW2","thetastarW1","theta1",
"qq_pt","theta2","W_Pt_virtual1","W_Pt_virtual2","Mqq", "RptHard", "ll_eta", "EWKHTsoft", "DeltaEtaQQ",  "Jet2q_pt","diffMassWWH", "ll_pt","Jet3_pt","ll_zstar","met_pt","softLeadingJet_pt","btagCMVA", "cosThetaStarJet","WWmass","impulsoZ",  "cosThetaPlane","softActivityEWK_njets2","softActivityEWK_njets5","softActivityEWK_njets10","W_eta_virtual1","W_eta_virtual2","E_parton1","E_parton2","deltaM"};

//

//,"W_mass_virtual2","thetastarHtoWW","thetastarW2","thetastarW1","theta1",
//"qq_pt","theta2","W_Pt_virtual1","W_Pt_virtual2","Mqq", "RptHard", "ll_eta", "EWKHTsoft", "DeltaEtaQQ",  "Jet2q_pt", "ll_pt",
//"W_eta_virtual1","W_eta_virtual2","E_parton1","E_parton2"}
//{{"ll_mass", "Mqq", "RptHard", "DeltaEtaQQ", "ll_pt", "ll_eta", "EWKHTsoft", "Jet2q_pt"}; variabili giugno

//, "EWKHTsoft","energytot"
/*,"energytot","diffMassWWH","W_mass_virtual1","W_mass_virtual2","thetastarHtoWW","thetastarW2","thetastarW1","theta1",
"qq_pt","theta2","W_Pt_virtual1","W_Pt_virtual2", "ll_eta", "EWKHTsoft", "DeltaEtaQQ",  "Jet2q_pt", "ll_pt",
"W_eta_virtual1","W_eta_virtual2","E_parton1","E_parton2"}


/*{"Inv_mass","ll_mass","energytot","diffMassWWH","W_mass_virtual1","W_mass_virtual2","thetastarHtoWW","thetastarW2","thetastarW1","theta1",
"qq_pt","theta2","W_Pt_virtual1","W_Pt_virtual2","Mqq", "RptHard", "ll_eta", "EWKHTsoft", "DeltaEtaQQ",  "Jet2q_pt", "ll_pt",
"W_eta_virtual1","W_eta_virtual2","E_parton1","E_parton2"}
*/




std::vector<TString> variables_names;
for ( int n = 0; n<primary_variables_number; ++n) {
variables_names.push_back(variables_names_array_primary[n]);
}

std::vector<TString> variableNames;
for(int i=0; i<max_variables_number;i++){
bool damettere=true;
for(int j=0; j<primary_variables_number;j++){
if(variables_names_array[i]==variables_names_array_primary[j]){
damettere=false;
}
}
if(damettere==true)
variableNames.push_back(variables_names_array[i]);

}
/*/
 for ( int n = 0; n<variableNames.size(); ++n) {
 TMVAClassification_main(variableNames[n], "mu", variables_names);
 }

/*/


TMVAClassification_main("nomore", "mu", variables_names);

for( int n = 0; n<variableNames.size(); ++n)
    cout<< variableNames[n] <<"\", \"";
//cout<<endl;
}





 






