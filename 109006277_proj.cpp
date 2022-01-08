#include <iostream>
#include <fstream>
#include <string>

#include "./include/vector.hpp"
#include "./include/list.hpp"
#include "./include/heap.hpp"
#include "./include/avltree.hpp"

#include "include/station.hpp"
#include "include/bike.hpp"
#include "include/fee.hpp"
#include "include/graph.hpp"
// #include "include/database.hpp"

#define TESTCASE 3
const std::string path("testcase/3/");

Fee *cashier;  
Graph *minim_time;
Vector<Bike*> bikes;
Vector<Station*> stations; 
AvlTree<Bike*> rented_bike;
int revenue = 0;

std::string filler0(int a){
    int i=0;
    std::string str;
    while(a/=10) i++; 
    str.append(4-i,'0');
    return str;
}

void init(int num){
    std::string map_txt("map.txt");
    std::string station_txt("station.txt");
    std::string fee_txt("fee.txt");
    map_txt.insert(0,path);
    station_txt.insert(0,path);
    fee_txt.insert(0,path);

    // <s[i]> <s[j]> <e[ij]>
    
    std::ifstream mapfile (map_txt);
    if(mapfile.is_open()){
        Vector<Edge*> *edges = new Vector<Edge*>();
        int max_vertex = 0;
        int tmp=0, i, j, e;
        while(!mapfile.eof()){
            mapfile>>i>>j>>e;
            // cout<<i<<' '<<j<<' '<<e<<endl; //DEBUG
            edges->push_back(new Edge(i, j, e));
            // cout<<(*edges)[tmp]->a<<endl;
            // cout<<(*edges)[tmp]->b<<endl;
            // cout<<(*edges)[tmp++]->dist<<endl;

            if(max_vertex<i) max_vertex = i;
            if(max_vertex<j) max_vertex = j;
        }
        minim_time = new Graph(edges, max_vertex); // create minimumTime each stations to graph map
        mapfile.close();
    }else{
        std::cout<< "ERR: " <<map_txt <<" not found"<<std::endl;
        exit(-1);
    }

    std::ifstream stationfile (station_txt);
    // s[i] b-elec b-lady b-road
    
    if(stationfile.is_open()){
        int id, b[3];
        while(!stationfile.eof()){
            stationfile>>id;
            stationfile>>b[0]>>b[1]>>b[2];
            //std::cout<<id<<' '<<b[0]<<' '<<b[1]<<' '<<b[2]<<"\n"; //DEBUG
            stations.push_back(new Station(id)); // create station and its bike
            int bike_id = id*100;
            for(int type=0;type<3;type++){
                for(int i=0;i<b[type];i++){
                    
                    //std::cout<<type+1<<" "<<bike_id+i<<" "<<id<<" \n";
                    //bikes.push_back(new Bike(type+1, bike_id+i, id));
                    //for(int i=0;i<bikes.size();i++){
                    //    std::cout<<" "<<*(bikes[i]);
                    //}
                    //std::cout<<"\n"<<bikes.size()-1<<" \n";

                    stations[id-1]->return_bike(new Bike(type+1, bike_id+i, id), -1);
                }
            }
            //std::cout<<"StationsInsert"<<stations[i-1]->get_id()<<"DONE\n";
        }
        //std::cout<<"DONE_STATION_TXT\n";
        stationfile.close();
    }else{
        std::cout<< "ERR: " <<station_txt <<" not found"<<std::endl;
        exit(-1);
    }


    std::ifstream feefile (fee_txt);
    // bikeType discRate regRate x3
    // waiting fee
    // reduced rate (due to switching bike type)
    // rate of transferring fee
    
    if(feefile.is_open()){
        std::string type[3];
        int disc_rate[3], reg_rate[3];
        int waiting_fee, reduce_rate, transfer_rate;
        for(int i=0; (i<3) && !feefile.eof(); i++){
            feefile >>type[i]>>disc_rate[i]>>reg_rate[i];
        }
        feefile >> waiting_fee;
        feefile >> reduce_rate;
        feefile >> transfer_rate;
        cashier = new Fee(disc_rate, reg_rate, waiting_fee, reduce_rate, transfer_rate);
        // cout<<"DONE_FEE_TXT\n";
        feefile.close();
    }else{
        std::cout<< "ERR: " << fee_txt <<" not found"<<std::endl;
        exit(-1);
    }
}

