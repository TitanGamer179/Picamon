class OkFieldsMethods {
    public static int a, b;
    public static double c;
    public static boolean flag;

    public static int sum(int x, int y) {
        int z;
        z = x + y;
        return z;
    }

    public static void main(String[] args) {
        int n;
        n = Integer.parseInt(args[0]);
        if (n > 0) {
            System.out.print(sum(n, 1));
        } else {
            System.out.print("zero_or_negative");
        }
    }
}
