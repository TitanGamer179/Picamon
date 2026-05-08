class OkGBooleanRelationalMix {
    public static boolean test(int a, int b) {
        boolean r;
        r = (a < b) && !(a == 0) || (b >= 10);
        return r;
    }

    public static void main(String[] args) {
        boolean x;
        x = test(2, 11);
        System.out.print(x);
    }
}
