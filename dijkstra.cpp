//vertex max 100 -> 0-9
//panjang jalan (km)

// BATASAN :
// 1 <= vertex <= 100 (zero-index)
// vertex-1 <= edge <= vertex*(vertex-1)/2
// 1 <= panjang jalan (km) <= 100
// 1 <= kendaraan umum <= 100
// 1 <= ongkos kendaraan umum <= 100.000
// 1 <= kecepatan (km/jam) <= 100
// headway (menit) : faktor dari 60 (1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60) 

#include<bits/stdc++.h>
#define pii pair<int, int>
#define maks INT_MAX
using namespace std;

struct edg{
	int distance;			//jarak
	int speed;				//kecepatan rata-rata jika dilalui kendaraan pribadi
	vector< pii > vehicle;	//pilihan kendaraan (indeks kendaraan dan menit keberangkatan)
};

struct transp{
	string name;			//nama kendaraan
	int speed;				//kecepatan kendaraan
	int headway;			//headway kendaraan
	int spot;				//banyak titik yang dilalui rute kendaraan
	vector<int>route;		//rute kendaraan
};

struct prv{
	int vertex;
	int waiting_time;
	int vehicle;
};

int v, e, x, y, z, s, pt, d, src, dest, temp, hw, spd, mnt, wt, dt, weight, trav_time, jam, menit, vhc_now, prev_vhc;
edg edge[105][105];			//menyimpan informasi panjang jalan, kecepatan rata-rata (kendaraan pribadi), transportasi umum yang tersedia
int dist[105];				//menyimpan jarak dari vertex asal ke masing-masing vertex lainnya
int travel_time[105];		//menyimpan waktu tempuh dari vertex asal ke masing-masing vertex lainnya
prv prev[105];				//menyimpan informasi vertex, waktu tunggu, dan kendaraan yang digunakan sebelumnya
transp pub_transp[105];		//menyimpan informasi kendaraan umum (nama, ongkos, kecepatan, headway)
vector<int>adj[105];		//adjacency list
priority_queue< pii , vector< pii > , greater< pii > >pq;	//priority queue atau min heap yang digunakan untuk algoritma dijkstra

void add_pub_transp_route(){
//	for(int i=0; i<pt; i++){
//		z = pub_transp[i].spot;		//banyak titik yang dilewati rute kendaraan
//		temp = 0;					//variable temp untuk menyimpan jarak jalan yang sudah ditempuh kendaraan dari titik
//		for(int j=0; j<z; j++){
//			x = pub_transp[i].route[j];			//titik sekarang
//			y = pub_transp[i].route[(j+1)%z];	//titik selanjutnya
//			
//			if(edge[x][y].distance == 0){
//				
//			}
//		}
//	}
}

void print_route(int now){
	if(prev[now].vertex == now)
		cout<<now;
	else{
		print_route(prev[now].vertex);
		cout<<" - "<<now;
	}
	return;
}

void print_pub_transp_route(int now){
	if(prev[now].vertex != now){
		print_pub_transp_route(prev[now].vertex);
		cout<<"\n\tdari titik "<<prev[now].vertex<<" : ";
		
		if(prev[now].waiting_time == 0)
			cout<<"tidak perlu menunggu ";
		else{
			cout<<"menunggu ";
			if(prev[now].waiting_time > 60)
				cout<<prev[now].waiting_time / 60<<" menit ";
			if(prev[now].waiting_time % 60 > 0)
				cout<<prev[now].waiting_time % 60<<" detik ";	
		}
		
		cout<<"-> naik "<<pub_transp[prev[now].vehicle].name<<" ke titik "<<now;
	}
	return;
}

void find_shortest_path(){
	//set distance pada semua vertex menjadi infinite
	for(int i=1; i<=v; i++)
		dist[i] = maks;
		
	//insert vertex awal ke dalam priority queue
	pq.push(make_pair(0,src));
	
	//set jarak dan waktu tempuh awalnya 0
	dist[src] = 0;
	travel_time[src] = 0;
	prev[src].vertex = src;
	
	//looping sampai isi priority queue habis / kosong
	while(!pq.empty()){
		//ambil vertex pada top of priority queue yang merupakan vertex dengan jarak paling minimum
		x = pq.top().second;	//index vertex yang ada di top priority queue
		pq.pop();				//keluarkan vertex yang ada di top priority queue
		
		//cek semua tetangga dari vertex x
		for(int i=0; i<adj[x].size(); i++){
			y = adj[x][i];												//index vertex tetangga
			weight = edge[x][y].distance;								//jarak dari vertex x ke vertex y (km)
			trav_time = 3600 * edge[x][y].distance / edge[x][y].speed;	//waktu tempuh dari vertex x ke vertex y (second)
			
			//jika jarak dari x ke y lebih kecil daripada jarak yang ada di y
			//atau
			//jika jarak x ke y sama dengan jarak yang ada di y dan waktu tempuh x ke y lebih kecil daripada waktu tempuh yang ada di y
			//maka update nilainya
			if(dist[y] > dist[x] + weight or ((dist[y] == dist[x] + weight) and (travel_time[y] > travel_time[x] + trav_time))){
				dist[y] = dist[x] + weight;
				travel_time[y] = travel_time[x] + trav_time;
				prev[y].vertex = x;
				pq.push(make_pair(dist[y],y));
			}
		}
	}
	
	//outputkan jarak terpendek untuk menuju ke vertex tujuan
	if(dist[dest] == maks)	//jika jarak pada vertex tujuan = infinite (berarti tidak ditemukan rute)
		cout<<"\n - Tidak ditemukan rute dari vertex "<<src<<" ke vertex "<<dest<<endl;
	else{
		cout<<"\n - Jarak terpendek = "<<dist[dest]<<" km"<<endl<<" - Rute : ";
		print_route(dest);
		cout<<"\n - Waktu yang dibutuhkan : ";
		if(travel_time[dest] >= 3600)
			cout<<travel_time[dest] / 3600<<" jam ";
		if(travel_time[dest] % 3600 >= 60)
			cout<<(travel_time[dest] % 3600) / 60<<" menit ";
		if(travel_time[dest] % 60 > 0)
			cout<<travel_time[dest]%60<<" detik ";
		cout<<endl;
	}
}

