class C4OkDenseButValid {
    public static int id(int x) {
        return x;
    }

    public static void main(String[] args) {
        int a;
        a = 0;

        {
            if (a == 0)
                a = id(a + 1);
            else
                ;
        }

        while (a < 3) {
            a = a + 1;
        }

        return;
    }
}