void status(int num){
    std::cout<<"CEk";
    std::string status_txt ("part");
    status_txt.append(1, num+'0');
    status_txt.append("_status.txt");
    
    std::ofstream stat (status_txt);
    if(!stat.is_open()){
        std::cout<< "ERR: " << status_txt <<" not found"<<std::endl;
        exit(-1);
    }
    
    std::string type_name[3] = {"electric", "lady", "road"};
    for(int i=0; i<stations.size();i++){
        stat << stations[i]->get_id() <<":\n";
        for(int id=0;id<3;id++){
            stat << type_name[id] << ":";
            Bike* bike_tmp = NULL;
            while(bike_tmp = stations[i]->rent_bike(id+1,-1)){
                stat << " " << *(bike_tmp);
                delete bike_tmp;
            }
            stat << "\n";
        }
    }   
    
    stat << cashier->get_revenue();

    stat.close();
}

int stoi_type(const std::string s){
    if(s == "electric") return 1;
    if(s == "lady") return 2;
    if(s == "road") return 3;
    std::cout << "ERR: " << s << " is not a bikeType.\n";
    exit(1);
}

void response(int num){
    std::string user_txt("user.txt");
    user_txt.insert(0,path);

    std::ifstream user (user_txt);
    if(!user.is_open()){
        std::cout<< "ERR: " << user_txt <<" not found"<<std::endl;
        exit(-1);
    }

    std::string response_txt ("part");
    response_txt.append(1, num+'0');
    response_txt.append("_response.txt");
    std::ofstream resp (response_txt);
    if(!resp.is_open()){
        std::cout<< "ERR: " << response_txt <<" not found"<<std::endl;
        exit(-1);
    }

    std::string cmd, bikeStr;
    int stat_id, bikeType, user_id, time, delta_time;

    // Fee *cashier;  
    // Graph *minim_time;
    // Vector<Station*> stations; 
    // AvlTree<Bike*> rented_bike;
    // int revenue = 0;

    Bike* cur_bike = NULL;
    while(!user.eof()){
        user >> cmd;
        user >> stat_id;
        if(cmd == "rent"){
            user >> bikeStr;
            user >> user_id >> time;
            resp << cmd <<" "<< stat_id <<" "<< bikeStr <<" "<<filler0(user_id)<< user_id <<" "<< time <<"\n";

            //std::cout << cmd <<" "<< stat_id <<" "<< bikeStr <<" ["<< user_id <<"] "<< time <<"\n";
            bikeType = stoi_type(bikeStr);

            cur_bike = stations[stat_id-1]->rent_bike(bikeType, time);
            if(cur_bike != NULL){
                rented_bike.insert(cur_bike, user_id);
                resp <<"accept\n";
                // std::cout <<"accept\n";
            }else{
                resp <<"reject\n";
                // std::cout <<"reject\n";
            }
        }
        if(cmd == "return"){
            user >> user_id >> time;
            resp << cmd <<" "<< stat_id <<" "<< user_id <<" "<< time <<"\n";
            //std::cout << cmd <<" "<< stat_id <<" ["<< user_id <<"] "<< time <<"\n";
            cur_bike = rented_bike.search_delete(user_id);
            if(cur_bike != NULL){
                // std::cout<<"bikeID: "<< cur_bike->get_id()<<".\n";
                int from_stat_id = cur_bike->get_station_id();
                bikeType = cur_bike->get_type();
                delta_time = stations[stat_id-1]->return_bike(cur_bike, time);
                cashier->pay(bikeType, delta_time, minim_time->shortest(from_stat_id, stat_id));
                //std::cout<<" from:"<<from_stat_id<<" to:"<<stat_id<<" type:"<< bikeType <<" time:"<<delta_time<<" min:"<<minim_time->shortest(from_stat_id,stat_id)<<"\n"; // DEBUG!
            }
        }
        
        cur_bike = NULL;
        
        // DEBUG !
        //std::cout<<".:.\t\n";
        // std::string type_name[3] = {"electric", "lady", "road"};
        // for(int i=0; i<stations.size();i++){
        //     std::cout << stations[i]->get_id() <<":\n";
        //     for(int id=0;id<3;id++){
        //         std::cout << type_name[id] << ":";
        //         Vector<Bike*> v = stations[i]->get_sorted_bike_data(id+1);
        //         for(int d=v.size()-1 ;d>=0; d--){
        //             std::cout << " " << *(v[d]);
        //         }
        //         std::cout << "\n";
        //     }
        // }   
        // std::cout<<"Revenue: "<<cashier->get_revenue()<<"\n"; // DEBUG!

        // rented_bike.inOrder();
        // std::cout<<"\n";
    }

    // DEBUG preset:
    // std::cout << cmd <<" "<< stat_id <<" "<< user_id <<" "<< time <<"\n";
    // std::cout <<" "<< cmd <<" "<< stat_id <<" "<< bikeType <<" "<< user_id <<" "<< time <<"\n";
    user.close();
    resp.close();
}

