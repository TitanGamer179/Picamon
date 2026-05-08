class C4ErrNestedElseStorm {
    public static void main(String[] args) {
        int a;
        a = 0;

        if (a > 0)
            if (a > 1)
                a = 2;
            else
                ;
        else
            else
                a = 3;
    }
}
