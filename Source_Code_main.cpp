/* 
 * File:   main.cpp
 * Course: CSCE 575
 * Project: Simulating 802.11 DCF MAC (Phase 3)
 * Author: Md Farhadur Reza
 * CLID: mxr7945
 *
 */

#include <cstdlib>
#include<iostream>
#include<cmath>
#include<vector>
#include<limits>
#include <fstream>
#include "header.h"


using namespace std;



/*
 * 
 */

int main() {
    
    cout<<"802.11g Protocol Parameters are: "<<"\n";
    cout<<"Short Slot time (when no legacy stations are present) for 802.11g is 9 microseconds"<<"\n";
    cout<<"Inter-Frame Spacing SIFS and DIFS for 802.11g is 10 microseconds and 28 microseconds respectively"<<"\n";
    cout<<"Maximum data rate for 802.11g is 54Mbps"<<"\n";
    cout<<"Maximum payload size for 802.11g frame is 2312 bytes"<<"\n";
    cout<<"Minimum and Maximum contention window for 802.11g is 16 slots and 1024 slots respectively"<<"\n";
    cout<<"Contention window for Colliding Stations is doubled using Binary Exponential Algorithm"<<"\n";
    cout<<"Transmission range of an antenna is 150 meters. In this Simulation, Distance of 1 means 150 meters";
    cout<<"RTS, CTS, and ACK frame sizes are 20 bytes, 20 bytes, and 14 bytes respectively";
    
    DIFS = 28;
    SIFS=10;
    slot_time =9;
    data_rate = 54; // 54 Mbps
    max_frame_size = 2312 *8;     
    cwmin_slot= 16; 
    cwmax_slot= 1024; 
    
    
    RTS_time=2.96;
    CTS_time=2.96;
    Ack_time=2.07;
    
    string topology;
    int min;
    string out_filename;
    ofstream out;
    
    cout<<"\n"<<"Output File Name: ";
    cin>>out_filename;
    
    out.open(out_filename.c_str());
    
    frame_time= (max_frame_size)/ data_rate;
    timer= timer+frame_time; //increase the global timer as we assume channel is busy 
                        //for the first frame time
    
   
    
    cout<<"\n"<<"RTS Enable/Disable ";
    cin>>RTS_Enable;
    
    cout<<"\n"<<"Topology Type ";
    cin>>topology;
    
    if(topology=="Grid")
    {
    cout<<"\n"<<"Dimension of the Grid Size: ";
    cin>>Grid_size;
    }
    
    else if(topology=="String")
    {
      cout<<"\n"<<"Total no of nodes in String Topology: ";
      cin>>string_nodes;  
    }
    
    
    cout<<"No. of Sources: ";
    cin>>S;
    cout<<"\n";
    cout<<"No. of Destinations: ";
    cin>>D;
    
    //cout<<"\n"<<"No of stations contending for a channel: ";
    //cin>>N;
    cout<<"SEED No: ";
    cin>>SEED;
    
    cout<<"\n"<<"Probability of remaining in Good state: ";
    cin>>Pgg;
    
    cout<<"\n"<<"Probability of remaining in Bad state: ";
    cin>>Pbb;
    
    
    station_state=new int[S];
    collision_state=new int[S];
    channel_state=channel_busy; //assume channel is busy
    backoff_time=new int[S];
    backoff_slot=new int[S];
    
    
    trans_attempt=0; //total no. of frame transmission attempt
    trans_success=0; //total no. of frame transmission successful
    trans_collision=0; //total no. of frame transmission Collided
    trans_discarded=0; //total no. of frames Discarded
    channel_bad_count=0; //to count bad channel state
    channel_good_count=0; //to count good channel state
    
    sum_delay=0; //sum of transmission delay of all the station
    avg_delay=0; //average of transmission delay of all the station
    no_of_station=S; //for keeping track of no. of station that has finished its transmission
    total_no_frames;
    
    
    
    Pgb= float(1- Pgg);
    Pbg= float (1-Pbb);
    
    
    
    
    cout<<"\n";
    cout<<"Input File Name is : ";
    string filename;
    cin>>filename;
    cout<<"\n";
    ifstream inFile(filename.c_str());
    
    
    cwnd= new int[S];  
    source_timer= new int[S];
    channel_state_node= new float[S];
    
    for(int i=0;i<S;i++)
        cwnd[i]= cwmin_slot; //contention window for every source is assigned to cwmin_slot
    for(int i=0;i<S;i++)
        source_timer[i]= 0;
    
    for(int i=0;i<S;i++)
        channel_state_node[i]= 0;
    
    
    
    
     if(!inFile)
    {
        cout << "File does not exist!";
        exit(0);
    }
      
    inFile>>row>>col;
    
    source3= new int[S];
    destination3= new int[S];
    packet3= new int[S];
    
     matrix = new double* [row]; 
     for (int i=0; i<row; i++) {
        matrix[i] = new double[col];
        for (int j=0; j<col; j++)
            matrix[i][j] = 0;
    }
   
    while(inFile)
    { 
   
         for(int i=0; i <row; i++)
         {
                for(int j=0; j<col; j++)
                {
                inFile >> matrix[i][j];
                }
        }
         
    }
    
      for(int i=0;i<S;i++)
     {
          source3[i]= matrix[i][0];
          destination3[i]=matrix[i][1]; 
          packet3[i]= matrix[i][2];
     }
     
     
     
    for(int i=0;i<S;i++)
     {
     cout<< "Source "<< source3[i]<<"\t"; 
     cout<< "Destination "<< destination3[i]<<"\t";
     cout<< "Packet "<< packet3[i]<<"\t";
     
     cout<<"\n";
     }
    
    for(int i=0;i<S;i++)
     {
        for(int j=0;j<col;j++)
        {
            cout<<matrix[i][j]<<"\t";
        }
        cout<<"\n";
    }
    
     if(RTS_Enable==0){
         RTS_time=0;
         CTS_time=0;
     }
     
      for(int i=0;i<S;i++)
     {
      total_no_frames+= packet3[i];
      }
     
     frame_generated=total_no_frames;
     for(int i=0;i<S;i++)
     {
      collision_state[i]=station_not_collide; 
     }
     
   
     for(int i=0;i<S;i++)
        station_state[i]=0; //assume station is in backoff state
    
    
  //  for(int k=0;k<5;k++) //SEED array loop for 5 simulations
    //{
    int *station_array;
    int *collision_count;
    station_array=new int[S];
    collision_count=new int[S];
   // int collision_attempt=0;
    
    for(int i=0;i<S;i++)
     station_array[i]=station_busy ; //assume station is idle   
    
    for(int i=0;i<S;i++)
      collision_count[i]=0 ; //assume station is idle   
    
    srand(SEED); //randomized seed
    
    int i,j;
    int count=0; //for keeping track of collision count
    
    for(int i=0;i<S;i++)
     backoff_slot[i]=(rand()%cwmin_slot); //backoff for a random amount of time
    
    //cout<<"\n"<<"Transmission Attempt "<<(attempt)<<"\n";
    
    cout<<"\n"<<"Back-off time of the stations are: ";
    for(int i=0;i<S;i++)
            cout<<backoff_slot[i]<<" ";
    
    for(int i=0;i<S;i++)
     backoff_time[i]=(backoff_slot[i])* slot_time; //calculate backoff_time from backoff_slot
    
     //backoff for a random amount of time
  
   
    cwmin_time = cwmin_slot * slot_time; //calculate contention_time from contention_slot
    
   // attempt++;
    channel_state= channel_state_good; //assume initial channel state is good
    
    
    
    while(total_no_frames>0)
    {   //check whether any station is there to transmit
        
        for(int l=0;l<S;l++)
          {
            if(packet3[l]>0)
            {
             channel_trans= float((rand()%10001)/float(10000)); //randomize channel state number
          
             channel_state_node[l]= channel_state_model(channel_trans, channel_state, Pgg, Pbb, channel_bad_count,channel_good_count);
            }
          // cout<<"\n"<<"\n"<<"Random Channel no is: "<<channel_trans;
          //cout<<"\n"<<"Channel State is: "<<channel_state;
               
          }
         
    
         cout<<"\n"<<"Back off Time of the stations are: ";
         out<<"\n"<<"Back off Time of the stations are: ";
        for(int i=0;i<S ;i++)
           {
            out<<backoff_slot[i]<<"\t";
           }
        
        
          count=0;        
    
          //Find the minimum backoff slot and its index
       
          min=1024;
          min_index=0;
    
          for (int i = 0; i < S; i++)
             {
                if ((backoff_slot[i] < min) && (packet3[i]> 0) && station_state[i]!=station_finish_transmit)
                {
                 min = backoff_slot[i];
                 min_index=i;
         
                 }
         
             }
    
           cout<<"\n"<<"Minimum Backoff Time is: "<<min<<"\n";
           out<<"\n"<<"Minimum Backoff Time is: "<<min<<"\n";
           //out<<"station"<<source3[min_index] <<"has the minimum backoff time"<<"\n";
    
           //check whether more than one stations have same backoff time and update collision status 
          //and count accordingly
          for(int i=0;i<S ;i++)
              collision_state[i]=0;
    
    
          for(int i=0;i<S;i++)
           {
             if(i!=min_index && packet3[i]!=0)
                 station_state[i]=0;
           }
    
          for(int i=0;i<S ;i++)
           {
                  
              if(topology=="Grid")
              {
                if((backoff_slot[i] == min) && (i!=min_index) && (packet3[i]>0) && (sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2))
                {
                        count++;
                        collision_state[i]=station_collide;
                        collision_state[min_index]=station_collide;  
                        out<<"station "<<source3[i]<<" has the same backoff time as station: "<<source3[min_index]<<"\n";
                        
                }
              }
              
              else if(topology=="String")
              {
                if((backoff_slot[i] == min) && (i!=min_index) && (packet3[i]>0) && (abs(source3[i]-source3[min_index])<=2))
                {
                        count++;
                        collision_state[i]=station_collide;
                        collision_state[min_index]=station_collide;   
                        out<<"station "<<source3[i]<<" has the same backoff time as station: "<<source3[min_index]<<"\n";
                }
              }
              
           }
    
            cout<<"No of station has same back-off time: "<<(count+1)<<"\n";
    
            //timer+=DIFS; //timer increased by DIFS before the stations can try to access the channel
    
          /*  for(int i=0;i<S;i++)
            {
            if(channel_state_node[i]==channel_state_bad && (packet3[i]>0))
            {
                //if channel state is bad, make this station channel state bad, and make the nodes
                //channel state within its 2 hop distance also bad, and set all the station to collision state
                
                //channel_state_node[min_index]=channel_state_bad;
                collision_state[i]=station_collide;
   
            }
            }*/
    
            channel_state= channel_state_node[min_index];
            
            channel_state_increment(channel_state,channel_good_count,channel_bad_count);
              
            
            
            
       if(count>0){
                   
           //collision occurs, change the contention window
           
           out<<"Station "<<source3[min_index]<<"'s RTS frame (RTS enabled) or Data frame (RTS Disabled) face collision"<<"\n";
           
         for(int i=0;i<S;i++)
         {
             if(RTS_Enable==1)
             {
                 //out<<"Station "<<source3[min_index]<<"'s RTS frame face collision"<<"\n";
                
                 if(topology=="Grid")
                {
                 if((collision_state[i]==station_collide) && (packet3[i]>0) && (collision_count[i]<7) && station_state[i]!=station_finish_transmit)  //change the backoff time of the colliding stations
                {                                
                  contention_window(cwnd,i);
                  collision_count[i] = collision_count[i] +1;
                 
                  //source_timer[i]=source_timer[i]+DIFS+ RTS_time+(min*slot_time);
                } 
                
                if((packet3[i]>0) && (collision_count[i]<7) && station_state[i]!=station_finish_transmit && (sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2))  //change the backoff time of the colliding stations
                {                                
                  
                 
                  source_timer[i]=source_timer[i]+DIFS+ RTS_time+(min*slot_time);
                  backoff_slot[i]= backoff_slot[i]-min;
                } 
                }
                 
                 
                 else if(topology=="String")
                {
                 if((collision_state[i]==station_collide) && (packet3[i]>0) && (collision_count[i]<7) && station_state[i]!=station_finish_transmit)  //change the backoff time of the colliding stations
                {                                
                  contention_window(cwnd,i);
                  collision_count[i] = collision_count[i] +1;
                 
                  //source_timer[i]=source_timer[i]+DIFS+ RTS_time+(min*slot_time);
                } 
                
                if((packet3[i]>0) && (collision_count[i]<7) && station_state[i]!=station_finish_transmit && (abs(source3[i]-source3[min_index])<=2))  //change the backoff time of the colliding stations
                {                                
                  
                 
                  source_timer[i]=source_timer[i]+DIFS+ RTS_time+(min*slot_time);
                  backoff_slot[i]= backoff_slot[i]-min;
                } 
                } 
                
                
             }
             
             else if(RTS_Enable==0)
             {
                 //out<<"Station "<<source3[min_index]<<"'s Data frame face collision"<<"\n";
               
                if(topology=="Grid")
                {
                if((collision_state[i]==station_collide) && (packet3[i]>0) && (collision_count[i]<4) && station_state[i]!=station_finish_transmit)  //change the backoff time of the colliding stations
                {                              
                 contention_window(cwnd,i);
                 collision_count[i] = collision_count[i] +1;
                 //source_timer[i]=source_timer[i]+ DIFS+ RTS_time+(min*slot_time);
                                  
                } 
                
                 if((packet3[i]>0) && (collision_count[i]<4) && station_state[i]!=station_finish_transmit && (sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2))  //change the backoff time of the colliding stations
                {                                  
                  source_timer[i]=source_timer[i]+DIFS+frame_time+(min*slot_time);
                  backoff_slot[i]= backoff_slot[i]-min;
                } 
                
                }
                
                else if(topology=="String")
                {
                if((collision_state[i]==station_collide) && (packet3[i]>0) && (collision_count[i]<4) && station_state[i]!=station_finish_transmit)  //change the backoff time of the colliding stations
                {                              
                 contention_window(cwnd,i);
                 collision_count[i] = collision_count[i] +1;
                 //source_timer[i]=source_timer[i]+ DIFS+ RTS_time+(min*slot_time);
                                  
                } 
                
                 if((packet3[i]>0) && (collision_count[i]<4) && station_state[i]!=station_finish_transmit && (abs(source3[i]-source3[min_index])<=2))  //change the backoff time of the colliding stations
                {                                  
                  source_timer[i]=source_timer[i]+DIFS+frame_time+(min*slot_time);
                  backoff_slot[i]= backoff_slot[i]-min;
                } 
                
                }
                
             }
             
             
             
         }
         
         //timer+=frame_time; //timer increased by frame_time after the collision occurs
         
         for(int i=0;i<S;i++)
         {
           if(RTS_Enable==1)
           {
             //collision_count[i] = collision_count[i] +1;
             if((collision_state[i]==station_collide) && (packet3[i]>0) && (collision_count[i]<7) && station_state[i]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {
                backoff_slot[i]=(rand()% (cwnd[i])); //backoff for a random amount of time
                backoff_time[i]=(backoff_slot[i])* slot_time; //calculate backoff_time from backoff_slot
                station_state[i]=1;
                //collision_count[i] = collision_count[i] +1;
                trans_attempt++;
                trans_collision++;
                out<<"Collision count of station "<<source3[i]<<"for this frame is: "<<collision_count[i]<<"\n";
             }
             if ((collision_state[i]==station_collide) && (collision_count[i]>=7) && packet3[i]>0 && station_state[i]!=station_finish_transmit) 
                 
             {
                // station_state[i]=3;
                //no_of_station--;
                // cout<<"\n"<<"station "<<(i+1)<< " has exceed the maximum transmission attempt"<<"\n";
                out<<"Collision count of station "<<source3[i]<<" for this frame reached maximum collision count threshold of 7 and so is discarded "<<"\n";
                 trans_discarded++;
                 collision_count[i]=0;
                 trans_attempt++;
                 packet3[i]=packet3[i]-1;
                 
                 cwnd[i]=cwmin_slot;
                 backoff_slot[i]=(rand()% (cwnd[i]));
                 //trans_attempt++;
                
                 total_no_frames--;
                 
                  //also have to back off??
              //  backoff_slot[i]=(rand()%cwnd[i]);
                 
             }
           }
             
          else if(RTS_Enable==0)
             {
              collision_count[i] = collision_count[i] +1;
             if((collision_state[i]==station_collide) && (packet3[i]>0) && (collision_count[i]<4) && station_state[i]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {
                backoff_slot[i]=(rand()% (cwnd[i])); //backoff for a random amount of time
                backoff_time[i]=(backoff_slot[i])* slot_time; //calculate backoff_time from backoff_slot
                station_state[i]=1;
                //collision_count[i] = collision_count[i] +1;
                trans_attempt++;
                trans_collision++;
                out<<"Collision count of station "<<source3[i]<<"for this frame is: "<<collision_count[i]<<"\n";
             }
             if ((collision_state[i]==station_collide) && (collision_count[i]>=4) && packet3[i]>0 && station_state[i]!=station_finish_transmit) 
                 
             {
                 //station_state[i]=3;
                 //no_of_station--;
                 //cout<<"\n"<<"station "<<(i+1)<< " has exceed the maximum transmission attempt"<<"\n";
                // if(packet3[i]>0)
                 //{
                 out<<"Collision count of station "<<source3[i]<<" for this frame reached maximum collision count threshold of 4 and so is discarded "<<"\n";
                 trans_discarded++;
                 collision_count[i]=0;
                 trans_attempt++;
                 packet3[i]=packet3[i]-1;
                 cwnd[i]=cwmin_slot;
                 backoff_slot[i]=(rand()% (cwnd[i]));
                 total_no_frames--;
                 
              //   }
                 
                  //also have to back off??
               // backoff_slot[i]=(rand()%cwnd[i]);
                 
             }
             }
                
         }
        
      /*   cout<<"\n"<<"New Back-off time of the stations are: ";
         out<<"\n"<<"New Back-off time of the stations are: "<<"\n";
         for(int i=0;i<S;i++)
             cout<<backoff_slot[i]<<" ";*/
         
         //trans_collision++;
         
        
         
     }
    
    else if(channel_state== channel_state_bad)
    {
        //cout<<"Station "<<source3[min_index]<<"'s RTS frame face bad channel state"<<"\n";
       
        
        //only change the backoff slot of min_index station which faces bad channel state
        //other stations timer increased, and backoff slot decreased by min
     
       // for(int i=0;i<S;i++)
         //{
          if(RTS_Enable==1)
          {
               out<<"Station "<<source3[min_index]<<"'s RTS frame face bad channel state"<<"\n";
              collision_count[min_index] = collision_count[min_index] +1;
             if((packet3[min_index]>0) && (collision_count[min_index]<7) && station_state[min_index]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {              
                  contention_window(cwnd,min_index);
                  source_timer[min_index]=source_timer[min_index]+ DIFS+ RTS_time+(min*slot_time);
                  backoff_slot[min_index]=(rand()% (cwnd[min_index])); //backoff for a random amount of time
                  backoff_time[min_index]=(backoff_slot[min_index])* slot_time; //calculate backoff_time from backoff_slot
                  station_state[min_index]=1;
                  // collision_count[min_index] = collision_count[min_index] +1;
                  trans_attempt++;
                  trans_collision++;
                  out<<"Collision count of station "<<source3[min_index]<<" for this frame is: "<<collision_count[min_index]<<"\n";
             }
             
             if((packet3[min_index]>0) && (collision_count[min_index]>=7) && station_state[min_index]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {
                 out<<"Collision count of station "<<source3[min_index]<<" for this frame reached maximum collision count threshold of 7 and so is discarded "<<"\n";
                 trans_discarded++;
                 collision_count[min_index]=0;
                 trans_attempt++;
                 packet3[min_index]=packet3[min_index]-1;
                 cwnd[min_index]=cwmin_slot;
                 backoff_slot[min_index]= rand()%cwnd[min_index];
                 total_no_frames--;
             }
             
             
          }
             
        else if(RTS_Enable==0)
          {
             out<<"Station "<<source3[min_index]<<"'s Data frame face bad channel state"<<"\n";
            collision_count[min_index] = collision_count[min_index] +1;
             if((packet3[min_index]>0) && (collision_count[min_index]<4) && station_state[min_index]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {              
                  contention_window(cwnd,min_index);
                  source_timer[min_index]=source_timer[min_index]+ DIFS+frame_time+(min*slot_time);
                  //contention_window(&cwnd[min_index],min_index);
                 // source_timer[min_index]=source_timer[min_index]+ DIFS+ RTS_time+(min*slot_time);
                  backoff_slot[min_index]=(rand()% (cwnd[min_index])); //backoff for a random amount of time
                  backoff_time[min_index]=(backoff_slot[min_index])* slot_time; //calculate backoff_time from backoff_slot
                  station_state[min_index]=1;
                 // collision_count[min_index] = collision_count[min_index] +1;
                  trans_attempt++;
                  trans_collision++;
                  out<<"Collision count of station "<<source3[min_index]<<" for this frame is: "<<collision_count[min_index]<<"\n";
             }
             
             if((packet3[min_index]>0) && (collision_count[min_index]>=4) && station_state[min_index]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {
                 out<<"Collision count of station "<<source3[min_index]<<" for this frame reached maximum collision count threshold of 4 and so is discarded "<<"\n";
                 trans_discarded++;
                 collision_count[min_index]=0;
                 trans_attempt++;
                 packet3[min_index]=packet3[min_index]-1;
                 cwnd[min_index]=cwmin_slot;
                 backoff_slot[min_index]= rand()%cwnd[min_index];
                 total_no_frames--;
             }
          }
             
            for(int i=0;i<S;i++)
          {
           // for(j=0;j<Grid_size*Grid_size && j!=i;j++)
            //{
              if(topology=="Grid")
              {
                  
           if(RTS_Enable==1)
           {
            if(station_state[i]!=station_finish_transmit && packet3[i]>0 && i!=min_index && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2)))
            {
                //increase the timer and decrease the backoff slot of all the stations within the
                //2 hot distance of source station
                
                source_timer[i]= source_timer[i]+(min*slot_time)+DIFS+RTS_time; //from the 802.11 slides
                  
                backoff_slot[i]= backoff_slot[i]-min;
                //what would be the back slot of the station that has successfully sent
                
                out<<"Stations "<<source3[i]<< " are within the 2 hop distance of station "<<source3[min_index]<<"\n";
            }
           }
            
           
           else if(RTS_Enable==0)
           {
            if(station_state[i]!=station_finish_transmit && packet3[i]>0 && i!=min_index && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2)))
            {
                //increase the timer and decrease the backoff slot of all the stations within the
                //2 hot distance of source station
                
                source_timer[i]= source_timer[i]+(min*slot_time)+DIFS+frame_time; //from the 802.11 slides
                  
                backoff_slot[i]= backoff_slot[i]-min;
                //what would be the back slot of the station that has successfully sent
                
                out<<"Stations "<<source3[i]<< " are within the 2 hop distance of station "<<source3[min_index]<<"\n";
            }
           }
            
           }
              
              else if(topology=="String")
              {
                  
               if(RTS_Enable==1)
               {
            if(station_state[i]!=station_finish_transmit && packet3[i]>0 && i!=min_index && (abs(source3[i]-source3[min_index])<=2))
            {
                //increase the timer and decrease the backoff slot of all the stations within the
                //2 hot distance of source station
                
                source_timer[i]= source_timer[i]+(min*slot_time)+DIFS+RTS_time; //from the 802.11 slides
                  
                backoff_slot[i]= backoff_slot[i]-min;
                //what would be the back slot of the station that has successfully sent
                
                out<<"Stations "<<source3[i]<< " are within the 2 hop distance of station "<<source3[min_index]<<"\n";
            }
           }
               else if(RTS_Enable==0)
               {
            if(station_state[i]!=station_finish_transmit && packet3[i]>0 && i!=min_index && (abs(source3[i]-source3[min_index])<=2))
            {
                //increase the timer and decrease the backoff slot of all the stations within the
                //2 hot distance of source station
                
                source_timer[i]= source_timer[i]+(min*slot_time)+DIFS+frame_time; //from the 802.11 slides
                  
                backoff_slot[i]= backoff_slot[i]-min;
                //what would be the back slot of the station that has successfully sent
                
                out<<"Stations "<<source3[i]<< " are within the 2 hop distance of station "<<source3[min_index]<<"\n";
            }
           }   
               
           }
           // }
          }
          
            
             
             
             //Ack and CTS frame can also face bad channel state; need to handle it
          
        //}
        
       // srand(SEED);
        //timer+=frame_time; //timer increased by frame_time after the collision occurs
     /*   for(int i=0;i<S;i++)
         {
            if(RTS_Enable==1)
           {
             if((collision_state[i]==station_collide) && (packet3[i]>0) && (collision_count[i]<8)&& station_state[i]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {
                backoff_slot[i]=(rand()% (cwnd[i])); //backoff for a random amount of time
                backoff_time[i]=(backoff_slot[i])* slot_time; //calculate backoff_time from backoff_slot
                station_state[i]=1;
                collision_count[i] = collision_count[i] +1;
                trans_attempt++;
                trans_collision++;
                out<<"Collision count of station "<<source3[i]<<"for this frame is: "<<collision_count[i]<<"\n";
             }
             if ((collision_state[i]==station_collide) && (collision_count[i]>=8) && (packet3[i]>0)&& station_state[i]!=station_finish_transmit) 
                 
             {
                 //station_state[i]=3;
                 //no_of_station--;
                 //cout<<"\n"<<"station "<<(i+1)<< " has exceed the maximum transmission attempt"<<"\n";
                // if(packet3[i]>0)
                 //{
                 trans_discarded++;
                 collision_count[i]=0;
                 trans_attempt++;
                 packet3[i]=packet3[i]-1;
                 cwnd[i]=cwmin_slot;
                 backoff_slot[i]= rand()%cwnd[i];
                 total_no_frames--;
                // }
                 
                 //also have to back off??
               // backoff_slot[i]=(rand()%cwnd[i]);
                 
             }
            }
            
            if(RTS_Enable==0)
           {
             if((collision_state[i]==station_collide) && (packet3[i]>0) && (collision_count[i]<5) && station_state[i]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {
                backoff_slot[i]=(rand()% (cwnd[i])); //backoff for a random amount of time
                backoff_time[i]=(backoff_slot[i])* slot_time; //calculate backoff_time from backoff_slot
                station_state[i]=1;
                collision_count[i] = collision_count[i] +1;
                trans_attempt++;
                trans_collision++;
                out<<(source3[i])<<" Collision count for this frame is: "<<collision_count[i]<<"\n";
             }
             if ((collision_state[i]==station_collide) && (collision_count[i]>=4) && (packet3[i]>0) && station_state[i]!=station_finish_transmit) 
                 
             {
                 //station_state[i]=3;
                 //no_of_station--;
                 //cout<<"\n"<<"station "<<(i+1)<< " has exceed the maximum transmission attempt"<<"\n";
                 
                 trans_discarded++;
                 collision_count[i]=0;
                 out<<"\n"<<"station "<<(source3[i])<< " has exceed the maximum transmission attempt for this frame"<<"\n";
                 trans_attempt++;
                 packet3[i]=packet3[i]-1;
                 
                 cwnd[i]=cwmin_slot;
                 backoff_slot[i]= rand()%cwnd[i];
                 total_no_frames--;
         
                  //also have to back off??
                //backoff_slot[i]=(rand()%cwnd[i]);
                 
             }
            }
                
         }*/
    
       
        
       // trans_collision++;
         
        
    }
    
     else
     {
          //station has to wait DIFS amount of time after channel becomes idle
        
         //channel_state=channel_busy; 
         
         //this channel randomization is for to check whether CTS frame faces bad channel state
        
        if(RTS_Enable==1)
        {
        out<<"Station "<<source3[min_index]<<"'s RTS frame doesn't face collision"<<"\n";
        
        for(int l=0;l<S;l++)
          {
          if(packet3[l]>0)
          {
          channel_trans= float((rand()%10001)/float(10000)); //randomize channel state number
          
          channel_state_node[l]= channel_state_model(channel_trans, channel_state, Pgg, Pbb, channel_bad_count,channel_good_count);
          }
          // cout<<"\n"<<"\n"<<"Random Channel no is: "<<channel_trans;
          //cout<<"\n"<<"Channel State is: "<<channel_state;
               
          }
        channel_state=channel_state_node[min_index];
        channel_state_increment(channel_state,channel_good_count,channel_bad_count);
        
         //channel_trans= float((rand()%10001)/float(10000)); //randomize channel state number
          
         //channel_state_model(channel_trans, channel_state, Pgg, Pbb, channel_bad_count,channel_good_count);
         
         if(channel_state!=channel_state_bad)
         {
             out<<"Station "<<source3[min_index]<<"'s CTS frame doesn't face Bad Channel state"<<"\n";
             //this channel randomization is for to check whether ACK frame faces bad channel state
            // channel_trans= float((rand()%10001)/float(10000)); //randomize channel state number
          
            // channel_state_model(channel_trans, channel_state, Pgg, Pbb, channel_bad_count,channel_good_count);
         
          for(int l=0;l<S;l++)
          {
            if(packet3[l]>0)
                 {
            channel_trans= float((rand()%10001)/float(10000)); //randomize channel state number
          
             channel_state_node[l]= channel_state_model(channel_trans, channel_state, Pgg, Pbb, channel_bad_count,channel_good_count);
                 }
          // cout<<"\n"<<"\n"<<"Random Channel no is: "<<channel_trans;
          //cout<<"\n"<<"Channel State is: "<<channel_state;
               
            }
        channel_state=channel_state_node[min_index];
        channel_state_increment(channel_state,channel_good_count,channel_bad_count);
        
             
        if(channel_state!=channel_state_bad)
             {
            
            out<<"Station "<<source3[min_index]<<"'s ACK frame doesn't face Bad Channel state"<<"\n";
             
           // if(packet3[min_index]==0)
           //  station_state[min_index]=station_finish_transmit; //station with the min backoff slot will
                                                           //transmit
            //what about SIFS time
          if(packet3[min_index]>0)
          {
           source_timer[min_index]=source_timer[min_index]+ (min*slot_time)+DIFS+ RTS_time+CTS_time+(3*SIFS)+frame_time+Ack_time;
          
           cwnd[min_index]=cwmin_slot;
           backoff_slot[min_index]= rand()%cwnd[min_index];
            
            packet3[min_index]=packet3[min_index]-1;
            trans_attempt++;
            trans_success++;
            total_no_frames--;
            collision_count[min_index]=0;
            cout<<"Station "<<source3[min_index]<<" successfully transmit a frame at  "<<source_timer[min_index]<<" microseconds"<<"/"<<(source_timer[min_index]/slot_time)<<" slot time"<<"\n";
            out<<"Station "<<source3[min_index]<<" successfully transmit a frame at  "<<source_timer[min_index]<<" microseconds"<<"/"<<(source_timer[min_index]/slot_time)<<" slot time"<<"\n";    
          }
            
          
         ////Ack and CTS frame can also face bad channel state; need to handle it
         
          for(int i=0;i<S;i++)
          {
           // for(j=0;j<Grid_size*Grid_size && j!=i;j++)
            //{
              if(topology=="Grid")
              {
            if(station_state[i]!=station_finish_transmit && packet3[i]>0 && i!=min_index && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2)))
            {
                //increase the timer and decrease the backoff slot of all the stations within the
                //2 hot distance of source station
                
                source_timer[i]= source_timer[i]+(min*slot_time)+DIFS+CTS_time+(3*SIFS)+frame_time+Ack_time; //from the 802.11 slides
                  
                backoff_slot[i]= backoff_slot[i]-min;
                //what would be the back slot of the station that has successfully sent
                
                out<<"Stations "<<source3[i]<< "are within the 2 hop distance of station "<<source3[min_index]<<"\n";
            }
           }
              
              else if(topology=="String")
              {
            if(station_state[i]!=station_finish_transmit && packet3[i]>0 && i!=min_index && (abs(source3[i]-source3[min_index])<=2))
            {
                //increase the timer and decrease the backoff slot of all the stations within the
                //2 hot distance of source station
                
                source_timer[i]= source_timer[i]+(min*slot_time)+DIFS+CTS_time+(3*SIFS)+frame_time+Ack_time; //from the 802.11 slides
                  
                backoff_slot[i]= backoff_slot[i]-min;
                //what would be the back slot of the station that has successfully sent
                
                out<<"Stations "<<source3[i]<< "are within the 2 hop distance of station "<<source3[min_index]<<"\n";
            }
           }
           // }
          }
          
          
           
         
         
       
          
         cout<<"Back-off time of the stations are: "<<"n";
         out<<"Back-off time of the stations are: "<<"n";
         for(int i=0;i<S;i++)
             cout<<backoff_slot[i]<<" ";
         
        // timer= timer+ frame_time; //timer increased by frame_time after the successful transmission
         
         
          
         }
        
        else if (channel_state==channel_state_bad)
         {
              //this is for ACK frame channel BAD state
             //ACK frame will not face collision, will only face bad channel state
             
             
             
         if(RTS_Enable==1)
         {
             cout<<"Bad Channel State for ACK Frame"<<"\n";
             out<<"Station "<<source3[min_index]<<"'s ACK frame face bad channel state"<<"\n"; 
             
             collision_count[min_index] = collision_count[min_index] +1;
             if((packet3[min_index]>0) && (collision_count[min_index]<7) && station_state[min_index]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {
                 //ACK frame is sent for data frame only, so maximum of 4 retransmission for data frame?
                 //have doubt with it, as we are only considering bad channel state, and we are using same collision_count array
                 //we need to check for 7 max retransmission
                
                contention_window(cwnd,min_index);
                source_timer[min_index]=source_timer[min_index]+ (min*slot_time)+DIFS+ RTS_time+CTS_time+frame_time+Ack_time+(3*SIFS);
                // collision_count[min_index] = collision_count[min_index] +1;
                backoff_slot[min_index]=(rand()%cwnd[min_index]); //backoff for a random amount of time
                backoff_time[min_index]=(backoff_slot[min_index])* slot_time; //calculate backoff_time from backoff_slot
                station_state[min_index]=1;
                trans_attempt++;
                trans_collision++;
               out<<"Station "<<(source3[min_index])<<" Collision count for this frame is: "<<collision_count[min_index]<<"\n";;
                              
              } 
             
             
              
             /*  if((packet3[min_index]!=0) && (collision_count[min_index]<5))  //change the backoff time of the colliding stations
             {
                backoff_slot[min_index]=(rand()%cwnd[min_index]); //backoff for a random amount of time
                backoff_time[min_index]=(backoff_slot[min_index])* slot_time; //calculate backoff_time from backoff_slot
                station_state[min_index]=1;
                collision_count[min_index] = collision_count[min_index] +1;
             }*/
              else if ((collision_count[min_index]>=7) && (packet3[min_index]>0) && station_state[min_index]!=station_finish_transmit) 
                 
                {
                 //station_state[min_index]=3;
                 //no_of_station--;
                 cout<<"\n"<<"station "<<(source3[min_index])<< " has exceed the maximum frame transmission attempt for this frame"<<"\n";
                 //out<<"\n"<<"station "<<(source3[min_index])<< " has exceed the maximum frame transmission attempt for this frame"<<"\n";
                 out<<"Collision count of station "<<source3[min_index]<<" for this frame reached maximum collision count threshold of 7 and so is discarded "<<"\n";
                 trans_discarded++;
                 collision_count[min_index]=0;
                 trans_attempt++;
                 
                 //if(packet3[min_index]>0)
                 //{
                 packet3[min_index]=packet3[min_index]-1;
                 //}
                 //else
                   //  station_state[min_index]=station_finish_transmit;
                 
                 cwnd[min_index]=cwmin_slot;
                 backoff_slot[min_index]= rand()%cwnd[min_index];
                 total_no_frames--;
                 
                 
                 //do the station needs to back off, after ACK frame collision: i think Yes it needs to backoff
                 
                // contention_window(&cwnd[min_index],min_index);
               //  source_timer[min_index]=source_timer[min_index]+ DIFS+ RTS_time+CTS_time;
                 
                 //backoff_slot[min_index]=(rand()%cwnd[min_index]);
                    //backoff_slot[min_index]=(rand()%cwnd[min_index]); //contention window is doubled before
                 
                 
                 
                }
               
             //if ACK frame faces bad channel state (when RTS is enabled), then increase the timer, and backoff
             
       /*  for(int i=0;i<S;i++)
          { 
             if((packet3[i]!=0) && i!=min_index && (collision_count[i]<5) && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2)))  //change the backoff time of the colliding stations
             {
                contention_window(&cwnd[i],i);
                backoff_slot[i]=(rand()%cwnd[i]); //backoff for a random amount of time
                backoff_time[i]=(backoff_slot[i])* slot_time; //calculate backoff_time from backoff_slot
                station_state[i]=1;
                collision_count[i] = collision_count[i] +1;
                source_timer[i]=source_timer[i]+DIFS+CTS_time+frame_time+Ack_time+(3*SIFS);
             }
              else if ((collision_count[i]>4) && i!=min_index && (packet3[i]!=0) && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2))) 
                 
             {
                 //station_state[min_index]=3;
                 //no_of_station--;
                 cout<<"\n"<<"station "<<(source3[min_index])<< " has exceed the maximum transmission attempt"<<"\n";
                 trans_discarded++;
                 packet3[min_index]=packet3[min_index]-1;
                 source_timer[i]=source_timer[i]+DIFS+CTS_time+frame_time+Ack_time+(3*SIFS);
                 
                 backoff_slot[i]= backoff_slot[i]-backoff_slot[min_index];
                 
                 //back off due to frame failure
                // contention_window(&cwnd[i],i);
                //backoff_slot[i]=(rand()%cwnd[i]);
             }
           } */
             
             
             }
             
         /*    else if(RTS_Enable==0)
             {
                 ////this is for ACK frame channel BAD state
                 //if RTS is disabled, ACK frame will not face bad channel state nor collision
                 
                 out<<"Station "<<source3[min_index]<<"'s ACK frame don't face bad channel state as RTS is disabled"<<"\n";
          
                 if(packet3[min_index]>0 && station_state[min_index]!=station_finish_transmit)
                 {
                 source_timer[min_index]=source_timer[min_index]+ (min*slot_time) +DIFS+ RTS_time+CTS_time+(3*SIFS)+frame_time+Ack_time;
                 cwnd[min_index]=cwmin_slot;
                 backoff_slot[min_index]= rand()%cwnd[min_index];                 
                 packet3[min_index]=packet3[min_index]-1;
                 trans_success++;
                 collision_count[min_index]=0;
                 trans_attempt++;
                 
                 }
                // else
                     
                 
                 cout<<"Station "<<source3[min_index]<<" successfully transmit a frame at  "<<source_timer[min_index]<<" microseconds"<<"/"<<(source_timer[min_index]/slot_time)<<" slot time"<<"\n";
                 out<<"Station "<<source3[min_index]<<" successfully transmit a frame at  "<<source_timer[min_index]<<" microseconds"<<"/"<<(source_timer[min_index]/slot_time)<<" slot time"<<"\n";

                 total_no_frames--;
               */  
                 
                for(int i=0;i<S;i++)
                {
                    if(topology=="Grid")
                    {
                        if(station_state[i]!=station_finish_transmit && packet3[i]>0 && i!=min_index && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2)))
                        {
            // ACK frame will not face collision when RTS is disabled
                            //increase the timer and decrease the backoff slot of all the stations within the
                //2 hot distance of source station
                
                        source_timer[i]= source_timer[i]+(min*slot_time)+DIFS+CTS_time+(3*SIFS)+frame_time+Ack_time;
                        backoff_slot[i]= backoff_slot[i]-min;
                        }
                    }
                    
                    else if(topology=="String")
                    {
                        if(station_state[i]!=station_finish_transmit && packet3[i]>0 && i!=min_index && (abs(source3[i]-source3[min_index])<=2))
                        {
            // ACK frame will not face collision when RTS is disabled
                            //increase the timer and decrease the backoff slot of all the stations within the
                //2 hot distance of source station
                
                        source_timer[i]= source_timer[i]+(min*slot_time)+DIFS+CTS_time+(3*SIFS)+frame_time+Ack_time;
                        backoff_slot[i]= backoff_slot[i]-min;
                        }
                    }
            
                }
                
         
             
              
              
         } 
        
   }
             
    else if (channel_state==channel_state_bad)
   { 
             
             
             //what should we do, when CTS frame collided
             
             //this is for CTS frame channel BAD state
             //for the min_index, increase the timer, collision count; if collision count 
             //is less than RTS threshold then backoff, or else discrad this RTS frame
          
        out<<"Station "<<source3[min_index]<<"'s CTS frame face Bad Channel state"<<"\n";
              
        if(RTS_Enable==1)
        {
              collision_count[min_index] = collision_count[min_index] +1;
              if((packet3[min_index] > 0) && (collision_count[min_index]<7) && station_state[min_index]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {
               source_timer[min_index]=source_timer[min_index]+(min*slot_time)+ DIFS+ RTS_time+CTS_time+(1*SIFS);
               channel_state_node[min_index]= channel_state_bad;
              // collision_count[min_index] = collision_count[min_index] +1;
             
                contention_window(cwnd,min_index);//if collision count is less than maximum, then
                              //double the contention window, and back off again
                backoff_slot[min_index]=(rand()%cwnd[min_index]); //backoff for a random amount of time
                backoff_time[min_index]=(backoff_slot[min_index])* slot_time; //calculate backoff_time from backoff_slot
                station_state[min_index]=1;
                trans_attempt++;
                trans_collision++;
                out<<"Station" <<(source3[min_index])<<" Collision count for this frame is: "<<collision_count[min_index]<<"\n";
               
             }
              else if(( packet3[min_index]>0) && (collision_count[min_index]>=7) && station_state[min_index]!=station_finish_transmit)      
             {
                 //station_state[min_index]=3;
                 //no_of_station--;
                 cout<<"\n"<<"station "<<(source3[min_index])<< " has exceed the maximum transmission attempt for this frame"<<"\n";
                 //out<<"\n"<<"station "<<(source3[min_index])<< " has exceed the maximum transmission attempt for this frame"<<"\n";
                 out<<"Collision count of station "<<source3[min_index]<<" for this frame reached maximum collision count threshold of 7 and so is discarded "<<"\n";
                 trans_discarded++;
                 collision_count[min_index]=0;
                 trans_attempt++;
                 packet3[min_index]=packet3[min_index]-1;
                 
                 cwnd[min_index]=cwmin_slot;
                 backoff_slot[min_index]= rand()%cwnd[min_index];
                 total_no_frames--;
                 
                 //do this min_index station needs to backoff for other frames also
                 
               // backoff_slot[min_index]=(rand()%cwnd[min_index]);
                 
             }
        }
        
       /* if(RTS_Enable==0)
        {
              collision_count[min_index] = collision_count[min_index] +1;
              if((packet3[min_index] > 0) && (collision_count[min_index]<4) && station_state[min_index]!=station_finish_transmit)  //change the backoff time of the colliding stations
             {
               source_timer[min_index]=source_timer[min_index]+(min*slot_time)+ DIFS+ RTS_time+CTS_time+(1*SIFS);
               channel_state_node[min_index]= channel_state_bad;
              // collision_count[min_index] = collision_count[min_index] +1;
             
                contention_window(cwnd,min_index);//if collision count is less than maximum, then
                              //double the contention window, and back off again
                backoff_slot[min_index]=(rand()%cwnd[min_index]); //backoff for a random amount of time
                backoff_time[min_index]=(backoff_slot[min_index])* slot_time; //calculate backoff_time from backoff_slot
                station_state[min_index]=1;
                trans_attempt++;
                trans_collision++;
                out<<"Station" <<(source3[min_index])<<" Collision count for this frame is: "<<collision_count[min_index]<<"\n";
               
             }
              else if(( packet3[min_index]>0) && (collision_count[min_index]>=4) && station_state[min_index]!=station_finish_transmit)      
             {
                 //station_state[min_index]=3;
                 //no_of_station--;
                 cout<<"\n"<<"station "<<(source3[min_index])<< " has exceed the maximum transmission attempt for this frame"<<"\n";
                 //out<<"\n"<<"station "<<(source3[min_index])<< " has exceed the maximum transmission attempt for this frame"<<"\n";
                 out<<"Collision count of station "<<source3[min_index]<<" for this frame reached maximum collision count threshold of 7 and so is discarded "<<"\n";
                 trans_discarded++;
                 collision_count[min_index]=0;
                 trans_attempt++;
                 packet3[min_index]=packet3[min_index]-1;
                 
                 cwnd[min_index]=cwmin_slot;
                 backoff_slot[min_index]= rand()%cwnd[min_index];
                 total_no_frames--;
                 
                 //do this min_index station needs to backoff for other frames also
                 
               // backoff_slot[min_index]=(rand()%cwnd[min_index]);
                 
             }
        }*/
        
             
             //CTS frame will not face collision
              
            for(int i=0;i<S;i++)
          {
                ////for other station within two hop_distance of min_index station, increase the timer, collision count; if collision count 
             //is less than RTS threshold then backoff, or else discrad this RTS frame
               //their back_off slot has to decreased, as they are waiting for that amount of time
                
           if(topology=="Grid")
           {        
              if(RTS_Enable==1)
             {
                if(station_state[i]!=station_finish_transmit && (packet3[i]>0) && i!=min_index && (collision_count[i]<7) && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2)))  //change the backoff time of the colliding stations
                {              
                 // contention_window(&cwnd[i],i);
                  source_timer[i]=source_timer[i]+ (min*slot_time)+DIFS+ RTS_time+CTS_time+(1*SIFS);
                  backoff_slot[i]= backoff_slot[i]-min;
                 
                 
                } 
                
                //when min_index node faces bad channel state, what will happen to other station within two hop distance
                //as backoff_slot is not same for those stations, we don't need to change contention window
             
             }
             
           /*  else if(RTS_Enable==0)
             {
                 //this is for CTS frame channel BAD state
             if(station_state[i]!=station_finish_transmit && (packet3[i]>0) && i!=min_index && (collision_count[i]<4) && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2)))  //change the backoff time of the colliding stations
             {              
                  //contention_window(&cwnd[i],i);
                  source_timer[i]=source_timer[i]+ (min*slot_time)+DIFS+ RTS_time+CTS_time+(1*SIFS);
                  backoff_slot[i]= backoff_slot[i]-min;
                                  
              } 
             }*/
           }
           
           else if(topology=="String")
           {        
              if(RTS_Enable==1)
             {
                if(station_state[i]!=station_finish_transmit && (packet3[i]>0) && i!=min_index && (collision_count[i]<7) && (abs(source3[i]-source3[min_index])<=2))  //change the backoff time of the colliding stations
                {              
                 // contention_window(&cwnd[i],i);
                  source_timer[i]=source_timer[i]+(min*slot_time)+ DIFS+ RTS_time+CTS_time+(1*SIFS);
                  backoff_slot[i]= backoff_slot[i]-min;
                 
                 
                } 
             
             
             }
             
         /*    else if(RTS_Enable==0)
             {
                 //this is for CTS frame channel BAD state
             if((station_state[i]!=station_finish_transmit && packet3[i]>0) && i!=min_index && (collision_count[i]<4) && (abs(source3[i]-source3[min_index])<=2))  //change the backoff time of the colliding stations
             {              
                  //contention_window(&cwnd[i],i);
                  source_timer[i]=source_timer[i]+ (min*slot_time)+DIFS+ RTS_time+CTS_time+(1*SIFS);
                  backoff_slot[i]= backoff_slot[i]-min;
                                  
              } 
             }*/
           }
              
        }  
            //other stations within 2 hop of source station need to back off
          //CTS frame will not face collision
         /*  for(int i=0;i<S;i++)
          { 
             if((packet3[i]!=0) && i!=min_index && (collision_count[i]<8) && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2)))  //change the backoff time of the colliding stations
             {
                backoff_slot[i]=(rand()%cwnd[i]); //backoff for a random amount of time
                backoff_time[i]=(backoff_slot[i])* slot_time; //calculate backoff_time from backoff_slot
                station_state[i]=1;
                collision_count[i] = collision_count[i] +1;
             }
              else if ((collision_count[i]>7) && i!=min_index && (packet3[i]!=0) && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2))) 
                 
             {
                 
                 cout<<"\n"<<"station "<<(source3[min_index])<< " has exceed the maximum transmission attempt"<<"\n";
                 trans_discarded++;
                 packet3[i]=packet3[i]-1;
                 
                 //one frame is failed, so they need to back off also
                 
                // backoff_slot[i]=(rand()%cwnd[i]); //have a doubt on that
                 
             }
           } */
              
              
           }
        
           }
        
        
        else if(RTS_Enable==0)
             {
            
            out<<"Station "<<source3[min_index]<<"'s ACK frame doesn't face Bad Channel state"<<"\n";
             
           // if(packet3[min_index]==0)
           //  station_state[min_index]=station_finish_transmit; //station with the min backoff slot will
                                                           //transmit
            //what about SIFS time
          if(packet3[min_index]>0 && station_state[min_index]!=station_finish_transmit )
          {
           source_timer[min_index]=source_timer[min_index]+ (min*slot_time)+DIFS+(1*SIFS)+frame_time+Ack_time;
          
           cwnd[min_index]=cwmin_slot;
           backoff_slot[min_index]= rand()%cwnd[min_index];
            
            packet3[min_index]=packet3[min_index]-1;
            trans_attempt++;
            trans_success++;
            total_no_frames--;
            collision_count[min_index]=0;
            cout<<"Station "<<source3[min_index]<<" successfully transmit a frame at  "<<source_timer[min_index]<<" microseconds"<<"/"<<(source_timer[min_index]/slot_time)<<" slot time"<<"\n";
            out<<"Station "<<source3[min_index]<<" successfully transmit a frame at  "<<source_timer[min_index]<<" microseconds"<<"/"<<(source_timer[min_index]/slot_time)<<" slot time"<<"\n";    
          }
            
            
            
           for(int i=0;i<S;i++)
            {
            if(topology=="Grid")
               {
            if(station_state[i]!=station_finish_transmit && (packet3[i]>0) && i!=min_index && (collision_count[i]<4) && ((sqrt( pow((source3[i]%Grid_size)-(source3[min_index]%Grid_size),2)+pow(floor(source3[i]/Grid_size)-floor(source3[min_index]/Grid_size),2))<=2)))  //change the backoff time of the colliding stations
             {              
                  //contention_window(&cwnd[i],i);
                  source_timer[i]=source_timer[i]+ (min*slot_time)+DIFS+ frame_time+(1*SIFS)+Ack_time;
                  backoff_slot[i]= backoff_slot[i]-min;
                                  
              } 
             }
            
            else if(topology=="String")
            {
              if(station_state[i]!=station_finish_transmit && (packet3[i]>0) && i!=min_index && (collision_count[i]<4) && (abs(source3[i]-source3[min_index])<=2))  //change the backoff time of the colliding stations
                {              
                 // contention_window(&cwnd[i],i);
                  source_timer[i]=source_timer[i]+(min*slot_time)+ DIFS+ frame_time+(1*SIFS)+Ack_time;
                  backoff_slot[i]= backoff_slot[i]-min;
                 
                 
                }   
              }
            
            
           }
                  
        }
        
        
        //
        
          
   }
         
             
     
    
   
   // trans_attempt++;
    for(int i=0;i<S;i++)
    {
        if(packet3[i]==0)
        {
            station_state[i]=station_finish_transmit;
        }
    
    }
    
   /* cout<<"\n"<<"State of the stations are: ";
    for(int i=0;i<S;i++)
    {
            // cout<<station_state[i]<<" ";
             
             if(station_state[i]==0) cout<<"idle ";
             if(station_state[i]==1) cout<<" backed-off ";
             if(station_state[i]==2) cout<<" transmitted ";
             if(station_state[i]==3) cout<<" Exceed Retransmission ";
    }
    */
    
   
            
    cout<<"\n"<<"\n"<<"Frame Transmission Delay of all Stations "<<"\n";
    out<<"\n"<<"Frame Transmission Delay of all Stations "<<"\n";
    
     for(int i=0;i<S;i++)
     {
         cout<<source_timer[i]<<"\n";
         out<<source_timer[i]<<"\n";
     }
    
    
    
     
    
    
    cout<<"\n"<<"Backoff slot of all Stations "<<"\n";
    out<<"\n"<<"Backoff slot of all Stations "<<"\n";
     for(int i=0;i<S;i++)
     {
         cout<<backoff_slot[i]<<"\n";
         out<<backoff_slot[i]<<"\n";
     }
    
    cout<<"\n"<<"Contention window of all Stations "<<"\n";
    out<<"\n"<<"Contention window of all Stations "<<"\n";
     for(int i=0;i<S;i++)
     {
         cout<<cwnd[i]<<"\n";
         out<<cwnd[i]<<"\n";
     }
    
     cout<<"\n"<<"No of packet left to transmit for all Stations "<<"\n";
     out<<"\n"<<"No of packet left to transmit for all Stations "<<"\n";
     for(int i=0;i<S;i++)
     {
         cout<<packet3[i]<<"\n";
         out<<packet3[i]<<"\n";
     }
    
    
    cout<<"\n"<<"\n"<<"Channel States Count: ";
    cout<<"\n"<<"Total no. of Good states: "<<channel_good_count;
    cout<<"\n"<<"Total no. of bad states: "<<channel_bad_count;
    
    cout<<"\n"<<"Total no. of frame transmission attempt: "<<trans_attempt;
    cout<<"\n"<<"Total no. of frames successfully transmitted: "<<trans_success;
    cout<<"\n"<<"Total no. of frames collisions occurred: "<<trans_collision;
    cout<<"\n"<<"Total no. of frames discarded: "<<trans_discarded;
    
    
    out<<"\n"<<"\n"<<"Channel States Count: ";
    out<<"\n"<<"Total no. of Good states: "<<channel_good_count;
    out<<"\n"<<"Total no. of bad states: "<<channel_bad_count;
    
    out<<"\n"<<"Total no. of frame transmission attempt: "<<trans_attempt;
    out<<"\n"<<"Total no. of frames successfully transmitted: "<<trans_success;
    out<<"\n"<<"Total no. of frames collisions occurred: "<<trans_collision;
    out<<"\n"<<"Total no. of frames discarded: "<<trans_discarded<<"\n";
    out<<"\n"<<"Collision due to same back off time: "<<(trans_attempt-trans_success-channel_bad_count)<<"\n";
    
    }      
    
    //avg_delay= sum_delay/S;  //average total delay
    
   // float prob_good_state=float (float (channel_good_count)/float (channel_good_count+channel_bad_count));
    //float prob_bad_state=float (float (channel_bad_count)/float (channel_good_count+channel_bad_count));
    
    
   /*
    cout<<"\n"<<"\n"<<"Frame Delay Statistics: "<<"\n";
    cout<<"Total delay to start frame transmission of all "<<N<<" stations is "<<(timer-frame_time)<<" microseconds"<<"/"<<(timer/slot_time)<<" slot time"<<"\n";
    cout<<"Total delay to finish frame transmission of all " <<N<<" stations is "<<(timer)<<" microseconds"<<"/"<<((timer+frame_time)/slot_time)<<" slot time"<<"\n";
   // cout<<"\n"<<"Sum Delay of "<< N<<" station is : "<<sum_delay;
    cout<<"Average of all "<< N<<" station delays to successfully start frame transmission is : "<<avg_delay<<" microseconds"<<"/"<<(avg_delay/slot_time)<<" slot time"<<"\n";
    
    cout<<"\n"<<"\n"<<"Probability of States and State Transition of 2-state Markov Model: ";
    cout<<"\n"<<"Probability of remaining in Good state: "<<Pgg;
    cout<<"\n"<<"Probability of State Transition from Good state to Bad State: "<<Pgb;
    cout<<"\n"<<"Probability of remaining in Bad state: "<<Pbb;
    cout<<"\n"<<"Probability of State Transitions from Bad state to Good state: "<<Pbg;
     
     
    cout<<"\n"<<"\n"<<"Channel States Count: ";
    cout<<"\n"<<"Total no. of Good states: "<<channel_good_count;
    cout<<"\n"<<"Total no. of bad states: "<<channel_bad_count;
    */
    
    for(int i=0;i<S;i++)
     {
         sum_delay+=source_timer[i];
         
     }  
     
     avg_delay= sum_delay/S;
    
    cout<<"\n"<<"Frame Transmission Delay of all Stations "<<"\n";
    
     for(int i=0;i<S;i++)
     {
         cout<<source_timer[i]<<"\n";
     }
    

     
     
     cout<<"\n"<<"\n"<<"Average Frame Transmission Delay of all Stations "<<"\n";
     out<<"\n"<<"Average Frame Transmission Delay of all Stations "<<"\n";
    
         cout<<avg_delay<<"\n";
         out<<avg_delay<<"\n";
    
    cout<<"\n"<<"\n"<<"Channel States Count: ";
    cout<<"\n"<<"Total no. of Good states: "<<channel_good_count;
    cout<<"\n"<<"Total no. of bad states: "<<channel_bad_count;
    
    
    
    cout<<"\n"<<"\n"<<"No. of Stations, Frame Generation, Successful and Collision Statistics: ";
    cout<<"\n"<<"Total no. of Source stations: "<<S;
    cout<<"\n"<<"Total no. of frames generated: "<< frame_generated;
    cout<<"\n"<<"Total no. of frame transmission attempt: "<<trans_attempt;
    cout<<"\n"<<"Total no. of frames successfully transmitted: "<<trans_success;
    cout<<"\n"<<"Total no. of frames collisions occurred: "<<trans_collision;
    cout<<"\n"<<"Total no. of frames discarded: "<<trans_discarded;
    cout<<"\n"<<"Collision due to same back off time: "<<(trans_attempt-trans_success-channel_bad_count)<<"\n";
    
    cin.get();
    
    return 0;
}



