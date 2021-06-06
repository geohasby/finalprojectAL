// BATASAN :
// 1 <= vertex <= 100
// vertex-1 <= edge <= vertex*(vertex-1)/2
// 1 <= panjang 1 jalan (m) <= 10.000
// 1 <= kendaraan umum <= 100
// 1 <= ongkos kendaraan umum <= 100.000
// 1 <= kecepatan (km/jam) <= 100
// headway (menit) : faktor dari 60 (1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60) 

#include<bits/stdc++.h>
#include<string>
#define pii pair<int, int>
#define maks INT_MAX
using namespace std;

struct transp{
	string name;
	int fare;
	int speed;
	int headway;
};

int v, e, x, y, z, pt, a, b, d, hw, spd, mnt, wt, weight, jam, menit, vhc_now;
string circuit;
int prev[105];					//menyimpan informasi indeks vertex yang dilalui sebelumnya
transp pub_transp[105];			//menyimpan informasi kendaraan umum (nama, ongkos, kecepatan, headway)
vector< int >option[105][105];	//pilihan kendaraan yang bisa digunakan untuk masing-masing pasangan vertex	
vector< pii >adj[105];			//adjacency list
priority_queue< pii , vector< pii > , greater< pii > >pq;	//priority queue atau min heap yang digunakan untuk algoritma dijkstra
int dist[105];					//menampung jarak terpendek dari vertex asal ke masing-masing vertex lainnya
int cost[105];					//menampung biaya termurah dari vertex asal ke masing-masing vertex lainnya
int travel_time[105];			//menampung waktu tempuh tercepat (detik) dari vertex asal ke masing-masing vertex lainnya

string route(int now, string s){
	if(prev[now] == now)
		return char(now+48) + s;
	else{
		s = char(now+48) + s;
		s = '-' + s;
		return route (prev[now], s);	
	}
}

void find_shortest_path(){
	//set distance pada semua vertex menjadi infinite
	for(int i=1; i<=v; i++)
		dist[i] = maks;
		
	//insert vertex awal dan jarak awalnya masih 0
	pq.push(make_pair(0,a));
	dist[a] = 0;
	prev[a] = a;
	
	//looping sampai isi priority queue habis / kosong
	while(!pq.empty()){
		//ambil vertex pada top of priority queue yang merupakan vertex dengan jarak paling minimum
		x = pq.top().second;	//index vertex yang ada di top priority queue
		pq.pop();				//keluarkan vertex yang ada di top priority queue
		
		//cek semua tetangga dari vertex x
		for(int i=0; i<adj[x].size(); i++){
			y = adj[x][i].first;			//index vertex tetangga
			weight = adj[x][i].second;		//jarak dari vertex x ke vertex tetangga 
			
			//jika jarak dari x ke y lebih kecil daripada jarak yang sudah ada di y, maka update nilainya
			if(dist[y] > dist[x] + weight){
				dist[y] = dist[x] + weight;
				prev[y] = x;
				pq.push(make_pair(dist[y],y));
			}
		}
	}
	
	cout<<"\nMenggunakan kendaraan pribadi :"<<endl;
	//outputkan jarak terpendek untuk menuju ke vertex tujuan
	if(dist[b] == maks)	//jika jarak pada vertex tujuan = infinite (berarti tidak ditemukan rute)
		cout<<" - Tidak ditemukan rute dari vertex "<<a<<" ke vertex "<<b<<endl;
	else cout<<" - Jarak terpendek = "<<dist[b]<<" m"<<endl<<" - Rute : "<<route(b, "")<<endl;
}

