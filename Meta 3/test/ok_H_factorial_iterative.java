class OkHFactorialIterative {
    public static int factorial(int n) {
        int r;
        r = 1;
        while (n > 1) {
            r = r * n;
            n = n - 1;
        }
        return r;
    }

    public static void main(String[] args) {
        int n;
        n = Integer.parseInt(args[0]);
        System.out.print(factorial(n));
    }
}
