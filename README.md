# Simulating-802.11-DCF-MAC-Protocol
 * Course: CSCE 575
 * Project: Simulating 802.11 DCF MAC (Phase 1)
 * Author: Md Farhadur Reza
 


How to Run the Program:

1. Run the executable file or Create a project/source file with any C++ compiler and copy the main.cpp into source file.

2. After running the program, program will prompt for two user inputs: 

 i. No. of stations contending for the channel
 ii. Seed value

3. Program will output the follwing:
  i. Backoff time (in slot) for all the stations.
  ii. Minimum backoff time and no. of station has the same backoff time (in both slot and microsecond)
  iii.Whether collision occurs.
  iv. Successfully frame transmission start time for the station.
  v. Status of all the station: idle, transmitted, backoff
  vi. Total delay and average delay for all stations to start and finish frame transmission.
