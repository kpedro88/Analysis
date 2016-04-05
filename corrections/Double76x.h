//code from Caterina Vernieri

double DoubleJet_L1(float x) { return (1-((0.5-(0.5*TMath::Erf((x-189.888)/40.7837)))*0.961039))-0.00764755;};
double DoubleJet_L1Up(float x) { return (1-((0.5-(0.5*TMath::Erf((x-189.505)/40.7837)))*0.946039))+0.00735245;};
double DoubleJet_L1Down(float x) { return (1-((0.5-(0.5*TMath::Erf((x-190.271)/40.7837)))*0.976039))-0.0226475;};
double DoubleJet_CaloPt4(float x) { return (1-((0.5-(0.5*TMath::Erf((x-24.1445)/22.6885)))*2))-0.067548;};
double DoubleJet_CaloPt4Up(float x) { return (1-((0.5-(0.5*TMath::Erf((x-23.8599)/22.6885)))*1.98))-0.047548;};
double DoubleJet_CaloPt4Down(float x) { return (1-((0.5-(0.5*TMath::Erf((x-24.4284)/22.6885)))*2.02))-0.087548;};
double DoubleJet_CaloPt2(float x) { return (1-((0.5-(0.5*TMath::Erf((x-96.1584)/15.6312)))*0.881715))-0.0794771;};
double DoubleJet_CaloPt2Up(float x) { return (1-((0.5-(0.5*TMath::Erf((x-95.7861)/15.6312)))*0.861715))-0.0594771;};
double DoubleJet_CaloPt2Down(float x) { return (1-((0.5-(0.5*TMath::Erf((x-96.5292)/15.6312)))*0.901715))-0.0994771;};
double DoubleJet_CSV3(float x) { return (1-((0.5-(0.5*TMath::Erf((x-0.831866)/0.398092)))*0.926046))-5.47056e-11;};
double DoubleJet_CSV3Up(float x) { return (1-((0.5-(0.5*TMath::Erf((x-0.777729)/0.398092)))*0.896046))+0.03;};
double DoubleJet_CSV3Down(float x) { return (1-((0.5-(0.5*TMath::Erf((x-0.850966)/0.398092)))*0.956046))-0.03;};
double DoubleJet_PFPt4(float x) { return (1-((0.5-(0.5*TMath::Erf((x-11.4378)/20.7501)))*2))-0.00472764;};
double DoubleJet_PFPt4Up(float x) { return (1-((0.5-(0.5*TMath::Erf((x-9.70331)/20.7501)))*1.98))+0.0152724;};
double DoubleJet_PFPt4Down(float x) { return (1-((0.5-(0.5*TMath::Erf((x-13.0217)/20.7501)))*2.02))-0.0247276;};
double DoubleJet_PFPt2(float x) { return (1-((0.5-(0.5*TMath::Erf((x-97.442)/8.96807)))*0.392321))-0.00742174;};
double DoubleJet_PFPt2Up(float x) { return (1-((0.5-(0.5*TMath::Erf((x-93.0681)/8.96807)))*0.342321))+0.0425783;};
double DoubleJet_PFPt2Down(float x) { return (1-((0.5-(0.5*TMath::Erf((x-101.44)/8.96807)))*0.442321))-0.0574217;};

double TurnOnDouble(float pt1, float pt2, float pt3, float pt4, float CSV3) {
    if(isnan(CSV3)) CSV3=0;
    CSV3=CSV3>1?1:CSV3;CSV3=CSV3<0?0:CSV3;
    return DoubleJet_L1(pt1+pt2+pt3+pt4)*DoubleJet_CaloPt4(pt4)*DoubleJet_CSV3(-log(1-CSV3+1.e-7))*DoubleJet_PFPt4(pt4)*DoubleJet_PFPt2(pt2)*DoubleJet_CaloPt2(pt2);
}
double TurnOnDoubleUp(float pt1, float pt2, float pt3, float pt4, float CSV3) {
    if(isnan(CSV3)) CSV3=0;
    CSV3=CSV3>1?1:CSV3;CSV3=CSV3<0?0:CSV3;
    return DoubleJet_L1Up(pt1+pt2+pt3+pt4)*DoubleJet_CaloPt4Up(pt4)*DoubleJet_CSV3Up(-log(1-CSV3+1.e-7))*DoubleJet_PFPt4Up(pt4)*DoubleJet_PFPt2Up(pt2)*DoubleJet_CaloPt2Up(pt2);
}
double TurnOnDoubleDown(float pt1, float pt2, float pt3, float pt4, float CSV3) {
    if(isnan(CSV3)) CSV3=0;
    CSV3=CSV3>1?1:CSV3;CSV3=CSV3<0?0:CSV3;
    return DoubleJet_L1Down(pt1+pt2+pt3+pt4)*DoubleJet_CaloPt4Down(pt4)*DoubleJet_CSV3Down(-log(1-CSV3+1.e-7))*DoubleJet_PFPt4Down(pt4)*DoubleJet_PFPt2Down(pt2)*DoubleJet_CaloPt2Down(pt2);
}
