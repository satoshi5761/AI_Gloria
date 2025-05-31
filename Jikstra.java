import java.util.*;

public class Jikstra{

    static final int INF = Integer.MAX_VALUE;
    static final int N = 12; // jumlah gedung
    static final int M = 16; // jumlah building

    static String getGedung(int gedung) {
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

    public static void solve(Scanner sc) {
        for (int i = 1; i <= 12; i++) {
            System.out.println(i + " = " + getGedung(i));
        }

        System.out.print("asal gedung (1 - 12): ");
        int start = sc.nextInt();
        System.out.print("tujuan gedung (1 - 12): ");
        int goal = sc.nextInt();

        List<List<int[]>> neighbours = new ArrayList<>();
        for (int i = 0; i <= N; i++) {
            neighbours.add(new ArrayList<>());
        }

        System.out.println("beri input");
        for (int i = 0; i < M; i++) {
            int a = sc.nextInt();
            int b = sc.nextInt();
            int w = sc.nextInt();
            neighbours.get(a).add(new int[]{w, b});
            neighbours.get(b).add(new int[]{w, a});
        }

        long[] minDistance = new long[N + 1];
        Arrays.fill(minDistance, INF);
        minDistance[start] = 0;

        int[] path = new int[N + 1];
        Arrays.fill(path, -1);
        path[start] = 0;

        PriorityQueue<long[]> pq = new PriorityQueue<>(Comparator.comparingLong(a -> a[0]));
        pq.add(new long[]{0, start});


        while (!pq.isEmpty()) {
            long[] current = pq.poll();

            long distance = current[0];
            int now = (int) current[1];

            if (distance != minDistance[now]) continue;

            if (now == goal) {
                List<Integer> ans = new ArrayList<>();
                while (now != 0) {
                    ans.add(now);
                    now = path[now];
                }

                Collections.reverse(ans);
                for (int i = 0; i < ans.size() - 1; i++) {
                    System.out.print(ans.get(i) + " -> ");
                }
                System.out.println(ans.get(ans.size() - 1));
                return;
            }

            for (int[] edge : neighbours.get(now)) {
                int edgeWeight = edge[0];
                int target = edge[1];
                long totalDistance = distance + edgeWeight;

                if (minDistance[target] > totalDistance) {
                    pq.add(new long[]{totalDistance, target});
                    minDistance[target] = totalDistance;
                    path[target] = now;
                }
            }
        }

        System.out.println(-1);
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        solve(sc);
    }
}
