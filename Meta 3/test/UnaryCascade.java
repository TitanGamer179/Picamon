class UnaryCascade {
    public static void main(String[] args) {
        int a, b, c;
        // Teste de unários múltiplos encadeados
        a = + - ! - + b;
        // Mistura de unários com multiplicação (unários têm maior precedência)
        b = -2 * -3 + !true;
        // Múltiplos NOTs
        c = !!!false;
    }
}