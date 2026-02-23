#include <stdio.h>       // Biblioteca standard pentru operații de intrare/ieșire
#include <stdlib.h>      // Biblioteca pentru funcții generale (ex. malloc, exit)
#include <string.h>      // Biblioteca pentru manipularea șirurilor de caractere
#include <stdbool.h>     // Biblioteca pentru a permite utilizarea tipului bool
#include <math.h>        // Biblioteca pentru funcții matematice (ex. fabs)

#define MAX_NODES 100    // Definirea numărului maxim de noduri în graf
#define INFINITY 1e9     // Valoare mare pentru a reprezenta infinitul (costul inițial mare)

typedef struct {
    int u, v;            // Nod sursă (u) și nod destinație (v)
    double weight;       // Ponderea muchiei (costul)
} Edge;                  // Structură pentru a reprezenta o muchie

Edge edges[MAX_NODES * MAX_NODES];  // Vector de muchii (lista muchiilor)
int num_edges = 0;                  // Contor pentru numărul de muchii adăugate
double M[MAX_NODES + 1][MAX_NODES + 1]; // Matricea de adiacență (cu ponderi)

void initialize_matrix(int n) {
    // Inițializează matricea de adiacență: 0 pe diagonală, infinit în rest
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            M[i][j] = (i == j) ? 0.0 : INFINITY; // zerografiaza diagonala principala
        }
    }
}

int read_edges(int m, int n) {
    // Citește m muchii de la utilizator și le adaugă în listă și în matrice
    int edge_count = 0;
    printf("Introduceti %d muchii in formatul 'NodSursa NodDest Pondere':\n", m);

    while (edge_count < m) {
        int u, v;
        double w;
        printf("Muchia %d: ", edge_count + 1);
        if (scanf("%d %d %lf", &u, &v, &w) != 3) {
            // Verificare format introducere
            printf("Format invalid. Introduceti 'Nod1 Nod2 Pondere'.\n");
            continue;
        }

        if (u < 1 || u > n || v < 1 || v > n) {
            // Verificare validitate noduri
            printf("Nodurile trebuie sa fie intre 1 si %d.\n", n);
            continue;
        }

        edges[num_edges++] = (Edge){u, v, w};   // Adaugă muchia în vector
        if (M[u][v] > w) M[u][v] = w;           // Actualizează matricea dacă e o pondere mai mică
        edge_count++;
    }
    return 0;
}


/*
Găsește cele mai scurte drumuri de la un nod sursă către toate celelalte noduri dintr-un graf orientat, care poate avea ponderi negative (dar fără cicluri negative).

🔹 Pași principali:
Se inițializează distanța către sursă cu 0 și toate celelalte cu infinit.
Se repetă de n-1 ori (unde n = nr. de noduri):
Pentru fiecare muchie (u → v): initial -> final
Dacă drumul prin u este mai scurt către v, actualizează distanța.
Opțional, se mai face o ultimă iterație pentru a verifica dacă există cicluri negative.
🔹 Rezultat: Distanțele minime de la nodul sursă la toate celelalte și traseul optim (dacă îl reții).

*/
void ford_algorithm(int start, int n, double dist[], int prev[]) { //vectorul dist pastreaza toate distantele care ne aflacm si vector prev pastreazsa structura initiala a drumului
    // Algoritmul lui Ford (Bellman-Ford) pentru cel mai scurt drum de la un nod
    for (int i = 1; i <= n; i++) dist[i] = INFINITY; // Inițializare distanțe
    dist[start] = 0.0;      // Distanța de la nodul de start la el însuși este 0
    // Parcurgem graful de (n - 1) ori pentru a relaxa toate muchiile
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < num_edges; j++) {
            int u = edges[j].u;         // nod sursă
            int v = edges[j].v;         // nod destinație
            double w = edges[j].weight; // ponderea muchiei

            // Dacă drumul de la start la u + muchia (u,v) este mai scurt decât drumul actual la v
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;  // Actualizăm distanța minimă către v
                prev[v] = u;            // Ținem minte că v a fost atins prin u (pentru reconstrucția drumului)
            }
        }
    }
}
/*
🔹 Scop: Găsește cele mai scurte drumuri către un nod final din toate celelalte noduri (invers față de Ford), folosind matricea de adiacență.
🔹 Pași principali:
Se inițializează vectorul de pondere:
Costul către nodul final e 0.
Pentru celelalte, se folosește ponderea direct dacă există (altfel infinit).
Se repetă (maxim n pași):
Pentru fiecare nod i, se caută dacă există un nod j intermediar care oferă un drum mai scurt la end.
Dacă da, se actualizează costul.
Se oprește când nu mai sunt modificări.
🔹 Rezultat: Ponderea  minim de ajungere în nodul final din fiecare alt nod.
*/

