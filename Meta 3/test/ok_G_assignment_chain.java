class OkGAssignmentChain {
    public static void main(String[] args) {
        int a;
        int b;
        int c;

        a = b = c = 3;
        a = a + (b * c) - (a / 1);
        System.out.print(a);
    }
}
