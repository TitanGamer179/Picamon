#!/bin/bash
pass=0; fail=0
for f in meta2/*.java; do
    base="${f%.java}"
    ./jucompiler -t < "$f" > "${base}.myout"
    # Normalize expected output names (old spec used Id, DecLit, etc.)
    sed -E 's/\bId\b/Identifier/g; s/\bDecLit\b/Natural/g; s/\bRealLit\b/Decimal/g; s/\bMethodCall\b/Call/g' "${base}.out" > "${base}.out_fixed"
    if diff -q "${base}.myout" "${base}.out_fixed" > /dev/null; then
        pass=$((pass+1))
    else
        echo "[FAIL] ${f##*/}"
        diff "${base}.myout" "${base}.out_fixed" | head -8
        echo "..."
    fi
done
echo "Pass: $pass, Fail: $fail"
