import os
import filecmp

for f in os.listdir("meta2"):
    if f.endswith(".myout"):
        base = f[:-6]
        out_f = f"{base}.out_fixed"
        if not filecmp.cmp(os.path.join("meta2", f), os.path.join("meta2", out_f), shallow=False):
            print(f"[FAIL] {base}")
