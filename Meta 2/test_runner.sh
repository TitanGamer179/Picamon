#!/bin/bash
cd "meta2" || exit 1
pass=0
fail=0
for f in *.java; do
    base="${f%.java}"
    ../jucompiler -t < "$f" > "${base}.myout"
    if diff -q "${base}.myout" "${base}.out" > /dev/null; then
        echo "[PASS] $f"
        pass=$((pass+1))
    else
        echo "[FAIL] $f"
        fail=$((fail+1))
        # Uncomment to see diff
        # diff "${base}.myout" "${base}.out"
    fi
done
echo "Passed: $pass, Failed: $fail"
