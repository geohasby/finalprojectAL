// BATASAN :
// 1 <= vertex <= 100
// 1 <= panjang 1 jalan (m) <= 10000
// 1 <= kendaraan umum <= 100

#include<bits/stdc++.h>
#include<string>
#define pii pair<int, int>
#define maks 1000000
using namespace std;

struct vhc{
	string name;
	int cost;
	int speed;
};

string route(int parent[], int now, string s){
	if(parent[now] == now)
		return char(now+48) + s;
	else{
		s = char(now+48) + s;
		s = '-' + s;
		return route (parent, parent[now], s);	
	}
}

void find_shortest_path(){
	
}

void find_cheapest_path(){
	
}

void find_fastest_path(){
	
}


int main(){ 
	int v, e, x, y, z, veh, a, b, weight;
	string circuit;
	int dist[105];					//menampung jarak terpendek dari vertex asal ke masing-masing vertex lainnya
	int parent[105];				//menyimpan informasi indeks vertex yang dilalui sebelumnya
	vhc vehicle[105];				//menyimpan informasi kendaraan umum (nama, ongkos, kecepatan)
	vector< int >option[105][105];	//pilihan kendaraan yang bisa digunakan untuk masing-masing pasangan vertex	
	vector< pii >adj[105];			//adjacency list
	priority_queue< pii , vector< pii > , greater< pii > >pq;	//priority queue atau min heap yang digunakan untuk algoritma dijkstra
	
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
	cin>>veh;
	
	cout<<"Masukkan keterangan untuk "<<veh<<" kendaran umum (nama, ongkos, kecepatan, dan rute(titik awal = titik akhir) ) : "<<endl;
	for(int i=0; i<veh; i++){
		cin>>vehicle[i].name>>vehicle[i].cost>>vehicle[i].speed>>circuit;
		
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
	
	//set distance pada semua vertex menjadi infinite
	for(int i=1; i<=v; i++)
		dist[i] = maks;
		
	//insert vertex awal dan jarak awalnya masih 0
	pq.push(make_pair(0,a));
	dist[a] = 0;
	parent[a] = a;
	
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
				parent[y] = x;
				pq.push(make_pair(dist[y],y));
			}
		}
	}
	
	//outputkan jarak terpendek untuk menuju ke vertex tujuan
	if(dist[b] == maks)	//jika jarak pada vertex tujuan = infinite (berarti tidak ditemukan rute)
		cout<<"Tidak ditemukan rute dari vertex "<<a<<" ke vertex "<<b<<endl;
	else cout<<"Jarak terpendek = "<<dist[b]<<" m"<<endl<<"Rute : "<<route(parent, b, "")<<endl;
}

//REFERENSI : https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/