void find_fastest_path(){
	//set waktu tempuh pada semua vertex menjadi infinite
	for(int i=1; i<=v; i++)
		travel_time[i] = maks;
		
	//insert vertex awal ke priority queue
	pq.push(make_pair(0,src));
	
	//set waktu tempuh dan jarak awalnya 0
	travel_time[src] = 0;
	dist[src] = 0;
	prev[src].vertex = src;
	
	//looping sampai isi priority queue habis / kosong
	while(!pq.empty()){
		//ambil vertex pada top of priority queue yang merupakan vertex dengan waktu tempuh paling minimum
		x = pq.top().second;	//index vertex yang ada di top priority queue
		pq.pop();				//keluarkan vertex yang ada di top priority queue
		
		//cek semua tetangga dari vertex x
		for(int i=0; i<adj[x].size(); i++){
			y = adj[x][i];												//index vertex tetangga
			trav_time = 3600 * edge[x][y].distance / edge[x][y].speed;	//waktu tempuh dari vertex x ke vertex y (second)
			weight = edge[x][y].distance;								//jarak dari vertex x ke vertex y (km)
			
			//jika waktu tempuh dari x ke y lebih kecil daripada waktu tempuh yang ada di y
			//atau
			//jika waktu tempuh x ke y sama dengan waktu tempuh yang ada di y dan jarak x ke y lebih kecil daripada jarak yang ada di y
			//maka update nilainya
			if(travel_time[y] > travel_time[x] + trav_time or ((travel_time[y] == travel_time[x] + trav_time) and (dist[y] > dist[x] + weight))){
				travel_time[y] = travel_time[x] + trav_time;
				dist[y] = dist[x] + weight;
				prev[y].vertex = x;
				pq.push(make_pair(travel_time[y],y));
			}
		}
	}
	
	//outputkan jarak dengan waktu tempuh tercepat untuk menuju ke vertex tujuan
	if(travel_time[dest] == maks)	//jika waktu tempuh pada vertex tujuan = infinite (berarti tidak ditemukan rute)
		cout<<"\n - Tidak ditemukan rute dari vertex "<<src<<" ke vertex "<<dest<<endl;
	else{
		cout<<"\n - Jarak dengan waktu tempuh tercepat = "<<dist[dest]<<" km"<<endl<<" - Rute : ";
		print_route(dest);
		cout<<"\n - Waktu yang dibutuhkan : ";
		if(travel_time[dest] >= 3600)
			cout<<travel_time[dest] / 3600<<" jam ";
		if(travel_time[dest] % 3600 >= 60)
			cout<<(travel_time[dest] % 3600) / 60<<" menit ";
		if(travel_time[dest] % 60 > 0)
			cout<<travel_time[dest]%60<<" detik ";
		cout<<endl;
	}
}

