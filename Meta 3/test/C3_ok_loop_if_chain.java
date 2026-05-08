class C3OkLoopIfChain {
    public static void main(String[] args) {
        int a;
        a = 0;

        while (a < 4)
            if (a == 2)
                a = a + 2;
            else
                a = a + 1;

        return;
    }
}
