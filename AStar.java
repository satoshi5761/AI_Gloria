import java.util.*;

public class AStar {

    static final int N = 12; // jumlah gedung
    static final long INF = Long.MAX_VALUE;

    static class Pos {
        double x, y;
        Pos(double x, double y) { this.x = x; this.y = y; }
    }

    static Map<Integer, Pos> coordinates = new HashMap<>();
    static {
        coordinates.put(1, new Pos(0, 0));
        coordinates.put(2, new Pos(2, 3));
        coordinates.put(3, new Pos(4, 1));
        coordinates.put(4, new Pos(1, 5));
        coordinates.put(5, new Pos(5, 5));
        coordinates.put(6, new Pos(3, 6));
        coordinates.put(7, new Pos(6, 2));
        coordinates.put(8, new Pos(7, 5));
        coordinates.put(9, new Pos(8, 3));
        coordinates.put(10, new Pos(9, 6));
        coordinates.put(11, new Pos(10, 4));
        coordinates.put(12, new Pos(11, 6));
    }


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

    static double heuristic(int a, int b) {
        Pos pa = coordinates.get(a);
        Pos pb = coordinates.get(b);
        double dx = pa.x - pb.x;
        double dy = pa.y - pb.y;
        return Math.sqrt(dx*dx + dy*dy);
    }

    public static void solve(Scanner sc) {

        for (int i = 1; i <= N; i++) {
            System.out.println(i + " = " + getGedung(i));
        }

        System.out.print("Asal gedung (1 - 12): ");
        int start = sc.nextInt();
        System.out.print("Tujuan gedung (1 - 12): ");
        int goal = sc.nextInt();

        if (start < 1 || start > N || goal < 1 || goal > N) {
            System.out.println("Input tidak valid.");
            return;
        }

        System.out.print("Jumlah edge (jalur): ");
        int m = sc.nextInt();

        List<List<int[]>> neighbours = new ArrayList<>();
        for (int i = 0; i <= N; i++) {
            neighbours.add(new ArrayList<>());
        }

        System.out.println("Masukkan jalur antar gedung (format: a b jarak):");
        for (int i = 0; i < m; i++) {
            int a = sc.nextInt();
            int b = sc.nextInt();
            int w = sc.nextInt();
            neighbours.get(a).add(new int[]{w, b});
            neighbours.get(b).add(new int[]{w, a});
        }

        PriorityQueue<double[]> pq = new PriorityQueue<>(Comparator.comparingDouble(a -> a[0]));
        double[] g_score = new double[N + 1];
        Arrays.fill(g_score, Double.MAX_VALUE);
        g_score[start] = 0;

        int[] path = new int[N + 1];
        Arrays.fill(path, -1);

        pq.add(new double[]{heuristic(start, goal), start});

        while (!pq.isEmpty()) {
            double[] current = pq.poll();
            double f = current[0];
            int u = (int) current[1];

            if (u == goal) {
                List<Integer> ans = new ArrayList<>();
                while (u != -1) {
                    ans.add(u);
                    u = path[u];
                }
                Collections.reverse(ans);

                System.out.println("Rute terbaik:");
                for (int i = 0; i < ans.size(); i++) {
                    System.out.print(getGedung(ans.get(i)));
                    if (i != ans.size() - 1) System.out.print(" -> ");
                }
                System.out.println("\nTotal jarak: " + g_score[goal]);
                return;
            }

            for (int[] edge : neighbours.get(u)) {
                int cost = edge[0];
                int v = edge[1];
                double tentative_g = g_score[u] + cost;
                if (tentative_g < g_score[v]) {
                    g_score[v] = tentative_g;
                    double f_score = tentative_g + heuristic(v, goal);
                    pq.add(new double[]{f_score, v});
                    path[v] = u;
                }
            }
        }

        System.out.println("Tidak ada jalur yang ditemukan.");
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        solve(sc);
    }
}
