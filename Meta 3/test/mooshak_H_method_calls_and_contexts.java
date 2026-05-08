class mooshak_H_method_calls_and_contexts {
    public static int one(int x) {
        return x;
    }

    public static boolean flag(boolean x) {
        return x;
    }

    public static void main(String[] args) {
        while (one(miss(1))) {
        }

        System.out.print(flag(one(true)));
        one(miss2(one(miss3(1))));
    }
}
