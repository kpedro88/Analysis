//code from Caterina Vernieri

double QuaJet_L1(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-189.438)*((1+sqrt(0))/40.7426))))*0.961533))-0.00782829;};
double QuaJet_L1Up(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-189.055)*((1+sqrt(0))/40.7426))))*0.946533))+0.00717171;};
double QuaJet_L1Down(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-189.821)*((1+sqrt(0))/40.7426))))*0.976533))-0.0228283;};
double QuaJet_CaloPt4(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-52.3024)*((1+sqrt(0))/17.2244))))*0.970185))-0.0846321;};
double QuaJet_CaloPt4Up(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-51.8193)*((1+sqrt(0))/17.2244))))*0.945185))-0.0596321;};
double QuaJet_CaloPt4Down(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-52.786)*((1+sqrt(0))/17.2244))))*0.995185))-0.109632;};
double QuaJet_CSV3(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-0.831006)*((1+sqrt(0))/0.373221))))*0.903257))-4.67918e-11;};
double QuaJet_CSV3Up(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-0.779494)*((1+sqrt(0))/0.373221))))*0.873257))+0.03;};
double QuaJet_CSV3Down(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-0.851379)*((1+sqrt(0))/0.373221))))*0.933257))-0.03;};
double QuaJet_PFPt4(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-46.6623)*((1+sqrt(0))/10.6251))))*0.311284))-0.00583469;};
double QuaJet_PFPt4Up(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-45.3441)*((1+sqrt(0))/10.6251))))*0.296284))+0.00916531;};
double QuaJet_PFPt4Down(float x) { return  (1-((0.5-(0.5*TMath::Erf((x-47.8689)*((1+sqrt(0))/10.6251))))*0.326284))-0.0208347;};

double TurnOnQuad(float pt1, float pt2, float pt3, float pt4, float CSV3) {
    if(isnan(CSV3)) CSV3=0;
    CSV3=CSV3>1?1:CSV3;CSV3=CSV3<0?0:CSV3;
    return QuaJet_L1(pt1+pt2+pt3+pt4)*QuaJet_CaloPt4(pt4)*QuaJet_CSV3(-log(1-CSV3+1.e-7))*QuaJet_PFPt4(pt4);
}
double TurnOnQuadUp(float pt1, float pt2, float pt3, float pt4, float CSV3) {
    if(isnan(CSV3)) CSV3=0;
    CSV3=CSV3>1?1:CSV3;CSV3=CSV3<0?0:CSV3;
    return QuaJet_L1Up(pt1+pt2+pt3+pt4)*QuaJet_CaloPt4Up(pt4)*QuaJet_CSV3Up(-log(1-CSV3+1.e-7))*QuaJet_PFPt4Up(pt4);
}
double TurnOnQuadDown(float pt1, float pt2, float pt3, float pt4, float CSV3) {
    if(isnan(CSV3)) CSV3=0;
    CSV3=CSV3>1?1:CSV3;CSV3=CSV3<0?0:CSV3;
    return QuaJet_L1Down(pt1+pt2+pt3+pt4)*QuaJet_CaloPt4Down(pt4)*QuaJet_CSV3Down(-log(1-CSV3+1.e-7))*QuaJet_PFPt4Down(pt4);
}
