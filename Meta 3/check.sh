#!/bin/bash

# Cores para feedback visual
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# 1. TENTA COMPILAR MANUALMENTE SE O MAKE FALHAR
# lex jucompiler.l && yacc -d jucompiler.y && gcc *.c -o jucompiler

# 2. PASTA DOS TESTES (Confirma se é "./meta3")
TEST_DIR="./test"

if [ ! -d "$TEST_DIR" ]; then
    echo "Erro: Pasta $TEST_DIR não encontrada!"
    exit 1
fi

for f in "$TEST_DIR"/*.java; do
    base=$(basename "$f" .java)
    expected="$TEST_DIR/$base.out"
    
    if [ ! -f "$expected" ]; then
        echo "Aviso: Ficheiro esperado $expected não encontrado. A saltar..."
        continue
    fi

    echo -n "A testar $base... "
    
    # Corre o teu compilador e guarda o output
    ./jucompiler -s < "$f" > "$base.myout" 2>&1
    
    # Compara ignorando espaços em branco (-w) e mostra diferenças lado a lado (-y)
    if diff -w "$base.myout" "$expected" > /dev/null; then
        echo -e "${GREEN}PASSOU${NC}"
        rm "$base.myout"
    else
        echo -e "${RED}FALHOU${NC}"
        echo "--- Diferenças (O Teu vs Esperado) ---"
        diff -y -w --suppress-common-lines "$base.myout" "$expected"
        echo "--------------------------------------"
    fi
done