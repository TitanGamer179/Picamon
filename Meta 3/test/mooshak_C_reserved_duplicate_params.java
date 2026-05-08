class mooshak_C_reserved_duplicate_params {
    public static int foo(int a, int a) {
        return a;
    }

    public static int foo(int x, int y) {
        return x;
    }

    public static int _(double x) {
        return 1;
    }

    public static void main(String[] args) {
    }
}
