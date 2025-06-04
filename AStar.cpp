#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <string>
#include <queue>
#include <cfloat>
#include <map>

using namespace std;
using ll = long long;

const int n = 12; // jumlah gedung



struct Node {
    double x; // longitude gmaps
    double y; // latitude gmaps
};

vector<Node> coordinates = {
    /* formatnya adalah (longitude, latitude) */
    {110.37811638436851, -7.786892527493072},  // 1. agape
    {110.37825953130029, -7.786562258126049},  // 2. biblos
    {110.37842314604282, -7.78631777007192},   // 3. chara
    {110.37836850018982, -7.786012910895829},  // 4. didaktos
    {110.37811203519209, -7.785926844542299},  // 5. euodia
    {110.37804171414969, -7.786487300114521},  // 6. filia
    {110.37850078264714, -7.785753220073825},  // 7. gnosis
    {110.37836550377382, -7.785523171156505},  // 8. hagios
    {110.37855178209473, -7.7852138021969335}, // 9. iama
    {110.37854751118998, -7.784999599965238},  // 10. koinonia
    {110.37821452033853, -7.78528341456613},   // 11. logos
    {110.37854481744473, -7.785369062426726}   // 12. makarios
};

const double latitude_to_meter = 111000; // 1 latitude = 111_000 meter
const double longitude_to_meter = 72000; // 1 longitude = 72_000 meter

vector<Node> get_planar() {

    vector<Node> planar;

    double min_x = DBL_MAX;
    double min_y = DBL_MAX;
    for (auto &[longitude, latitude] : coordinates) {
        double x = longitude * longitude_to_meter; 
        double y = latitude * latitude_to_meter; 
        
        min_x = min(min_x, x);
        min_y = min(min_y, y);

        planar.push_back({x, y});
    }

    for (auto &[x, y] : planar) {
        x = x - min_x;
        y = y - min_y;
    }
    return planar;
}


double heuristic(int start, int goal) {
    static vector<Node> planar = get_planar();

    start--; goal--;

    double delta_x = planar[start].x - planar[goal].x;
    double delta_y = planar[start].y - planar[goal].y;
    return sqrt( delta_x*delta_x + delta_y*delta_y );
}

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


void solve() {
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

    vector<vector<pair<double, int>>> neighbours(n + 1);

    cout << "Masukkan jalur antar gedung (format: a b jarak):\n";
    for (int i = 0; i < m; i++) {
        int a, b; double w;
        cin >> a >> b >> w;
        neighbours[a].emplace_back(w, b);
        neighbours[b].emplace_back(w, a);
    }

    using T = pair<double, int>; // f_score, node
    priority_queue<T, vector<T>, greater<T>> pq;

    vector<double> g_score(n + 1, 1e18);
    vector<int> path(n + 1, -1);

    g_score[start] = 0;
    pq.emplace(heuristic(start, goal), start);

    while (!pq.empty()) {
        auto [f, current] = pq.top(); pq.pop();

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

        for (auto &[cost, neighbor] : neighbours[current]) {
            double tentative_g = g_score[current] + cost;
            if (tentative_g < g_score[neighbor]) {
                g_score[neighbor] = tentative_g;
                double f_score = tentative_g + heuristic(neighbor, goal);
                pq.emplace(f_score, neighbor);
                path[neighbor] = current;
            }
        }
    }

    cout << "Tidak ada jalur yang ditemukan.\n";
}



int main() {
    solve();
}