void find_cheapest_path(){
//	//set cost pada semua vertex menjadi infinite
//	for(int i=1; i<=v; i++)
//		cost[i] = maks;
//		
//	//insert vertex awal dan cost awalnya masih 0
//	pq.push(make_pair(0,a));
//	cost[a] = 0;
//	
//	//set prev vertex menjadi vertex awal dan prev vehiclenya -1 (belum menggunakan kendaraan apapun)
//	prev[a].vertex = a;
//	prev[a].vehicle = -1;
//	
//	//looping sampai isi priority queue habis / kosong
//	while(!pq.empty()){
//		//ambil vertex pada top of priority queue yang merupakan vertex dengan ongkos paling minimum
//		x = pq.top().second;	//index vertex yang ada di top priority queue
//		pq.pop();				//keluarkan vertex yang ada di top priority queue
//		
//		//cek semua tetangga dari vertex x
//		for(int i=0; i<adj[x].size(); i++){
//			y = adj[x][i].first;			//index vertex tetangga
//			
//			//cek semua kemungkinan kendaraan umum yang tersedia dari vertex x ke y
//			for(int j=0; j<option[x][y].size(); j++){
//				prev_vhc = prev[x].vehicle;	//kendaraan yang digunakan sebelumnya
//				vhc_now = option[x][y][j];	//kendaraan yang (mungkin) akan digunakan sekarang
//				
//				if(prev_vhc == vhc_now)		//jika kendaraan yang digunakan sebelumnya sama dengan sekarang
//					vhc_now_fare = 0;		//maka ongkosnya 0 (karena sudah dibayar di sebelumnya)
//				else vhc_now_fare = pub_transp[vhc_now].fare;	//ongkos kendaraan sekarang
//				
//				//jika biaya dari x ke y lebih kecil daripada biaya yang sudah ada di y, maka update nilainya
//				if(cost[y] > cost[x] + vhc_now_fare){
//					cost[y] = cost[x] + vhc_now_fare;
//					prev[y].vertex = x;
//					prev[y].vehicle = vhc_now;
//					pq.push(make_pair(cost[y],y));
//				}			
//			}
//		}
//	}
//	
//	//outputkan biaya termurah untuk menuju ke vertex tujuan
//	if(cost[b] == maks)	//jika biaya pada vertex tujuan = infinite (berarti tidak ditemukan rute)
//		cout<<"Tidak ditemukan rute dari vertex "<<a<<" ke vertex "<<b<<endl;
//	else cout<<"Biaya termurah = "<<cost[b]<<" rupiah"<<endl<<"Rute : "<<route(b, "")<<endl;
}

void find_fastest_path(){
	//set travel time pada semua vertex menjadi infinite
	for(int i=1; i<=v; i++)
		travel_time[i] = maks;
	
	//anggap keberangkatan dimulai dari menit 00
	pq.push(make_pair(menit,a));	//insert vertex awal
	travel_time[a] = menit*60;		//waktu (detik) yang sudah ditempuh sama dengan menit keberangkatan * 60
	prev[a] = a;
	
	//looping sampai isi priority queue habis / kosong
	while(!pq.empty()){
		//ambil vertex pada top of priority queue yang merupakan vertex dengan waktu tempuh paling minimum
		x = pq.top().second;	//index vertex yang ada di top priority queue
		pq.pop();				//keluarkan vertex yang ada di top priority queue
		
		//cek semua tetangga dari vertex x
		for(int i=0; i<adj[x].size(); i++){
			y = adj[x][i].first;			//index vertex tetangga
			d = adj[x][i].second;			//jarak dari vertex x ke vertex tetangga ke-i
			
			//cek semua kemungkinan kendaraan umum yang tersedia dari vertex x ke y
			for(int j=0; j<option[x][y].size(); j++){
				
				vhc_now = option[x][y][j];				//kendaraan yang (mungkin) akan digunakan sekarang
				hw = pub_transp[vhc_now].headway*60;	//headway kendaraan (detik)
				spd = pub_transp[vhc_now].speed;		//kecepatan kendaraan
				spd = spd*1000/3600;					//convert dari km/h ke m/s
				
				mnt = travel_time[x] % 3600;			//waktu yang sudah ditempuh (menitnya saja (dalam satuan detik))
				
				//waiting time
				if(mnt % hw == 0) wt = 0;		//tidak perlu menunggu
				else wt = hw - (mnt % hw);		//waktu tunggu (menit)
				
				weight = wt + d/spd;			//total waktu yang dibutuhkan (detik) untuk menuju ke vertex y dengan kendaraan tersebut
				
				//jika waktu tempuh dari x ke y lebih cepat daripada waktu yang sudah ada di y, maka update nilainya
				if(travel_time[y] > travel_time[x] + weight){
					travel_time[y] = travel_time[x] + weight;
					prev[y] = x;
					pq.push(make_pair(travel_time[y],y));
//					cout<<x<<' '<<y<<' '<<vhc_now<<' '<<travel_time[y]-travel_time[a]<<" ("<<weight<<' '<<d/spd<<' '<<wt<<')'<<endl;
				}			
			}
		}
	}
	
	cout<<"\nMenggunakan kendaraan umum : "<<endl;
	//outputkan jarak terpendek untuk menuju ke vertex tujuan
	if(travel_time[b] == maks)	//jika jarak pada vertex tujuan = infinite (berarti tidak ditemukan rute)
		cout<<" - Tidak ditemukan rute dari vertex "<<a<<" ke vertex "<<b<<endl;
	else cout<<" - Waktu tempuh tercepat = "<<(travel_time[b] - travel_time[a]) / 60<<" menit"<<endl<<" - Rute : "<<route(b, "")<<endl;	
}

