#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <string>
#include <queue>
#include <tuple>
#include <map>

#define fastio ios::sync_with_stdio(false); cin.tie(NULL);
using namespace std;
using ll = long long;

const int n = 12; // jumlah gedung
const ll INF = LLONG_MAX;

struct Pos {
    double x, y;
};

map<int, Pos> coordinates = {
    {1, {0, 0}},    // Agape
    {2, {2, 3}},    // Biblos
    {3, {4, 1}},    // Chara
    {4, {1, 5}},    // Didaktos
    {5, {5, 5}},    // Euodia
    {6, {3, 6}},    // Filia
    {7, {6, 2}},    // Gnosis
    {8, {7, 5}},    // Hagios
    {9, {8, 3}},    // Iama
    {10, {9, 6}},   // Koinonia
    {11, {10, 4}},  // Logos
    {12, {11, 6}}   // Makarios
};

string get_gedung(int gedung) {
    switch (gedung) {
        case 1: return "Agape";
        case 2: return "Biblos";
        case 3: return "Chara";
        case 4: return "Didaktos";
        case 5: return "Euodia";
        case 6: return "Filia";
        case 7: return "Gnosis";
        case 8: return "Hagios";
        case 9: return "Iama";
        case 10: return "Koinonia";
        case 11: return "Logos";
        case 12: return "Makarios";
        default: return "Unknown";
    }
}

double heuristic(int a, int b) {
    double dx = coordinates[a].x - coordinates[b].x;
    double dy = coordinates[a].y - coordinates[b].y;
    return sqrt(dx * dx + dy * dy);
}

void solve() {
    fastio

    for (int i = 1; i <= n; i++) {
        cout << i << " = " << get_gedung(i) << '\n';
    }

    int start, goal;
    cout << "Asal gedung (1 - 12): ";
    cin >> start;
    cout << "Tujuan gedung (1 - 12): ";
    cin >> goal;

    if (start < 1 || start > n || goal < 1 || goal > n) {
        cout << "Input tidak valid.\n";
        return;
    }

    int m;
    cout << "Jumlah edge (jalur): ";
    cin >> m;

    vector<vector<pair<int, int>>> neighbours(n + 1);

    cout << "Masukkan jalur antar gedung (format: a b jarak):\n";
    for (int i = 0; i < m; i++) {
        int a, b, w;
        cin >> a >> b >> w;
        neighbours[a].push_back(make_pair(w, b));
        neighbours[b].push_back(make_pair(w, a));
    }

    using T = pair<double, int>; // f_score, node
    priority_queue<T, vector<T>, greater<T>> pq;

    vector<double> g_score(n + 1, INF);
    vector<int> path(n + 1, -1);

    g_score[start] = 0;
    pq.push(make_pair(heuristic(start, goal), start));

    while (!pq.empty()) {
        T top = pq.top();
        double f = top.first;
        int current = top.second;
        pq.pop();

        if (current == goal) {
            vector<int> ans;
            while (current != -1) {
                ans.push_back(current);
                current = path[current];
            }
            reverse(ans.begin(), ans.end());
            cout << "Rute terbaik:\n";
            for (int i = 0; i < (int)ans.size(); i++) {
                cout << get_gedung(ans[i]);
                if (i != (int)ans.size() - 1) cout << " -> ";
            }
            cout << "\nTotal jarak: " << g_score[goal] << '\n';
            return;
        }

        for (auto &p : neighbours[current]) {
            int cost = p.first;
            int neighbor = p.second;
            double tentative_g = g_score[current] + cost;
            if (tentative_g < g_score[neighbor]) {
                g_score[neighbor] = tentative_g;
                double f_score = tentative_g + heuristic(neighbor, goal);
                pq.push(make_pair(f_score, neighbor));
                path[neighbor] = current;
            }
        }
    }

    cout << "Tidak ada jalur yang ditemukan.\n";
}

int main() {
    solve();
}
