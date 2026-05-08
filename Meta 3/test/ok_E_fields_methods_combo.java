class OkEFieldsMethodsCombo {
    public static int x, y, z;
    public static double rate;
    public static boolean ready;

    public static int add(int a, int b) {
        return a + b;
    }

    public static double twice(double d) {
        return d + d;
    }

    public static void main(String[] args) {
        int n;
        n = Integer.parseInt(args[0]);
        System.out.print(add(n, 1));
    }
}
