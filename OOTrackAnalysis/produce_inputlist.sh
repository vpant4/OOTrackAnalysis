#!/bin/bash

count=1
for subdir in /eos/cms/store/group/phys_heavyions/vpant/OOCraboutput/Hijing_NoPU_100kEvents_OO_5360GeV_GenSim_030825/crab_preliminary2018Cuts_effTables_generalTracks_LooseSelection/250316_061550/000*; do
  output_file="/afs/cern.ch/user/v/vpant/OOTrackAnalysis/OOTrackAnalysis/filelists/OO2024filelist.txt"
  find "$subdir" -type f -name "*.root" > "$output_file"
  ((count++))
done
