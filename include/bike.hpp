#ifndef _BIKE_HPP
#define _BIKE_HPP

#include <iostream>
#include <iomanip>    

#include "station.hpp"
class Station;

class Bike{
public:
	typedef int delta_time;
	typedef int time_type;
private:
	int id;
	int type;
	time_type rent_time;	// (-1) not being used

	int station_id;
public:
	Bike();
    Bike(int type, int id, int station_id);

	friend std::ostream& operator<<(std::ostream& out, const Bike& bike);
	
	bool operator> (const Bike& rhs) const{
    	return this->id > rhs.id;
	}
	bool operator< (const Bike& rhs) const{
    	return this->id < rhs.id;
	}

	int get_type()const;
	int get_id()const;
	time_type get_time()const;
	int get_station_id()const;

    void rent(time_type time);
 	delta_time returned(time_type time, int station_id); // **IMPORTANT: reset Station* from and rent_time

	int key();

	~Bike();
};

std::ostream& operator<< (std::ostream& out, const Bike& bike){
    out << bike.get_id();
    return out;
}

Bike::Bike(){}

Bike::Bike(int type, int id, int station_id):
    id(id), 
    type(type), 
    rent_time(-1), 
	station_id(station_id)
{}

int Bike::get_type()const{
    return this->type;
}
int Bike::get_id()const{
    return this->id;
}
Bike::time_type Bike::get_time()const{
	return this->rent_time;
}
int Bike::get_station_id()const{
	return this->station_id;
}

void Bike::rent(time_type time){
    this->rent_time = time;
}
Bike::delta_time Bike::returned(Bike::time_type time, int station_id){
    time -= this->rent_time;
    this->rent_time = -1;
    this->station_id = station_id;
    return time;
}

int Bike::key(){
	return get_id();
}

Bike::~Bike() { // Destructor

}

