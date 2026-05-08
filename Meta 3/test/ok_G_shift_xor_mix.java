class OkGShiftXorMix {
    public static int calc(int a, int b) {
        int r;
        r = (a << 2) ^ (b >> 1);
        r = r + (a & b);
        return r;
    }

    public static void main(String[] args) {
        int x;
        int y;
        x = 8;
        y = 3;
        System.out.print(calc(x, y));
    }
}
