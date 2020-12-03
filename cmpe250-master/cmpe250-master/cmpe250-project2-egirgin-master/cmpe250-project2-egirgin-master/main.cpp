#include <iostream>
#include "Passenger.h"
#include <queue>
#include "Simulator.h"
#include "LuggageCounter.h"
#include "SecurityCounter.h"
#include <fstream>

using namespace std;
int main(int argc, char* argv[]) {

    clock_t start = clock();
/*
    Passenger p1(1,10,6,3,'N','L');
    Passenger p2(3,16,3,6,'N','N');
    Passenger p3(4,11,2,3,'V','N');
    Passenger p4(5,15,2,5,'N','N');
    Passenger p5(6,9,2,1,'V','N');
    Passenger p6(16,19,1,1,'N','L');
    Passenger p7(17,26,3,5,'N','L');
    Passenger p8(18,22,1,3,'N','L');

    Simulator s(2,2);
*/



    int N,L,S;
    int at,bt,lt,st;
    char v,o;

    queue<Passenger> data;

    ifstream scanner;
    scanner.open(argv[1]);
    scanner>>N>>L>>S;
    for(int i=0;i<N;i++) {
        scanner>>at>>bt>>lt>>st>>v>>o;
        //cout<<v<<o<<endl;
        Passenger p = Passenger(at,bt,lt,st,v,o);
        data.push(p);
    }
    scanner.close();


    ofstream stream_f;
    stream_f.open(argv[2]);


    Simulator s(L,S);
    Passenger::fly_first=false;
    Simulator::vip_enabled=false;
    Simulator::online_enabled=false;
    s.arrival_queue = data;
    s.timestamp();
    cout<<(float)s.total_spending_time/N<<" ";
    cout<<s.number_of_fails<<endl;
    stream_f << (float)s.total_spending_time/N <<" ";
    stream_f <<s.number_of_fails<<endl;

    Simulator s1(L,S);
    Passenger::fly_first=true;
    Simulator::vip_enabled=false;
    Simulator::online_enabled=false;
    s1.arrival_queue = data;
    s1.timestamp();
    cout<<(float)s1.total_spending_time/N<<" ";
    cout<<s1.number_of_fails<<endl;
    stream_f << (float)s1.total_spending_time/N <<" ";
    stream_f <<s1.number_of_fails<<endl;

    Simulator s2(L,S);
    Passenger::fly_first=false;
    Simulator::vip_enabled=true;
    Simulator::online_enabled=false;
    s2.arrival_queue = data;
    s2.timestamp();
    cout<<(float)s2.total_spending_time/N<<" ";
    cout<<s2.number_of_fails<<endl;
    stream_f << (float)s2.total_spending_time/N <<" ";
    stream_f <<s2.number_of_fails<<endl;

    Simulator s3(L,S);
    Passenger::fly_first=true;
    Simulator::vip_enabled=true;
    Simulator::online_enabled=false;
    s3.arrival_queue = data;
    s3.timestamp();
    cout<<(float)s3.total_spending_time/N<<" ";
    cout<<s3.number_of_fails<<endl;
    stream_f << (float)s3.total_spending_time/N <<" ";
    stream_f <<s3.number_of_fails<<endl;


    Simulator s4(L,S);
    Passenger::fly_first=false;
    Simulator::vip_enabled=false;
    Simulator::online_enabled=true;
    s4.arrival_queue = data;
    s4.timestamp();
    cout<<(float)s4.total_spending_time/N<<" ";
    cout<<s4.number_of_fails<<endl;
    stream_f << (float)s4.total_spending_time/N <<" ";
    stream_f <<s4.number_of_fails<<endl;

    Simulator s5(L,S);
    Passenger::fly_first=true;
    Simulator::vip_enabled=false;
    Simulator::online_enabled=true;
    s5.arrival_queue = data;
    s5.timestamp();
    cout<<(float)s5.total_spending_time/N<<" ";
    cout<<s5.number_of_fails<<endl;
    stream_f << (float)s5.total_spending_time/N <<" ";
    stream_f <<s5.number_of_fails<<endl;


    Simulator s6(L,S);
    Passenger::fly_first=false;
    Simulator::vip_enabled=true;
    Simulator::online_enabled=true;
    s6.arrival_queue = data;
    s6.timestamp();
    cout<<(float)s6.total_spending_time/N<<" ";
    cout<<s6.number_of_fails<<endl;
    stream_f << (float)s6.total_spending_time/N <<" ";
    stream_f <<s6.number_of_fails<<endl;


    Simulator s7(L,S);
    Passenger::fly_first=true;
    Simulator::vip_enabled=true;
    Simulator::online_enabled=true;
    s7.arrival_queue = data;
    //cout<<s7.arrival_queue.top().arrival_time<<s.arrival_queue.size()<<endl;
    s7.timestamp();
    //cout<<s7.total_spending_time<<endl;
    cout<<(float)s7.total_spending_time/N<<" ";
    cout<<s7.number_of_fails<<endl;
    stream_f << (float)s7.total_spending_time/N <<" ";
    stream_f <<s7.number_of_fails<<endl;



/*
    s.arrival_queue.push(p1);
    s.arrival_queue.push(p2);
    s.arrival_queue.push(p3);
    s.arrival_queue.push(p4);
    s.arrival_queue.push(p5);
    s.arrival_queue.push(p6);
    s.arrival_queue.push(p7);
    s.arrival_queue.push(p8);

    //Case 1
    Passenger::fly_first=false;
    Simulator::vip_enabled=false;
    Simulator::online_enabled=false;
    s.timestamp();
    cout<<(float)s.total_spending_time/8<<endl;
    cout<<s.number_of_fails<<endl;
*/

/*
    s.arrival_queue.push(p1);
    s.arrival_queue.push(p2);
    s.arrival_queue.push(p3);
    s.arrival_queue.push(p4);
    s.arrival_queue.push(p5);
    s.arrival_queue.push(p6);
    s.arrival_queue.push(p7);
    s.arrival_queue.push(p8);
    //Case 2
    Passenger::fly_first=true;
    Simulator::vip_enabled=false;
    Simulator::online_enabled=false;
    s.timestamp();
    cout<<(float)s.total_spending_time/8<<endl;
    cout<<s.number_of_fails<<endl;

 */

/*
    s.arrival_queue.push(p1);
    s.arrival_queue.push(p2);
    s.arrival_queue.push(p3);
    s.arrival_queue.push(p4);
    s.arrival_queue.push(p5);
    s.arrival_queue.push(p6);
    s.arrival_queue.push(p7);
    s.arrival_queue.push(p8);
    //Case 3
    Passenger::fly_first=false;
    Simulator::vip_enabled=true;
    Simulator::online_enabled=false;
    s.timestamp();
    cout<<(float)s.total_spending_time/8<<endl;
    cout<<s.number_of_fails<<endl;

 */
/*
    s.arrival_queue.push(p1);
    s.arrival_queue.push(p2);
    s.arrival_queue.push(p3);
    s.arrival_queue.push(p4);
    s.arrival_queue.push(p5);
    s.arrival_queue.push(p6);
    s.arrival_queue.push(p7);
    s.arrival_queue.push(p8);
    //Case 4
    Passenger::fly_first=true;
    Simulator::vip_enabled=true;
    Simulator::online_enabled=false;
    s.timestamp();
    cout<<(float)s.total_spending_time/8<<endl;
    cout<<s.number_of_fails<<endl;
    */
/*
    s.arrival_queue.push(p1);
    s.arrival_queue.push(p2);
    s.arrival_queue.push(p3);
    s.arrival_queue.push(p4);
    s.arrival_queue.push(p5);
    s.arrival_queue.push(p6);
    s.arrival_queue.push(p7);
    s.arrival_queue.push(p8);
    //Case 5
    Passenger::fly_first=false;
    Simulator::vip_enabled=false;
    Simulator::online_enabled=true;
    s.timestamp();
    cout<<(float)s.total_spending_time/8<<endl;
    cout<<s.number_of_fails<<endl;
*/

/*
    s.arrival_queue.push(p1);
    s.arrival_queue.push(p2);
    s.arrival_queue.push(p3);
    s.arrival_queue.push(p4);
    s.arrival_queue.push(p5);
    s.arrival_queue.push(p6);
    s.arrival_queue.push(p7);
    s.arrival_queue.push(p8);
    //Case 6
    Passenger::fly_first=true;
    Simulator::vip_enabled=false;
    Simulator::online_enabled=true;
    s.timestamp();
    cout<<(float)s.total_spending_time/8<<endl;
    cout<<s.number_of_fails<<endl;
    */
/*
    s.arrival_queue.push(p1);
    s.arrival_queue.push(p2);
    s.arrival_queue.push(p3);
    s.arrival_queue.push(p4);
    s.arrival_queue.push(p5);
    s.arrival_queue.push(p6);
    s.arrival_queue.push(p7);
    s.arrival_queue.push(p8);
    //Case 7
    Passenger::fly_first=false;
    Simulator::vip_enabled=true;
    Simulator::online_enabled=true;
    s.timestamp();
    cout<<(float)s.total_spending_time/8<<endl;
    cout<<s.number_of_fails<<endl;
*/
/*
    s.arrival_queue.push(p1);
    s.arrival_queue.push(p2);
    s.arrival_queue.push(p3);
    s.arrival_queue.push(p4);
    s.arrival_queue.push(p5);
    s.arrival_queue.push(p6);
    s.arrival_queue.push(p7);
    s.arrival_queue.push(p8);
    //Case 8
    Passenger::fly_first=true;
    Simulator::vip_enabled=true;
    Simulator::online_enabled=true;
    s.timestamp();
    cout<<s.total_spending_time<<endl;
    cout<<(float)s.total_spending_time/8<<endl;
    cout<<s.number_of_fails<<endl;

*/
    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);

    /*
    priority_queue<Passenger, vector<Passenger>, greater<Passenger>> dene;
    dene.push(p1);
    dene.push(p2);
    cout<<dene.top().arrival_time<<endl;
    */
    return 0;
}