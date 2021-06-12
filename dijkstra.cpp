// BATASAN :
// 1 <= banyak titik / vertex <= 100 (zero-index)
// vertex-1 <= banyak jalan / edge <= vertex*(vertex-1)/2
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
	vector< pii > vehicle;	//pilihan kendaraan (indeks kendaraan dan menit keberangkatan / ongkos)
};

struct transp{
	string name;		//nama kendaraan
	int fare;			//ongkos (rupiah) jauh dekat sama
	int speed;			//kecepatan kendaraan
	int headway;		//headway kendaraan
	int spot;			//banyak titik yang dilalui rute kendaraan
	vector<int>route;	//rute kendaraan
};

struct prv{
	int vertex;				//index vertex
	int waiting_time;		//waktu tunggu
	int vehicle;			//index kendaraan ummum yang digunakan
};

int v, e, x, y, z, s, pt, d, src, dest, temp, hw, spd, mnt, wt, dt, weight, trav_time, jam, menit, vhc_now, prev_vhc;
edg edge[205][205];			//menyimpan informasi panjang jalan, kecepatan rata-rata (kendaraan pribadi), transportasi umum yang tersedia
int dist[105];				//menyimpan jarak dari vertex asal ke masing-masing vertex lainnya
int travel_time[105];		//menyimpan waktu tempuh dari vertex asal ke masing-masing vertex lainnya
int cost[205];				//menyimpan biaya transportasi umum dari vertex asal ke masing-masing vertex lainnya
prv prev[205];				//menyimpan informasi vertex, waktu tunggu, dan kendaraan yang digunakan sebelumnya
transp pub_transp[105];		//menyimpan informasi kendaraan umum (nama, ongkos, kecepatan, headway)
vector<int>adj[205];		//adjacency list
priority_queue< pii , vector< pii > , greater< pii > >pq;	//priority queue atau min heap yang digunakan untuk algoritma dijkstra

