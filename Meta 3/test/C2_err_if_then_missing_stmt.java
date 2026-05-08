class C2ErrIfThenMissingStmt {
    public static void main(String[] args) {
        int a;
        a = 1;

        if (a > 0)
        else {
            a = 2;
        }
    }
}
