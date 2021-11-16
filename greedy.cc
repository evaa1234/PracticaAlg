#include <iostream>
#include <vector>
using namespace std;


typedef vector<vector<int> > Graf;

// 	Pre : Cada vertex ha de tenir un identificador corresponent a un numero de 
// 	0 a NumVert-1, i hi ha NumVert vertexs al graf.
//	Post: G conté la llista de vertexs que son adjacents a cada vertex, de 0 a NumVert.
//	G estara representat amb una llista d'adjacencies.
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

void OrdenarVertexsGrauAscendent() {

}


int main() {
	int NumVert;
	cin >> NumVert;
	Graf G(NumVert);
	llegirGraf(G, NumVert);
	OrdenarVertexsGrauAscendent();
}