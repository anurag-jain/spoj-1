import java.util.*;

public class INTSS2 {
	static int[] w;
	static long res;

	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		int t = sc.nextInt();

		while (t-- > 0) {
			int n = sc.nextInt();
			int k = sc.nextInt();

			w = new int[n];
			int[] all = new int[n];
			for (int i = 0; i < n; i++) {
				w[i] = sc.nextInt();
				all[i] = i;
			}

			boolean[][] g = new boolean[n][n];

			for (int i = 0; i < k; i++) {
				int v1 = sc.nextInt() - 1;
				int v2 = sc.nextInt() - 1;
				g[v1][v2] = g[v2][v1] = true;
			}

			res = 0;
			maximumIndependentSet(g, all, 0, n, 0);

			System.out.println(res);
		}

	}

	public static void maximumIndependentSet(boolean[][] graph, int[] oldSet, int ne, int ce, long curSum) {
		int nod = 0;
		int minnod = ce;
		int fixp = -1;
		int s = -1;

		for (int i = 0; i < ce && minnod != 0; i++) {
			int p = oldSet[i];
			int cnt = 0;
			int pos = -1;

			for (int j = ne; j < ce; j++)
				if (graph[p][oldSet[j]]) {
					if (++cnt == minnod)
						break;
					pos = j;
				}

			if (minnod > cnt) {
				minnod = cnt;
				fixp = p;
				if (i < ne)
					s = pos;
				else {
					s = i;
					nod = 1;
				}
			}
		}

		int[] newSet = new int[ce];

		for (int k = minnod + nod; k >= 1; k--) {
			int sel = oldSet[s];
			oldSet[s] = oldSet[ne];
			oldSet[ne] = sel;

			int newne = 0;
			for (int i = 0; i < ne; i++)
				if (!graph[sel][oldSet[i]])
					newSet[newne++] = oldSet[i];

			int newce = newne;
			long remain = 0;
			for (int i = ne + 1; i < ce; i++)
				if (!graph[sel][oldSet[i]]) {
					newSet[newce++] = oldSet[i];
					remain += w[oldSet[i]];
				}

			curSum += w[sel];

			if (newce == 0) {
				res = Math.max(res, curSum);
			} else if (newne < newce) {
				if (curSum + remain > res)
					maximumIndependentSet(graph, newSet, newne, newce, curSum);
			}

			curSum -= w[sel];

			++ne;
			if (k > 1)
				for (s = ne; !graph[fixp][oldSet[s]]; s++)
					;
		}
	}

}
