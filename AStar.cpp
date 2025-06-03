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
    double lat, lon;
};

map<int, Pos> coordinates = {
    {1, {-7.786892527493072, 110.37811638436851}},    // Agape
    {2, {-7.786562258126049, 110.37825953130029}},    // Biblos
    {3, {-7.78631777007192, 110.37842314604282}},     // Chara
    {4, {-7.786012910895829, 110.37836850018982}},    // Didaktos
    {5, {-7.785926844542299, 110.37811203519209}},    // Euodia
    {6, {-7.786487300114521, 110.37804171414969}},    // Filia
    {7, {-7.785753220073825, 110.37850078264714}},    // Gnosis
    {8, {-7.785523171156505, 110.37836550377382}},    // Hagios
    {9, {-7.7852138021969335, 110.37855178209473}},   // Iama
    {10, {-7.784999599965238, 110.37854751118998}},   // Koinonia
    {11, {-7.78528341456613, 110.37821452033853}},    // Logos
    {12, {-7.785369062426726, 110.37854481744473}}    // Makarios
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

const double R = 6371000.0; 

double toRadians(double degree) {
    return degree * M_PI / 180.0;
}

double haversine(int a, int b) {
    Pos pa = coordinates[a];
    Pos pb = coordinates[b];
    double dLat = toRadians(pb.lat - pa.lat);
    double dLon = toRadians(pb.lon - pa.lon);
    double lat1 = toRadians(pa.lat);
    double lat2 = toRadians(pb.lat);

    double a_val = sin(dLat/2) * sin(dLat/2) + cos(lat1) * cos(lat2) * sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a_val), sqrt(1 - a_val));
    return R * c;
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
        neighbours[a].emplace_back(w, b);
        neighbours[b].emplace_back(w, a);
    }

    using T = pair<double, int>; // f_score, node
    priority_queue<T, vector<T>, greater<T>> pq;

    vector<double> g_score(n + 1, INF);
    vector<int> path(n + 1, -1);

    g_score[start] = 0;
    pq.emplace(haversine(start, goal), start);

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
            cout << "\nTotal jarak: " << g_score[goal] << " meter\n";
            return;
        }

        for (auto &[cost, neighbor] : neighbours[current]) {
            double tentative_g = g_score[current] + cost;
            if (tentative_g < g_score[neighbor]) {
                g_score[neighbor] = tentative_g;
                double f_score = tentative_g + haversine(neighbor, goal);
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