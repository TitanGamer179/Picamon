class OkHArgsSum {
    public static void main(String[] args) {
        int i;
        int sum;
        i = 0;
        sum = 0;

        while (i < args.length) {
            sum = sum + Integer.parseInt(args[i]);
            i = i + 1;
        }

        System.out.print(sum);
    }
}
