class AstFCallsParseargsPrint {
    public static int id(int x) {
        return x;
    }

    public static void main(String[] args) {
        int a;
        a = Integer.parseInt(args[0]);

        id(a);
        System.out.print(id(a));

        if (a > 1) {
            Integer.parseInt(args[1]);
        } else {
            ;
        }
    }
}
