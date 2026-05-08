class OkExprPrecedence {
    public static int calc(int a, int b, int c) {
        int r;
        r = a + b * c << 1;
        r = r ^ (a - b) % c;
        return r;
    }

    public static void main(String[] args) {
        int x;
        x = 1;
        while (!(x == 5) && x < 10) {
            x = x + 1;
        }
        System.out.print(calc(x, 2, 3));
    }
}