void find_fastest_path_pub_transp(){
	//set travel time pada semua vertex menjadi infinite
	for(int i=1; i<=v; i++)
		travel_time[i] = maks;
	
	//insert vertex awal ke priority queue dan anggap waktu keberangkatan (detik) dimulai dari menit 00
	pq.push(make_pair(menit*60,src));
	travel_time[src] = menit*60;
	prev[src].vertex = src;
	
	//looping sampai isi priority queue habis / kosong
	while(!pq.empty()){
		//ambil vertex pada top of priority queue yang merupakan vertex dengan waktu tempuh paling minimum
		x = pq.top().second;	//index vertex yang ada di top priority queue
		pq.pop();				//keluarkan vertex yang ada di top priority queue
		
		//cek semua tetangga dari vertex x
		for(int i=0; i<adj[x].size(); i++){
			y = adj[x][i];					//index vertex tetangga
			d = edge[x][y].distance;		//jarak x ke y
			mnt = travel_time[x] % 3600;	//waktu yang sudah ditempuh (menit dan detiknya saja (dalam satuan detik))
			
			//cek semua transportasi umum yang tersedia dari vertex x ke y
			for(int j=0; j<edge[x][y].vehicle.size(); j++){
				vhc_now = edge[x][y].vehicle[j].first;	//kendaraan yang (mungkin) akan digunakan sekarang
				dt = edge[x][y].vehicle[j].second;		//departure time
				hw = pub_transp[vhc_now].headway * 60;	//headway (detik)
				spd = pub_transp[vhc_now].speed;		//kecepatan kendaraan
				
				//waiting time (detik)
				if(mnt % hw == dt)
					wt = 0;
				else if(mnt % hw > dt)
					wt = dt - (mnt % hw) + hw;
				else if(mnt % hw < dt)
					wt = dt - (mnt % hw);
				
				weight = wt + 3600*d/spd;		//total waktu yang dibutuhkan (detik) untuk menuju ke vertex y dengan kendaraan tersebut
				
				//jika waktu tempuh dari x ke y lebih cepat daripada waktu yang sudah ada di y, maka update nilainya
				if(travel_time[y] > travel_time[x] + weight){
					travel_time[y] = travel_time[x] + weight;
					prev[y].vertex = x;
					prev[y].waiting_time = wt;
					prev[y].vehicle = vhc_now;
					pq.push(make_pair(travel_time[y],y));
				}
			}
		}
	}
	
	//outputkan jarak terpendek untuk menuju ke vertex tujuan
	if(travel_time[dest] == maks)	//jika jarak pada vertex tujuan = infinite (berarti tidak ditemukan rute)
		cout<<" - Tidak ditemukan rute dari vertex "<<src<<" ke vertex "<<dest<<endl;
	else{
		cout<<"\n - Waktu tempuh tercepat : ";
		if(travel_time[dest] >= 3600)
			cout<<travel_time[dest] / 3600<<" jam ";
		if(travel_time[dest] % 3600 >= 60)
			cout<<(travel_time[dest] % 3600) / 60<<" menit ";
		if(travel_time[dest] % 60 > 0)
			cout<<travel_time[dest]%60<<" detik ";
		cout<<"\n - Rute : ";
		print_pub_transp_route(dest);
		
	} 	
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

int main(){
	
	cout<<"Masukkan banyak titik / tempat : ";
	cin>>v;
	
	cout<<"Masukkan banyak jalan : ";
	cin>>e;
	
	cout<<"Masukkan keterangan untuk masing-masing jalan : "<<endl;
	cout<<"(titik asal, titik tujuan, jarak (km), dan kecepatan rata-rata kendaraan pribadi (km/h))"<<endl;
	for(int i=0; i<e; i++){
		cin>>x>>y>>z>>s;		//jalan menghubungkan titik x ke y dengan jarakn z km dan kecepatan rata2 kendaraan pribadi s km/h
		
		//masukkan datanya ke adjacency list
		adj[x].push_back(y);
		adj[y].push_back(x);
		
		//masukkan data panjang jalan
		edge[x][y].distance = z;
		edge[y][x].distance = z;
		
		//masukkan data kecepatan rata-rata kendaraan pribadi
		edge[x][y].speed = s;
		edge[y][x].speed = s;
	}
	
	cout<<"Masukkan banyak pilihan transportasi umum : ";
	cin>>pt;
	
	cout<<"Masukkan keterangan untuk masing-masing jenis transportasi umum : "<<endl;
	cout<<"(nama, kecepatan (km/h), headway (menit), jumlah titik yang dilalui dalam 1x putaran, dan titik mana saja yang dilalui)"<<endl;
	for(int i=0; i<pt; i++){
		cin>>pub_transp[i].name>>pub_transp[i].speed>>pub_transp[i].headway>>pub_transp[i].spot;
		trav_time = 0;
		
		//input titik pemberhentian kendaraan umum
		//lalu masukkan index kendaraan tersebut ke opsi kendaraan yang ada di array edge
		for(int j=0; j<=pub_transp[i].spot; j++){
			if(j < pub_transp[i].spot){
				cin>>temp;
				pub_transp[i].route.push_back(temp);
			}	
			if(j > 0){
				x = pub_transp[i].route[j-1];
				y = pub_transp[i].route[j % pub_transp[i].spot];
				edge[x][y].vehicle.push_back(make_pair(i, trav_time % (pub_transp[i].headway * 60)));
				trav_time += (3600 * edge[x][y].distance / pub_transp[i].speed);
			}
		}
	}
	
	cout<<"Masukkan vertex awal : ";
	cin>>src;
	
	cout<<"Masukkan vertex tujuan : ";
	cin>>dest;
	
	cout<<"Masukkan waktu keberangkatan (jam, menit) : ";
	cin>>jam>>menit;
	
	cout<<"\nMenggunakan kendaraan pribadi :";
	find_shortest_path();
	memset(prev, 0, sizeof(prev));	//reset value array prev	
	find_fastest_path();
	
	cout<<"\nMenggunakan transportasi umum :";
	memset(prev, 0, sizeof(prev));	//reset value array prev
	find_fastest_path_pub_transp();
}