void add_pub_transp_edge(){
	for(int i=0; i<pt; i++){
		z = pub_transp[i].spot;			//banyak titik yang dilewati rute kendaraan
		for(int j=0; j<z; j++){
			x = pub_transp[i].route[j];	//titik saat ini
			y = v + i;					//titik kendaraan
			
			adj[x].push_back(y);		//buat edge baru dari x ke y
			adj[y].push_back(x);		//buat edge baru dari y ke x
			
			edge[x][y].vehicle.push_back(make_pair(i, pub_transp[i].fare));		//naik kendaraan = bayar
			edge[y][x].vehicle.push_back(make_pair(i, 0));						//turun kendaraan = gratis
		}
	}
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

void print_fastest_pub_transp_route(int now){
	if(prev[now].vertex != now){
		int a = now;
		int b = prev[now].vertex;
		
		while((prev[b].vertex != b) and (prev[b].vehicle == prev[now].vehicle)){
			a = b;
			b = prev[b].vertex;
		}
		
		print_fastest_pub_transp_route(b);

		cout<<"\n\tdari titik "<<b;
		
		if(prev[a].waiting_time == 0)
			cout<<" tidak perlu menunggu ";
		else{
			cout<<" menunggu";
			if(prev[a].waiting_time > 60)
				cout<<' '<<prev[a].waiting_time / 60<<" menit";
			if(prev[a].waiting_time % 60 > 0)
				cout<<' '<<prev[a].waiting_time % 60<<" detik";	
		}
		
		cout<<", lalu naik "<<pub_transp[prev[a].vehicle].name<<" sampai ke titik "<<now;
	}
	return;
}

void print_cheapest_pub_transp_route(int now){
	if(prev[now].vertex != now){
		if(prev[now].vertex >= v){
			temp = prev[now].vertex;
			print_cheapest_pub_transp_route(prev[temp].vertex);
			cout<<"\n\tdari titik "<<prev[temp].vertex<<" naik "<<pub_transp[temp-v].name<<" sampai ke titik "<<now<<" dengan ongkos Rp."<<pub_transp[temp-v].fare;
		}
		else{
			print_cheapest_pub_transp_route(prev[now].vertex);
			cout<<"\n\tdari titik "<<prev[now].vertex<<" naik "<<pub_transp[prev[now].vehicle].name<<" sampai ke titik "<<now<<" dengan ongkos Rp."<<pub_transp[prev[now].vehicle].fare;
		}		
	}
	
	return;
}

void find_shortest_path(){
	//set distance pada semua vertex menjadi infinite
	for(int i=0; i<v; i++)
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
				cout<<x<<" -> "<<y<<" : "<<dist[y]<<endl;
				pq.push(make_pair(dist[y],y));
			}
		}
	}
	
	//outputkan jarak terpendek untuk menuju ke vertex tujuan
	if(dist[dest] == maks)	//jika jarak pada vertex tujuan = infinite (berarti tidak ditemukan rute)
		cout<<"\n - Tidak ditemukan rute dari vertex "<<src<<" ke vertex "<<dest<<endl;
	else{
		cout<<"\n - Rute dengan jarak terpendek : ";
		print_route(dest);
		cout<<"\n - Jarak = "<<dist[dest]<<" km";
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
	for(int i=0; i<v; i++)
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
		cout<<"\n - Rute dengan waktu tempuh tercepat : ";
		print_route(dest);
		cout<<"\n - Jarak = "<<dist[dest]<<" km";
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
	for(int i=0; i<v; i++)
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
				
				trav_time = wt + 3600*d/spd;		//total waktu yang dibutuhkan (detik) untuk menuju ke vertex y dengan kendaraan tersebut
				
				//jika waktu tempuh dari x ke y lebih cepat daripada waktu yang sudah ada di y, maka update nilainya
				if(travel_time[y] > travel_time[x] + trav_time){
					travel_time[y] = travel_time[x] + trav_time;
					dist[y] = dist[x] + d;
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
		cout<<"\n - Rute dengan waktu tempuh tercepat : ";
		print_fastest_pub_transp_route(dest);
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

void find_cheapest_path_pub_transp(){
	//set cost pada semua vertex menjadi infinite
	for(int i=0; i<v+pt; i++)
		cost[i] = maks;
		
	//insert vertex awal ke dalam priority queue
	pq.push(make_pair(0,src));
	
	//set ongkos awalny 0
	cost[src] = 0;
	prev[src].vertex = src;
	
	//looping sampai isi priority queue habis / kosong
	while(!pq.empty()){
		//ambil vertex pada top of priority queue yang merupakan vertex dengan ongkos paling minimum
		x = pq.top().second;	//index vertex yang ada di top priority queue
		pq.pop();				//keluarkan vertex yang ada di top priority queue
		
		//cek semua tetangga dari vertex x
		for(int i=0; i<adj[x].size(); i++){
			y = adj[x][i];										//index vertex tetangga
			
			for(int j=0; j<edge[x][y].vehicle.size(); j++){
				vhc_now = edge[x][y].vehicle[j].first;			//kendaraan yang (mungkin) akan digunakan sekarang
				
				if(x < v)										//jika titik keberangkatan dari titik asli
					weight = pub_transp[vhc_now].fare;			//	maka bobot jalannya sesuai ongkos kendaraan
				else weight = 0;								//jika titik keberangkatan dari titik semu, maka bobot jalannya 0
				
				//jika ongkos dari x ke y lebih kecil daripada ongkos yang ada di y, maka update nilainya
				if(cost[y] > cost[x] + weight){
					cost[y] = cost[x] + weight;
					prev[y].vertex = x;
					prev[y].vehicle = vhc_now;
					pq.push(make_pair(cost[y],y));
				}
			}
		}
	}
	
	//outputkan jarak terpendek untuk menuju ke vertex tujuan
	if(cost[dest] == maks)	//jika jarak pada vertex tujuan = infinite (berarti tidak ditemukan rute)
		cout<<"\n - Tidak ditemukan rute dari vertex "<<src<<" ke vertex "<<dest<<endl;
	else{
		cout<<"\n - Rute dengan ongkos termurah : ";
		print_cheapest_pub_transp_route(dest);
		cout<<"\n - Ongkos = Rp."<<cost[dest]<<endl;
	}
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
	cout<<"(nama, harga, kecepatan (km/h), headway (menit), jumlah titik yang dilalui dalam 1x putaran, dan titik mana saja yang dilalui)"<<endl;
	for(int i=0; i<pt; i++){
		cin>>pub_transp[i].name>>pub_transp[i].fare>>pub_transp[i].speed>>pub_transp[i].headway>>pub_transp[i].spot;
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
	
	memset(prev, 0, sizeof(prev));	//reset value array prev	
	find_shortest_path();
	
	memset(prev, 0, sizeof(prev));	//reset value array prev	
	find_fastest_path();
	
	cout<<"\nMenggunakan transportasi umum :";
	
	memset(prev, 0, sizeof(prev));	//reset value array prev
	find_fastest_path_pub_transp();
	
	add_pub_transp_edge();
	
	memset(prev, 0, sizeof(prev));	//reset value array prev
	find_cheapest_path_pub_transp();
}
