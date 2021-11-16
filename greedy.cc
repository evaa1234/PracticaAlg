#include <iostream>
#include <vector>
using namespace std;


typedef vector < vector<int> > Graf;

// 	Pre : Cada vertex ha de tenir un identificador corresponent a un numero de 
// 	0 a NumVert-1, i hi ha NumVert vertexs al graf.
//	Post: G conté la llista de vertexs que son adjacents a cada vertex, de 0 a NumVert.
void llegirGraf(Graf& G, NumVert) {
	for(int i = 0; i < NumVert; ++i) { // Els vertexs van numerats del 0 a NumVert - 1
		int U; 
		cin >> U;
		while(U != -1) { // Mentres siguin diferents seguim amb la mateixa llista
			G[i].pushback(U);
			cin >> U;
		}
	}
}

void OrdenarVertexsGrauAscendent() {
	
}


main() {
	int NumVert, NumArestes;
	cin >> NumVert;
	Graf G(NumVert);
	llegirGraf(G, NumVert);
	OrdenarVertexsGrauAscendent();
}