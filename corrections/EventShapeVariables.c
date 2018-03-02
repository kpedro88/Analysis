#include "EventShapeVariables.h"

#include "TMath.h"

/// constructor from XYZ coordinates
EventShapeVariables::EventShapeVariables(const std::vector<math::XYZVector>& inputVectors) 
  : inputVectors_(inputVectors), fwmom_computed_(false)
{
  //default value
  set_r(2.);
}

/// constructor from rho eta phi coordinates
EventShapeVariables::EventShapeVariables(const std::vector<math::RhoEtaPhiVector>& inputVectors) : fwmom_computed_(false)
{
  inputVectors_.reserve( inputVectors.size() );
  for (const auto& vec : inputVectors){
    inputVectors_.push_back(math::XYZVector(vec.x(), vec.y(), vec.z()));
  }
  //default value
  set_r(2.);
}

/// constructor from r theta phi coordinates
EventShapeVariables::EventShapeVariables(const std::vector<math::RThetaPhiVector>& inputVectors) : fwmom_computed_(false)
{
  inputVectors_.reserve( inputVectors.size() );
  for (const auto& vec : inputVectors){
    inputVectors_.push_back(math::XYZVector(vec.x(), vec.y(), vec.z()));
  }
  //default value
  set_r(2.);
}
  
/// the return value is 1 for spherical events and 0 for events linear in r-phi. This function 
/// needs the number of steps to determine how fine the granularity of the algorithm in phi 
/// should be
double 
EventShapeVariables::isotropy(const unsigned int& numberOfSteps) const
{
  const double deltaPhi=2*TMath::Pi()/numberOfSteps;
  double phi = 0, eIn =-1., eOut=-1.;
  for(unsigned int i=0; i<numberOfSteps; ++i){
    phi+=deltaPhi;
    double sum=0;
    for(unsigned int j=0; j<inputVectors_.size(); ++j){
      // sum over inner product of unit vectors and momenta
      sum+=TMath::Abs(TMath::Cos(phi)*inputVectors_[j].x()+TMath::Sin(phi)*inputVectors_[j].y());
    }
    if( eOut<0. || sum<eOut ) eOut=sum;
    if( eIn <0. || sum>eIn  ) eIn =sum;
  }
  return (eIn-eOut)/eIn;
}

/// the return value is 1 for spherical and 0 linear events in r-phi. This function needs the
/// number of steps to determine how fine the granularity of the algorithm in phi should be
double 
EventShapeVariables::circularity(const unsigned int& numberOfSteps) const
{
  const double deltaPhi=2*TMath::Pi()/numberOfSteps;
  double circularity=-1, phi=0, area = 0;
  for(unsigned int i=0;i<inputVectors_.size();i++) {
    area+=TMath::Sqrt(inputVectors_[i].x()*inputVectors_[i].x()+inputVectors_[i].y()*inputVectors_[i].y());
  }
  for(unsigned int i=0; i<numberOfSteps; ++i){
    phi+=deltaPhi;
    double sum=0, tmp=0.;
    for(unsigned int j=0; j<inputVectors_.size(); ++j){
      sum+=TMath::Abs(TMath::Cos(phi)*inputVectors_[j].x()+TMath::Sin(phi)*inputVectors_[j].y());
    }
    tmp=TMath::Pi()/2*sum/area;
    if( circularity<0 || tmp<circularity ){
      circularity=tmp;
    }
  }
  return circularity;
}

/// set exponent for computation of momentum tensor and related products
void 
EventShapeVariables::set_r(double r)
{
  r_ = r;
  /// invalidate previous cached computations
  tensors_computed_ = false;
  momentumTensor_ = TMatrixDSym(3); momentumTensor_.Zero();
  momentumTensorNoNorm_ = TMatrixDSym(3); momentumTensorNoNorm_.Zero();
  eigenValues_ = TVectorD(3); eigenValues_.Zero();
  eigenValuesList_ = std::vector<double>(3,0);
  eigenVectors_ = TMatrixD(3,3); eigenVectors_.Zero();
}

