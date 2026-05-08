class AstFSemicolonBlockShapes {
    public static void main(String[] args) {
        int a;
        a = 0;

        ;
        ;

        {
            ;
            a = a + 1;
            {
                ;
                a = a + 2;
            }
        }

        if (a > 0)
            ;
        else
            a = a - 1;
    }
}
