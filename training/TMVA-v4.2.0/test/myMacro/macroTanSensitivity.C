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
#include <sstream>
#include "math.h"


void setHistoStyle(TH1D* histo, int LineWidth) {
        histo->SetLineWidth(LineWidth);

        double titleOffset = 1.2;
        double titleSize = 0.06;
        double labelOffset = 0.01;
        double labelSize = 0.04;

        histo->GetYaxis()->SetTitleOffset(titleOffset);
        histo->GetXaxis()->SetTitleOffset(titleOffset);
        histo->GetYaxis()->SetLabelOffset(labelOffset);
        histo->GetXaxis()->SetLabelOffset(labelOffset);
        histo->GetYaxis()->SetTitleSize(titleSize);
        histo->GetXaxis()->SetTitleSize(titleSize);
        histo->GetYaxis()->SetLabelSize(labelSize*1.5);
        histo->GetXaxis()->SetLabelSize(labelSize);
}



//int main(){
void macroTanSensitivity(){

        TFile *file = new TFile("/afs/cern.ch/user/a/abonavit/private/tesi/training/CMSSW_8_0_28/src/training/TMVA-v4.2.0/test/output/TMVA_main_v25_Np1_nomoremuaxis2jet2q.root");

        TTree *tree     = (TTree*)file->Get("TestTree");
        int Nbins = 60;
        float max = 4.;
        TH1D *hist_BDT_S = new TH1D ("hist_BDT_S","", Nbins, 0., max);
        TH1D *hist_BDT_B = new TH1D ("hist_BDT_B","", Nbins, 0., max);
        TH1D *hist_sensitivity = new TH1D ("","", Nbins, 0., max);
        hist_BDT_B->Sumw2();    
        hist_BDT_S->Sumw2();
        TCanvas * canv = new TCanvas("canv", "", 800, 600);    
        canv->cd();
        tree->Draw("atanh((BDTG+1.)/2.)>>hist_BDT_S", "(classID ==0 )");
        tree->Draw("atanh((BDTG+1.)/2.)>>hist_BDT_B", "(classID ==1 )");

        gStyle->SetOptStat(0000);


        hist_BDT_S->SetLineColor(kRed);

        hist_BDT_S->SetFillColorAlpha(kRed, 0.2);
        hist_BDT_S->SetFillStyle(3004);


        hist_BDT_B->SetLineColor(kBlue);
        hist_BDT_B->SetFillColorAlpha(kBlue, 0.2);
        hist_BDT_B->SetFillStyle(3005);

        setHistoStyle(hist_BDT_S, 2);
        setHistoStyle(hist_BDT_B, 2);

        hist_BDT_S->GetXaxis()->SetTitle("tanh^{-1}((BDT response + 1)/2)");
        hist_BDT_S->GetYaxis()->SetTitle("Expected events");
        hist_BDT_B->GetXaxis()->SetTitle("tanh^{-1}((BDT response + 1)/2)");
        hist_BDT_B->GetYaxis()->SetTitle("Expected events");



        TLegend *leg = new TLegend(0.55,0.7,0.89,0.85);
        leg->SetFillColor(0);
        leg->SetBorderSize(0);
        leg->SetTextSize(0.05);
        leg->AddEntry(hist_BDT_S,"Signal","f");
        leg->AddEntry(hist_BDT_B,"Background","f");


Double_t ErrDy=0;


        canv->SetBottomMargin(.15);
        canv->SetLeftMargin(.15);
        gPad->SetLogy();
        hist_BDT_S->Draw("hist");
        canv->Print("sensitivityTanPlot/hist_BDT_S.png");
        
        hist_BDT_B->Draw("hist");
        canv->Print("sensitivityTanPlot/hist_BDT_B.png");
        
        
        hist_BDT_S->SetTitle("");
       //hist_BDT_S->Scale(10.3/hist_BDT_S->Integral());
       // hist_BDT_B->Scale(18000./hist_BDT_B->Integral());
        hist_BDT_S->Scale(9.4/hist_BDT_S->Integral());
        hist_BDT_B->Scale(8400./hist_BDT_B->Integral()); 
      for(int n=50;n<60;n++)
       cout<<"Integrale_bkg(x>"<<n/20.<<")=" << hist_BDT_B->IntegralAndError(n,100,ErrDy)<<"+-"<<ErrDy<<endl; 
         //(h2->IntegralAndError(0,h2->GetNbinsX(),ErrDy));     
        hist_BDT_S->GetYaxis()->SetRangeUser(0.01, 100000); 
        hist_BDT_S->Draw("hist");
        hist_BDT_B->Draw("hist same");
        leg->Draw("same");

        canv->Print("figure/sensitivityTanPlot/hist_BDT_SuperImposed.png");



        for (int n = 1; n <= hist_BDT_S->GetXaxis()->GetNbins(); n++) {
            
            double SignalScan = 0.;
            double BackgroundScan = 0.;
            

            SignalScan = hist_BDT_S->GetBinContent(n);
            BackgroundScan = hist_BDT_B->GetBinContent(n);


//            for (int m = n; m <= hist_MVA_BDTG_S->GetXaxis()->GetNbins(); m++) {
//                SignalScan += hist_MVA_BDTG_S->GetBinContent(m);
//                BackgroundScan += hist_MVA_BDTG_B->GetBinContent(m);
//            }

            hist_sensitivity->SetBinContent(n, (SignalScan > 0.0001) ? SignalScan/sqrt(BackgroundScan+SignalScan) : 0.); 
        }


        float totalSensitivity = 0.;
        for (int n = 1; n < hist_sensitivity->GetXaxis()->GetNbins(); ++n) {
            totalSensitivity += hist_sensitivity->GetBinContent(n)*hist_sensitivity->GetBinContent(n);
            std::cout << "bin " << n << " sensitivity   " << hist_sensitivity->GetBinContent(n) << " \t Sigma:   " << totalSensitivity << std::endl;
        }
        totalSensitivity = sqrt(totalSensitivity);
        std::cout << "Sigma: " << totalSensitivity << std::endl;

        std::ostringstream sigmaString;
        sigmaString<<std::setprecision(3)<<totalSensitivity;
        TLatex* tex = new TLatex(0.50,0.80,("Tot sigma = " + sigmaString.str() ).c_str());
        tex->SetNDC();
        tex->SetTextAlign(35);
        tex->SetTextFont(42);
        tex->SetTextSize(0.06);
        tex->SetLineWidth(2);




        hist_sensitivity->SetLineColor(kOrange+4);
        hist_sensitivity->SetLineWidth(2);
        gPad->SetGridy();
        
        hist_sensitivity->GetXaxis()->SetTitle("tanh^{-1}((BDT response + 1)/2)");
        hist_sensitivity->GetYaxis()->SetTitle("bin per bin sensitivity");  
        setHistoStyle(hist_sensitivity, 3);


        hist_sensitivity->Draw();
        tex->Draw();
        canv->Print("figure/sensitivityTanPlot/hist_sensitivity.png");
        
        

        


}
