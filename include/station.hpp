#ifndef _STATION_HPP
#define _STATION_HPP

#include <iostream>
#include "bike.hpp"
#include "heap.hpp"
class Bike;

template <class T>
class MinHeap;

class Station{
public:
    typedef int delta_time;
    typedef int time_type;
private:
    int id;

    MinHeap<Bike*> elecs;
    MinHeap<Bike*> ladys;
    MinHeap<Bike*> roads;
public:
    Station();
    Station(int);
	// Station(int, int, int, int);
    
    // void set_data(int, int, int, int);

    friend std::ostream& operator<< (std::ostream& out, const Station&);

    int get_id()const;
    int get_bike_size(int bikeType)const; // data of bike_id
    // Vector<Bike*> get_sorted_bike_data(int bikeType) const;
    Bike* rent_bike(int bikeType, time_type rent_time);
	int return_bike(Bike *bike, time_type return_time);   // return delta_time
	void status();
    ~Station();
};

Station::Station(){
}

Station::Station(int id){
    this->id = id;
}

// Station::Station(int id, int elec, int lady, int road){
//     set_data(id, elec, lady, road);
// }

// void Station::set_data(int id, int elec, int lady, int road){
//     this->id = id;
//     int bike_id = id * 100, i = 0;
//     for(int d=0;d<3;d++){
//         for(int i=0; i<elec; i++){
//             this->return_bike(new Bike(d+1, bike_id+i, id), -1);
//         }
//     }
// }


// std::ostream& operator<< (std::ostream& out, const Station& s){
//     out << s.get_id() << ":";
//     return out;

//     out << "\nelectric:";
//     out << (s.get_sorted_bike_data(1));
//     out << "\nlady:";
//     out << (s.get_sorted_bike_data(2));
//     out << "\nroad:";
//     out << (s.get_sorted_bike_data(3));
//     return out;
// }

// Vector<Bike*> Station::get_sorted_bike_data(int bikeType) const{
//     Vector<Bike*> v;
//     switch (bikeType){
//     case 1:
//         v = (this->elecs.get_vector());
//         break;
//     case 2:
//         v = (this->ladys.get_vector());
//         break;
//     case 3:
//         v = (this->roads.get_vector());
//         break;
//     default:
//         std::cout<<"ERR: bikeType error: ["<<bikeType<<"] is not a type"<<std::endl;
//         exit(-1);
//         break;
//     }
//     heap_sort(&v);
//     return v;
// }

int Station::get_id()const{
    return this->id;
}

int Station::get_bike_size(int id)const{
    if(id==1){
        return elecs.get_size();
    }else if(id==2){
        return ladys.get_size();
    }else if(id==3){
        return roads.get_size();
    }else{
        return -1;
    }
}

Bike* Station::rent_bike(int bikeType, int rent_time){
    Bike* bike = NULL;

    switch (bikeType){
    case 1:
        if(this->elecs.is_empty()) break;
        else{
            bike = this->elecs.get_top();
            elecs.remove_top();
        }
        break;
    case 2:
        if(this->ladys.is_empty()) break;
        else{
            bike = this->ladys.get_top();
            ladys.remove_top();
        }
        break;
    case 3:
        if(this->roads.is_empty()) break;
        else{
            bike = this->roads.get_top();
            roads.remove_top();
        }
        break;
    default:
        std::cout<<"ERR: bikeType error: ["<<bikeType<<"] is not a type"<<std::endl;
        exit(-1);
        break;
    }

    // Bike got rent and record the time picked up
    if(bike != NULL){
        bike->rent(rent_time);
        return bike;
    }else{
        return NULL; // it will cause Reject
    }
}

Station::delta_time Station::return_bike(Bike* bike, Station::time_type return_time){    
    switch (bike->get_type()){
    case 1:
        this->elecs.insert(bike);
        break;
    case 2:
        this->ladys.insert(bike);
        break;
    case 3:
        this->roads.insert(bike);
        break;
    default:
        std::cout<<"ERR: bikeTypeError ["<<bike->get_type()<<"] is not a type.\n";
        exit(-1);
        break;
    }
    // return a delta_time
    return bike->returned(return_time, this->get_id());
}


Station::~Station() { // Destructor
}

#endif