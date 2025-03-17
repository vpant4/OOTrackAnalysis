#!/bin/bash

# echo "Setup CMSSW (ROOT version)"
cd /afs/cern.ch/user/v/vpant/pprefanalysis

echo "Submit jobs at "
echo PWD: $PWD

root -l -b -q "Track_Analyzer.C(\"$1\",\"$2\")"