using namespace std;


void testbench(){
    cout<<".:VECTOR:.\n";
    int type = 1;
    int id = 101;
    Bike* biked;
    

    biked = new Bike(type,id,1);
    Vector<Bike*> a, b;
    a.push_back(biked);
    biked = new Bike(type+1, id+3, 1);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.push_back(biked);
    a.pop_back();
    a.pop_back();
    a.pop_back();
    a.pop_back();
    a.pop_back();
    b=a;
    for(int i=0; i<b.size();i++){
        cout<<"id"<<i<<": "<<*(b[i])<<endl;
    }
    bool x = a[1]>a[0];
    cout<<"tf:"<<x<<endl;
    MinHeap<Bike*> c;
    c.insert(biked);
    cout<<".:MINHEAP:.\n";
    //MinHeap<Bike*> c;
    c.insert(a[1]);
    cout<<"cek";
    c.insert(a[0]);

    
    cout <<" top: "<< *((c.get_top()));
    c.remove_top();
    cout <<" top: "<< *((c.get_top()));
    c.remove_top();

    c.insert(new Bike(type,id+2,1));
    c.insert(new Bike(type,id+10,1));
    c.insert(new Bike(type,id+12,1));
    c.insert(new Bike(type,id+13,1));
    c.insert(new Bike(type,id+8,1));
    cout<<"\n"<<c.get_size()<<" "<<c.is_empty()<<" "<<*(c.get_top())<<"\n";
    c.print_list();
    cout<<"\n";
    for(int i=0;i<3;i++){
        Bike* tmp=(c.get_top());
        cout<<" ";
        cout << *(tmp);
        c.remove_top();
    }
    // c.insert(a[1]);
    // c.insert(a[0]);
    c.insert(new Bike(type,id+11,1));
    c.insert(new Bike(type,id+16,1));
    c.insert(new Bike(type,id+17,1));
    c.insert(new Bike(type,id+18,1));
    c.insert(new Bike(type,id+4,1));
    for(int i=0;i<c.is_empty();i++){
        Bike* tmp=(c.get_top());
        cout<<" ";
        cout << *(tmp);
        c.remove_top();
    }
    cout<<"\n"<<c.get_size()<<" "<<c.is_empty()<<" "<<*(c.get_top())<<"\n";
    c.print_list();
    cout<<"\n";
    for(int i=0;!c.is_empty();i++){
        cout << *(c.get_top());
        c.remove_top();
    }
    cout<<"\n";
}

int main(){
    //testbench();
    cout<<"TESTBENCH DONE\n";
    init(TESTCASE);
    cout<<"INIT DONE\n";
    response(TESTCASE);
    cout<<"RESPONSE DONE\n";
    status(TESTCASE);
    cout<<"STATUS DONE\n";
    return 0;
}

