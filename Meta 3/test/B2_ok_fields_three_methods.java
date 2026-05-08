class B2OkFieldsThreeMethods {
    public static int a, b;
    public static double d;
    public static boolean ok;

    public static int add(int x, int y) {
        return x + y;
    }

    public static void ping() {
        ;
    }

    public static double twice(double x) {
        return x + x;
    }

    public static void main(String[] args) {
        int n;
        n = 0;
        System.out.print(add(n, 1));
    }
}
