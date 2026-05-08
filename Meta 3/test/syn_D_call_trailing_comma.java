class SynDCallTrailingComma {
    public static int f(int a, int b, int c) {
        return a + b + c;
    }

    public static void main(String[] args) {
        int x;
        x = f(1, 2, );
    }
}
