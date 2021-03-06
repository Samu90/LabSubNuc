

//Fa una media di tutte le forme d'onda contenenti eventi
//root[0] .x MediaForme("nomeFileInput","nomeFileOutput")


void MediaForme2(const char* fileName,const char* fileName2)
{
gROOT->Reset();
    // apre file prende il TTree di nome "newtree" dal file
    TFile* file = new TFile(fileName);
    TTree* tree = (TTree*)file->Get("newtree");

    TFile* fileout = new TFile(fileName2, "RECREATE");
    TTree* datatree = new TTree("datatree", "alberomedie");
    
    // setta indirizzi delle variabili di interesse
    Double_t ch0[1000],ch2[1000],ch4[1000],ch6[1000];
    Int_t Nevent, Nsample, entrate;
    Int_t t0,t2,t4,t6;
    Int_t t0sc,t2sc,t4sc,t6sc;
   
    tree->SetBranchAddress("v0",&ch0);
    tree->SetBranchAddress("v2",&ch2);
    tree->SetBranchAddress("v4",&ch4);
    tree->SetBranchAddress("v6",&ch6);
	
    tree->SetBranchAddress("Nevent", &Nevent);
    tree->SetBranchAddress("Nsample",&Nsample);

    tree->SetBranchAddress("t0",&t0);
    tree->SetBranchAddress("t2",&t2);
    tree->SetBranchAddress("t4",&t4);
    tree->SetBranchAddress("t6",&t6);
    

    tree->SetBranchAddress("t0sc",&t0sc);
    tree->SetBranchAddress("t2sc",&t2sc);
    tree->SetBranchAddress("t4sc",&t4sc);

    datatree->Branch("nsample",&Nsample,"nsample/I");
    datatree->Branch("nevent",&Nevent,"nevent/I");
    
    
    
    
    Double_t v0[1000],v2[1000],v4[1000],v6[1000];
    Double_t temp0[1000],temp2[1000],temp4[1000],temp6[1000],time[1000];   //Arrey su cui vanno fatte le somme 
    int i,j;
    int timeLow=90;


    datatree->Branch("ch0",temp0,"ch0[nsample]/D");
    datatree->Branch("ch2",temp2,"ch2[nsample]/D");
    datatree->Branch("ch4",temp4,"ch4[nsample]/D");
    datatree->Branch("ch6",temp6,"ch6[nsample]/D");

    
    //azzero i temporanei
    for(i=0;i<Nsample;i++){
	temp0[i]=0;
	temp2[i]=0;
	temp4[i]=0;
	temp6[i]=0;
	time[i]=i;
      }

      //inizilizzo tempi di sincronizzazione
   
      entrate=tree->GetEntries();
      
    for(j=0;j<entrate;j++){
      tree->GetEntry(j);

      for(i=0;i<Nsample;i++){
	v0[i]=ch0[i];
	v2[i]=ch2[i];
	v4[i]=ch4[i];
	v6[i]=ch6[i];
      }
   
    //sommo su tutti i V(i)
      for(i=t6-20; i<Nsample-1; i++) {
	temp6[100+i-t6]+=v6[i];}
      for(i=t4-20; i<Nsample-1; i++){
	temp4[100+i-t4]+=v4[i];}
      for(i=t0-20;i<Nsample-1;i++){
	temp0[100+i-t0]+=v0[i];}
      for(i=t2-20;i<Nsample-1;i++){
	temp2[100+i-t2]+=v2[i];
      }
      
      
      if( j%10 == 0 ) cout<< "Entry: " << j << " / " << entrate <<endl;
    }// chiudo for j
    
    
    // riscalo
    for(i=0;i<Nsample;i++){
      temp0[i]/=(entrate);
      temp2[i]/=(entrate);
      temp4[i]/=(entrate);
      temp6[i]/=(entrate);
    }// chiudo for i

    Nevent=1; // correggo il numero dell'evento a mano
    
    cout<<"vdsg"<<Nsample<<endl;
    datatree->Fill();
    
    
    TCanvas *cha0 = new TCanvas("cha0", "Canale 0");
    cha0->Divide(2,2);
    cha0->cd(1);

    TGraph *antonio = new TGraph(Nsample, time, temp0);
    antonio->SetTitle("Canale 0");
    //antonio->SetMinimum(-400);
    //  antonio->SetMaximum(50);
    antonio->Draw();
    
    
    cha0->cd(2);
    TGraph *gigetto = new TGraph(Nsample, time, temp2);
    gigetto->SetTitle("Canale 2");
//  gigetto->SetMinimum(-400);
//  gigetto->SetMaximum(50);
    gigetto->Draw();



    cha0->cd(3);
    TGraph *pablo = new TGraph(Nsample, time, temp4);
    pablo->SetTitle("Canale 4");
//  pablo->SetMinimum(-250);
//  pablo->SetMaximum(50);
    pablo->Draw();

    
    cha0->cd(4);
    TGraph *antoniocalabro = new TGraph(Nsample, time, temp6);
    antoniocalabro->SetTitle("Canale 6");
//  antoniocalabro->SetMinimum(-250);
//  antoniocalabro->SetMaximum(50);
    antoniocalabro->Draw();

    fileout->cd();
    datatree->Write();
    fileout->Close();
    
    
    file->Close();
    
}
