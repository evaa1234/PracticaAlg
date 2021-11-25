/***************************************************************************
    greedy.cpp 
    (C) 2021 by C. Blum & M. Blesa
    
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Timer.h"
#include "Random.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <vector>
#include <set>
#include <limits>
#include <iomanip>
#include <map>

// First sera un vertex u, i second seran tots els vertexs v amb els quals u esta 
// connectat.
typedef vector<vector<int> > Graf;

// global variables concerning the random number generator (in case needed)
time_t t;
Random* rnd;

// Data structures for the problem data
int n_of_nodes;
int n_of_arcs;
vector< set<int> > neighbors;

// string for keeping the name of the input file
string inputFile;

// dummy parameters as examples for creating command line parameters 
// see function read_parameters(...)
int dummy_integer_parameter = 0;
int dummy_double_parameter = 0.0;


inline int stoi(string &s) {

  return atoi(s.c_str());
}

inline double stof(string &s) {

  return atof(s.c_str());
}

void read_parameters(int argc, char **argv) {

    int iarg = 1;

    while (iarg < argc) {
        if (strcmp(argv[iarg],"-i")==0) inputFile = argv[++iarg];
        
        // example for creating a command line parameter param1 
        //-> integer value is stored in dummy_integer_parameter
        else if (strcmp(argv[iarg],"-param1")==0) 
            dummy_integer_parameter = atoi(argv[++iarg]); 
            
        // example for creating a command line parameter param2 
        //-> double value is stored in dummy_double_parameter
        else if (strcmp(argv[iarg],"-param2")==0) 
            dummy_double_parameter = atof(argv[++iarg]);  
            
        iarg++;
    }
}

/*****************
Funcions Auxiliars
******************/

