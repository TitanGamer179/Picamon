class AstFNestedIfWhileBlocks {
    public static void main(String[] args) {
        int a;
        a = 0;

        while (a < 3) {
            if (a == 1) {
                a = a + 2;
            } else {
                a = a + 1;
            }
        }

        {
            {
                ;
            }
            a = a + 1;
        }

        return;
    }
}
