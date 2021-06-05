#include<bits/stdc++.h>
#define pii pair<int, int>
using namespace std;

int main(){
	//BATASAN MAKSIMAL ADA 100 VERTEX ATAU TITIK ATAU TEMPAT
	int v, e, x, y, z, a, b, weight;
	int dist[105];			//untuk menampung jarak terpendek dari vertex asal ke masing-masing vertex lainnya
	vector< pii >adj[105];	//adjacency list
	priority_queue< pii , vector< pii > , greater< pii > >pq;	//priority queue atau min heap yang digunakan untuk algo dijkstra
	
	cout<<"Masukkan banyak vertex : ";
	cin>>v;
	
	cout<<"Masukkan banyak edge : ";
	cin>>e;
	
	cout<<"Masukkan keterangan untuk "<<e<<" jalan (titik asal, titik tujuan, jarak) : "<<endl;
	for(int i=0; i<e; i++){
		cin>>x>>y>>z;		//jalan menghubungkan kota x ke y dan jaraknya z
		
		//masukkan datanya ke adjacency list
		adj[x].push_back(make_pair(y,z));
		adj[y].push_back(make_pair(x,z));
	}
	
	cout<<"Masukkan titik awal : ";
	cin>>a;
	
	cout<<"Masukkan titik tujuan : ";
	cin>>b;
	
	//set distance pada semua vertex menjadi infinite
	for(int i=0; i<v; i++)
		dist[i] = INT_MAX;
	
	//insert titik awal dan jarak awalnya masih 0
	pq.push(make_pair(0,a));
	dist[a] = 0;
	
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
				pq.push(make_pair(dist[y],y));
			}
		}
	}
	
	cout<<dist[b]<<endl;	//outputkan jarak terpendek untuk menuju ke vertex tujuan
}

//REFERENSI : https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/