// Pre : 	Cada vertex ha de tenir un identificador corresponent a un numero de 
// 			0 a NumVert-1, i hi ha NumVert vertexs al graf.
// Post: 	G conté la llista de vertexs que son adjacents a cada vertex, de 0 a NumVert.
// 			G estara representat amb una llista d'adjacencies.
void llegirGraf(Graf& G, int NumVert, const vector<set<int> >& neighbors) {
	for(int i = 0; i < NumVert; ++i) { // Els vertexs van numerats del 0 a NumVert - 1
        set<int>::iterator it = neighbors[i].begin();
        int j = 0;
		while(it != neighbors[i].end()) {
            G[i].push_back(*it);
            ++it;
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

// Pre :    True.
// Post:    Escriu els nodes que son solucio.
void EscriureSolucio(const vector<int>& S) {
    cout << "Els nodes solucio son: " << endl << endl;
    for(int i = 0; i < S.size(); ++i) {
        if(S[i] == 1) cout << i << " ";
    }
    cout << endl;
    
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
	map<int, vector<int> >::iterator it = GrauVert.begin();
	// 	Iterarem sobre cada vertex de grau 1 (vertex penjats):
	if(it != GrauVert.end() and it->first == 1) { 
        vector<int> vertexs_grau_1 = it->second;
        for(int i = 0; i < vertexs_grau_1.size(); ++i) {
            int u = vertexs_grau_1[i];
            // Afegim a la solucio els vertexs que son veins de nodes de grau 1:
            int unicVei = G[u][0]; // Unic node connectat amb u.
            S[unicVei] = 1;
            // A més, si el grau d'un vertex que es vei únic == 2 i u no pertany a la solucio, afegim
            // el vei de unicVei que no es u:
            map<int, int>::iterator it2 = VertGrau.find(unicVei);
            if(it2->second == 2 and S[u] == 0) {
                for(int j = 0; j < 2; ++j) {
                    if(G[unicVei][j] != u) {
                        int w = G[unicVei][j];
                        S[w] = 1;
                    }
                }
            }
            
        }
	}
	return S;
}

// Pre :    S es el vector solucio que te marcats a 1 els vertexs solucio
// Post:    Retorna el set de vertexs solucio.
set<int> convertSToSet(const vector<int>& S) {
    set<int> D;
    for(int i = 0; i < S.size(); ++i) {
        if(S[i] == 1) D.insert(i);
    }
    
    return D;
}

// Pre :    v es un node de G.
// Post:    retorna la quantitat de veins de v que formen part del conjuny solucio PIDS.
int numVeinsQueSonSolucio(const int& v, const vector<int>& S, const Graf& G) {
    int numVeinsSol = 0;
    for(int i = 0; i < G[v].size(); ++i) {
        if(S[G[v][i]] == 1) ++numVeinsSol;
    }
    return numVeinsSol;
}

// Pre :    True.
// Post:    Retorna el valor absolut d'Ns.
int valorAbs(const int& Ns) {
    if(Ns < 0) return -Ns;
    return Ns;
}

// Pre :    V es un vertex que pertany a VertGrau
// Post:    Retorna Part_entera_per_dalt(grau de v/2) - valor_absolut(num. de veins de v que son solucio).
int h_s(const int& v, map<int, int> VertGrau, const vector<int>& S, const Graf& G) {
    map<int, int>::iterator it = VertGrau.find(v);
    int hs;
    if(it->second%2 == 0) hs = it->second/2;
    else hs = it->second/2 + 1;
    int Ns = numVeinsQueSonSolucio(v, S, G);
    hs -= valorAbs(Ns);
    return hs;
}

// Pre :    True.
// Post:    Retorna el conjunt de nodes que encara no tenen la meitat o mes dels seus veins 
//          a PIDS.
set<int> uncoverVertexs(const vector<int>& S, const vector<vector<int> >& G, map<int, int> VertGrau) {
    set<int> C;
    for(int i = 0; i < G.size(); ++i) {
        int hs = h_s(i, VertGrau, S, G);
        if(hs > 0) C.insert(i);
    }
    return C;
}
                    //cout << "El valor p es: " << p << endl;

// Pre :    v es un node del graf G
// Post:    retorna el vei de v tal que no forma part de la solucio i s'ha d'afegir a la solució per 
//          a que v tingui almenys la meitat dels seus nodes a PIDS.
int argMaxNeedDegree(const vector<int>& v, const vector<vector<int> >& G, map<int, int> VertGrau, vector<int> S) {
    int argMax = 0;
    int MaxNeedDegree = 0;
    for(int i = 0; i < v.size(); ++i) {
        int sumatori = 0;
        for(int u = 0; u < G[v[i]].size(); ++u) {
            int hs = h_s(G[v[i]][u], VertGrau, S, G);
            if(hs > 0) sumatori += hs;
        }
        if(sumatori > MaxNeedDegree) {
            MaxNeedDegree = sumatori;
            argMax = v[i];
        }
    }
    return argMax;
}

// Pre :    v es un node del graf G
// Post:    retorna el vei de v tal que no forma part de la solucio i s'ha d'afegir a la solució per 
//          a que v tingui almenys la meitat dels seus nodes a PIDS.
int computeCdMax(const int& v, const vector<vector<int> >& G, const vector<int>& S, map<int, int> VertGrau) {
    int cd_max = 0;
    // Mirem quin es el cd_max:
    for(int i = 0; i < G[v].size(); ++i) {
        if(S[G[v][i]] == 0) {
            int hs = h_s(G[v][i], VertGrau, S, G);
            hs = valorAbs(hs);
            if(hs > cd_max) cd_max = hs;
            
        }
    }
    // Afegim al vector els vertexs que tenen cd_max:
    vector<int> vertexsWithMaxCd(0);
    for(int i = 0; i < G[v].size(); ++i) {
        if(S[G[v][i]] == 0) {
            int hs = h_s(G[v][i], VertGrau, S, G);
            hs = valorAbs(hs);
            if(hs == cd_max) vertexsWithMaxCd.push_back(G[v][i]);
        }
    }
    // Desempatem:
    return argMaxNeedDegree(vertexsWithMaxCd, G, VertGrau, S);
}

// Pre :    S conte una solucio al problema, que pot tenir veretxs redundants
// Post:    S conte una solucio al problema, sense vertexs redundants
void Reduce(vector<int>& S, const Graf& G, const map<int, int>& VertGrau) {
    for(int i = 0; i < S.size(); ++i) {
        if(S[i] == 1) { // Per cada vertex pertanyent a la solucio:
            int v = i;
            bool tots_hs_menors_zero = true;
            for(int j = 0; j < G[v].size(); ++j) {
                int hs = h_s(G[v][j], VertGrau, S, G);
                if(hs >= 0) tots_hs_menors_zero = false;
            }
            // Si tots els veins de v tenen h_s < 0:
            if(tots_hs_menors_zero) S[i] = 0; 
        }
    }
}


// Pre : 	Un Graf G connex i no-dirigit.
// Post: 	S sera una solucio de MPIDS.
vector<int> greedyMPIDS(Graf& G, int NumVert) {

	map<int, vector<int> > GrauVert = OrdenarVertexsGrauAscendent(G); // Grau->Vertex
	map<int, int> VertGrau = CadaVertexQuinGrau(GrauVert); // Vertex -> Grau
	vector<int> S = GrafPruning(G, GrauVert, VertGrau); // S = Solucio parcial 
    set<int> D = convertSToSet(S); // D = S convertit a set (es necessari per la funcio checkPIDS, ja implementada)
    set<int> C = uncoverVertexs(S, G, VertGrau); // C = vertexs que no estan coberts de moment
    while(C.size() > 0) { // Mentre no hi hagi una solucio:
        map<int, vector<int> >::iterator it = GrauVert.begin();
        while(it != GrauVert.end()) { // Recorrem tots els vertexs en grau ascendent
            vector<int> vertexs = it->second;
            for(int i = 0; i < vertexs.size(); ++i) {
                int v = vertexs[i];
                // Si el vertex[i] no esta cobert:
                if(C.find(v) != C.end()) {
                    int p = h_s(v, VertGrau, S, G);
                    for(int j = 1; j <= p; ++j) { // Iterem sobre els veins de v que no son part de la solucio i que s'han de marcar per tal que la meitat dels seus veins siguin solucio i v quedi cobert:
                        int u = computeCdMax(v, G, S, VertGrau); 
                        S[u] = 1;
                        D.insert(u);
                    }
                    C.erase(v); // v esta cobert
                    cout << "Nodes que no estan coberts:" << C.size() << endl;
                }
            }
            ++it;
        }
    }
    Reduce(S, G, VertGrau); // Aqui es podria implementar una funcio Reduce que fes més petit el conjunt de nodes solucio (Es deixa com a tasca pendent per millorar el programa)
    return S;
}


/************
Main function
*************/

int main( int argc, char **argv ) {

    read_parameters(argc,argv);
    
    // setting the output format for doubles to 2 decimals after the comma
    std::cout << std::setprecision(2) << std::fixed;

    // initializing the random number generator. 
    // A random number in (0,1) is obtained with: double rnum = rnd->next();
    rnd = new Random((unsigned) time(&t));
    rnd->next();

    // variables for storing the result and the computation time 
    // obtained by the greedy heuristic
    double results = std::numeric_limits<int>::max();
    double time = 0.0;

    // opening the corresponding input file and reading the problem data
    ifstream indata;
    indata.open(inputFile.c_str());
    if(not indata) { // file couldn't be opened
        cout << "Error: file could not be opened" << endl;
    }

    indata >> n_of_nodes;
    indata >> n_of_arcs;
    neighbors = vector< set<int> >(n_of_nodes);
    int u, v;
    while(indata >> u >> v) {
        neighbors[u - 1].insert(v - 1);
        neighbors[v - 1].insert(u - 1);
    }
    indata.close();

    // the computation time starts now
    Timer timer;


    // HERE GOES YOUR GREEDY HEURISTIC
    
    int NumVert = n_of_nodes;
    Graf G(NumVert);
	llegirGraf(G, NumVert, neighbors);
	vector<int> S = greedyMPIDS(G, NumVert);
	EscriureSolucio(S);
    // Example for requesting the elapsed computation time at any moment: 
    double ct = timer.elapsed_time(Timer::VIRTUAL);
    cout << "\ttime " << ct << endl;
}

