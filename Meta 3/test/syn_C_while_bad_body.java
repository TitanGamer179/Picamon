class SynCWhileBadBody {
    public static void main(String[] args) {
        int a;
        a = 0;

        while (a < 5)
            if (a == 1)
            else
                a = a + 1;
    }
}