void bellman_calaba(int end, int n, double V[]) {
    // Inițializează vectorul V cu costurile de ajungere în nodul final
    // V[i] va reprezenta costul minim de la nodul i la nodul `end`
    for (int i = 1; i <= n; i++)
        V[i] = (i == end) ? 0 : M[i][end];  // cost direct dacă există, altfel INFINITY

    bool updated;  // Flag care indică dacă s-a făcut o modificare în ultima iterație
    int k = 0;     // Numără câte iterații s-au făcut (maxim n)

    do {
        updated = false;  // Presupunem că nu se vor face modificări

        double newV[MAX_NODES + 1];
        memcpy(newV, V, sizeof(newV));  // Copiem valorile vechi pentru actualizare

        // Parcurgem toate nodurile sursă (i)
        for (int i = 1; i <= n; i++) {
            if (i == end) continue;  // Nu actualizăm pentru nodul final

            // Verificăm toate nodurile destinație (j)
            for (int j = 1; j <= n; j++) {
                // Dacă există o muchie de la i la j și drumul prin j e mai ieftin
                if (i != j && newV[i] > M[i][j] + V[j]) {
                    newV[i] = M[i][j] + V[j];  // Relaxăm drumul
                    updated = true;           // Am făcut o modificare
                }
            }
        }

        // Dacă s-au făcut modificări, actualizăm vectorul V
        if (updated)
            memcpy(V, newV, sizeof(newV));

        k++;  // Incrementăm contorul de pași
    } while (updated && k < n);  // Oprim dacă nu se mai fac modificări sau s-au făcut deja n pași
}


void afisare_drum(int start, int curent, int prev[]) {
    // Funcție recursivă pentru afișarea drumului de la start la un nod
    if (curent == start) {
        printf("%d", curent);
        return;
    }
    afisare_drum(start, prev[curent], prev);  // Apelează recursiv pentru nodul anterior
    printf(" -> %d", curent);                 // Afișează nodul curent
}

int main() {
    int n, m;
    printf("Numarul de noduri: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_NODES) {
        printf("Numar invalid de noduri.\n");
        return 1;
    }

    printf("Numarul de muchii: ");
    if (scanf("%d", &m) != 1 || m < 1 || m > n * (n - 1) / 2) {
        printf("Numar invalid de muchii.\n");
        return 1;
    }

    initialize_matrix(n);         // Inițializează matricea de adiacență
    if (read_edges(m, n) != 0)    // Citește muchiile
        return 1;

    // Execută algoritmul Ford (Bellman-Ford)
    int start;
    printf("\nNod de start pentru Ford: ");
    scanf("%d", &start);
    if (start < 1 || start > n) {
        printf("Nod invalid.\n");
        return 1;
    }

    double dist_ford[MAX_NODES + 1];     // Vector distanțe
    int prev_ford[MAX_NODES + 1] = {0};  // Vector părinți
    ford_algorithm(start, n, dist_ford, prev_ford); // Aplică algoritmul

    printf("\nRezultate Ford:\n");
    for (int i = 1; i <= n; i++) {
        if (i == start) continue;
        printf("Distanta de la %d la %d: %.2f\n", start, i, dist_ford[i]);
        printf("Drum: ");
        afisare_drum(start, i, prev_ford);  // Afișează drumul complet
        printf("\n");
    }

    // Execută algoritmul Bellman-Calaba pentru distanța minimă spre un nod
    int end;
    printf("\nNod final pentru Bellman-Calaba: ");
    scanf("%d", &end);
    if (end < 1 || end > n) {
        printf("Nod invalid.\n");
        return 1;
    }

    double V[MAX_NODES + 1];           // Vector pentru costurile minime
    bellman_calaba(end, n, V);         // Aplică algoritmul

    printf("\nRezultate Bellman-Calaba:\n");
    for (int i = 1; i <= n; i++) {
        if (i == end) continue;
        printf("Distanta de la %d la %d: %.2f\n", i, end, V[i]);
    }

    return 0;
}
