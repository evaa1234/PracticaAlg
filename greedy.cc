#include <iostream>
#include <vector>
#include <map>
using namespace std;

/* Compliar:
	g++ -c greedy.cc
	g++ -o ./greedy greedy.cc
*/

// First sera un vertex u, i second seran tots els vertexs v amb els quals u esta 
// connectat.
typedef vector<vector<int> > Graf;

// Pre : 	Cada vertex ha de tenir un identificador corresponent a un numero de 
// 			0 a NumVert-1, i hi ha NumVert vertexs al graf.
// Post: 	G conté la llista de vertexs que son adjacents a cada vertex, de 0 a NumVert.
// 			G estara representat amb una llista d'adjacencies.
void llegirGraf(Graf& G, int NumVert) {
	for(int i = 0; i < NumVert; ++i) { // Els vertexs van numerats del 0 a NumVert - 1
		int U; 
		cin >> U;
		while(U != -1) { // Mentres siguin diferents seguim amb la mateixa llista
			G[i].push_back(U);
			cin >> U;
		}
	}
}

// Pre : 	True.
// Post: 	S'escriu el graf G a l'output.
void escriureGraf(Graf G) {
	int NumVert = G.size();
	for(int i = 0; i < NumVert; ++i) {
		cout << "El vertex " << i << " te aquests veins: ";
		for(int j = 0; j < G[i].size(); ++j) {
			cout << G[i][j] << " ";
		}
		cout << endl;
	}
}

// Pre : 	G es un graf connex i no dirigit, on cada vertex te un
// 			identificador de 0 a NumVert-1, i hi ha NumVert vertexs.
// Post:	Retorna un conjunt de parells (Grau, conjunt de Vertexs).
map<int, vector<int> > OrdenarVertexsGrauAscendent(Graf G) {
	map<int, vector<int> > m;
	for(int i = 0; i < G.size(); ++i) {
		map<int, vector<int> >::iterator it = m.find(G[i].size());
		if(it == m.end()) {
			vector<int> v(1, i);
			m.insert(pair<int, vector<int> > (G[i].size(), v));
		}
		else it->second.push_back(i);
	}
	cout << "GrauVert:" << endl;
	map<int,vector<int> >::iterator it2 = m.begin();
	while(it2 != m.end()) {
		cout << it2->first << " ";
		for(int j = 0; j < it2->second.size(); ++j) cout << it2->second[j] << " ";
		cout << endl;
		++it2;
	}
	return m;
}

// Pre : 	A GrauVert hi ha emmagatzemats els valors dels graus dels vertexs (Grau, conjunt de vertexs)
// Post:	Retorna un conjunt de parells (Vertex, Grau).
map<int, int> CadaVertexQuinGrau(map<int, vector<int> > GrauVert) {
	map<int, int> VertGrau;
	map<int, vector<int> >::iterator it = GrauVert.begin();
	while(it != GrauVert.end()) {
		for(int i = 0; i < it->second.size(); ++i) {
			VertGrau.insert(pair<int, int> (it->second[i], it->first));
		}
		++it;
	}
	cout << "VertGrau:" << endl;
	map<int,int>::iterator it2 = VertGrau.begin();
	while(it2 != VertGrau.end()) {
		cout << it2->first << " " << it2->second << endl;
		++it2;
	}
	return VertGrau;
}

// Pre : 	G es un graf connex i no dirigit, on cada vertex te un
// 			identificador de 0 a NumVert-1, i hi ha NumVert vertexs.
//			GrauVert son parells (Grau, conjunt de Vertexs).
// Post:	Retorna una solucio parcial al problema, marcant a 1 els vertexs
//			que escollim del vector que es retorna.
vector<int> GrafPruning(Graf G, map<int, vector<int> > GrauVert, map<int, int> VertGrau) {
	// Vector on emmagatzemarem la solucio. Inicialment tots els nodes estan a 0:
	vector<int> S(G.size(), 0); 
	map<int,int>::iterator it = GrauVert.begin();
	// 	Iterarem sobre cada vertex de grau 1 (vertex penjats):
	while(it != GrauVert.end() and it->first == 1) { 
		int u = it->second; // Node que nomes te un vei
		int unicVei = G[u][1]; // Unic node connectat amb u.
		S[unicVei] = 1;
		//( ... )
		++it;
	}
	return S;
}

// Pre : 	Un Graf G connex i no-dirigit.
// Post: 	S sera una solucio parcial de MPIDS.
void greedyMPIDS(Graf& G, int NumVert) {

	map<int, vector<int> > GrauVert = OrdenarVertexsGrauAscendent(G);
	map<int, int> VertGrau = CadaVertexQuinGrau(GrauVert);
	vector<int> S = GrafPruning(G, GrauVert, VertGrau);
}


int main() {
	int NumVert;
	cin >> NumVert;
	Graf G(NumVert);
	llegirGraf(G, NumVert);
	greedyMPIDS(G, NumVert);
	escriureGraf(G);
}



/*
	map<int,int>::iterator it = m.begin();
	while(it != m.end()) {
		cout << it->first << " " << it->second << endl;
		++it;
	}*/



