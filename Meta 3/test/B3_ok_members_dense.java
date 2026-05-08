class B3OkMembersDense {
    public static int a, b, c;
    public static double d;
    public static boolean ready;

    public static int inc(int x) {
        return x + 1;
    }

    public static int add(int x, int y) {
        return x + y;
    }

    public static void main(String[] args) {
        int n;
        n = 0;
        n = add(inc(n), 2);
        System.out.print(n);
    }
}
