#!/bin/bash

# Cores para feedback visual
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# 1. COMPILAÇÃO DO JUPOMPILER
# Certifica-te que o teu Makefile está funcional
make > /dev/null

# 2. PASTA DOS TESTES
TEST_DIR="./meta4"

if [ ! -d "$TEST_DIR" ]; then
    echo "Erro: Pasta $TEST_DIR não encontrada!"
    exit 1
fi

for f in "$TEST_DIR"/*.java; do
    base=$(basename "$f" .java)
    expected="$TEST_DIR/$base.out"
    args_file="$TEST_DIR/$base.args" # Ficheiro opcional com argumentos de linha de comando
    
    if [ ! -f "$expected" ]; then
        echo "Aviso: Ficheiro esperado $expected não encontrado. A saltar..."
        continue
    fi

    echo -n "A testar $base... "
    
    # 3. GERAÇÃO DO CÓDIGO LLVM IR (.ll)
    # Na Meta 4, sem flags, o jucompiler deve cuspir LLVM IR para o stdout
    ./jucompiler < "$f" > "$base.ll" 2> "$base.err"
    
    # Se houver erros semânticos/sintáticos, o jucompiler não deve gerar código (ou o .ll estará vazio)
    if [ ! -s "$base.ll" ]; then
        # Se o teste esperava output mas falhou a geração, compara o erro
        cp "$base.err" "$base.myout"
    else
        # 4. COMPILAÇÃO E EXECUÇÃO DO CÓDIGO GERADO
        # Lemos os argumentos do ficheiro .args se ele existir (ex: "7" para o Factorial)
        cmd_args=""
        if [ -f "$args_file" ]; then
            cmd_args=$(cat "$args_file")
        fi

        # Usamos o 'lli' para executar o LLVM IR diretamente
        lli "$base.ll" $cmd_args > "$base.myout" 2>> "$base.err"
    fi

    # 5. COMPARAÇÃO DO RESULTADO DA EXECUÇÃO
    if diff -w "$base.myout" "$expected" > /dev/null; then
        echo -e "${GREEN}PASSOU${NC}"
        rm -f "$base.myout" "$base.ll" "$base.err"
    else
        echo -e "${RED}FALHOU${NC}"
        echo "--- Diferenças (Execução vs Esperado) ---"
        diff -y -w --suppress-common-lines "$base.myout" "$expected"
        echo "------------------------------------------"
    fi
done