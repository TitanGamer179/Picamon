class CommaBug {
    public static void main(String[] args) {
        // Chamada normal (deve ser aceite na nova gramática)
        metodo(1, 2);

        // Chamada com VÍRGULA INICIAL (Erro sintático esperado)
        metodo(, 1);

        // Chamada com VÍRGULA DUPLA (Erro sintático esperado)
        metodo(1, , 2);
    }
}