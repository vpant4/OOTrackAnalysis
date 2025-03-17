#!/bin/bash

# Set path to CMSSW
#source $HOME/setup_cmsenv.sh

# Initial parameters to run
EXEC_PATH=${HOME}/OOTrackAnalysis/OOTrackAnalysis
cd $EXEC_PATH

# Date of submission
formatted_date=$(date +"%Y%m%d")

# Read input parameters


input_file_list="$1"
sample_prefix=$(basename "$input_file_list" | sed 's/\.[^.]*$//')



# Specify number of files per list to split
files_per_job=1
#input_file_list=$HOME/filelists/test_list.t

echo -e "Splitting input file list: ${input_file_list}"
n_sublists=$(./split_pprefinput.sh ${input_file_list} ${files_per_job} ${sample_prefix}) 
echo -e "Input file list is splitted into ${n_sublists}"

if [ ! -d "condor/sub/OO2024/${formatted_date}" ]; then
    echo "Directory 'condor/sub/OO2024/${formatted_date}' does not exist. Creating..."
    mkdir -p "condor/sub/OO2024/${formatted_date}"
    if [ $? -eq 0 ]; then
        echo "Directory created successfully."
    else
        echo "Failed to create directory. Terminating"
        exit 1
    fi
fi

if [ ! -d "condor/log/OO2024/${formatted_date}" ]; then
    echo "Directory 'condor/log/OO2024/${formatted_date}' does not exist. Creating..."
    mkdir -p "condor/log/OO2024/${formatted_date}"
    if [ $? -eq 0 ]; then
        echo "Directory created successfully."
    else
        echo "Failed to create directory. Terminating"
        exit 1
    fi
fi

cat <<EOF > condor/sub/OO2024/${formatted_date}/OO2024_${sample_prefix}.sub
universe = vanilla
executable = ${EXEC_PATH}/sub.sh
+JobFlavour           = "longlunch"
getenv     = True
requirements =((OpSysAndVer =?= "AlmaLinux9") && (CERNEnvironment =?= "qa"))
RequestCpus = 1
transfer_input_files  = voms_proxy.txt
environment = "X509_USER_PROXY=voms_proxy.txt"
EOF


for ((jobId = 1; jobId <= $n_sublists; jobId++)); do
    cat <<EOF >>condor/sub/ppref2024/${formatted_date}/ppref2024_${sample_prefix}.sub
arguments             = input/ppref2024/${formatted_date}/${sample_prefix}_$jobId.txt /eos/user/v/vpant/OO2024/OO2024outputfiles/${sample_prefix}_$jobId.root 
output                = condor/log/ppref2024/${formatted_date}/${sample_prefix}_$jobId.out
error                 = condor/log/ppref2024/${formatted_date}/${sample_prefix}_$jobId.err
log                   = condor/log/ppref2024/${formatted_date}/${sample_prefix}_$jobId.log
queue 

EOF

done

condor_submit condor/sub/ppref2024/${formatted_date}/OO2024_${sample_prefix}.sub
