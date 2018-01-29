#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFile.h"
#include "TROOT.h"
#include <TLatex.h>

//int main(){

void setHistoStyle(TH1F *histo) {
histo->GetYaxis()->SetLabelSize(0.05);
histo->GetXaxis()->SetLabelSize(0.05);
histo->GetYaxis()->SetTitleSize(0.06);
histo->GetYaxis()->SetTitleOffset(1.);
}
//std::string chiq_string)
void drawSomething(TH1D* htest,TH1D* htrain,std::string figname,std::string kolmog_string,std::string prob_chiq_string){
TCanvas *c=new TCanvas("c","",1000,800);
c->cd();
gPad->SetLogy();
htest->SetLineColor(2);
htest->Draw("E0");
htrain->Draw("same,E0");
float Xbottom = 0.2;
float Ybottom = 0.2;
float Xtop = 0.3;
float Ytop = 0.3;
TLegend *leg = new TLegend(Xbottom,Ybottom,Xtop,Ytop);
		leg->SetFillColor(0);
		leg->SetBorderSize(0.7);
		leg->SetTextSize(0.04);
		leg->SetTextFont(42);
       
	    leg->AddEntry(htest,"test","l");
		leg->AddEntry(htrain,"train","l");
        leg->Draw();
        
float yt2 = 0.2;
float xt2 = 0.7;
TLatex *t2 =new TLatex(xt2,yt2,("KS= "+kolmog_string).c_str());
t2->SetNDC();
t2->SetTextAlign(22);
t2->SetTextSize(0.05);
t2->Draw();
        
float yt3 = 0.5;
float xt3 = 0.7;
TLatex *t3 =new TLatex(xt3,yt3,( "Prob(Chi2)="+prob_chiq_string).c_str());
t3->SetNDC();
t3->SetTextAlign(22);
t3->SetTextSize(0.05);
t3->Draw();
cdc->Print(("figure/BDToutput/"+figname).c_str());
}
void efficiency_roc_Np1(){
//	gROOT->ProcessLine(".x setTDRStyle.C");

//	const int n_variables =25;


//        std::string variables_names[n_variables]={ "Mqq", "DeltaEtaQQ", "q1_eta", "ll_pt", "ll_mass","met_pt", "EWKHTsoft", "qq_pt", "RptHard", "softActivityEWK_njets5", "btagCMVA", "qgl_1q", "qgl_2q", "ll_zstar", "ll_ystar", "Jet1q_pt",  "Jet2q_pt", "ll_eta", "cosThetaPlane", "cosThetaStarJet", "all"}
//        std::string variables_names[n_variables]={ "Mqq", "ll_pt", "ll_mass","met_pt", "RptHard", "softActivityEWK_njets5",  "Jet2q_pt", "all"}


//          std::string variables_names[n_variables]={"Inv_mass", "energytot", "diffMassWWH", "W_mass_virtual1", "W_mass_virtual2", "thetastarHtoWW", "thetastarW2", "thetastarW1", "theta1", "qq_pt", "theta2", "W_Pt_virtual1", "W_Pt_virtual2", "RptHard", "ll_eta", "EWKHTsoft", "DeltaEtaQQ", "Jet2q_pt", "ll_pt", "W_eta_virtual1", "W_eta_virtual2", "E_parton1", "E_parton2", "deltaM", "nomore"};
           //8+1


   /*     const int n_variables =9;
          std::string variables_names[n_variables]={ "nomore_LO_Pray_",  "nomore_LO_IgnoreNegWeightsInTraining_",  "nomore_NLO_Pray_", "nomore_NLO_IgnoreNegWeightsInTraining_" ,"nomore_NLO_noWeight_" ,"nomore_RpHard_OldSample_LO_","nomore_TT_LO_", "nomore_Random_","nomore_Random_LO_TT_"};/*/
const int n_variables =31;
std::string variables_names[n_variables]={"Inv_mass", "energytot", "W_mass_virtual1", "W_mass_virtual2", "qgl_1q", "qgl_2q", "thetastarW2", "thetastarW1", "theta1", "qq_pt", "theta2", "W_Pt_virtual1", "W_Pt_virtual2", "diffMassWWH", "Jet3_pt", "ll_zstar", "met_pt", "softLeadingJet_pt", "btagCMVA", "cosThetaStarJet", "WWmass", "impulsoZ", "cosThetaPlane", "softActivityEWK_njets2", "softActivityEWK_njets5", "softActivityEWK_njets10", "W_eta_virtual1", "W_eta_virtual2", "E_parton1", "E_parton2", "nomore"};                     
  /* "Inv_mass","energytot","W_mass_virtual1","qgl_1q","qgl_2q","thetastarW2", "thetastarW1", "theta1", "qq_pt", "theta2", "W_Pt_virtual2","ll_eta","EWKHTsoft", "DeltaEtaQQ","Jet2q_pt", "diffMassWWH", "Jet3_pt", "met_pt", "softLeadingJet_pt", "btagCMVA", "cosThetaStarJet", "WWmass", "impulsoZ", "cosThetaPlane", "softActivityEWK_njets2", "softActivityEWK_njets10", "W_eta_virtual1","W_eta_virtual2","E_parton1","E_parton2","nomore"/*/
        /*  std::string variables_names[n_variables]={"nomore_LO_PairNegWeightsGlobal_", "nomore_LO_Pray_", "nomore_LO_InverseBoostNegWeights_", "nomore_LO_IgnoreNegWeightsInTraining_", "nomore_NLO_PairNegWeightsGlobal_", "nomore_NLO_Pray_", "nomore_NLO_InverseBoostNegWeights_", "nomore_NLO_IgnoreNegWeightsInTraining_" , "nomore_NLO_noWeight_","nomore_RpHard_OldSample_LO_", "nomore_LO_noWeight_","nomore_RpHard_","nomore_TT_LO_","nomore_Random_","nomore_Random_Ignore_LO_TT_","nomore_Random_LO_TT_"};/*/
//	const int n_variables =13  ;
//        std::string variables_names[n_variables]={ "q1_eta", "met_pt", "qq_pt", "softActivityEWK_njets5", "btagCMVA", "qgl_1q", "qgl_2q", "ll_zstar", "ll_ystar", "Jet1q_pt", "cosThetaPlane", "cosThetaStarJet", "NoMore"}
//"AdaBoostR2_","Grad","nTrees1000","AdaBoostBeta","PruneMethodCostComplexity","CrossEntropy","GiniIndexWithLaplace","MisClassificationError","NoPruning","PruneMethodExpectedError"


//	std::string variables_names[n_variables]={"Mqq", "DeltaEtaQQ", "axis2_jet1", "axis2_jet2",  "Zll_zstar","all"}; 
	std::string file_names[n_variables];
	Double_t frame2_axisx[n_variables];
	Double_t Chi_S_axisx[n_variables];
	Double_t Chi_B_axisx[n_variables];
	Double_t totalProbS_axisx[n_variables];
	Double_t totalProbB_axisx[n_variables];
	for (int i=0;i<n_variables;i++){
		frame2_axisx[i] = 0.5+i;
		Chi_S_axisx[i] = 0.5+i;
		Chi_B_axisx[i] = 0.5+i;
		totalProbS_axisx[i] = 0.5+i;
		totalProbB_axisx[i] = 0.5+i;
		file_names[i] = variables_names[i];
	}


	std::string end = "axis2jet2q";
	Double_t totalSensitivity[n_variables];
	Double_t totalChiS[n_variables];
    Double_t totalChiB[n_variables];
    Double_t totalProbS[n_variables];
    Double_t totalProbB[n_variables];
    
	for (int i=0;i<n_variables;i++){
		file_names[i] = "/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/training/TMVA-v4.2.0/test/output/TMVA_main_v25_Np1_"+file_names[i];
//		file_names[i] = "/afs/cern.ch/user/g/gimandor/private/CMSSW_8_0_25/src/NadyaTreeAfterTMVA/TMVA_main_v25_Np1_"+file_names[i];
		file_names[i] = file_names[i]+"mu"+end+".root";
//		file_names[i] = file_names[i]+"_2var.root";
	}



    int Nbins = 100;
    float max = 5.;

	for (int current_file=0;current_file<n_variables;current_file++){

		TFile *file = new TFile(file_names[current_file].c_str());
		TFile *file2= new TFile(file_names[current_file].c_str());
        TTree *tree     = (TTree*)file->Get("TestTree");
        TTree *train_tree  = (TTree*)file->Get("TrainTree");
        std::cout << file_names[current_file] << std::endl;
        TH1D *hist_BDT_S = new TH1D ("hist_BDT_S", variables_names[current_file].c_str(), Nbins, 0., max);
        TH1D *hist_BDT_B = new TH1D ("hist_BDT_B", variables_names[current_file].c_str(), Nbins, 0., max);
        TH1D *hist_BDT_S_train = new TH1D ("hist_BDT_S_train","", Nbins, 0., max);
        TH1D *hist_BDT_B_train = new TH1D ("hist_BDT_B_train","", Nbins, 0., max);
        TH1D *hist_sum=new TH1D ("hist_sum","Sum_signal+bkg",Nbins,0.,max);
        
        hist_BDT_S->Sumw2();
        hist_BDT_B->Sumw2();
        hist_BDT_S_train->Sumw2();
        hist_BDT_B_train->Sumw2();
        
//        float bdt = 0;
//			tree->SetBranchAddress("BDTG",&bdt);
//			for (int evt=0;evt<tree->GetEntries();evt++){
//				tree->GetEntry(evt);
//				float atanh_bdt= TMath::ATanH((bdt+1)/2.0);
//                hist_BDT_S->Fill(atanh_bdt);
//                hist_BDT_B->Fill(atanh_bdt);
//            }


        
        tree->Draw("atanh((BDTG+1.)/2.)>>hist_BDT_S", "(classID ==0 )");
        tree->Draw("atanh((BDTG+1.)/2.)>>hist_BDT_B", "(classID ==1 )");
        train_tree->Draw("atanh((BDTG+1.)/2.)>>hist_BDT_S_train", "(classID ==0 )");
        train_tree->Draw("atanh((BDTG+1.)/2.)>>hist_BDT_B_train", "(classID ==1 )");
       // atanh((BDT_VBF+1.)/2.
        hist_BDT_S->Scale(9.4*0.944/hist_BDT_S->Integral());
        hist_BDT_B->Scale(8500./hist_BDT_B->Integral());
        hist_BDT_S_train->Scale(9.4*0.944/hist_BDT_S_train->Integral());
        hist_BDT_B_train->Scale(8500./hist_BDT_B_train->Integral());
        
    std::ostringstream curr_file;
    curr_file << current_file;
    std::string curr_file_string(curr_file.str());
        
      
        totalSensitivity[current_file] = 0.;
        for (int n = 1; n <= hist_BDT_S->GetXaxis()->GetNbins(); n++) {
            double SignalScan = 0.;
            double BackgroundScan = 0.;
            double signalIntegr=0.;
         
            SignalScan = hist_BDT_S->GetBinContent(n);
            BackgroundScan = hist_BDT_B->GetBinContent(n);
            hist_sum->SetBinContent(n,SignalScan+BackgroundScan);
            signalIntegr=hist_BDT_S->Integral();
            printf("bin= %6.3f \t",n) ;
            printf("signal= %6.3f \t ",SignalScan);
            printf("background = %6.3f \t",BackgroundScan);
            printf("totalSignal= %6.3f \t",signalIntegr);
            printf("sensitivity =% 6.3f \t",SignalScan*SignalScan/(BackgroundScan+SignalScan));
            totalSensitivity[current_file] += (((BackgroundScan+SignalScan) > 0.0001) && (SignalScan > 0.0001)) ? SignalScan*SignalScan/(BackgroundScan+SignalScan) : 0.;
          printf("totalSensitivity =% 6.3f  \n",totalSensitivity[current_file]);     
        }
        totalSensitivity[current_file] = sqrt(totalSensitivity[current_file]);
        cout<<totalSensitivity[current_file]<<endl;
       
	
	    TCanvas *R2d2 = new TCanvas("R2d2", "R2d2", 1000, 700);
		R2d2->SetBottomMargin(.15);
		R2d2->SetLeftMargin(.15);
        gPad->SetLogy();
        R2d2->cd();   
        hist_sum->Draw();
        hist_BDT_S->SetLineColor(2);
        hist_BDT_S->Draw("same");
        R2d2->Print("sum_sign_bkg.png");
	/*	
		TLegend *l = new TLegend(0.7,0.86,0.9,0.9);
		l->SetFillColor(0);
		l->SetBorderSize(0);
		l->SetTextSize(0.04);
		l->AddEntry(hist_BDT_S,"Dimuon","PL");
		leg->AddEntry(gr,"Dilepton","PL");
//		leg->Draw("same");/*/
double X_S= hist_BDT_S_train->Chi2Test(hist_BDT_S,"CHI2/NDF");
double X_B= hist_BDT_B_train->Chi2Test(hist_BDT_B,"CHI2/NDF");
double prob_S=TMath::Prob( hist_BDT_S_train->Chi2Test(hist_BDT_S,"CHI2"),hist_BDT_S_train->GetNbinsX()-1);
double prob_B=TMath::Prob(hist_BDT_B_train->Chi2Test(hist_BDT_B,"CHI2"),hist_BDT_B_train->GetNbinsX()-1);
double K_s=hist_BDT_S_train->KolmogorovTest(hist_BDT_S);
double K_b= hist_BDT_B_train->KolmogorovTest(hist_BDT_B);

    std::ostringstream kolmog_s;
    kolmog_s << K_s; 
    std::string K_s_string(kolmog_s.str());
    std::ostringstream kolmog_b;
    kolmog_b << K_b; 
    std::string K_b_string(kolmog_b.str());
    
    std::ostringstream prob_s;
    prob_s << prob_S; 
    std::string Prob_S_string(prob_s.str());
    std::ostringstream prob_b;
    prob_b <<prob_B ; 
    std::string Prob_B_string(prob_b.str());
    
    
    
    
       drawSomething(hist_BDT_B, hist_BDT_B_train,"BDT_bkg"+curr_file_string+".png",K_b_string,Prob_B_string);
       drawSomething(hist_BDT_S, hist_BDT_S_train,"BDT_signal"+curr_file_string+".png",K_s_string,Prob_S_string);

totalChiS[current_file]=X_S;
totalChiB[current_file]=X_B;
totalProbS[current_file]=prob_S;
totalProbB[current_file]=prob_B;
cout<< totalChiS <<" " << totalChiB << endl;
    }

        gPad->SetGridx();
        gPad->SetGridy();

        
		TCanvas *c2 = new TCanvas("c2", "c2", 1200, 700);
		c2->SetBottomMargin(.15);
		c2->SetLeftMargin(.15);
		
		TCanvas *cS = new TCanvas("cS", "cS", 1000, 700);
		cS->SetBottomMargin(.15);
		cS->SetLeftMargin(.15);
		
		
	    TCanvas *cB = new TCanvas("cB", "cB", 1000, 700);
		cB->SetBottomMargin(.15);
		cB->SetLeftMargin(.15);
	

		
       
        gStyle->SetOptStat(0000);
		TH1F *frame2 = new TH1F("frame2","",n_variables,0.,n_variables);
		TH1F *hChi_S=new TH1F("hChi_S","",n_variables,0.,n_variables);
		TH1F *hChi_B=new TH1F("hChi_B","",n_variables,0.,n_variables);
	    TH1F *hProb_S=new TH1F("hProb_S","",n_variables,0.,n_variables);
	    TH1F *hProb_B=new TH1F("hProb_B","",n_variables,0.,n_variables);
	    
	  frame2->SetMinimum(0.75);
      frame2->SetMaximum(.84);
      frame2->SetMinimum(totalSensitivity[n_variables-1]*0.5);
      frame2->SetMaximum(totalSensitivity[n_variables-1]*1.5);
      frame2->GetYaxis()->SetTitleOffset(1.5);
      frame2->GetXaxis()->SetTitleOffset(0.5);
      frame2->SetStats(0);
      frame2->SetTitleFont(42,"x");
	  frame2->SetTitleFont(42,"y");
      frame2->SetTitleSize(0.05, "XYZ");
		frame2->SetYTitle("Sensitivity");
		frame2->SetXTitle("");	
		frame2->GetXaxis()->SetLabelSize(0.001);
		
		hChi_S->SetYTitle("Chi2");
		hChi_S->GetXaxis()->SetLabelSize(0.05);
        hChi_B->SetYTitle("Chi2");
		hChi_B->GetXaxis()->SetLabelSize(0.05);
		hProb_S->SetYTitle("Probability");
		hProb_S->GetXaxis()->SetLabelSize(0.05);
		hProb_B->SetYTitle("Probability");
		hProb_B->GetXaxis()->SetLabelSize(0.05);
		hProb_B->SetMinimum(-0.1);
		hProb_B->SetMaximum(1.1);
		
		hist_BDT_B->SetMinimum(0.001);
		hist_BDT_S->SetMinimum(0.001);
		
		//hChi_B->SetLogy();
		//hChi_S->SetLogy();
		
		for (int i=0;i<n_variables;i++){
			frame2->GetXaxis()->SetBinLabel(i+1,variables_names[i].c_str());
			hChi_S->GetXaxis()->SetBinLabel(i+1,variables_names[i].c_str());
		    hChi_B->GetXaxis()->SetBinLabel(i+1,variables_names[i].c_str());
		    hProb_S->GetXaxis()->SetBinLabel(i+1,variables_names[i].c_str());
		    hProb_B->GetXaxis()->SetBinLabel(i+1,variables_names[i].c_str());
		    
}
        setHistoStyle(frame2);
        setHistoStyle(hChi_S);
        setHistoStyle(hChi_B);
        		

        c2->cd();
		frame2->Draw();
		c2->SetBottomMargin(.3);
        gPad->SetGridx();
        gPad->SetGridy();
		
		
		TGraph *gr = new TGraph(n_variables,frame2_axisx,totalSensitivity);
		gr->SetMarkerStyle(20);
		gr->SetLineWidth(2);
		gr->Draw("PLsame");
		
		TGraph *grS = new TGraph(n_variables,totalProbS_axisx,totalProbS);
		grS->SetMarkerStyle(20);
		grS->SetLineWidth(2);

		cS->cd();
		hProb_S->Draw();
		grS->Draw("PLsame");
	    gPad->SetGridx();
        gPad->SetGridy();
        
        
		TGraph *grB = new TGraph(n_variables,totalProbB_axisx,totalProbB);
	
		grB->SetMarkerStyle(20);
		grB->SetLineWidth(2);

        cB->cd();
		hProb_B->Draw();
		grB->Draw("PLsame");
		
		TLegend *leg = new TLegend(0.7,0.86,0.9,0.9);
		leg->SetFillColor(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.04);
	//	leg->AddEntry(gr,"Dimuon","PL");
		leg->AddEntry(gr,"Dilepton","PL");
//		leg->Draw("same");
		TLine *line2 = new TLine(0.,totalSensitivity[n_variables-1],(Double_t)n_variables,  totalSensitivity[n_variables-1]);
		line2->SetLineStyle(2);
		line2->SetLineColor(2);
		line2->SetLineWidth(2);
		line2->Draw("Lsame");
//		c2->Print("pippo.png");
		c2->Print("efficiency_v2m5_mu.png");
        cS->Print("figure/Chi2&KS_prob/chi2_signal.png");
        cB->Print("figure/Chi2&KS_prob/chi2_bkg.png");

       
}






