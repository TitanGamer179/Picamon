class AstFEmptyNestedBlocks {
    public static void main(String[] args) {
        int a;
        a = 0;

        ;
        {
            ;
        }
        {
            a = 1;
            {
                a = a + 1;
                ;
            }
        }
    }
}
