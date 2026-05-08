class C3ErrUnclosedNestedBlocks {
    public static void main(String[] args) {
        int a;
        a = 0;

        if (a == 0) {
            while (a < 2) {
                a = a + 1;
            }

        {
            a = a + 1;
    }
}
