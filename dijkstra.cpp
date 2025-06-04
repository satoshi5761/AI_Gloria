#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <string>
#include <queue>
#include <tuple>
#include <cfloat>

#define debug 1
#define DEBUG if (debug)
#define fastio ios::sync_with_stdio(false); cin.tie(NULL);
using ll = long long;
using ull = unsigned long long;
using namespace std;
const ll INF = LONG_LONG_MAX;

const int n = 12; // jumlah gedung
const int m = 16; // jumlah baris input 

string get_gedung(int gedung) {
    switch (gedung)
    {
        case (1):
        return "Agape";
        case (2):
        return "Biblos";
        case (3):
        return "Chara";
        case(4):
        return "Didaktos";
        case(5):
        return "Euodia";
        case(6):
        return "Filia";
        case(7):
        return "Gnosis";
        case(8):
        return "Hagios";
        case(9):
        return "Iama";
        case(10):
        return "Koinonia";
        case(11):
        return "Logos";
        case(12):
        return "Makarios";
    default:
        return "Unknown";
    }
}


void solve() {

    for (int i = 1; i <= 12; i++) {
        cout << i << " = " << get_gedung(i) << '\n';
    }

    int start, goal;
    cout << "asal gedung (1 - 12): ";
    cin>>start;
    cout << "tujuan gedung (1 - 12): ";
    cin>>goal;

    vector< vector<pair<double, int>> > neighbours(n + 1);

    for (int i = 0; i < m; i++) {
        int a, b;
        double w;
        cin>>a>>b>>w;
        neighbours[a].push_back({w, b});
        neighbours[b].push_back({w, a});
    }

    using T = pair<double, int>;
    priority_queue<T, vector<T>, greater<T>> pq;

    pq.push({0, start});

    vector<double> min_distance(n+1, DBL_MAX);
    vector<int> path(n+1, -1);

    path[start] = 0;
    min_distance[start] = 0;

    while (!pq.empty()) {
        auto [distance, vertex_now] = pq.top();
        pq.pop();

        if (distance != min_distance[vertex_now]) continue;

        if (vertex_now == goal) {
            vector<int> ans;
            while (vertex_now != 0) {
                ans.push_back(vertex_now);
                vertex_now = path[vertex_now];
            }

            reverse(ans.begin(), ans.end());
            for (int i = 0; i < ans.size() - 1; i++) {
                cout << get_gedung(ans[i]) << " -> ";
            } cout << get_gedung(ans[ans.size()-1]) << '\n';
            cout << min_distance[goal] << '\n';
            return;
        }

        for (auto &[current_distance, vertex_target] : neighbours[vertex_now]) {
            double total_distance = distance + current_distance;

            if (min_distance[vertex_target] > total_distance) {
                pq.push({total_distance, vertex_target});
                min_distance[vertex_target] = total_distance;
                path[vertex_target] = vertex_now;
            }
        }
    }

    cout << -1 << '\n';
}

int main() {

        solve();

}