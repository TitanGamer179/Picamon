class AstFWhileMix {
    public static int inc(int x) {
        return x + 1;
    }

    public static void main(String[] args) {
        int a;
        a = 0;

        while (a < 3)
            a = inc(a);

        while (a < 5) {
            a = a + 1;
            ;
        }

        return;
    }
}