/// helper function to fill the 3 dimensional momentum tensor from the inputVectors where needed
/// also fill the 3 dimensional vectors of eigen-values and eigen-vectors;
/// the largest (smallest) eigen-value is stored at index position 0 (2)
void 
EventShapeVariables::compTensorsAndVectors()
{
  if(tensors_computed_) return;

  if ( inputVectors_.size() < 2 ){
    tensors_computed_ = true;
    return;
  }

  TMatrixDSym momentumTensor(3);
  momentumTensor.Zero();

  // fill momentumTensor from inputVectors
  double norm = 0.;
  for ( int i = 0; i < (int)inputVectors_.size(); ++i ){
    double p2 = inputVectors_[i].Dot(inputVectors_[i]);
    double pR = ( r_ == 2. ) ? p2 : TMath::Power(p2, 0.5*r_);
    norm += pR;
    double pRminus2 = ( r_ == 2. ) ? 1. : TMath::Power(p2, 0.5*r_ - 1.);
    momentumTensor(0,0) += pRminus2*inputVectors_[i].x()*inputVectors_[i].x();
    momentumTensor(0,1) += pRminus2*inputVectors_[i].x()*inputVectors_[i].y();
    momentumTensor(0,2) += pRminus2*inputVectors_[i].x()*inputVectors_[i].z();
    momentumTensor(1,0) += pRminus2*inputVectors_[i].y()*inputVectors_[i].x();
    momentumTensor(1,1) += pRminus2*inputVectors_[i].y()*inputVectors_[i].y();
    momentumTensor(1,2) += pRminus2*inputVectors_[i].y()*inputVectors_[i].z();
    momentumTensor(2,0) += pRminus2*inputVectors_[i].z()*inputVectors_[i].x();
    momentumTensor(2,1) += pRminus2*inputVectors_[i].z()*inputVectors_[i].y();
    momentumTensor(2,2) += pRminus2*inputVectors_[i].z()*inputVectors_[i].z();
  }

  // momentumTensor normalized to determinant 1
  momentumTensor_ = (1./norm)*momentumTensor;
  momentumTensorNoNorm_ = momentumTensor;

  // now get eigens
  eigenValues_ = TVectorD(3);
  if( momentumTensor_.IsSymmetric() && ( momentumTensor_.NonZeros() != 0 ) ){
    eigenVectors_ = momentumTensor_.EigenVectors(eigenValues_);
  }
  eigenValuesList_[0] = eigenValues_(0);
  eigenValuesList_[1] = eigenValues_(1);
  eigenValuesList_[2] = eigenValues_(2);
  eigenValuesNoNorm_ = TVectorD(3);
  if( momentumTensorNoNorm_.IsSymmetric() && ( momentumTensorNoNorm_.NonZeros() != 0 ) ){
    momentumTensorNoNorm_.EigenVectors(eigenValuesNoNorm_);
  }

  tensors_computed_ = true;  
}

/// 1.5*(q1+q2) where q0>=q1>=q2>=0 are the eigenvalues of the momentum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2} 
/// normalized to 1. Return values are 1 for spherical, 3/4 for plane and 0 for linear events
double 
EventShapeVariables::sphericity()
{
  if(!tensors_computed_) compTensorsAndVectors();
  return 1.5*(eigenValuesList_[1] + eigenValuesList_[2]);
}

/// 1.5*q2 where q0>=q1>=q2>=0 are the eigenvalues of the momentum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2} 
/// normalized to 1. Return values are 0.5 for spherical and 0 for plane and linear events
double 
EventShapeVariables::aplanarity()
{
  if(!tensors_computed_) compTensorsAndVectors();
  return 1.5*eigenValuesList_[2];
}

/// 3.*(q0*q1+q0*q2+q1*q2) where q0>=q1>=q2>=0 are the eigenvalues of the momentum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2} 
/// normalized to 1. Return value is between 0 and 1 
/// and measures the 3-jet structure of the event (C vanishes for a "perfect" 2-jet event)
double 
EventShapeVariables::C()
{
  if(!tensors_computed_) compTensorsAndVectors();
  return 3.*(eigenValuesList_[0]*eigenValuesList_[1] + eigenValuesList_[0]*eigenValuesList_[2] + eigenValuesList_[1]*eigenValuesList_[2]);
}

