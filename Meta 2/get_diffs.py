import difflib

files = ["exemplo_erro", "field_decl", "all_errors", "assign_error", "MethodFieldError5"]

for base in files:
    print(f"--- {base} ---")
    with open(f"meta2/{base}.myout") as f1, open(f"meta2/{base}.out_fixed") as f2:
        l1 = f1.readlines()
        l2 = f2.readlines()
        diff = difflib.unified_diff(l1, l2, fromfile=f"{base}.myout", tofile=f"{base}.out_fixed")
        for i, line in enumerate(diff):
            if i > 15:
                break
            print(line, end="")