/*
//g++ -g *.cpp ./include/*.cpp -o 109006277_proj -std=c++11
//g++ -g *.cpp -o 109006277_proj -std=c++11
// list check
int main(){
    List<int> a;
    a.push_back(10);
    a.push_back(23);
    a.push_front(1);
    a.print_list();
    a.reverse();
    a.print_list();
    List<int> b;
    
    b.push_front(42);
    cout<<"bb"<<endl;
    b.print_list();
    cout<<endl;
    b.reverse();
    b.print_list();

    a.push_back(1);
    a.push_back(1);
    a.push_back(3);
    a.push_back(1);
    a.push_front(1);
    cout<<"delete"<<endl;
    a.print_list();
    cout<<endl;
    a.reverse();
    a.print_list();
    cout<<endl;
    a.Delete(1);
    a.print_list();
    cout<<endl;
    a.Delete(3);
    a.print_list();
    cout<<endl;
    a.Delete(21);
    a.Delete(23);
    a.print_list();
    cout<<endl;
    a.Delete(10);
    a.reverse();
    cout<<"DONE"<<endl;
    a.print_list();
    cout<<"LOL"<<endl;
}
*/
/*
// TEST VECTOR
int main(){
    cout<<"START";
    Vector<int> a;
    a.push_back(1);
    a.push_back(5);
    a.push_back(10);
    a.insert(1,5,3);
    a.push_back(19);
    a.pop_back();
    a.pop_back();
    a.pop_back();
}*/


/*
int main(){
    cout<<"Start"<<endl;
    init(); // Initial all data

    Vector<Station*> stations;

    std::string path = "/testcase/";
    path.append(1,TESTCASE+'0');
    std::string user_txt(path);
    user_txt.append("user.txt");
    ifstream userfile (user_txt);
    

    int station_id, user_id, bike_type, time;
    std::string cmd;
    // rent <stationIdRent> <bikeType> <userId> <timeRent>
    // return <stationIdReturn> <userId> <timeReturn>
    if(userfile.is_open()){
        while(!userfile.eof()){
            userfile >> cmd;
            if(cmd == "rent"){
                userfile >> station_id >> bike_type >> user_id >> time;
                Bike* bike = stations[station_id]->rent_bike(bike_type, time);
                if (bike){
                    // TODO: accept

                }else{
                    // TODO: reject

                }
            }else if (cmd == "return"){
                userfile >> station_id >> user_id >> time;
            }else{

            }
        }
        userfile.close();
    }else{
        cout<< user_txt <<" not found"<<endl;
        exit(-1);
    }
    return 0;
}*/

/*

I. accept (BASIC)
// The renter gets a bike.
Note: The “accept” response is only valid when there is no 
shortage. If your tool accepts a rental request and there is no bike 
to give, your code would be considered incorrect.

II. wait (ADVANCED)
// Ask the renter to wait.

III. discount <tp> (ADVANCED)
// Give the renter a discount to rent another type (tp) of bike.
Note: You can offer the renter discount even if there is no bike 
shortage in the advanced version.

IV. reject (BASIC)
// reject the rental request.
Note: You may deliberately reject a rental request for revenue 
consideration in the advanced version.

V. transfer <𝑠𝑗> <𝑠𝑖> <tp> number time (ADVANCED)
// at time, start transferring the number of type tp bikes from <𝑠𝑗> to <𝑠𝑖>.
Note: In the advanced version, you may make a transfer decision at 
any time


File-Output:

(An example of “part1_response.txt”)
rent 3 lady 00002 0
accept
rent 2 electric 00001 0
accept
return 1 00001 35
return 1 00002 85
rent 3 electric 00003 90
reject
return 1 00003 1440 // the original return of rejected rents should still be outputted

(An example of “part2_response.txt”)
rent 3 lady 00002 0
accept
rent 2 electric 00001 0
accept
transfer 2 3 electric 1 10
return 1 00001 35
return 1 00002 85
rent 3 electric 00003 90
accept
return 1 00003 1440

*/