/*
//header-start

#include <sstream>
#include <cstring>
#include <iostream>
#include <cstdio>
using namespace std;

//HUMAN//////////////////////////////////////////////
class Human {
public:
	string name;
	int money;
	int skill;
	int bets;
	int cards;

	Human(string n, int m, int ski);

	virtual void Pay(Human *human) {}
	virtual void Bet() {}
	void Draw();
	virtual int Win(Human *human) {}
	virtual bool Bankrupt() {}
	virtual bool Cheat(Human *human) {}
	virtual void Order(Human *banker, Human *server) {}

	~Human();
};
Human::Human(string n, int m, int ski){
    this->name=n;
    this->money=m;
    this->skill=ski;
    this->cards=0;
    this->bets=0;
}

void Human::Draw() {
    this->cards = 0;
    string res = "";
    while (res.size() <= 0){
        getline(cin, res);
    }
    stringstream ss(res);
    while (ss >> res) {
        int temp = 0;
        for (auto i : res)
            temp = temp * 10 + i - '0';
        this->cards += temp;
    }
    //cout<<'/t'<<this->cards<<endl;//debug
}
Human::~Human(){
}

//GUARD//////////////////////////////////////////////
class Guard : public virtual Human {
public:
	Guard(int ski);
	void Pay(Human *human);
};
Guard::Guard(int ski):Human::Human("Guard",0,ski){}
void Guard::Pay(Human *human){
    int m=human->skill-this->skill;
    if(m>0){
        this->money-=m;
        human->money+=m;
    }
}

//BANKER//////////////////////////////////////////////
class Banker : public virtual Human {
public:
	Banker(int ski);
	void Pay(Human *human);
	int Win(Human *human);
	~Banker();
};
Banker::Banker(int ski):Human::Human("Banker",0,ski){}
void Banker::Pay(Human *human){
    int m;
    if(human->name=="Guard"){
        m=100;
        human->money+=m;
        this->money-=m;
    }else{
        m=human->bets;
        if(human->skill<this->skill){
            //cout<<"skill";//debug
            m+=(10*human->cards);
        }
        if(human->cards==21){
            //cout<<"21";//debug
            m*=2;
        }
        human->money+=m;
        this->money-=m;
        this->bets=human->bets=m;
    }
}
int Banker::Win(Human *human){
    if(human->cards > 21 && this->cards > 21){
        return -1;
    }else if(human->cards <= 21 && (this->cards > 21 || human->cards > this->cards)){
        return 0;
    }else if(this->cards <= 21 && (human->cards > 21 || this->cards >= human->cards)){
        return 1;
    }
}
Banker::~Banker(){}

//SERVER//////////////////////////////////////////////
class Server : public virtual Human {
public:
	Server(int ski);
	~Server();
};
Server::Server(int ski):Human::Human("Server",0,ski){}
Server::~Server(){}

//PLAYER//////////////////////////////////////////////
class Player : public virtual Human {
public:
	Player(string name, int m, int ski);
	void Pay(Human *human);
	void Bet();
	bool Bankrupt();
	bool Cheat(Human *human);
	void Order(Human *banker, Human *server);
	~Player();
};
Player::Player(string name, int m, int ski):Human::Human(name,m,ski){}
void Player::Pay(Human *human){
    int m=this->bets;
    if(m>=this->money){
        human->money+=this->money;
        this->money=0;
    }else{
        human->money+=m;
        this->money-=m;
    }
}
void Player::Bet(){
    cin.ignore();
    cin>>this->bets;
    //cout<<'\t'<<this->bets<<endl;//debug
}
bool Player::Bankrupt(){
    return(this->money<=0);
}
bool Player::Cheat(Human *human){
    return ((3*this->skill)<human->bets);
}
void Player::Order(Human *banker, Human *server){
    //cout<<'\t'<<banker->bets<<endl;//debug
    if(this->money>server->skill){
        banker->money+=2000;
        server->money+=1000;
        this->money-=3000;
    }
}
Player::~Player(){

}

class Casino {
public:
	Casino();
	~Casino();
	void Enterance(int f, int u);
	void Guest(string s, int m, int ski);
	void TwentyOne();
	void EndDay();
	void Result();

	int fee, U;

	Human *banker, *guard, *server;
	int player_num;
	Human *player[1009];

	int total_income, today_income;
	int blacklist_num;
	string blacklist[100009];

};

Casino::Casino(){
    this->fee=0;
    this->U=0;
    this->player_num=0;
	this->total_income=0;
	this->today_income=0;
	this->blacklist_num=0;

}
Casino::~Casino(){
}

void Casino::Enterance(int f, int u){
    this->fee=f;
    this->U=u;
}
void Casino::Guest(string s, int m, int ski){
    for(int i=0;i<blacklist_num;i++){
        if(s==blacklist[i])return;
    }
    for(int i=0;i<player_num;i++){
        if(s==player[i]->name)return;
    }
    m-=fee;
    if(m<=0){
        //this->today_income+=(fee+m);
        blacklist[blacklist_num++]=s;
    }else{
        //cout<<'\t'<<this->fee<<endl;//debug
        this->today_income+=this->fee;
        player[player_num]= new Player(s,m,ski);
        player_num++;
    }
}
void Casino::TwentyOne(){
    int ski,id[201]={0};
    cin>>ski;
    banker=new Banker(ski);
    cin>>ski;
    guard=new Guard(ski);
    cin>>ski;
    server= new Server(ski);
    int K;
    cin>>K;
    string n;
    int record[1009];
    for (int i = 0; i < 1009; i++) record[i] = 0;
    for (int i = 0; i < K; i++) {
        cin >> n;
        int flag = 1;
        Player* new_player = new Player(n, 0, 0);
        new_player->Bet();
        new_player->Draw();
        //cout<<"\tplay:"<<new_player->bets<<' '<<new_player->cards<<endl;//debug
        for (int j = 0; j < this->blacklist_num; j++) {
            if (this->blacklist[j] == n) {
                flag = 0;
                break;
            }
        }
        if (flag) {
            for (int j = 0; j < this->player_num; j++) {
                if (this->player[j]->name == n) {
                    record[j] = 1;
                    this->player[j]->bets = new_player->bets;
                    this->player[j]->cards = new_player->cards;
                }
            }
        }
        delete new_player;
    }
    // banker draw
    this->banker->Draw();

    // your twnety one rule
    for (int i = 0; i < this->player_num; i++) {
        if (record[i] == 1) {
            if(banker->Win(player[i])==1){//banker win
                //cout<<'\t'<<player[i]->name<<"LOSE"<<endl;//debug
                player[i]->Pay(banker);
                if(player[i]->Bankrupt()){
                    //cout<<"bankrupt"<<endl;//debug
                    blacklist[blacklist_num++]=player[i]->name;
                    banker->Pay(guard);
                }
            }else if(banker->Win(player[i])==0){//player win
                //cout<<'\t'<<player[i]->name<<"WIN"<<endl;//debug
                banker->Pay(player[i]);
                if(player[i]->Cheat(banker)){
                    //cout<<"cheater"<<endl;//debug
                    blacklist[blacklist_num++]=player[i]->name;
                    banker->Pay(guard);
                    guard->Pay(player[i]);
                }
                player[i]->Order(banker,server);
            }

        }
    }
    cout<<banker->money<<' '<<guard->money<<' '<<server->money<<endl;
    for(int i=0;i<201;i++){
        if(record[i]==1){
            cout<<player[i]->name<<' '<<player[i]->money<<endl;
        }
    }
    this->today_income+=banker->money;
    //cout<<'\t'<<"bankmoney: "<<banker->money<<endl;//debug
}
void Casino::EndDay(){
    //cout<<'\t'<<this->U<<endl;//debug
    if(this->today_income>this->U){
        blacklist_num=0;
    }
    this->total_income+=this->today_income;
    this->today_income=0;
    //cout<<"\t normal:"<<player_num<<endl;//debug

    //cout<<"\t normall"<<endl;//debug
    this->player_num=0;
}
void Casino::Result(){
    cout<<this->total_income<<endl;
    for(int i=0;i<blacklist_num;i++){
        cout<<blacklist[i]<<endl;
    }
}










//header-end
*/


#endif

// enum bikeType{
//     ELECTRIC = 1,
//     LADY = 2,
//     ROAD = 3,
// };