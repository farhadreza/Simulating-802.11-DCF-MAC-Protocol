/* 
 * File:   header.h
 * Author: Farhad
 *
 * Created on April 3, 2014, 12:03 AM
 */

#ifndef HEADER_H
#define	HEADER_H

#define channel_idle 0
#define channel_busy 1
#define station_idle 0
#define station_backing_off 1
#define station_finish_transmit 2
#define station_busy 3

#define station_collide 1
#define station_not_collide 0

#define RTS_ON 1
#define RTS_OFF 0

//#define Pgg 50
//#define Pbb 50

#define channel_state_good 0
#define channel_state_bad 1

double timer=0; //global timer to save the time

 //int N;//no. of stations contending for the channel
    int SEED; //to randomize number
    //int SEED_array[5]={1000,2000,3000,4000,5000};
    
    double frame_time;
    int DIFS;
    int SIFS;
    int slot_time;
    int data_rate; // 54 Mbps
    int max_frame_size;     
    int cwmin_slot; 
    int cwmax_slot; 
    int cwmin_time;
    
    int RTS_time;
    int CTS_time;
    int Ack_time;
       
    float channel_trans; //to keep track of random channel state number 
    float Pgg;//Probability of remaining in Good state:
    float Pbb; //Probability of remaining in Bad state:
    float Pgb; //Probability of State Transition from Good state to Bad state
    float Pbg; //Probability of State Transition from Bad state to Good state
    
    float channel_state; //for storing channel state
    
    int min_index;
     int trans_attempt; //total no. of frame transmission attempt
    int trans_success; //total no. of frame transmission successful
    int trans_collision; //total no. of frame transmission Collided
    int trans_discarded; //total no. of frames Discarded
    int channel_bad_count; //to count bad channel state
    int channel_good_count; //to count good channel state
    
    
    int S,D;
    int RTS_Enable;
    int Grid_size;
    int string_nodes;
    
    int *station_state; // for storing station state
    int *collision_state; //for keeping track of colliding stations
    int *backoff_time;
    int *backoff_slot; //backoff_slot to back off for random no of slots
    
    double sum_delay; //sum of transmission delay of all the station
    double avg_delay; //average of transmission delay of all the station
    int no_of_station; //for keeping track of no. of station that has finished its transmission
    int total_no_frames;
    
    int *cwnd;
    int *source_timer;
    float *channel_state_node;
    
    int *source3;
    int *destination3;
    int *packet3;
    int row;
    int col;
    double **matrix;
    int frame_generated;
    
    

float channel_state_model(float channel_trans, float &channel_state, float Pgg, float Pbb, int &channel_bad_count, int &channel_good_count);
void contention_window(int *cwnd, int min_index);
void channel_state_increment(int channel_state, int &channel_good_count,int &channel_bad_count);



#endif	/* HEADER_H */