/// 27.*(q0*q1*q2) where q0>=q1>=q2>=0 are the eigenvalues of the momemtum tensor sum{p_j[a]*p_j[b]}/sum{p_j**2} 
/// normalized to 1. Return value is between 0 and 1 
/// and measures the 4-jet structure of the event (D vanishes for a planar event)
double 
EventShapeVariables::D()
{
  if(!tensors_computed_) compTensorsAndVectors();
  return 27.*eigenValuesList_[0]*eigenValuesList_[1]*eigenValuesList_[2];
}

//========================================================================================================

double EventShapeVariables::getFWmoment( int l ) {

   if ( l < 0 || l > fwmom_maxl_ ) return 0. ;

   if ( !fwmom_computed_ ) {
      computeFWmoments() ;
      fwmom_computed_ = true ;
   }

   return fwmom_[l] ;

} // getFWmoment

//========================================================================================================

void EventShapeVariables::computeFWmoments() {

  double esum_total(0.) ;
  for ( unsigned int i = 0 ; i < inputVectors_.size() ; i ++ ) {
     esum_total += inputVectors_[i].R() ;
  } // i
  double esum_total_sq = esum_total * esum_total ;

  for ( int i=0; i<=fwmom_maxl_; i++ ) { fwmom_[i] = 0. ; }

  for ( unsigned int i = 0 ; i < inputVectors_.size() ; i ++ ) {

     double p_i = inputVectors_[i].R() ;
     if ( p_i <= 0 ) continue ;

     for ( unsigned int j = 0 ; j < inputVectors_.size() ; j ++ ) {

        double p_j = inputVectors_[j].R() ;
        if ( p_j <= 0 ) continue ;

        double cosTheta = inputVectors_[i].Dot( inputVectors_[j] ) / (p_i * p_j) ;
        double pi_pj_over_etot2 = p_i * p_j / esum_total_sq ;

        fwmom_[0] += pi_pj_over_etot2 ;

        fwmom_[1] += pi_pj_over_etot2 * cosTheta ;

        fwmom_[2] += pi_pj_over_etot2 * 0.5 * ( 3. * pow( cosTheta, 2. ) - 1. ) ;
        if ( fwmom_maxl_ <= 2 ) continue ;

        fwmom_[3] += pi_pj_over_etot2 * 0.5 * ( 5. * pow( cosTheta, 3. ) - 3. * cosTheta ) ;
        if ( fwmom_maxl_ <= 3 ) continue ;

        fwmom_[4] += pi_pj_over_etot2 * 0.125 * ( 35. * pow( cosTheta, 4. ) - 30. * pow( cosTheta, 2. ) + 3. ) ;
        if ( fwmom_maxl_ <= 4 ) continue ;

        fwmom_[5] += pi_pj_over_etot2 * 0.125 * ( 63. * pow( cosTheta, 5. ) - 70. * pow( cosTheta, 3. ) + 15. * cosTheta ) ;
        if ( fwmom_maxl_ <= 5 ) continue ;

        fwmom_[6] += pi_pj_over_etot2 * 0.0625 * ( 231. * pow( cosTheta, 6. ) - 315. * pow( cosTheta, 4. ) + 105. * pow( cosTheta, 2. ) - 5. ) ;
        if ( fwmom_maxl_ <= 6 ) continue ;

        fwmom_[7] += pi_pj_over_etot2 * 0.0625 * ( 429. * pow( cosTheta, 7. ) - 693. * pow( cosTheta, 5. ) + 315. * pow( cosTheta, 3. ) - 35. * cosTheta ) ;
        if ( fwmom_maxl_ <= 7 ) continue ;

        fwmom_[8] += pi_pj_over_etot2 * 0.0078125 * ( 6435. * pow( cosTheta, 8. ) - 12012. * pow( cosTheta, 6. ) + 6930. * pow( cosTheta, 4. ) - 1260. * pow( cosTheta, 2 ) + 35. ) ;
        if ( fwmom_maxl_ <= 8 ) continue ;

        fwmom_[9] += pi_pj_over_etot2 * 0.0078125 * ( 12155. * pow( cosTheta, 9. ) - 25740. * pow( cosTheta, 7. ) + 18018. * pow( cosTheta, 5. ) - 4620. * pow( cosTheta, 3 ) + 315. * cosTheta ) ;
        if ( fwmom_maxl_ <= 9 ) continue ;

       fwmom_[10] += pi_pj_over_etot2 * 0.00390625 * ( 46189. * pow( cosTheta, 10. ) - 109395. * pow( cosTheta, 8. ) + 90090. * pow( cosTheta, 6. ) - 30030. * pow( cosTheta, 4 ) + 3465. * pow( cosTheta, 2 ) - 63  )  ;
        if ( fwmom_maxl_ <= 10 ) continue ;

       fwmom_[11] += pi_pj_over_etot2 * 0.003906250 * (  + 88179 * pow( cosTheta, 11 )  - 230945 * pow( cosTheta, 9 )  + 218790 * pow( cosTheta, 7 )  - 90090 * pow( cosTheta, 5 )  + 15015 * pow( cosTheta, 3 )  - 693 * cosTheta  ) ;
        if ( fwmom_maxl_ <= 11 ) continue ;

       fwmom_[12] += pi_pj_over_etot2 * 0.000976562 * (  + 676039 * pow( cosTheta, 12 )  - 1939938 * pow( cosTheta, 10 )  + 2078505 * pow( cosTheta, 8 )  - 1021020 * pow( cosTheta, 6 )  + 225225 * pow( cosTheta, 4 )  - 18018 * pow( cosTheta, 2 )  + 231  ) ;
        if ( fwmom_maxl_ <= 12 ) continue ;

       fwmom_[13] += pi_pj_over_etot2 * 0.000976562 * (  + 1300075 * pow( cosTheta, 13 )  - 4056234 * pow( cosTheta, 11 )  + 4849845 * pow( cosTheta, 9 )  - 2771340 * pow( cosTheta, 7 )  + 765765 * pow( cosTheta, 5 )  - 90090 * pow( cosTheta, 3 )  + 3003 * cosTheta  ) ;
        if ( fwmom_maxl_ <= 13 ) continue ;

       fwmom_[14] += pi_pj_over_etot2 * 0.000488281 * (  + 5014575 * pow( cosTheta, 14 )  - 16900975 * pow( cosTheta, 12 )  + 22309287 * pow( cosTheta, 10 )  - 14549535 * pow( cosTheta, 8 )  + 4849845 * pow( cosTheta, 6 )  - 765765 * pow( cosTheta, 4 )  + 45045 * pow( cosTheta, 2 )  - 429  ) ;
        if ( fwmom_maxl_ <= 14 ) continue ;

       fwmom_[15] += pi_pj_over_etot2 * 0.000488281 * (  + 9694845 * pow( cosTheta, 15 )  - 35102025 * pow( cosTheta, 13 )  + 50702925 * pow( cosTheta, 11 )  - 37182145 * pow( cosTheta, 9 )  + 14549535 * pow( cosTheta, 7 )  - 2909907 * pow( cosTheta, 5 )  + 255255 * pow( cosTheta, 3 )  - 6435 * cosTheta  ) ;
        if ( fwmom_maxl_ <= 15 ) continue ;

       fwmom_[16] += pi_pj_over_etot2 * 0.000030518 * (  + 300540195 * pow( cosTheta, 16 )  - 1163381400 * pow( cosTheta, 14 )  + 1825305300 * pow( cosTheta, 12 )  - 1487285800 * pow( cosTheta, 10 )  + 669278610 * pow( cosTheta, 8 )  - 162954792 * pow( cosTheta, 6 )  + 19399380 * pow( cosTheta, 4 )  - 875160 * pow( cosTheta, 2 )  + 6435  ) ;
        if ( fwmom_maxl_ <= 16 ) continue ;

       fwmom_[17] += pi_pj_over_etot2 * 0.000030518 * (  + 583401555 * pow( cosTheta, 17 )  - 2404321560 * pow( cosTheta, 15 )  + 4071834900 * pow( cosTheta, 13 )  - 3650610600 * pow( cosTheta, 11 )  + 1859107250 * pow( cosTheta, 9 )  - 535422888 * pow( cosTheta, 7 )  + 81477396 * pow( cosTheta, 5 )  - 5542680 * pow( cosTheta, 3 )  + 109395 * cosTheta  ) ;
        if ( fwmom_maxl_ <= 17 ) continue ;

       fwmom_[18] += pi_pj_over_etot2 * 0.000015259 * (  + 2268783825 * pow( cosTheta, 18 )  - 9917826435 * pow( cosTheta, 16 )  + 18032411700 * pow( cosTheta, 14 )  - 17644617900 * pow( cosTheta, 12 )  + 10039179150 * pow( cosTheta, 10 )  - 3346393050 * pow( cosTheta, 8 )  + 624660036 * pow( cosTheta, 6 )  - 58198140 * pow( cosTheta, 4 )  + 2078505 * pow( cosTheta, 2 )  - 12155  ) ;
        if ( fwmom_maxl_ <= 18 ) continue ;

       fwmom_[19] += pi_pj_over_etot2 * 0.000015259 * (  + 4418157975 * pow( cosTheta, 19 )  - 20419054425 * pow( cosTheta, 17 )  + 39671305740 * pow( cosTheta, 15 )  - 42075627300 * pow( cosTheta, 13 )  + 26466926850 * pow( cosTheta, 11 )  - 10039179150 * pow( cosTheta, 9 )  + 2230928700 * pow( cosTheta, 7 )  - 267711444 * pow( cosTheta, 5 )  + 14549535 * pow( cosTheta, 3 )  - 230945 * cosTheta  ) ;
        if ( fwmom_maxl_ <= 19 ) continue ;

       fwmom_[20] += pi_pj_over_etot2 * 0.000003815 * (  + 34461632205 * pow( cosTheta, 20 )  - 167890003050 * pow( cosTheta, 18 )  + 347123925225 * pow( cosTheta, 16 )  - 396713057400 * pow( cosTheta, 14 )  + 273491577450 * pow( cosTheta, 12 )  - 116454478140 * pow( cosTheta, 10 )  + 30117537450 * pow( cosTheta, 8 )  - 4461857400 * pow( cosTheta, 6 )  + 334639305 * pow( cosTheta, 4 )  - 9699690 * pow( cosTheta, 2 )  + 46189  ) ;
        if ( fwmom_maxl_ <= 20 ) continue ;

       fwmom_[21] += pi_pj_over_etot2 * 0.0000038146973 * (  + 67282234305 * pow( cosTheta, 21 )  - 344616322050 * pow( cosTheta, 19 )  + 755505013725 * pow( cosTheta, 17 )  - 925663800600 * pow( cosTheta, 15 )  + 694247850450 * pow( cosTheta, 13 )  - 328189892940 * pow( cosTheta, 11 )  + 97045398450 * pow( cosTheta, 9 )  - 17210021400 * pow( cosTheta, 7 )  + 1673196525 * pow( cosTheta, 5 )  - 74364290 * pow( cosTheta, 3 )  + 969969 * cosTheta  ) ;
        if ( fwmom_maxl_ <= 21 ) continue ;

       fwmom_[22] += pi_pj_over_etot2 * 0.0000019073486 * (  + 263012370465 * pow( cosTheta, 22 )  - 1412926920405 * pow( cosTheta, 20 )  + 3273855059475 * pow( cosTheta, 18 )  - 4281195077775 * pow( cosTheta, 16 )  + 3471239252250 * pow( cosTheta, 14 )  - 1805044411170 * pow( cosTheta, 12 )  + 601681470390 * pow( cosTheta, 10 )  - 124772655150 * pow( cosTheta, 8 )  + 15058768725 * pow( cosTheta, 6 )  - 929553625 * pow( cosTheta, 4 )  + 22309287 * pow( cosTheta, 2 )  - 88179  ) ;
        if ( fwmom_maxl_ <= 22 ) continue ;

       fwmom_[23] += pi_pj_over_etot2 * 0.0000019073486 * (  + 514589420475 * pow( cosTheta, 23 )  - 2893136075115 * pow( cosTheta, 21 )  + 7064634602025 * pow( cosTheta, 19 )  - 9821565178425 * pow( cosTheta, 17 )  + 8562390155550 * pow( cosTheta, 15 )  - 4859734953150 * pow( cosTheta, 13 )  + 1805044411170 * pow( cosTheta, 11 )  - 429772478850 * pow( cosTheta, 9 )  + 62386327575 * pow( cosTheta, 7 )  - 5019589575 * pow( cosTheta, 5 )  + 185910725 * pow( cosTheta, 3 )  - 2028117 * cosTheta  ) ;
        if ( fwmom_maxl_ <= 23 ) continue ;

       fwmom_[24] += pi_pj_over_etot2 * 0.0000002384186 * (  + 8061900920775 * pow( cosTheta, 24 )  - 47342226683700 * pow( cosTheta, 22 )  + 121511715154830 * pow( cosTheta, 20 )  - 178970743251300 * pow( cosTheta, 18 )  + 166966608033225 * pow( cosTheta, 16 )  - 102748681866600 * pow( cosTheta, 14 )  + 42117702927300 * pow( cosTheta, 12 )  - 11345993441640 * pow( cosTheta, 10 )  + 1933976154825 * pow( cosTheta, 8 )  - 194090796900 * pow( cosTheta, 6 )  + 10039179150 * pow( cosTheta, 4 )  - 202811700 * pow( cosTheta, 2 )  + 676039  ) ;
        if ( fwmom_maxl_ <= 24 ) continue ;

       fwmom_[25] += pi_pj_over_etot2 * 0.0000002384186 * (  + 15801325804719 * pow( cosTheta, 25 )  - 96742811049300 * pow( cosTheta, 23 )  + 260382246760350 * pow( cosTheta, 21 )  - 405039050516100 * pow( cosTheta, 19 )  + 402684172315425 * pow( cosTheta, 17 )  - 267146572853160 * pow( cosTheta, 15 )  + 119873462177700 * pow( cosTheta, 13 )  - 36100888223400 * pow( cosTheta, 11 )  + 7091245901025 * pow( cosTheta, 9 )  - 859544957700 * pow( cosTheta, 7 )  + 58227239070 * pow( cosTheta, 5 )  - 1825305300 * pow( cosTheta, 3 )  + 16900975 * cosTheta  ) ;
        if ( fwmom_maxl_ <= 25 ) continue ;

       fwmom_[26] += pi_pj_over_etot2 * 0.0000001192093 * (  + 61989816618513 * pow( cosTheta, 26 )  - 395033145117975 * pow( cosTheta, 24 )  + 1112542327066950 * pow( cosTheta, 22 )  - 1822675727322450 * pow( cosTheta, 20 )  + 1923935489951475 * pow( cosTheta, 18 )  - 1369126185872445 * pow( cosTheta, 16 )  + 667866432132900 * pow( cosTheta, 14 )  - 222622144044300 * pow( cosTheta, 12 )  + 49638721307175 * pow( cosTheta, 10 )  - 7091245901025 * pow( cosTheta, 8 )  + 601681470390 * pow( cosTheta, 6 )  - 26466926850 * pow( cosTheta, 4 )  + 456326325 * pow( cosTheta, 2 )  - 1300075  ) ;
        if ( fwmom_maxl_ <= 26 ) continue ;

       fwmom_[27] += pi_pj_over_etot2 * 0.0000001192093 * (  + 121683714103007 * pow( cosTheta, 27 )  - 805867616040669 * pow( cosTheta, 25 )  + 2370198870707850 * pow( cosTheta, 23 )  - 4079321865912150 * pow( cosTheta, 21 )  + 4556689318306125 * pow( cosTheta, 19 )  - 3463083881912655 * pow( cosTheta, 17 )  + 1825501581163260 * pow( cosTheta, 15 )  - 667866432132900 * pow( cosTheta, 13 )  + 166966608033225 * pow( cosTheta, 11 )  - 27577067392875 * pow( cosTheta, 9 )  + 2836498360410 * pow( cosTheta, 7 )  - 164094946470 * pow( cosTheta, 5 )  + 4411154475 * pow( cosTheta, 3 )  - 35102025 * cosTheta  ) ;
        if ( fwmom_maxl_ <= 27 ) continue ;

       fwmom_[28] += pi_pj_over_etot2 * 0.0000000298023 * (  + 956086325095055 * pow( cosTheta, 28 )  - 6570920561562378 * pow( cosTheta, 26 )  + 20146690401016725 * pow( cosTheta, 24 )  - 36343049350853700 * pow( cosTheta, 22 )  + 42832879592077575 * pow( cosTheta, 20 )  - 34630838819126550 * pow( cosTheta, 18 )  + 19624141997505045 * pow( cosTheta, 16 )  - 7823578204985400 * pow( cosTheta, 14 )  + 2170565904431925 * pow( cosTheta, 12 )  - 408140597414550 * pow( cosTheta, 10 )  + 49638721307175 * pow( cosTheta, 8 )  - 3610088822340 * pow( cosTheta, 6 )  + 136745788725 * pow( cosTheta, 4 )  - 2035917450 * pow( cosTheta, 2 )  + 5014575  ) ;
        if ( fwmom_maxl_ <= 28 ) continue ;

       fwmom_[29] += pi_pj_over_etot2 * 0.0000000298023 * (  + 1879204156221315 * pow( cosTheta, 29 )  - 13385208551330770 * pow( cosTheta, 27 )  + 42710983650155457 * pow( cosTheta, 25 )  - 80586761604066900 * pow( cosTheta, 23 )  + 99943385714847675 * pow( cosTheta, 21 )  - 85665759184155150 * pow( cosTheta, 19 )  + 51946258228689825 * pow( cosTheta, 17 )  - 22427590854291480 * pow( cosTheta, 15 )  + 6845630929362225 * pow( cosTheta, 13 )  - 1447043936287950 * pow( cosTheta, 11 )  + 204070298707275 * pow( cosTheta, 9 )  - 18050444111700 * pow( cosTheta, 7 )  + 902522205585 * pow( cosTheta, 5 )  - 21037813650 * pow( cosTheta, 3 )  + 145422675 * cosTheta  ) ;
        if ( fwmom_maxl_ <= 29 ) continue ;

       fwmom_[30] += pi_pj_over_etot2 * 0.0000000149012 * (  + 7391536347803839 * pow( cosTheta, 30 )  - 54496920530418135 * pow( cosTheta, 28 )  + 180700315442965395 * pow( cosTheta, 26 )  - 355924863751295475 * pow( cosTheta, 24 )  + 463373879223384675 * pow( cosTheta, 22 )  - 419762220002360235 * pow( cosTheta, 20 )  + 271274904083157975 * pow( cosTheta, 18 )  - 126155198555389575 * pow( cosTheta, 16 )  + 42051732851796525 * pow( cosTheta, 14 )  - 9888133564634325 * pow( cosTheta, 12 )  + 1591748329916745 * pow( cosTheta, 10 )  - 166966608033225 * pow( cosTheta, 8 )  + 10529425731825 * pow( cosTheta, 6 )  - 347123925225 * pow( cosTheta, 4 )  + 4508102925 * pow( cosTheta, 2 )  - 9694845  ) ;
        if ( fwmom_maxl_ <= 30 ) continue ;


     } // j
  } // i

} // computeFWmoments

//========================================================================================================













