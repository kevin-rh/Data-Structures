#ifndef _FEE_HPP
#define _FEE_HPP

class Station;
class Bike;

class Fee{
private:
    int disc_bike[3];
    int reg_bike[3];
    int wait;
    int bike_switch;
    int transfer;
    int revenue;
public:
    Fee(int*, int*, int, int, int);
    int get_disc(int bikeType) const;
    int get_reg(int bikeType) const;
    int get_wait() const;
    int get_switch_bike() const;
    int get_revenue() const;
    void pay(int bikeType, int delta_time, int minim);

    ~Fee();
};


Fee::Fee(int disc_price[], int reg_price[], int wait, int bike_switch, int transfer){
    for(int i=0;i<3;i++){
        //std::cout<<i<<": "<< ((disc_price[i]))<<" "<<((reg_price[i]))<<"\n";//debug
        this->disc_bike[i] = ((disc_price[i]));
        this->reg_bike[i] = ((reg_price[i]));
    }
    this->wait = wait;
    this->bike_switch = bike_switch;
    this->transfer = transfer;
    this->revenue = 0;
}
int Fee::get_disc(int bikeType)const{
    return this->disc_bike[bikeType-1];
}
int Fee::get_reg(int bikeType)const{
    return this->reg_bike[bikeType-1];
}
int Fee::get_wait()const{
    return this->wait;
}
int Fee::get_switch_bike()const{
    return this->transfer;
}

void Fee::pay(int bikeType, int delta_time, int minim_time){
    if(delta_time == minim_time){
        this->revenue += delta_time*get_disc(bikeType);
    }else{
        this->revenue += delta_time*get_reg(bikeType);
    }
}
int Fee::get_revenue()const{
    return this->revenue;
}
#endif