#!/bin/bash
cd "meta2" || exit 1
pass=0
fail=0
for f in *.java; do
    base="${f%.java}"
    ../jucompiler -t < "$f" > "${base}.myout"
    # replace old names with new names in .out
    sed -E 's/\bId\b/Identifier/g; s/\bDecLit\b/Natural/g; s/\bRealLit\b/Decimal/g; s/\bMethodCall\b/Call/g' "${base}.out" > "${base}.out_fixed"
    if diff -q "${base}.myout" "${base}.out_fixed" > /dev/null; then
        pass=$((pass+1))
    else
        fail=$((fail+1))
        # echo "[DIFF] $base"
        # diff -u "${base}.out_fixed" "${base}.myout" | head -n 10
    fi
done
echo "Pass: $pass, Fail: $fail"
