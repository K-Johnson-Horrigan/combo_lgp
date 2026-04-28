# combo_lgp

Included here are files for my CS851 system. To run, compile and execute: 
g++ main.cpp -o main ; ./main

Edit system parameters (mutation probability, crossover probability) in main.cpp.

To combine all data files, run: 
awk FNR!=1 data/* > combo_data_file.csv 
