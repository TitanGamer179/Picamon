class C3OkStatementsDense {
    public static int id(int x) {
        return x;
    }

    public static void main(String[] args) {
        int a;
        a = 0;

        ;
        {
            a = id(a + 1);
            if (a > 0) {
                while (a < 3) {
                    a = a + 1;
                }
            } else {
                ;
            }
        }

        System.out.print(a);
    }
}
