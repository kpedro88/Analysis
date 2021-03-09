{
  //processline is necessary because putting #include outside of function means the function must be named rather than anonymous
  //and old versions of cling expect the function name to match the file name exactly, including the leading period, which is not allowed
  //this leads to "warning: cannot find function '.rootlogon()'; falling back to .L" and then the function is not loaded properly
  //(newer versions of cling check for _rootlogon instead to avoid this problem)
  gInterpreter->ProcessLine("#define LOCAL_ROOTLOGON");
  gInterpreter->ProcessLine("#include \"style-CMSTDR.C\"");
  setTDRStyle();
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libDataFormatsFWLite.so");
  gSystem->Load("libDataFormatsPatCandidates.so");
  FWLiteEnabler::enable();
}
