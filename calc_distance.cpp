

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <cfloat>

using namespace std;

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

int main() {

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

    for (int i = 0; i < 12; i++) {
        cout << i + 1 << " " << planar[i].x << " " << planar[i].y << '\n';
    }

}