float channel_state_model(float channel_trans, float &channel_state, float Pgg, float Pbb, int &channel_bad_count, int &channel_good_count)
{
   
          
          if(channel_trans >= Pgg)
          {
               channel_state= channel_state_bad;  //change channel state to bad state based on random channel 
                                                 //state number
               //channel_bad_count++;
          }
         
          
          else if(channel_trans >= Pbb)
          {
               channel_state= channel_state_good;  //change channel state to bad state based on random channel 
                                                 //state number     
               //channel_good_count++;
          }
          
          else if (channel_trans < Pbb )
          {  
                 channel_state= channel_state_bad; //change channel state to bad state based on random channel 
                                                 //state number                 
                 //channel_bad_count++;
          }
          
    return channel_state;
}


void contention_window(int *cwnd, int min_index)
{
     if(cwnd[min_index]<1024)
              {
                  cwnd[min_index]= 2* cwnd[min_index];
                  cout<<"Collision Occurred and so double the contention window to " <<cwnd[min_index];
                  
               }
       else 
                 {
                     cwnd[min_index]=1024;
                     cout<<"Truncate the contention window to 1024 slots";
                     
                 }
}


void channel_state_increment(int channel_state, int &channel_good_count,int &channel_bad_count)
{
    if(channel_state==channel_state_good)
                channel_good_count++;
    else channel_bad_count++;
    
}