int main(){
	cout<<"Masukkan banyak vertex : ";
	cin>>v;
	
	cout<<"Masukkan banyak edge : ";
	cin>>e;
	
	cout<<"Masukkan keterangan untuk "<<e<<" jalan (vertex asal, vertex tujuan, jarak) : "<<endl;
	for(int i=0; i<e; i++){
		cin>>x>>y>>z;		//jalan menghubungkan kota x ke y dan jaraknya z
		
		//masukkan datanya ke adjacency list
		adj[x].push_back(make_pair(y,z));
		adj[y].push_back(make_pair(x,z));
	}
	
	cout<<"Masukkan banyak kendaraan umum : ";
	cin>>pt;
	
	cout<<"Masukkan keterangan untuk "<<pt<<" kendaran umum (nama, ongkos, kecepatan, headway, dan rute(titik awal = titik akhir) ) : "<<endl;
	for(int i=0; i<pt; i++){
		cin>>pub_transp[i].name>>pub_transp[i].fare>>pub_transp[i].speed>>pub_transp[i].headway>>circuit;
		
		//masukkan indeks kendaraan tersebut ke array opsi kendaraan umum
		//untuk setiap jalan yang dilalui rute kendaraan tersebut
		for(int j=0; j<circuit.length()-1; j++){
			x = int(circuit[j]) - 48;
			y = int(circuit[j+1]) - 48;
			option[x][y].push_back(i);
		}
	}
	
	cout<<"Masukkan vertex awal : ";
	cin>>a;
	
	cout<<"Masukkan vertex tujuan : ";
	cin>>b;
	
	cout<<"Masukkan waktu keberangkatan (jam, menit) : ";
	cin>>jam>>menit;
	
//	for(int i=1; i<=v; i++)
//		for(int j=1; j<=v; j++){
//			if(i == j)
//				continue;
//			else if(option[i][j].empty())
//				cout<<"Tidak ada pilihan kendaraan umum dari vertex "<<i<<" ke vertex "<<j<<endl;
//			else{
//				cout<<"Ada "<<option[i][j].size()<<" pilihan opsi kendaraan umum dari vertex "<<i<<" ke vertex "<<j<<" : ";
//				for(int k=0; k<option[i][j].size(); k++)
//					cout<<option[i][j][k]<<' ';
//				cout<<endl;
//			}
//		}

	find_shortest_path();
	memset(prev, 0, sizeof(prev));
	find_fastest_path();
//	find_cheapest_path();
}

//REFERENSI : https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/
